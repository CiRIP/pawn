format pe64 efi at 0 on 'NUL'
entry main

section '.text' code executable readable writeable

include 'uefi.inc'
include 'game.inc'
include 'graphics.inc'
include 'utils.inc'
include 'events.inc'
include 'network.inc'

; RCX, RDX, R8, R9

; align 64
; background			EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xEE, 0xEE, 0xEE, 0x00
align 64
main:
	mov rax,rsp
	enter 1024,0
	; u_init
	mov	[ImageHandle], rcx		; ImageHandle
	mov	[SystemTable], rdx    		; pointer to SystemTable


	u_call	BootServices, SetWatchdogTimer, 0, 0, 0, 0
	debug
	jmp_err	exit
	;u_call	ConOut, ClearScreen, ConOut
	u_call	ConOut, EnableCursor, ConOut, 0
	debug
	u_call	ConOut, OutputString, ConOut, _hello
	debug
	jmp_err exit

	;ha: jmp ha

	;u_call	BootServices, LocateProtocol, gopGuid, 0, gop
	u_lp	gopGuid, gop
	debug
	jmp_err exit

	u_call	BootServices, LocateProtocol, sppGuid, 0, spp
	debug
	jmp_err exit

	u_call	BootServices, LocateProtocol, hipGuid, 0, hip
	debug
	jmp_err exit

	;u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.SetMode, [gop], 0

	mov	rax, qword [gop]
	mov	rax, qword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL.Mode]
	mov	ecx, dword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.MaxMode]
	dec	ecx

	loop_modes:
		push	rcx
		u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode, [gop], 0, tmp, gopInfo
		pop	rcx

		mov	eax, dword [gopInfo + EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.PixelFormat]
		cmp	eax, 1
		jle	@f

		dec	rcx
		jns	loop_modes
@@:	;u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.SetMode, [gop], rcx

	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode, [gop], 0, tmp, gopInfo

	; u_call	[spp], EFI_SIMPLE_POINTER_PROTOCOL.Reset, [spp], 0

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], background, EfiBltVideoFill, 0, 0, 0, 0, 200, 200, 0

	mov	rax, qword [gop]
	mov	qword [screen + EFI_IMAGE_OUTPUT.Image], rax
	mov	rax, qword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL.Mode]
	mov	rdx, qword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.FrameBufferBase]
	mov	qword [screen + EFI_IMAGE_OUTPUT.Image], rdx
	mov	rax, qword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.ModeInfo]
	mov	ecx, dword [rax + EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.HorizontalResolution]
	mov	edx, dword [rax + EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.VerticalResolution]

	mov	word [screen + EFI_IMAGE_OUTPUT.Width], cx
	mov	word [screen + EFI_IMAGE_OUTPUT.Height], dx

	mov	qword [screenWidth], rcx
	mov	qword [screenHeight], rdx

	mov	rdx, 7
	pieceRow:
		mov	rcx, 7
		pieceCol:
			mov	r8, tileLightImage
			mov	r9, tileDarkImage

			push	rcx
			push	rdx
			call	render_tile
			pop	rdx
			pop	rcx

		@@:	dec	rcx
			jns	pieceCol
		
		dec	rdx
		jns	pieceRow
			
	
	; e_call	blit, boardBltBuffer, 256, 256, 0, 0, [SPRITES + PIECE.Checker + COLOR.Red], 32, 32

	call	render_screen

	mov	rcx, 7
	mov	rdx, 7
	call	select_tile
	

setup_events:
	u_call	ConIn, Reset, ConIn
	mov	rax, qword [SystemTable]
	mov	rax, qword [rax + EFI_SYSTEM_TABLE.ConIn]
	mov	rax, qword [rax + SIMPLE_INPUT_INTERFACE.WaitForKey]
	mov	[events], rax

	u_call	[spp], EFI_SIMPLE_POINTER_PROTOCOL.Reset, [spp], 0
	mov	rax, qword [spp]
	mov	rdx, qword [rax + EFI_SIMPLE_POINTER_PROTOCOL.Mode]
	mov	rax, qword [rax + EFI_SIMPLE_POINTER_PROTOCOL.WaitForInput]
	mov	[events + 8], rax
	
	mov	rcx, qword [rdx + EFI_SIMPLE_POINTER_MODE.ResolutionX]
	mov	[resolutionX], rcx

	mov	rcx, qword [rdx + EFI_SIMPLE_POINTER_MODE.ResolutionY]
	mov	[resolutionY], rcx

event_loop:
	mov	[index], 0
	u_call	BootServices, WaitForEvent, 2, events, index
	mov	rcx, [index]
	call	qword [event_table + rcx * 8]

	jmp	event_loop

exit:	ret



section '.data' data readable writeable

align 64
_hello				du	'Hello World!', 13, 10, \
					0
_dbg				du	'.', 0
_dbg_wdt			du	'SetWatchdogTimer', 0

align 8
gopGuid				db	EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID
gop				rq	1
gopInfo				rq	1
tmp				rq	1

screen				EFI_IMAGE_OUTPUT

align 8
sppGuid				db	EFI_SIMPLE_POINTER_PROTOCOL_GUID
spp				rq	1

align 8
hipGuid				db	EFI_HII_IMAGE_PROTOCOL_GUID
hip				rq	1

events				rq	2
event_table:
				dq	key_event
				dq	pointer_event
index				rq	1


resolutionX			rq	1
resolutionY			rq	1
mouseX				dq	100
mouseY				dq	100
screenWidth			rq	1
screenHeight			rq	1

align 4				; apparently bltBuffers and pixels need to be aligned on 4 bytes
				; QEMU has no problems with them unaligned, but real UEFI implementations
				; shit themselves.
				; if this had been properly documented in the spec, I wouldn't have wasted
				; a whole fucking day debugging this stupid shit.
				; hopefully this will serve as a pointer to future lone coders trying to
				; develop for UEFI in ASM. I'm sorry for the hours you've wasted till you
				; found this :(
black				EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0x00, 0x00, 0x00, 0x00

align 8
cursor				file	'./assets/cursor.bgra'
align 8
hipBlt				dq	screen
cursorImage			EFI_IMAGE_INPUT			0x01, 32, 32, cursor



section '.reloc' fixups data discardable

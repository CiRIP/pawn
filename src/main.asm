format pe64 dll efi
entry main

section '.text' code executable readable

include 'uefi.inc'

include 'game.inc'

include 'graphics.inc'

; RCX, RDX, R8, R9

main:
	u_init


	u_call	BootServices, SetWatchdogTimer, 0, 0, 0, 0
	u_call	ConOut, Reset, ConOut
	u_call	ConOut, EnableCursor, ConOut, 0
	u_call	ConOut, OutputString, ConOut, _hello

	;u_call	BootServices, LocateProtocol, gopGuid, 0, gop
	u_lp	gopGuid, gop
	u_call	BootServices, LocateProtocol, sppGuid, 0, spp

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.SetMode, [gop], 0

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
@@:	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.SetMode, [gop], rcx

	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode, [gop], 0, tmp, gopInfo

	u_call	[spp], EFI_SIMPLE_POINTER_PROTOCOL.Reset, [spp], 0

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], background, EfiBltVideoFill, 0, 0, 0, 0, 200, 200, 0

	mov	rcx, boardBltBuffer
	call	init_board


	mov	rdx, 7
	pieceRow:
		mov	rcx, 7
		pieceCol:
			push	rcx
			push	rdx
			call	render_piece
			pop	rdx
			pop	rcx

		@@:	dec	rcx
			jns	pieceCol
		
		dec	rdx
		jns	pieceRow
			
	
	; e_call	blit, boardBltBuffer, 256, 256, 0, 0, [SPRITES + PIECE.Checker + COLOR.Red], 32, 32



	mov	rax, qword [gop]
	mov	rax, qword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL.Mode]
	mov	rax, qword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.ModeInfo]
	mov	ecx, dword [rax + EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.HorizontalResolution]
	mov	edx, dword [rax + EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.VerticalResolution]

	push	rcx
	push	rdx

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], background, EfiBltVideoFill, 0, 0, 0, 0, rcx, rdx
	
	mov	rdx, [rsp]
	sub	rdx, 288
	shr	rdx, 1
	mov	rcx, [rsp + 8]
	sub	rcx, 288
	shr	rcx, 1

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], frame, EfiBltVideoFill, 0, 0, rcx, rdx, 288, 288, 0
	
	mov	rdx, [rsp]
	sub	rdx, 256
	shr	rdx, 1
	mov	rcx, [rsp + 8]
	sub	rcx, 256
	shr	rcx, 1

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], boardBltBuffer, EfiBltBufferToVideo, 0, 0, rcx, rdx, 256, 256, 0
	
	; e_call	blit

	@@:
		jmp	@b
	
	exit:
	
	mov	eax, EFI_SUCCESS
	ret



section '.data' data readable writeable
_hello				du	'Hello World!', 13, 10, \
					0
_dbg				du	'.', 0

gopGuid				db	EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID
gop				dq	0
gopInfo				dq	0
tmp				dq	0

sppGuid				db	EFI_SIMPLE_POINTER_PROTOCOL_GUID
spp				dq	0

background			EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xEE, 0xEE, 0xEE, 0x00
frame				EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xA1, 0xC9, 0xE4, 0x00

boardBltBuffer	times 65536	EFI_GRAPHICS_OUTPUT_BLT_PIXEL	?


section '.reloc' fixups data discardable

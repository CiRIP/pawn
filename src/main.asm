format pe64 dll efi
entry main

section '.text' code executable readable

include 'uefi.inc'

include 'game.inc'

; RCX, RDX, R8, R9

main:
	u_init


	u_call	BootServices, SetWatchdogTimer, 0, 0, 0, 0
	u_call	ConOut, Reset, ConOut
	u_call	ConOut, EnableCursor, ConOut, 0
	u_call	ConOut, OutputString, ConOut, _hello

	u_call	BootServices, LocateProtocol, gopGuid, 0, gop

	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode, [gop], 0, tmp, gopInfo
	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.SetMode, [gop], 10

	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], pixel, EfiBltVideoFill, 0, 0, 0, 0, 200, 200, 0


	mov	rdx, 8
	boardRow:
		mov	rcx, 8
		boardCol:
			mov	rax, rcx
			add	rax, rdx
			imul	r9, rcx, 32
			sub	r9, 32
			imul	r8, rdx, 32
			sub	r8, 32

			and	rax, 1
			jz	light
		dark:	mov	rax, tileDark
			jmp	@f
		light:	mov	rax, tileLight
		@@:

			push	rcx
			push	rdx
			e_call	blit, boardBltBuffer, 256, 256, r9, r8, rax, 32, 32
			pop	rdx
			pop	rcx
			
			dec	rcx
			jnz	boardCol
		
		dec	rdx
		jnz	boardRow
	
	e_call	blit, boardBltBuffer, 256, 256, 0, 0, checkerRed, 32, 32



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


blit:
	push	rbp
	mov	rbp, rsp

	mov	rdi, rcx		; 1st arg (destination)		rcx
	imul	r10, rdx, 4		; 2nd arg (destination width)	rdx
					; 3rd arg (destination height)	r8
	imul	r9, 4			; 4th arg (destination X)	r9
	add	rdi, r9
	
	mov	r11, [rbp + 48]		; 5th arg (destination Y)
	imul	r11, r10
	add	rdi, r11

	mov	rsi, [rbp + 56]		; 6th arg (source)
	mov	r12, [rbp + 64]		; 7th arg (source width)
	imul	r14, r12, 4
	sub	r10, r14		; skip
	mov	r13, [rbp + 72]		; 8th arg (source height)

	mov	rdx, r13
	row:
		mov	rcx, r12
		col:
			mov	eax, dword [rsi]
			test	al, al
			jz	@f
			mov	dword [rdi], eax
		@@:	add	rdi, 4
			add	rsi, 4
			loop	col
		add	rdi, r10
		dec	rdx
		test	rdx, rdx
		jnz	row

	mov	rsp, rbp
	pop	rbp
	ret


section '.data' data readable writeable
_hello				du	'Hello World!', 13, 10, \
					0
_dbg				du	'.', 0

gopGuid				db	EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID
gop				dq	0
gopInfo				dq	0
tmp				dq	0

background			EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xEE, 0xEE, 0xEE, 0x00
frame				EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xA1, 0xC9, 0xE4, 0x00

boardBltBuffer	times 65536	EFI_GRAPHICS_OUTPUT_BLT_PIXEL	?

smile				file	'./assets/smile.bgra'
tileDark			file	'./assets/tile_dark.bgra'
tileLight			file	'./assets/tile_light.bgra'

checkerRed			file	'./assets/checker_red.bgra'




section '.reloc' fixups data discardable

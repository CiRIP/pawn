section '.text' code executable readable


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
			mov	r15d, eax
			and	r15d, -256		; test for last 8 bits of pixel (alpha)
			jz	@f			; skip if fully transparent
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


init_board:
	push	rbp
	mov	rbp, rsp

	mov	r15, rcx

	mov	rdx, 7
	boardRow:
		mov	rcx, 7
		boardCol:
			mov	rax, rcx
			add	rax, rdx
			imul	r9, rcx, 32
			imul	r8, rdx, 32

			and	rax, 1
			jz	light
		dark:	mov	rax, tileDark
			jmp	@f
		light:	mov	rax, tileLight
		@@:
			push	rcx
			push	rdx
			e_call	blit, r15, 256, 256, r9, r8, rax, 32, 32
			pop	rdx
			pop	rcx
			
			dec	rcx
			jns	boardCol
		
		dec	rdx
		jns	boardRow

	mov	rsp, rbp
	pop	rbp
	ret


render_piece:
	push	rbp
	mov	rbp, rsp

	imul	r9, rcx, 32
	imul	r8, rdx, 32

	xor	eax, eax
	mov	al, byte [board + rdx * 8 + rcx]
	test	al, al
	js	@f

	e_call	blit, boardBltBuffer, 256, 256, r9, r8, [SPRITES + rax], 32, 32

@@:	mov	rsp, rbp
	pop	rbp
	ret


render_screen:
	push	rbp
	mov	rbp, rsp

	mov	rax, qword [gop]
	mov	rax, qword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL.Mode]
	mov	rax, qword [rax + EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.ModeInfo]
	mov	ecx, dword [rax + EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.HorizontalResolution]
	mov	edx, dword [rax + EFI_GRAPHICS_OUTPUT_MODE_INFORMATION.VerticalResolution]

	push	rcx
	push	rdx

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], background, EfiBltVideoFill, 0, 0, 0, 0, rcx, rdx, 0
	
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

	mov	rsp, rbp
	pop	rbp
	ret



section '.data' data readable writeable

align 4				; apparently bltBuffers and pixels need to be aligned on 4 bytes
				; QEMU has no problems with them unaligned, but real UEFI implementations
				; shit themselves.
				; if this had been properly documented in the spec, I wouldn't have wasted
				; a whole fucking day debugging this stupid shit.
				; hopefully this will serve as a pointer to future lone coders trying to
				; develop for UEFI in ASM. I'm sorry for the hours you've wasted till you
				; found this :(

frame				EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xA1, 0xC9, 0xE4, 0x00
background			EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xEE, 0xEE, 0xEE, 0x00

boardBltBuffer	times 65536	EFI_GRAPHICS_OUTPUT_BLT_PIXEL	?

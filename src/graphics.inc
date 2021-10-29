section '.text' code executable readable


save_buffer:
	push	rbp
	mov	rbp, rsp

	mov	rsi, rcx		; 1st arg (source)		rcx
	imul	r10, rdx, 4		; 2nd arg (source width)	rdx
					; 3rd arg (source height)	r8
	imul	r9, 4			; 4th arg (source X)		r9
	add	rsi, r9
	
	mov	r11, [rbp + 48]		; 5th arg (source Y)
	imul	r11, r10
	add	rsi, r11

	mov	rdi, [rbp + 56]		; 6th arg (destination)
	mov	r12, [rbp + 64]		; 7th arg (destination width)
	imul	r14, r12, 4
	sub	r10, r14		; skip
	mov	r13, [rbp + 72]		; 8th arg (destination height)

	mov	rdx, r13
	row:
		mov	rcx, r12
		col:
			mov	eax, dword [rsi]
			mov	dword [rdi], eax
		@@:	add	rdi, 4
			add	rsi, 4
			loop	col
		add	rsi, r10
		dec	rdx
		test	rdx, rdx
		jnz	row

	mov	rsp, rbp
	pop	rbp
	ret


render_tile:
	push	rbp
	mov	rbp, rsp

	push	r8
	push	r9

	imul	r9, rcx, 32
	imul	r8, rdx, 32
	mov	rax, rcx
	add	rax, rdx

	and	rax, 1
	jz	slight
sdark:	mov	rax, [rbp - 16]
	jmp	@f
slight:	mov	rax, [rbp - 8]

@@:	push	rcx
	push	rdx
	u_call	[hip], EFI_HII_IMAGE_PROTOCOL.DrawImage, [hip], 0x00, rax, boardImagePtr, r9, r8
	mov	rdx, [rsp]
	mov	rcx, [rsp + 8]

	
	imul	r9, rcx, 32
	imul	r8, rdx, 32

	xor	eax, eax
	mov	al, byte [board + rdx * 8 + rcx]
	test	al, al
	js	@f

	; e_call	blit, boardBltBuffer, 256, 256, r9, r8, [SPRITES + rax], 32, 32
	u_call	[hip], EFI_HII_IMAGE_PROTOCOL.DrawImage, [hip], 0x00, [SPRITES + rax], boardImagePtr, r9, r8

	mov	rdx, [rsp]
	mov	rcx, [rsp + 8]

	cmp	[activeTileX], rcx
	jne	@f
	cmp	[activeTileY], rdx
	jne	@f

	imul	r9, rcx, 32
	imul	r8, rdx, 32

	u_call	[hip], EFI_HII_IMAGE_PROTOCOL.DrawImage, [hip], 0x00, tileActiveImage, boardImagePtr, r9, r8

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
	sub	rdx, 192
	shr	rdx, 1
	mov	rcx, [rsp + 8]
	sub	rcx, 288
	shr	rcx, 1

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], frame, EfiBltVideoFill, 0, 0, rcx, rdx, 288, 288, 0
	
	; mov	rdx, [rsp]
	; sub	rdx, 160
	; shr	rdx, 1
	; mov	rcx, [rsp + 8]
	; sub	rcx, 256
	; shr	rcx, 1
	mov	rdx, [screenHeight]
	sub	rdx, 160
	shr	rdx, 1
	mov	rcx, [screenWidth]
	sub	rcx, 256
	shr	rcx, 1

	mov	[boardPosX], rcx
	mov	[boardPosY], rdx

	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], boardBltBuffer, EfiBltBufferToVideo, 0, 0, rcx, rdx, 256, 256, 0

	call	render_board

	mov	rcx, [rsp + 8]
	sub	rcx, 512
	shr	rcx, 1

	u_call	[hip], EFI_HII_IMAGE_PROTOCOL.DrawImage, [hip], 0x00, titleImage, hipBlt, rcx, 0

	call	render_turn

	mov	rsp, rbp
	pop	rbp
	ret


render_board:
	u_call	[hip], EFI_HII_IMAGE_PROTOCOL.DrawImage, [hip], 0x00, boardImage, hipBlt, [boardPosX], [boardPosY]

	ret


render_turn:
	mov	rcx, [screenWidth]
	mov	rdx, [screenHeight]

	sub	rdx, 64
	
	mov	rax, [currentTurn]
	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], [FILLS + rax], EfiBltVideoFill, 0, 0, 0, rdx, rcx, 64, 0

	mov	rcx, [screenWidth]
	mov	rdx, [screenHeight]

	sub	rdx, 64
	sub	rcx, 256
	shr	rcx, 1

	mov	rax, [currentTurn]
	u_call	[hip], EFI_HII_IMAGE_PROTOCOL.DrawImage, [hip], 0x00, [TURNS + rax], hipBlt, rcx, rdx

	ret


select_tile:
	push	rbp
	mov	rbp, rsp

	push	rcx
	push	rdx

	mov	r8, tileLightImage
	mov	r9, tileDarkImage

	mov	rcx, [selectedTileX]
	mov	rdx, [selectedTileY]

	call	render_tile

	pop	rdx
	pop	rcx
	
	mov	[selectedTileX], rcx
	mov	[selectedTileY], rdx

	mov	r8, tileLightSelectedImage
	mov	r9, tileDarkSelectedImage

	call	render_tile

	call	render_board


	mov	rsp, rbp
	pop	rbp
	ret


render_move:
	push	rbp
	mov	rbp, rsp

	cmp	[activeTileX], rcx
	jne	@f
	cmp	[activeTileY], rdx
	jne	@f

	jmp	render_move_end

@@:	push	rcx
	push	rdx

	xor	[currentTurn], COLOR.Black
	call	render_turn

	pop	rdx
	pop	rcx

	mov	r8, [activeTileX]
	mov	r9, [activeTileY]

	mov	al, byte [board + r9 * 8 + r8]
	mov	byte [board + r9 * 8 + r8], -1
	mov	byte [board + rdx * 8 + rcx], al

	call	render_tile

render_move_end:
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
whiteFill			EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xE7, 0xE0, 0xDA, 0x00
blackFill			EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0x34, 0x2a, 0x22, 0x00

FILLS:
	dq	whiteFill
	dq	blackFill

boardBltBuffer	times 65536	EFI_GRAPHICS_OUTPUT_BLT_PIXEL	?
boardImageOut			EFI_IMAGE_OUTPUT		256, 256, 0, boardBltBuffer
boardImagePtr			dq	boardImageOut
boardImage			EFI_IMAGE_INPUT			0x0, 256, 256, boardBltBuffer

boardPosX			rq	1
boardPosY			rq	1

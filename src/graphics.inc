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


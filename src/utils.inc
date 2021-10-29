macro print_d number {
	mov	rcx, number
	mov	rdx, _tmpbuf
	mov	r8, 10
	call	itoaw

	u_call	ConOut, OutputString, ConOut, _tmpbuf
}

macro print_b number, base {
	mov	rcx, number
	mov	rdx, _tmpbuf
	mov	r8, base
	call	itoaw

	u_call	ConOut, OutputString, ConOut, _tmpbuf
}

macro error string {
	local	..dbg

	push	rax
	test	rax, rax
	jns	..dbg
	u_call	ConOut, OutputString, ConOut, _error_pre
	jmp	@f
..dbg:	u_call	ConOut, OutputString, ConOut, _error_dbg
@@:	print_b	rax, 16
	u_call	ConOut, OutputString, ConOut, _error_in
	u_call	ConOut, OutputString, ConOut, string
	u_call	ConOut, OutputString, ConOut, _error_post
}

macro debug {
	local	..dbg

	push	rax
	test	rax, rax
	jns	..dbg
	u_call	ConOut, OutputString, ConOut, _error_pre
	jmp	@f
..dbg:	u_call	ConOut, OutputString, ConOut, _error_dbg
@@:	pop	rax
	print_b	rax, 16
	u_call	ConOut, OutputString, ConOut, _error_post
}

macro jmp_err addr {
	test	rax, rax
	jns	@f
	jmp	addr
@@:
}


section '.text' code executable readable


; --------------------------------------------------------------------
; Convert signed integer to wide string in a specified base
;
; Args:
;	rcx:	source integer
;	rdx:	target buffer (min size 21 words/42 bytes)
;	r8:	target base
; --------------------------------------------------------------------
itoaw:
	push	rbx
	push	rbp
	mov	rbp, rsp

	mov	rbx, rdx
	mov	rax, rcx

	; check sign bit
	test	rax, rax
	jns	@f

	mov	word [rbx], 0x2d	; '-'
	add	rbx, 2
	neg	rax

@@:	mov	rdi, rbx
	divloop:
		xor	rdx, rdx
		div	r8
		xor	rsi, rsi
		mov	sil, byte [_charset + rdx]
		mov	word [rbx], si
		add	rbx, 2

		test	rax, rax
		jnz	divloop
	
	mov	word [rbx], 0
	sub	rbx, 2

	reverse:
		cmp	rdi, rbx
		jge	@f

		mov	ax, word [rdi]
		mov	dx, word [rbx]
		mov	word [rdi], dx
		mov	word [rbx], ax

		add	rdi, 2
		sub	rbx, 2

		jmp	reverse
	
@@:	mov	rsp, rbp
	pop	rbp
	pop	rbx
	ret

section '.data' data readable writeable

_charset	db	'0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'
_tmpbuf		rd	21
_error_pre	du	'[ERROR] 0x', 0
_error_dbg	du	'[DEBUG] 0x', 0
_error_in	du	' in ', 0
_error_post	du	13, 10, 0

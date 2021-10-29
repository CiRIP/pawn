section '.text' code executable readable


; --------------------------------------------------------------------
; Handle keypress events
; --------------------------------------------------------------------
key_event:
	u_call	ConIn, ReadKeyStroke, ConIn, key
	mov	dx, word [key + EFI_INPUT_KEY.UnicodeChar]
	mov	cx, word [key + EFI_INPUT_KEY.ScanCode]

	cmp	cx, 0x01
	je	up_event

	cmp	cx, 0x02
	je	down_event

	cmp	cx, 0x03
	je	right_event

	cmp	cx, 0x04
	je	left_event

	cmp	dx, 'q'
	je	quit_event

	cmp	dx, ' '
	je	space_event

@@:	mov	word [op], dx

	u_call	ConOut, OutputString, ConOut, op
	ret


quit_event:
	u_call	ConOut, ClearScreen, ConOut
	u_call	BootServices, Exit, [ImageHandle], 0

	ret

up_event:
	mov	rcx, [selectedTileX]
	mov	rdx, [selectedTileY]

	dec	rdx
	and	rdx, 7

	call	select_tile

	ret

down_event:
	mov	rcx, [selectedTileX]
	mov	rdx, [selectedTileY]

	inc	rdx
	and	rdx, 7

	call	select_tile

	ret

right_event:
	mov	rcx, [selectedTileX]
	mov	rdx, [selectedTileY]

	inc	rcx
	and	rcx, 7

	call	select_tile

	ret

left_event:
	mov	rcx, [selectedTileX]
	mov	rdx, [selectedTileY]

	dec	rcx
	and	rcx, 7

	call	select_tile

	ret

space_event:
	mov	rcx, [selectedTileX]
	mov	rdx, [selectedTileY]

	cmp	[activeTileX], -1
	je	space_event_set_active

	call	render_move

	mov	rcx, [activeTileX]
	mov	rdx, [activeTileY]

	mov	[activeTileX], -1
	mov	[activeTileY], -1

	jmp	@f

space_event_set_active:
	mov	[activeTileX], rcx
	mov	[activeTileY], rdx

@@:	call	select_tile

	ret

; --------------------------------------------------------------------
; Handle mouse pointer events
; --------------------------------------------------------------------
pointer_event:
	u_call	[spp], EFI_SIMPLE_POINTER_PROTOCOL.GetState, [spp], state

	u_call	[hip], EFI_HII_IMAGE_PROTOCOL.DrawImage, [hip], 0x01, tmpBltImage, hipBlt, [mouseX], [mouseY]

	mov	eax, dword [state + EFI_SIMPLE_POINTER_STATE.RelativeMovementX]

	cdq
	mov	ecx, dword [resolutionX]
	idiv	ecx
	shl	eax, 1
	add	dword [mouseX], eax
	jns	@f

	mov	dword [mouseX], 0

@@:	mov	eax, dword [state + EFI_SIMPLE_POINTER_STATE.RelativeMovementY]

	cdq
	mov	ecx, dword [resolutionY]
	idiv	ecx
	shl	eax, 1
	add	dword [mouseY], eax
	jns	@f

	mov	dword [mouseY], 0

@@:	mov	rcx, [mouseX]
	sub	rcx, [boardPosX]
	cmp	rcx, 255
	ja	fin

	mov	rdx, [mouseY]
	sub	rdx, [boardPosY]
	cmp	rdx, 255
	ja	fin

	shr	rcx, 5
	shr	rdx, 5

	mov	al, byte [state + EFI_SIMPLE_POINTER_STATE.LeftButton]
	test	al, al
	jz	@f

	cmp	[activeTileX], -1
	je	setact

	call	render_move

	mov	rcx, [activeTileX]
	mov	rdx, [activeTileY]

	mov	[activeTileX], -1
	mov	[activeTileY], -1

	jmp	@f
	
setact:	mov	[activeTileX], rcx
	mov	[activeTileY], rdx

@@:	call	select_tile

fin:	e_call	save_buffer, [screen + EFI_IMAGE_OUTPUT.Image], [screenWidth], [screenHeight], [mouseX], [mouseY], tmpBltBuffer, 32, 32
	;call	render_screen
	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], cursor, EfiBltBufferToVideo, 0, 0, [mouseX], [mouseY], 32, 32, 0

	u_call	[hip], EFI_HII_IMAGE_PROTOCOL.DrawImage, [hip], 0x01, cursorImage, hipBlt, [mouseX], [mouseY]
	;debug

	ret


section '.data' data readable writeable
align 64
tmpBltBuffer	times 1024	EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0xEE, 0xEE, 0xEE, 0x00
tmpBltImage			EFI_IMAGE_INPUT	0x00, 32, 32, tmpBltBuffer
key				EFI_INPUT_KEY
state				EFI_SIMPLE_POINTER_STATE
op				du	'.', 0

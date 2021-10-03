format pe64 dll efi
entry main

section '.text' code executable readable

include 'uefi.inc'

main:
	u_init


	u_call	BootServices, SetWatchdogTimer, 0, 0, 0, 0
	u_call	ConOut, Reset, ConOut
	u_call	ConOut, EnableCursor, ConOut, 0
	u_call	ConOut, OutputString, ConOut, _hello

	u_call	BootServices, LocateProtocol, gopGuid, 0, gop

	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.QueryMode, [gop], 0, tmp, gopInfo
	; u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.SetMode, [gop], 10

	u_call	[gop], EFI_GRAPHICS_OUTPUT_PROTOCOL.Blt, [gop], pixel, EfiBltVideoFill, 0, 0, 200, 200, 200, 200, 0

	mov	rcx, qword [gop]
	mov	rcx, qword [rcx + EFI_GRAPHICS_OUTPUT_PROTOCOL.Mode]
	mov	ecx, dword [rcx + EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.MaxMode]
	
	aa:
		push 	rcx
		u_call	ConOut, OutputString, ConOut, _dbg
		pop	rcx
		loop	aa

	lp:
		jmp	lp
	
	exit:
	
	mov	eax, EFI_SUCCESS
	ret


section '.data' data readable writeable
_hello		du	'Hello World!', 13, 10, \
			0
_dbg		du	'.', 0

gopGuid		db	EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID
gop		dq	0
gopInfo		dq	0
tmp		dq	0

pixel		EFI_GRAPHICS_OUTPUT_BLT_PIXEL	0x9A, 0xA1, 0x42, 0x00

section '.reloc' fixups data discardable

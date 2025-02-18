;*********************************************************************
;*                                                                   *
;*          UEFI library for fasm by bzt, Public Domain              *
;*          With modifications by Ciprian Ionescu, MIT               *
;*                                                                   *
;*********************************************************************



macro enum var, [stuff] {
	common
		local x
		x = 0
		struc var \{. dd ?\}
	forward
		stuff = x
		x = x + 1
}
 
; include x86asm.net's efi.inc

struc int8 {
	.	db	?
}

struc int16 {
	align 2
	.	dw	?
}

struc int32 {
	align 4
	.	dd	?
}

struc int64 {
	align 8
	.	dq	?
}

struc intn {
	align 8
	.	dq	?
}

struc dptr {
	align 8
	.	dq	?
}
 
; ----------------------------- symbols ------------------------------
 
EFIERR				= 0x8000000000000000
EFI_SUCCESS			= 0
EFI_LOAD_ERROR			= EFIERR or 1
EFI_INVALID_PARAMETER		= EFIERR or 2
EFI_UNSUPPORTED 		= EFIERR or 3
EFI_BAD_BUFFER_SIZE		= EFIERR or 4
EFI_BUFFER_TOO_SMALL		= EFIERR or 5
EFI_NOT_READY			= EFIERR or 6
EFI_DEVICE_ERROR		= EFIERR or 7
EFI_WRITE_PROTECTED		= EFIERR or 8
EFI_OUT_OF_RESOURCES		= EFIERR or 9
EFI_VOLUME_CORRUPTED		= EFIERR or 10
EFI_VOLUME_FULL 		= EFIERR or 11
EFI_NO_MEDIA			= EFIERR or 12
EFI_MEDIA_CHANGED		= EFIERR or 13
EFI_NOT_FOUND			= EFIERR or 14
EFI_ACCESS_DENIED		= EFIERR or 15
EFI_NO_RESPONSE 		= EFIERR or 16
EFI_NO_MAPPING			= EFIERR or 17
EFI_TIMEOUT			= EFIERR or 18
EFI_NOT_STARTED 		= EFIERR or 19
EFI_ALREADY_STARTED		= EFIERR or 20
EFI_ABORTED			= EFIERR or 21
EFI_ICMP_ERROR			= EFIERR or 22
EFI_TFTP_ERROR			= EFIERR or 23
EFI_PROTOCOL_ERROR		= EFIERR or 24
 
; - helper macro for definition of relative structure member offsets -
 
macro struct name {
	virtual at 0
		name name
	end virtual
}
 
; ---------------------------- structures ----------------------------

EFI_SYSTEM_TABLE_SIGNATURE equ 49h, 42h, 49h, 20h, 53h, 59h, 53h, 54h

struc EFI_TABLE_HEADER {
	.Signature	int64
	.Revision	int32
	.HeaderSize	int32
	.CRC32		int32
	.Reserved	int32
}
struct EFI_TABLE_HEADER

struc EFI_SYSTEM_TABLE {
	.Hdr			EFI_TABLE_HEADER
	.FirmwareVendor		dptr
	.FirmwareRevision	int32
	.ConsoleInHandle	dptr
	.ConIn			dptr
	.ConsoleOutHandle	dptr
	.ConOut			dptr
	.StandardErrorHandle	dptr
	.StdErr			dptr
	.RuntimeServices	dptr
	.BootServices		dptr
	.NumberOfTableEntries	intn
	.ConfigurationTable	dptr
}
struct EFI_SYSTEM_TABLE

struc SIMPLE_TEXT_OUTPUT_INTERFACE {
	.Reset			dptr
	.OutputString		dptr
	.TestString		dptr
	.QueryMode		dptr
	.SetMode		dptr
	.SetAttribute		dptr
	.ClearScreen		dptr
	.SetCursorPosition	dptr
	.EnableCursor		dptr
	.Mode			dptr
}
struct SIMPLE_TEXT_OUTPUT_INTERFACE

struc SIMPLE_INPUT_INTERFACE {
	.Reset			dptr
	.ReadKeyStroke		dptr
	.WaitForKey		dptr
}
struct SIMPLE_INPUT_INTERFACE

struc EFI_INPUT_KEY {
	.ScanCode		int16
	.UnicodeChar		int16
}
struct EFI_INPUT_KEY

struc EFI_BOOT_SERVICES_TABLE {
	.Hdr					EFI_TABLE_HEADER
	.RaisePriority				dptr
	.RestorePriority			dptr
	.AllocatePages				dptr
	.FreePages				dptr
	.GetMemoryMap				dptr
	.AllocatePool				dptr
	.FreePool				dptr
	.CreateEvent				dptr
	.SetTimer				dptr
	.WaitForEvent				dptr
	.SignalEvent				dptr
	.CloseEvent				dptr
	.CheckEvent				dptr
	.InstallProtocolInterface		dptr
	.ReInstallProtocolInterface		dptr
	.UnInstallProtocolInterface		dptr
	.HandleProtocol				dptr
	.Void					dptr
	.RegisterProtocolNotify			dptr
	.LocateHandle				dptr
	.LocateDevicePath			dptr
	.InstallConfigurationTable		dptr
	.ImageLoad				dptr
	.ImageStart				dptr
	.Exit					dptr
	.ImageUnLoad				dptr
	.ExitBootServices			dptr
	.GetNextMonotonicCount			dptr
	.Stall					dptr
	.SetWatchdogTimer			dptr
	.ConnectController			dptr
	.DisConnectController			dptr
	.OpenProtocol				dptr
	.CloseProtocol				dptr
	.OpenProtocolInformation		dptr
	.ProtocolsPerHandle			dptr
	.LocateHandleBuffer			dptr
	.LocateProtocol				dptr
	.InstallMultipleProtocolInterfaces	dptr
	.UnInstallMultipleProtocolInterfaces	dptr
	.CalculateCrc32				dptr
	.CopyMem				dptr
	.SetMem					dptr
}
struct EFI_BOOT_SERVICES_TABLE

struc EFI_RUNTIME_SERVICES_TABLE {
	.Hdr			EFI_TABLE_HEADER
	.GetTime		dptr
	.SetTime		dptr
	.GetWakeUpTime		dptr
	.SetWakeUpTime		dptr
	.SetVirtualAddressMap	dptr
	.ConvertPointer		dptr
	.GetVariable		dptr
	.GetNextVariableName	dptr
	.SetVariable		dptr
	.GetNextHighMonoCount	dptr
	.ResetSystem		dptr
}
struct EFI_RUNTIME_SERVICES_TABLE

struc EFI_TIME {
	.Year			int16
	.Month			int8
	.Day			int8
	.Hour			int8
	.Minute			int8
	.Second			int8
	.Pad1			int8
	.Nanosecond		int32
	.TimeZone		int16
	.Daylight		int8
	.Pad2			int8
	.sizeof			rb 1
}
struct EFI_TIME

EFI_LOADED_IMAGE_PROTOCOL_GUID equ 0A1h, 31h, 1bh, 5bh, 62h, 95h, 0d2h, 11h, 8Eh, 3Fh, 0h, 0A0h, 0C9h, 69h, 72h, 3Bh
struc EFI_LOADED_IMAGE_PROTOCOL {
	.Revision		int32
	.ParentHandle		int64
	.SystemTable		dptr
	.DeviceHandle		int64
	.FilePath		dptr
	.Reserved		int64
	.LoadOptionsSize	int32
	.ImageBase		dptr
	.ImageSize		int64
	.ImageCodeType		int32
	.ImageDataType		int32
	.UnLoad			dptr
}
struct EFI_LOADED_IMAGE_PROTOCOL

EFI_BLOCK_IO_PROTOCOL_GUID equ 21h, 5bh, 4eh, 96h, 59h, 64h, 0d2h, 11h, 8eh, 39h, 00h, 0a0h, 0c9h, 69h, 72h, 3bh
struc EFI_BLOCK_IO_PROTOCOL {
	.Revision		int64
	.Media			dptr
	.Reset			dptr
	.ReadBlocks		dptr
	.WriteBlocks		dptr
	.FlushBlocks		dptr
}
struct EFI_BLOCK_IO_PROTOCOL

struc EFI_BLOCK_IO_MEDIA {
	.MediaId		int32
	.RemovableMedia		int8
	.MediaPresent		int8
	.LogicalPartition	int8
	.ReadOnly		int8
	.WriteCaching		int8
	.BlockSize		int32
	.IoAlign		int32
	.LastBlock		int64
}
struct EFI_BLOCK_IO_MEDIA

EFI_SIMPLE_POINTER_PROTOCOL_GUID equ 0x87, 0x8c, 0x87, 0x31, 0x75, 0x0b, 0xd5, 0x11, 0x9a, 0x4f, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d
struc EFI_SIMPLE_POINTER_PROTOCOL {
	.Reset			dptr
	.GetState		dptr
	.WaitForInput		dptr
	.Mode			dptr
}
struct EFI_SIMPLE_POINTER_PROTOCOL

struc EFI_SIMPLE_POINTER_MODE {
	.ResolutionX		int64
	.ResolutionY		int64
	.ResolutionZ		int64
	.LeftButton		int8
	.RightButton		int8
}
struct EFI_SIMPLE_POINTER_MODE

struc EFI_SIMPLE_POINTER_STATE {
	.RelativeMovementX	int32
	.RelativeMovementY	int32
	.RelativeMovementZ	int32
	.LeftButton		int8
	.RightButton		int8
}
struct EFI_SIMPLE_POINTER_STATE

EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID equ 0xde, 0xa9, 0x42, 0x90, 0xdc, 0x23, 0x38, 0x4a, 0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a
struc EFI_GRAPHICS_OUTPUT_PROTOCOL {
	.QueryMode		dptr
	.SetMode		dptr
	.Blt			dptr
	.Mode			dptr
}
struct EFI_GRAPHICS_OUTPUT_PROTOCOL

struc EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
	.MaxMode		int32
	.CurrentMode		int32
	.ModeInfo		dptr
	.SizeOfModeInfo		intn
	.FrameBufferBase	dptr
	.FrameBufferSize	intn
}
struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE

struc EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
	.Version		int32
	.HorizontalResolution	int32
	.VerticalResolution	int32
	.PixelFormat		int32
	.RedMask		int32
	.GreenMask		int32
	.BlueMask		int32
	.Reserved		int32
	.PixelsPerScanline	int32
}
struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION

enum EFI_GRAPHICS_OUTPUT_BLT_OPERATION, \
	EfiBltVideoFill, \
	EfiBltVideoToBltBuffer, \
	EfiBltBufferToVideo, \
	EfiBltVideoToVideo, \
	EfiGraphicsOutputBltOperationMax

TPL_APPLICATION				= 4
TPL_CALLBACK				= 8
TPL_NOTIFY				= 16
TPL_HIGH_LEVEL				= 3

EVT_TIMER				= 0x80000000
EVT_RUNTIME				= 0x40000000
EVT_NOTIFY_WAIT				= 0x00000100
EVT_NOTIFY_SIGNAL			= 0x00000200
EVT_SIGNAL_EXIT_BOOT_SERVICES		= 0x00000201
EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE	= 0x60000202

EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL	= 0x00000001
EFI_OPEN_PROTOCOL_GET_PROTOCOL		= 0x00000002
EFI_OPEN_PROTOCOL_TEST_PROTOCOL		= 0x00000004
EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER	= 0x00000008
EFI_OPEN_PROTOCOL_BY_DRIVER		= 0x00000010
EFI_OPEN_PROTOCOL_EXCLUSIVE		= 0x00000020


EFI_TCP4_SERVICE_BINDING_PROTOCOL_GUID equ 0x65, 0x06, 0x72, 0x00, 0xEB, 0x67, 0x99, 0x4A, 0xBA, 0xF7, 0xD3, 0xC3, 0x3A, 0x1C, 0x7C, 0xC9
struc EFI_SERVICE_BINDING_PROTOCOL {
	.CreateChild		dptr
	.DestroyChild		dptr
}
struct EFI_SERVICE_BINDING_PROTOCOL


EFI_TCP4_PROTOCOL_GUID equ 0xC7, 0x0B, 0x53, 0x65, 0x59, 0xA3, 0x0F, 0x41, 0xB0, 0x10, 0x5A, 0xAD, 0xC7, 0xEC, 0x2B, 0x62
struc EFI_TCP4_PROTOCOL {
	.GetModeData		dptr
	.Configure		dptr
	.Routes			dptr
	.Connect		dptr
	.Accept			dptr
	.Transmit		dptr
	.Receive		dptr
	.Close			dptr
	.Cancel			dptr
	.Poll			dptr
}
struct EFI_TCP4_PROTOCOL


EFI_HII_IMAGE_PROTOCOL_GUID equ 0x6A, 0x40, 0xA6, 0x31, 0xDF, 0x6B, 0x46, 0x4E, 0xB2, 0xA2, 0xEB, 0xAA, 0x89, 0xC4, 0x9, 0x20
struc EFI_HII_IMAGE_PROTOCOL {
	.NewImage		dptr
	.GetImage		dptr
	.SetImage		dptr
	.DrawImage		dptr
	.DrawImageId		dptr
}
struct EFI_HII_IMAGE_PROTOCOL


include 'struct.inc'

struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL
	Blue			db	?
	Green			db	?
	Red			db	?
	Reserved		db	?
ends


struct EFI_IMAGE_INPUT
	Flags			int32
	Width			int16
	Height			int16
	Bitmap			dptr
ends

struct EFI_IMAGE_OUTPUT
	Width			int16
	Height			int16
	Reserved		int32
	Image			dptr
ends


struct EFI_TCP4_COMPLETION_TOKEN
	Event			dq	?
	Status			dq	?
ends

struct EFI_TCP4_CONNECTION_TOKEN
	CompletionToken		EFI_TCP4_COMPLETION_TOKEN	?
ends

struct EFI_TCP4_LISTEN_TOKEN
	CompletionToken		EFI_TCP4_COMPLETION_TOKEN	?
	NewChildHandle		dq	?
ends

struct EFI_TCP4_IO_TOKEN
	CompletionToken		EFI_TCP4_COMPLETION_TOKEN	?
	Packet			dq	?
ends

struct EFI_TCP4_RECEIVE_DATA
	UrgentFlag		int8
	DataLength		int32
	FragmentCount		int32
	FragmentTable		dptr
ends

struct EFI_TCP4_TRANSMIT_DATA
	Push			int8
	Urgent			int8
	DataLength		int32
	FragmentCount		int32
	FragmentTable		dptr
ends

struct EFI_TCP4_FRAGMENT_DATA
	FragmentLength		int32
	FragmentBuffer		dptr
ends

struct EFI_TCP4_CLOSE_TOKEN
	CompletionToken		EFI_TCP4_COMPLETION_TOKEN
	AbortOnClose		int8
ends



; ------------------------------ macros ------------------------------

; call it early, after entry point is the best
macro u_init {
	clc
	test	rdx, rdx
	jz	.badout
	cmp	dword [rdx], 20494249h
	je	@f
	.badout:
		xor	ecx, ecx
		xor	edx, edx
		stc
	@@:
		mov	[ImageHandle], rcx		; ImageHandle
		mov	[SystemTable], rdx    		; pointer to SystemTable
}

macro u_lp guid, interface {
	mov	qword [tmpHandleNum], 400
	u_call	BootServices, LocateHandle, 2, guid, 0, tmpHandleNum, tmpHandle
	test	rax, rax
	jz	@f
		u_call	ConOut, OutputString, ConOut, _error_LocateHandle
		lkp: jmp lkp
@@:	xor	ecx, ecx
	loop_handles:
		push	rcx
		imul	rax, rcx, 8
		add	rax, tmpHandle
		u_call	BootServices, HandleProtocol, [rax], guid, interface
		pop	rcx
		test	rax, rax
		jz	@f
		inc	rcx
		cmp	rcx, [tmpHandleNum]
		jl	loop_handles
@@:	
}
 
; invoke an UEFI function
macro u_call interface, function, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11 {
	numarg = 0

	if ~ arg11 eq
		numarg = numarg + 1
		if ~ arg11 eq rdi
			mov	rdi, arg11
		end if
	end if

	if ~ arg10 eq
		numarg = numarg + 1
		if ~ arg10 eq rsi
			mov	rsi, arg10
		end if
	end if

	if ~ arg9 eq
		numarg = numarg + 1
		if ~ arg9 eq r14
			mov	r14, arg9
		end if
	end if

	if ~ arg8 eq
		numarg = numarg + 1
		if ~ arg8 eq r13
			mov	r13, arg8
		end if
	end if

	if ~ arg7 eq
		numarg = numarg + 1
		if ~ arg7 eq r12
			mov	r12, arg7
		end if
	end if

	if ~ arg6 eq
		numarg = numarg + 1
		if ~ arg6 eq r11
			mov	r11, arg6
		end if
	end if

	if ~ arg5 eq
		numarg = numarg + 1
		if ~ arg5 eq r10
			mov	r10, arg5
		end if
	end if

	if ~ arg4 eq
		numarg = numarg + 1
		if ~ arg4 eq r9
			mov	r9, arg4
		end if
	end if

	if ~ arg3 eq
		numarg = numarg + 1
		if ~ arg3 eq r8
			mov	r8, arg3
		end if
	end if

	if ~ arg2 eq
		numarg = numarg + 1
		if ~ arg2 eq rdx
			mov	rdx, arg2
		end if
	end if

	if ~ arg1 eq
		numarg = numarg + 1
		if ~ arg1 eq rcx
			if ~ arg1 in <ConsoleInHandle, ConIn, ConsoleOutHandle, ConOut, StandardErrorHandle, StdErr, RuntimeServices, BootServices>
				mov	rcx, arg1
			end if
		end if
	end if

	xor	eax, eax
	mov	al, numarg

	if interface in <ConsoleInHandle, ConIn, ConsoleOutHandle, ConOut, StandardErrorHandle, StdErr, RuntimeServices, BootServices>
		mov	rbx, [SystemTable]
		mov	rbx, [rbx + EFI_SYSTEM_TABLE.#interface]
	else
		if ~ interface eq rbx
			mov	rbx, interface
		end if
	end if

	if arg1 in <ConsoleInHandle, ConIn, ConsoleOutHandle, ConOut, StandardErrorHandle, StdErr, RuntimeServices, BootServices>
		mov	rcx, rbx
	end if

	if defined SIMPLE_INPUT_INTERFACE.#function
		mov	rbx, [rbx + SIMPLE_INPUT_INTERFACE.#function]
	else
		if defined SIMPLE_TEXT_OUTPUT_INTERFACE.#function
			mov	rbx, [rbx + SIMPLE_TEXT_OUTPUT_INTERFACE.#function]
		else
			if defined EFI_BOOT_SERVICES_TABLE.#function
				mov	rbx, [rbx + EFI_BOOT_SERVICES_TABLE.#function]
			else
				if defined EFI_RUNTIME_SERVICES_TABLE.#function
					mov	rbx, [rbx + EFI_RUNTIME_SERVICES_TABLE.#function]
				else
					if defined EFI_GRAPHICS_OUTPUT_PROTOCOL.#function
						mov	rbx, [rbx + EFI_GRAPHICS_OUTPUT_PROTOCOL.#function]
					else
						if defined EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.#function
							mov	rbx, [rbx + EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE.#function]
						else
							mov	rbx, [rbx + function]
						end if
					end if
				end if
			end if
		end if
	end if
	
	call	uefifunc
}

macro e_call function, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11 {
	push	rbx
	push	rbp
	push	rdi
	push	rsi
	push	r12
	push	r13
	push	r14
	push	r15
	numarg = 0

	if ~ arg11 eq
		numarg = numarg + 1
		if ~ arg11 eq rdi
			mov	rdi, arg11
		end if
	end if

	if ~ arg10 eq
		numarg = numarg + 1
		if ~ arg10 eq rsi
			mov	rsi, arg10
		end if
	end if

	if ~ arg9 eq
		numarg = numarg + 1
		if ~ arg9 eq r14
			mov	r14, arg9
		end if
	end if

	if ~ arg8 eq
		numarg = numarg + 1
		if ~ arg8 eq r13
			mov	r13, arg8
		end if
	end if

	if ~ arg7 eq
		numarg = numarg + 1
		if ~ arg7 eq r12
			mov	r12, arg7
		end if
	end if

	if ~ arg6 eq
		numarg = numarg + 1
		if ~ arg6 eq r11
			mov	r11, arg6
		end if
	end if

	if ~ arg5 eq
		numarg = numarg + 1
		if ~ arg5 eq r10
			mov	r10, arg5
		end if
	end if

	if ~ arg4 eq
		numarg = numarg + 1
		if ~ arg4 eq r9
			mov	r9, arg4
		end if
	end if

	if ~ arg3 eq
		numarg = numarg + 1
		if ~ arg3 eq r8
			mov	r8, arg3
		end if
	end if

	if ~ arg2 eq
		numarg = numarg + 1
		if ~ arg2 eq rdx
			mov	rdx, arg2
		end if
	end if

	if ~ arg1 eq
		numarg = numarg + 1
		if ~ arg1 eq rcx
			mov	rcx, arg1
		end if
	end if

	xor	eax, eax
	mov	al, numarg

	if ~ function eq rbx
		mov	rbx, function
	end if
	
	call	uefifunc

	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	rsi
	pop	rdi
	pop	rbp
	pop	rbx
}
 
;*********************************************************************
;*                       Library functions                           *
;*********************************************************************
 
section '.text' code executable readable
 
uefifunc:
	mov	qword [uefi_rsptmp], rsp	; save stack pointer
	and	esp, 0FFFFFFF0h			; set up new aligned stack
	bt	eax, 0				; alignment check on arguments
	jnc	@f
	push	rax

	; arguments
@@:	cmp	al, 11
	jb	@f
	push	rdi
@@:	cmp	al, 10
	jb	@f
	push	rsi
@@:	cmp	al, 9
	jb	@f
	push	r14
@@:	cmp	al, 8
	jb	@f
	push	r13
@@:	cmp	al, 7
	jb	@f
	push	r12
@@:	cmp	al, 6
	jb	@f
	push	r11
@@:	cmp	al, 5
	jb	@f
	push	r10
@@:	;sub	rsp, 4 * 8			; space for r9 r8 rdx rcx
	push	r9
	push	r8
	push	rdx
	push	rcx
	call	rbx				; call function
	mov	rsp, qword [uefi_rsptmp]	; restore old stack
	ret
 
section '.data' data readable writeable

ImageHandle:	dq	0
SystemTable:	dq	0
uefi_rsptmp:	dq	0

tmpHandle:	rq	50
tmpHandleNum:	dq	400






_error_LocateHandle:	du	'Error in LocateHandle', 13, 10, 0

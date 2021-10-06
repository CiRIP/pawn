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

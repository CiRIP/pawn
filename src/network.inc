section '.text' code executable readable


setup_tcp4:
	push	rbp
	mov	rbp, rsp

	mov	qword [tsbHandleNum], 400
	u_call	BootServices, LocateHandle, 2, tsbGuid, 0, tsbHandleNum, tsbHandles
	error	_dbg_tsb_lh

	u_call	BootServices, OpenProtocol, [tsbHandles], tsbGuid, tsb, [ImageHandle], 0, EFI_OPEN_PROTOCOL_GET_PROTOCOL
	error	_dbg_tsb_op

	u_call	[tsb], EFI_SERVICE_BINDING_PROTOCOL.CreateChild, [tsb], tcpHandle
	error	_dbg_tsb_cc

	u_call	BootServices, OpenProtocol, [tcpHandle], tcpGuid, tcpListener, [ImageHandle], 0, EFI_OPEN_PROTOCOL_GET_PROTOCOL
	error	_dbg_tsb_op_l

	u_call	BootServices, CreateEvent, EVT_NOTIFY_SIGNAL, TPL_CALLBACK, 

	mov	rsp, rbp
	pop	rbp

	ret


; --------------------------------------------------------------------
; Notification function for a connection accepted event
;
; Args:
;	rcx:	event which triggered this notification function
;	rdx:	pointer to the event context; in this case, the token
; --------------------------------------------------------------------
connection_accepted:
	push	rbp
	mov	rbp, rsp

	mov	r15, rdx		; ListenToken
	mov	rax, [r15 + EFI_TCP4_LISTEN_TOKEN.CompletionToken + EFI_TCP4_COMPLETION_TOKEN.Status]
	error	_dbg_ca_ce

	u_call	BootServices, OpenProtocol, [r15 + EFI_TCP4_LISTEN_TOKEN.NewChildHandle], tcpGuid, tcpConnection, [ImageHandle], 0, EFI_OPEN_PROTOCOL_GET_PROTOCOL
	error	_dbg_ca_open

	lea	rax, [ReceiveToken + EFI_TCP4_IO_TOKEN.CompletionToken + EFI_TCP4_COMPLETION_TOKEN.Event]
	u_call	BootServices, CreateEvent, EVT_NOTIFY_SIGNAL, TPL_NOTIFY, acknowledge_completion, RxDone, rax

	mov	rsp, rbp
	pop	rbp

	ret


; --------------------------------------------------------------------
; Notification function to acknowledge a completion of an action
;
; Args:
;	rcx:	event which triggered this notification function
;	rdx:	pointer to the event context; in this case, the flag
; --------------------------------------------------------------------
acknowledge_completion:
	test	rcx, rcx
	jz	@f
	test	rdx, rdx
	jz	@f

	mov	byte [rdx], 1

@@:	ret


section '.data' data readable writeable

_dbg_tsb_lh	du	'BootServices->LocateHandle() for EFI_TCP4_SERVICE_BINDING_PROTOCOL', 0
_dbg_tsb_op	du	'BootServices->OpenProtocol() for EFI_TCP4_SERVICE_BINDING_PROTOCOL', 0
_dbg_tsb_cc	du	'tsb->CreateChild()', 0
_dbg_tsb_op_l	du	'BootServices->OpenProtocol() for EFI_TCP4_PROTOCOL', 0

_dbg_ca_ce	du	'TCP: Connection status', 0
_dbg_ca_open	du	'TCP: Connection open'


tsbGuid		db	EFI_TCP4_SERVICE_BINDING_PROTOCOL_GUID
tsb		rq	1
tsbHandleNum	rq	1
tsbHandles	rq	400

tcpGuid		db	EFI_TCP4_PROTOCOL_GUID
tcpHandle	rq	1
tcpListener	rq	1
tcpConnection	rq	1

RxData		EFI_TCP4_RECEIVE_DATA	0, 0, 1, 0
TxData		EFI_TCP4_TRANSMIT_DATA	1, 0, 1, 0

RxDone		db	0
TxDone		db	0

ReceiveToken	EFI_TCP4_IO_TOKEN	0, RxData
TransmitToken	EFI_TCP4_IO_TOKEN	0, TxData
ListenToken	EFI_TCP4_LISTEN_TOKEN
CloseToken	EFI_TCP4_CLOSE_TOKEN

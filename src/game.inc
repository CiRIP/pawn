include 'struct.inc'

struc times [arg] {
common
	match n def, arg \{
		. def
		rept n-1 i:1 \\{ .\\#_\\#i def \\}
	\}
}

struct BOARD_TILE
	piece	db	?
	color	db	?
ends

section '.data' data readable writeable

board		times 64	BOARD_TILE	?

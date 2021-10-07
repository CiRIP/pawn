include 'struct.inc'

struc times [arg] {
common
	match n def, arg \{
		. def
		rept n-1 i:1 \\{ .\\#_\\#i def \\}
	\}
}

macro enum var, [stuff] {
	common
		local x
		x = 0
		struc var \{. dd ?\}
	forward
		stuff = x
		x = x + 1
}

struct BOARD_PIECE
	piece	db	?
	color	db	?
ends

PIECE.Checker = 0
PIECE.Test = 16

COLOR.Red = 0
COLOR.Black = 8

section '.data' data readable writeable

smile				file	'./assets/smile.bgra'
tileDark			file	'./assets/tile_dark.bgra'
tileLight			file	'./assets/tile_light.bgra'

checkerRed			file	'./assets/checker_red.bgra'
checkerBlack			file	'./assets/checker_black.bgra'


SPRITES:
	dq	checkerRed
	dq	checkerBlack
	dq	checkerRed
	dq	checkerBlack

board:
	db	4 dup (-1, PIECE.Checker + COLOR.Red)
	db	4 dup (PIECE.Checker + COLOR.Red, -1)
	db	4 dup (-1, PIECE.Checker + COLOR.Red)
	db	16 dup -1
	db	4 dup (PIECE.Checker + COLOR.Black, -1)
	db	4 dup (-1, PIECE.Checker + COLOR.Black)
	db	4 dup (PIECE.Checker + COLOR.Black, -1)

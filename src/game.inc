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

PIECE.Pawn = 0
PIECE.Bishop = 16
PIECE.Knight = 32
PIECE.Rook = 48
PIECE.Queen = 64
PIECE.King = 80

COLOR.White = 0
COLOR.Black = 8

section '.data' data readable writeable
align 16

title				file	'./assets/title.bgra'
titleImage			EFI_IMAGE_INPUT 0x1, 512, 256, title

turnWhite			file	'./assets/turn_white.bgra'
turnWhiteImage			EFI_IMAGE_INPUT	0x1, 256, 64, turnWhite
turnBlack			file	'./assets/turn_black.bgra'
turnBlackImage			EFI_IMAGE_INPUT	0x1, 256, 64, turnBlack

smile				file	'./assets/smile.bgra'
tileDark			file	'./assets/tile_dark.bgra'
tileDarkImage			EFI_IMAGE_INPUT	0x0, 32, 32, tileDark
tileLight			file	'./assets/tile_light.bgra'
tileLightImage			EFI_IMAGE_INPUT	0x0, 32, 32, tileLight

tileDarkSelected		file	'./assets/tile_dark_selected.bgra'
tileDarkSelectedImage		EFI_IMAGE_INPUT	0x0, 32, 32, tileDarkSelected
tileLightSelected		file	'./assets/tile_light_selected.bgra'
tileLightSelectedImage		EFI_IMAGE_INPUT	0x0, 32, 32, tileLightSelected

tileActive			file	'./assets/tile_active.bgra'
tileActiveImage			EFI_IMAGE_INPUT	0x1, 32, 32, tileActive

checkerRed			file	'./assets/checker_red.bgra'
checkerBlack			file	'./assets/checker_black.bgra'

pawnWhite			file	'./assets/pawn_white.bgra'
pawnWhiteImage			EFI_IMAGE_INPUT	0x1, 32, 32, pawnWhite
pawnBlack			file	'./assets/pawn_black.bgra'
pawnBlackImage			EFI_IMAGE_INPUT	0x1, 32, 32, pawnBlack

bishopWhite			file	'./assets/bishop_white.bgra'
bishopWhiteImage		EFI_IMAGE_INPUT	0x1, 32, 32, bishopWhite
bishopBlack			file	'./assets/bishop_black.bgra'
bishopBlackImage		EFI_IMAGE_INPUT	0x1, 32, 32, bishopBlack

knightWhite			file	'./assets/knight_white.bgra'
knightWhiteImage		EFI_IMAGE_INPUT	0x1, 32, 32, knightWhite
knightBlack			file	'./assets/knight_black.bgra'
knightBlackImage		EFI_IMAGE_INPUT	0x1, 32, 32, knightBlack

rookWhite			file	'./assets/rook_white.bgra'
rookWhiteImage			EFI_IMAGE_INPUT	0x1, 32, 32, rookWhite
rookBlack			file	'./assets/rook_black.bgra'
rookBlackImage			EFI_IMAGE_INPUT	0x1, 32, 32, rookBlack

queenWhite			file	'./assets/queen_white.bgra'
queenWhiteImage			EFI_IMAGE_INPUT	0x1, 32, 32, queenWhite
queenBlack			file	'./assets/queen_black.bgra'
queenBlackImage			EFI_IMAGE_INPUT	0x1, 32, 32, queenBlack

kingWhite			file	'./assets/king_white.bgra'
kingWhiteImage			EFI_IMAGE_INPUT	0x1, 32, 32, kingWhite
kingBlack			file	'./assets/king_black.bgra'
kingBlackImage			EFI_IMAGE_INPUT	0x1, 32, 32, kingBlack


SPRITES:
	dq	pawnWhiteImage
	dq	pawnBlackImage

	dq	bishopWhiteImage
	dq	bishopBlackImage

	dq	knightWhiteImage
	dq	knightBlackImage

	dq	rookWhiteImage
	dq	rookBlackImage

	dq	queenWhiteImage
	dq	queenBlackImage

	dq	kingWhiteImage
	dq	kingBlackImage

board:
	db	PIECE.Rook + COLOR.Black
	db	PIECE.Knight + COLOR.Black
	db	PIECE.Bishop + COLOR.Black
	db	PIECE.Queen + COLOR.Black
	db	PIECE.King + COLOR.Black
	db	PIECE.Bishop + COLOR.Black
	db	PIECE.Knight + COLOR.Black
	db	PIECE.Rook + COLOR.Black
	db	8 dup (PIECE.Pawn + COLOR.Black)
	db	32 dup -1
	db	8 dup (PIECE.Pawn + COLOR.White)
	db	PIECE.Rook + COLOR.White
	db	PIECE.Knight + COLOR.White
	db	PIECE.Bishop + COLOR.White
	db	PIECE.Queen + COLOR.White
	db	PIECE.King + COLOR.White
	db	PIECE.Bishop + COLOR.White
	db	PIECE.Knight + COLOR.White
	db	PIECE.Rook + COLOR.White

currentTurn			dq	COLOR.White

TURNS:
	dq	turnWhiteImage
	dq	turnBlackImage

selectedTileX			dq	7
selectedTileY			dq	7
activeTileX			dq	-1
activeTileY			dq	-1


#include "pch.h"
#include "Queen.h"

namespace chessy
{
	Queen::Queen(EColor color) :
		ChessPiece(color, EChessPieceType::Queen),
		Rook(color),
		Bishop(color)
	{
	}
}
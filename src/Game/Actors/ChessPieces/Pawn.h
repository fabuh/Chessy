#pragma once
#include "ChessPiece.h"

namespace chessy
{
	class Pawn :
		public ChessPiece
	{
	public:
		Pawn(EColor color);
	};
}

#pragma once
#include "ChessPiece.h"

namespace chessy
{
	class Rook :
		virtual public ChessPiece
	{
	public:
		Rook(EColor color);
	};
}

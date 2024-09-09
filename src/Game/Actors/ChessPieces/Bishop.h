#pragma once
#include "ChessPiece.h"

namespace chessy
{
	class Bishop :
		virtual public ChessPiece
	{
	public:
		Bishop(EColor color);
	};
}


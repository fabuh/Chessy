#pragma once
#include "Rook.h"
#include "Bishop.h"

namespace chessy
{
	class Queen :
		public Rook,
		public Bishop
	{
	public:
		Queen(EColor color);
	};
}

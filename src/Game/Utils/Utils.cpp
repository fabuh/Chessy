#include "pch.h"
#include "Utils.h"

namespace chessy
{
	EDirection GetOpositeDirection(EDirection direction)
	{
		switch (direction)
		{
		case chessy::ForwardLeft:
			return BackwardRight;
		case chessy::Forward:
			return Backward;
		case chessy::ForwardRight:
			return BackwardLeft;
		case chessy::Right:
			return Left;
		case chessy::BackwardRight:
			return ForwardLeft;
		case chessy::Backward:
			return Forward;
		case chessy::BackwardLeft:
			return ForwardRight;
		case chessy::Left:
			return Right;
		default:
			return NoDirection;
		}
	}
}
#include "pch.h"
#include "Bishop.h"

namespace chessy
{
	Bishop::Bishop(EColor color)
		:ChessPiece(color, EChessPieceType::Bishop)
	{
		m_chessMoves.emplace_back(EDirection::ForwardLeft, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::ForwardRight, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::BackwardLeft, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::BackwardRight, EMoveType::Multiple);
	}
}
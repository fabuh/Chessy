#include "pch.h"
#include "Rook.h"

namespace chessy
{
	Rook::Rook(EColor color)
		:ChessPiece(color, EChessPieceType::Rook)
	{
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::Backward, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::Left, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::Right, EMoveType::Multiple);
	}
}
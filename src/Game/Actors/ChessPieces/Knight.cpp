#include "pch.h"
#include "Knight.h"

namespace chessy
{
	Knight::Knight(EColor color)
		:ChessPiece(color, EChessPieceType::Knight)
	{
		m_chessMoves.emplace_back(EDirection::ForwardLeft, EMoveType::KnightMove);
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::KnightMove);
		m_chessMoves.emplace_back(EDirection::ForwardRight, EMoveType::KnightMove);
		m_chessMoves.emplace_back(EDirection::Right, EMoveType::KnightMove);
		m_chessMoves.emplace_back(EDirection::BackwardRight, EMoveType::KnightMove);
		m_chessMoves.emplace_back(EDirection::Backward, EMoveType::KnightMove);
		m_chessMoves.emplace_back(EDirection::BackwardLeft, EMoveType::KnightMove);
		m_chessMoves.emplace_back(EDirection::Left, EMoveType::KnightMove);
	}
}
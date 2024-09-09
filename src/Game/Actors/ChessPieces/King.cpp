#include "pch.h"
#include "King.h"

namespace chessy
{
	King::King(EColor color)
		:ChessPiece(color, EChessPieceType::King)
	{
		m_chessMoves.emplace_back(EDirection::ForwardLeft, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::ForwardRight, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Right, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::BackwardRight, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Backward, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::BackwardLeft, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Left, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Right, EMoveType::Castle);
		m_chessMoves.emplace_back(EDirection::Left, EMoveType::Castle);
	}
}

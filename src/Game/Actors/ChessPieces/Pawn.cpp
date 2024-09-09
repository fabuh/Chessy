#include "pch.h"
#include "Pawn.h"

namespace chessy
{
	Pawn::Pawn(EColor color)
		:ChessPiece(color, EChessPieceType::Pawn)
	{
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::PawnMove);
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::EnPassant);
		m_chessMoves.emplace_back(EDirection::ForwardLeft, EMoveType::PawnHit);
		m_chessMoves.emplace_back(EDirection::ForwardRight, EMoveType::PawnHit);
	}
}
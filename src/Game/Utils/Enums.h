#pragma once

namespace chessy
{
	enum EDirection
	{
		ForwardLeft,
		Forward,
		ForwardRight,
		Right,
		BackwardRight,
		Backward,
		BackwardLeft,
		Left,
		NoDirection
	};

	enum EMoveType
	{
		PawnMove,
		EnPassant,
		PawnHit,
		KnightMove,
		KingMove,
		Castle,
		Multiple,
		Error
	};

	enum EColor
	{
		White,
		Black,
		Colorless
	};

	enum class EChessPieceType
	{
		Pawn = 1,
		Knight,
		Bishop,
		Rook,
		Queen,
		King,
		Error
	};

	enum class EGameState
	{
		Initialize,
		Gameplay,
		Promote,
		Check,
		WhiteWin,
		BlackWin,
		Draw
	};
}
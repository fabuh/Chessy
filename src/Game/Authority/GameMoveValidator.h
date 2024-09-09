#pragma once

namespace chessy
{
	class GameState;
	class GameMoveCalculator;

	class GameMoveValidator
	{
	public:
		GameMoveValidator(std::shared_ptr<GameState> state, std::shared_ptr<GameMoveCalculator> calculator);
		~GameMoveValidator();

		bool CanMove(const EColor player, const Position& position) const;
		bool IsPossibleMove(const Position& selected, const Position& position) const;
		bool HasPossibleMoves(const Position& position) const;
		bool HasPossibleMoves(const EColor color) const;

	private:
		bool IsPossibleKing(const Position& position, const GameMove& move) const;//king
		bool IsPossiblePawn(const Position& position, const GameMove& move) const;//pawn
		bool IsPossibleChessPiece(const Position& position, const GameMove& move) const;//bishop, rook, queen, knight

		bool CastleCheck(const Position& position, const GameMove& move) const;

		bool CanMoveInCheck(const Position& position) const;

	private:
		std::shared_ptr<GameState> m_state;
		std::shared_ptr<GameMoveCalculator> m_calculator;
	};
}
#pragma once

namespace chessy
{
	class GameState;

	class GameMoveCalculator
	{
	public:
		GameMoveCalculator(std::shared_ptr<GameState> state);
		~GameMoveCalculator();

		void Initialize();

		void CalculateGameMoves(const Position& position);
		void ClearGameMoves(const Position& position);
		void RecalculateBlocked(const Position& position);

		const std::vector<Position> GetPath(const Position& position, const ChessPieceMove& chessPieceMove) const;

	private:
		//Calculate GameMode Moves
		void AddGameMoves(const Position& position, const ChessPieceMove& chessPieceMove);
		Position CalculateModifier(const ChessPieceMove& direction, EColor color) const;

	private:
		std::shared_ptr<GameState> m_state;
	};
}
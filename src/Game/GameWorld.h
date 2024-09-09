#pragma once

namespace chessy
{
	class Chessboard;
	class GameView;

	class GameWorld
	{
	public:
		GameWorld();
		~GameWorld();

		void Initialize(EColor friendly);

		void MoveChessPiece(const Position& from, const Position& to);
		void RemoveChessPiece(const Position& position);
		void PromoteChessPiece(const Position& position, EChessPieceType type, EColor color);

		const Chessboard& GetChessboard() const;
		const Position& GetKing(EColor color) const { return m_kings[color]; }
		int64_t GetChessPieceCount() const { return m_chessPieceCount; }

	private:
		std::shared_ptr<Chessboard> m_chessboard;
		std::array<Position, 2> m_kings;
		int64_t m_chessPieceCount = 32;
	};
}
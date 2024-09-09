#pragma once

namespace chessy
{
	class Position;
	struct EnPassantCache;

	struct GameMove;
	typedef std::list<GameMove> GameMoves;

	struct TileGameMoves
	{
		GameMoves hits;//incomming chess piece moves
		GameMoves moves;//chess piece out moves
	};

	typedef std::array<TileGameMoves, sChessboardSize> ChessboardState;

	class GameState
	{
	public:
		GameState();
		~GameState();

		void AddHit(const Position& position, const GameMove& hit);
		void RemoveHit(const Position& position, const GameMove& hit);
		const GameMoves& GetHits(const Position& position) const;

		void AddMove(const Position& position, const GameMove& move);
		void ClearMoves(const Position& position);
		const GameMoves& GetMoves(const Position& position) const;

		std::optional<Position>& GetPromotionCache();
		std::vector<GameMove>& GetCheckCache();
		std::optional<EnPassantCache>& GetEnPassantCache();

	private:
		ChessboardState m_chessboard;
		//SpecialMovesCaches m_caches;
		std::optional<Position> m_promotionCache;
		std::vector<GameMove> m_checkCache;
		std::optional<EnPassantCache> m_enPassantCache;
	};
}


#pragma once

#include "ChessboardTile.h"

namespace chessy
{
	typedef std::array<ChessboardTile, sChessboardSize> ChessboardTiles;

	class Chessboard
	{
	public:
		Chessboard();
		~Chessboard() = default;

		void Initialize(EColor friendly);
		void InitializePiece(EChessPieceType type, EColor color, const Position& position);

		const ChessboardTile& GetTile(const Position& position) const;
		ChessboardTile& GetMutableTile(const Position& position);

		const ChessboardTiles& GetTiles() const;

		const int8_t GetSideModifier(EColor color) const;

	private:
		static ChessPiece* CreateChessPiece(EChessPieceType type, EColor color);

	private:
		EColor m_friendly = EColor::Colorless;
		ChessboardTiles m_board;
	};
}
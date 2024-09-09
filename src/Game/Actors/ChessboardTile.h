#pragma once

#include "ChessPieces/ChessPiece.h"

namespace chessy
{
	class ChessboardTile
	{
	public:
		ChessboardTile() = default;
		~ChessboardTile() = default;

		void Initialize(const EColor color);

		const EColor GetColor() const;

		void SetChessPiece(ChessPiece&& chessPiece);
		void ClearChessPiece();
		const std::optional<ChessPiece>& GetChessPiece() const;
		std::optional<ChessPiece>& GetMutableChessPiece();

	private:
		EColor m_color = EColor::Colorless;
		std::optional<ChessPiece> m_chessPiece = std::nullopt;
	};
}

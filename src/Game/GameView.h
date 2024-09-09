#pragma once

namespace chessy
{
	struct RenderTile
	{
		EColor tileColor = Colorless;
		EColor pieceColor = Colorless;
		EChessPieceType pieceType = EChessPieceType::Error;
		EMoveType moveType = Error;
		bool selected = false;
	};
	typedef std::array<RenderTile, sChessboardSize> TileView;

	class GameView
	{
	public:
		GameView() = default;

		void Fill(const Position& position, EColor tile);
		void Fill(const Position& position, EColor pc, EChessPieceType pt);
		void Fill(const Position& position, EMoveType mt);
		void Fill(const Position& selected);

		const RenderTile& Get(const Position& position) const { return m_view[position.AsIndex()]; }

	private:
		TileView m_view;
	};
}
#include "pch.h"
#include "GameView.h"

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	void GameView::Fill(const Position& position, EColor tile)
	{
		m_view[position.AsIndex()].tileColor = tile;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameView::Fill(const Position& position, EColor pc, EChessPieceType pt)
	{
		m_view[position.AsIndex()].pieceColor = pc;
		m_view[position.AsIndex()].pieceType = pt;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameView::Fill(const Position& position, EMoveType mt)
	{
		m_view[position.AsIndex()].moveType = mt;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameView::Fill(const Position& selected)
	{
		m_view[selected.AsIndex()].selected = true;
	}
}

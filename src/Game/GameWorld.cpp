#include "pch.h"
#include "GameWorld.h"

#include "Game/Game.h"
#include "Game/GameState.h"

#include "Game/Actors/Chessboard.h"

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	GameWorld::GameWorld() :
		m_chessboard(std::make_unique<Chessboard>())
	{

	}

	//////////////////////////////////////////////////////////////////////////////////////////
	GameWorld::~GameWorld()
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameWorld::Initialize(EColor friendly)
	{
		m_chessboard->Initialize(friendly);

		for (int8_t i = 0; i < sChessboardSize; ++i)
		{
			Position pos(i);
			auto& tile = GetGame().GetChessboard().GetTile(pos);
			const auto& chessPiece = tile.GetChessPiece();
			ContinueUnless(chessPiece);
			ContinueUnless(chessPiece->GetType() == EChessPieceType::King);

			m_kings[chessPiece->GetColor()] = pos;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameWorld::MoveChessPiece(const Position& from, const Position& to)
	{
		auto& piece = m_chessboard->GetMutableTile(from).GetMutableChessPiece();

		//set has moved
		piece->SetHasMoved(true);
		if (piece->GetType() == EChessPieceType::King)
		{
			m_kings[piece->GetColor()] = to;
		}

		auto& toPiece = m_chessboard->GetTile(to).GetChessPiece();
		if (toPiece)
		{
			m_chessPieceCount--;
		}

		//move
		m_chessboard->GetMutableTile(to).SetChessPiece(std::move(*piece));

		//remove from old position
		RemoveChessPiece(from);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameWorld::RemoveChessPiece(const Position& position)
	{
		m_chessboard->GetMutableTile(position).ClearChessPiece();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameWorld::PromoteChessPiece(const Position& position, EChessPieceType type, EColor color)
	{
		m_chessboard->InitializePiece(type, color, position);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const Chessboard& GameWorld::GetChessboard() const
	{
		return *m_chessboard;
	}
}
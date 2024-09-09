#include "pch.h"
#include "Game.h"

#include "Services/Input.h"

#include "Game/GameWorld.h"
#include "Game/GameMode.h"
#include "Game/GameState.h"

#include "Game/Authority/GameMoveValidator.h"
#include "Game/Actors/Chessboard.h"
#include "Game/Player/PlayerController.h"
#include "Game/GameView.h"

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	Game::Game() :
		m_world(std::make_shared<GameWorld>()),
		m_gameMode(std::make_shared<GameMode>(m_world)),
		m_controller(std::make_unique<PlayerController>(m_gameMode)),
		m_view(std::make_unique<GameView>())
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	Game::~Game()
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Game::Initialize(EColor friendly)
	{
		m_world->Initialize(friendly);
		m_gameMode->Initialize(friendly);
		FillView();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Game::Play(const Position& input)
	{
		ReturnUnless(input.IsValid());

		m_controller->Play(input);
		
		FillView();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const Chessboard& Game::GetChessboard() const
	{
		return m_world->GetChessboard();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const GameState& Game::GetGameState() const
	{
		return m_gameMode->GetGameState();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const GameView& Game::GetView() const
	{
		return *m_view;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const EColor Game::GetTurn() const
	{
		return m_gameMode->GetTurn();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const EGameState Game::GetState() const
	{
		return m_gameMode->GetState();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Game::FillView()
	{
		m_view.reset(new GameView());
		const auto& tiles = m_world->GetChessboard().GetTiles();
		for (int8_t i = 0; i < sChessboardSize; ++i)
		{
			Position pos = Position(i);
			const auto& tile = tiles[i];

			m_view->Fill(pos, tile.GetColor());

			const auto& chessPiece = tile.GetChessPiece();
			ContinueUnless(chessPiece);

			m_view->Fill(pos, (*chessPiece).GetColor(), (*chessPiece).GetType());
		}

		const auto& selected = m_controller->GetSelected();
		if (selected)
		{
			m_view->Fill(*selected);
			const auto& moves = m_gameMode->GetGameState().GetMoves(*selected);
			for (const auto& move : moves)
			{
				ContinueUnless(m_gameMode->GetValidator().IsPossibleMove(*selected, move.pos));

				if (move.move.type == EMoveType::PawnHit && tiles[move.pos.AsIndex()].GetChessPiece().has_value() == false)
				{
					m_view->Fill(move.pos, EMoveType::EnPassant);
				}
				else
				{
					m_view->Fill(move.pos, move.move.type);
				}
			}
		}

		if (m_gameMode->GetState() == EGameState::Check)
		{
			m_view->Fill(m_world->GetKing(m_gameMode->GetTurn()));
		}
	}
}
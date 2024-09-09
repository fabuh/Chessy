#include "pch.h"
#include "GameMode.h"

#include "Game/Game.h"
#include "Game/GameState.h"
#include "Game/GameWorld.h"

#include "Game/Actors/Chessboard.h"
#include "Game/Authority/GameMoveValidator.h"
#include "Game/Authority/GameMoveCalculator.h"

namespace chessy
{
	EColor GameMode::m_turn = White;
	//////////////////////////////////////////////////////////////////////////////////////////
	GameMode::GameMode(std::shared_ptr<GameWorld> world) :
		m_world(world),
		m_state(std::make_shared<GameState>()),
		m_calculator(std::make_shared<GameMoveCalculator>(m_state)),
		m_validator(std::make_unique<GameMoveValidator>(m_state, m_calculator))
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	GameMode::~GameMode()
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::Initialize(EColor friendly)
	{
		m_calculator->Initialize();
		m_matchState = EGameState::Gameplay;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::MoveChessPiece(const Position& from, const Position& to)
	{
		const auto& chessPiece = GetGame().GetChessboard().GetTile(from).GetChessPiece();
		AssertReturnUnless(chessPiece);

		//Handle En Passant
		if (chessPiece->GetType() == EChessPieceType::Pawn)
		{
			HandleEnPassant(from, to);

			if (IsPawnPromotion(to))
			{
				auto& cache = m_state->GetPromotionCache();
				cache.emplace(to);

				m_matchState = EGameState::Promote;
			}
		}

		if (chessPiece->GetType() == EChessPieceType::King)
		{
			auto increment = from.GetCol() > to.GetCol() ? -1 : 1;
			if (from.GetCol() + increment * 2 == to.GetCol())
			{
				HandleCastling(from, to);
				return;
			}
		}

		//set chess piece to new position
		MovePiece(from, to);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::PromotePawn(const Position& input)
	{
		auto& promotionCache = m_state->GetPromotionCache();
		AssertReturnUnless(promotionCache);

		auto& pawn = GetGame().GetChessboard().GetTile(*promotionCache).GetChessPiece();
		AssertReturnUnless(pawn);

		auto type = input.AsIndex() - sPawnPromoteRow * sChessboardSide;
		auto color = pawn->GetColor();

		ReturnUnless(int(EChessPieceType::Pawn) < type && type < int(EChessPieceType::King));

		m_calculator->ClearGameMoves(*promotionCache);

		m_world->PromoteChessPiece(*promotionCache, EChessPieceType(type), color);

		m_calculator->CalculateGameMoves(*promotionCache);

		promotionCache.reset();
		m_matchState = EGameState::Gameplay;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::OnEndTurn()
	{
		ReturnIf(m_matchState == EGameState::Promote);

		//change turn color
		m_turn = m_turn == White ? Black : White;

		//handle check
		const auto& kingPos = m_world->GetKing(m_turn);
		if (IsInCheck(kingPos))
		{
			m_matchState = EGameState::Check;
			CheckmateCheck(kingPos);
			return;
		}

		//cleanup for next turn
		m_state->GetCheckCache().clear();
		m_matchState = EGameState::Gameplay;

		//handle draw
		DrawCheck();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const GameState& GameMode::GetGameState() const
	{
		return *m_state;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const GameMoveValidator& GameMode::GetValidator() const
	{
		return *m_validator;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const EColor GameMode::GetTurn() const
	{
		return m_turn;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EGameState GameMode::GetState() const
	{
		return m_matchState;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::MovePiece(const Position& from, const Position& to)
	{
		//clear moves
		m_calculator->ClearGameMoves(from);
		m_calculator->ClearGameMoves(to);

		//Move
		m_world->MoveChessPiece(from, to);

		//recalculate blocked movement
		m_calculator->RecalculateBlocked(from);
		m_calculator->RecalculateBlocked(to);

		//recalculate new game moves for chessy piece
		m_calculator->CalculateGameMoves(to);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::HandleCastling(const Position& from, const Position& to)
	{
		auto increment = from.GetCol() > to.GetCol() ? -1 : 1;
		Position rookPos(from.GetRow(), increment > 0 ? 7 : 0);

		auto kingTo = Position(from.GetRow(), from.GetCol() + increment * 2);
		auto rookTo = Position(from.GetRow(), from.GetCol() + increment);

		MovePiece(from, kingTo);
		MovePiece(rookPos, rookTo);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::HandleEnPassant(const Position& from, const Position& to)
	{
		auto& cache = m_state->GetEnPassantCache();
		if (cache && cache->takePosition == to)
		{
			TakeEnPassant(cache->piecePosition);
		}
		cache.reset();

		auto rowCheck1 = from.GetRow() == to.GetRow() + 2;
		auto rowCheck2 = from.GetRow() == to.GetRow() - 2;
		if (rowCheck1 || rowCheck2)
		{
			cache.emplace(Position(rowCheck1 ? from.GetRow() - 1 : from.GetRow() + 1, from.GetCol()), to);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::TakeEnPassant(const Position& position)
	{
		//clear chess piece hits
		m_calculator->ClearGameMoves(position);

		//clear chess piece
		m_world->RemoveChessPiece(position);

		//recalculate blocked movement
		m_calculator->RecalculateBlocked(position);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMode::IsPawnPromotion(const Position& position) const
	{
		return position.GetRow() == 0 || position.GetRow() == 7;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMode::IsInCheck(const Position& kingPos)
	{
		const auto& king = GetGame().GetChessboard().GetTile(kingPos).GetChessPiece();
		AssertReturnUnless(king, false);

		for (const auto& hit : m_state->GetHits(kingPos))
		{
			auto& hitChessPiece = GetGame().GetChessboard().GetTile(hit.pos).GetChessPiece();
			ContinueUnless(hitChessPiece);
			ContinueUnless(m_validator->IsPossibleMove(hit.pos, kingPos));

			ContinueIf(hit.move.type == EMoveType::PawnMove || hit.move.type == EMoveType::EnPassant);

			ReturnIf(hitChessPiece->GetColor() != king->GetColor(), true);
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::CheckmateCheck(const Position& kingPos)
	{
		const auto& king = GetGame().GetChessboard().GetTile(kingPos).GetChessPiece();
		AssertReturnUnless(king);

		if (m_validator->HasPossibleMoves(kingPos) == false)
		{
			if (m_state->GetCheckCache().size() > 1 || m_validator->HasPossibleMoves(king->GetColor()) == false)
			{
				m_matchState = king->GetColor() == White ? EGameState::BlackWin : EGameState::WhiteWin;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMode::DrawCheck()
	{
		if (m_validator->HasPossibleMoves(m_turn) == false)
		{
			m_matchState = EGameState::Draw;
		}

		if (m_world->GetChessPieceCount() == 3)
		{
			for (int8_t i = 0; i < sChessboardSize; ++i)
			{
				Position pos(i);
				auto& tile = GetGame().GetChessboard().GetTile(pos);
				const auto& chessPiece = tile.GetChessPiece();
				ContinueUnless(chessPiece);
				ContinueIf(chessPiece->GetType() == EChessPieceType::King);

				if (chessPiece->GetType() == EChessPieceType::Knight || chessPiece->GetType() == EChessPieceType::Bishop)
				{
					m_matchState = EGameState::Draw;
					break;
				}
			}
		}
	}
}
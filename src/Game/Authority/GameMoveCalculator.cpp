#include "pch.h"
#include "GameMoveCalculator.h"

#include "Game/Game.h"
#include "Game/GameState.h"

#include "Game/Actors/Chessboard.h"

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	GameMoveCalculator::GameMoveCalculator(std::shared_ptr<GameState> state) : m_state(state)
	{

	}

	//////////////////////////////////////////////////////////////////////////////////////////
	GameMoveCalculator::~GameMoveCalculator()
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMoveCalculator::Initialize()
	{
		for (int8_t i = 0; i < sChessboardSize; ++i)
		{
			CalculateGameMoves(Position(i));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMoveCalculator::CalculateGameMoves(const Position& position)
	{
		AssertReturnUnless(position.IsValid());

		const auto& chessPiece = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		ReturnUnless(chessPiece);

		auto chessPieceType = chessPiece->GetType();
		const auto& chessPieceMoves = chessPiece->GetChessPieceMoves();
		for (const auto& chessPieceMove : chessPieceMoves)
		{
			AddGameMoves(position, chessPieceMove);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMoveCalculator::ClearGameMoves(const Position& position)
	{
		const auto& chessPieceHits = m_state->GetMoves(position);
		for (const auto& hit : chessPieceHits)
		{
			m_state->RemoveHit(hit.pos, GameMove(hit.move, position));
		}
		m_state->ClearMoves(position);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMoveCalculator::RecalculateBlocked(const Position& position)
	{
		//copy hits since they'll be deleted in the process
		const auto hits = m_state->GetHits(position);
		for (const auto& hit : hits)
		{
			ClearGameMoves(hit.pos);
			CalculateGameMoves(hit.pos);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<Position> GameMoveCalculator::GetPath(const Position& position, const ChessPieceMove& chessPieceMove) const
	{
		std::vector<Position> path;
		auto& chessPiece = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		auto modifier = CalculateModifier(chessPieceMove, chessPiece->GetColor());
		bool blocked = false;
		Position pos(position);
		do
		{
			pos += modifier;
			BreakUnless(pos.IsValid());

			path.push_back(pos);

			blocked = GetGame().GetChessboard().GetTile(pos).GetChessPiece().has_value();
		} while (!blocked && chessPieceMove.type == EMoveType::Multiple);

		return path;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void GameMoveCalculator::AddGameMoves(const Position& position, const ChessPieceMove& chessPieceMove)
	{
		auto& chessPiece = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		auto modifier = CalculateModifier(chessPieceMove, chessPiece->GetColor());
		bool blocked = false;
		Position pos(position);
		do
		{
			pos += modifier;
			BreakUnless(pos.IsValid());

			m_state->AddMove(position, GameMove(chessPieceMove, pos));
			m_state->AddHit(pos, GameMove(chessPieceMove, position));

			auto& blockCP = GetGame().GetChessboard().GetTile(pos).GetChessPiece();
			if (blockCP && blockCP->GetType() == EChessPieceType::King && chessPiece->GetColor() != blockCP->GetColor())
			{
				auto& cache = m_state->GetCheckCache();
				cache.emplace_back(GameMove(chessPieceMove, position));
			}

			blocked = blockCP.has_value();
		} while (!blocked && chessPieceMove.type == EMoveType::Multiple);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// X - Move, Y - GameMove
	// Knight			   Normal
	//	 YXY			
	//	Y X Y				YYY
	//	XXOXX				YOY
	//	Y X Y				YYY
	//	 YXY			
	//////////////////////////////////////////////////////////////////////////////////////////
	Position GameMoveCalculator::CalculateModifier(const ChessPieceMove& move, EColor color) const
	{
		Position modifier;
		switch (move.direction)
		{
		case ForwardLeft:
			modifier = move.type == EMoveType::KnightMove ? Position(2, 1) : Position(1, 1);
			break;
		case Forward:
			modifier = move.type == EMoveType::KnightMove ? Position(2, -1) : Position(1, 0);
			break;
		case ForwardRight:
			modifier = move.type == EMoveType::KnightMove ? Position(1, 2) : Position(1, -1);
			break;
		case Right:
			modifier = move.type == EMoveType::KnightMove ? Position(-1, 2) : Position(0, -1);
			break;
		case BackwardRight:
			modifier = move.type == EMoveType::KnightMove ? Position(-2, 1) : Position(-1, -1);
			break;
		case Backward:
			modifier = move.type == EMoveType::KnightMove ? Position(-2, -1) : Position(-1, 0);
			break;
		case BackwardLeft:
			modifier = move.type == EMoveType::KnightMove ? Position(-1, -2) : Position(-1, 1);
			break;
		case Left:
			modifier = move.type == EMoveType::KnightMove ? Position(1, -2) : Position(0, 1);
			break;
		}

		int sideModifier = GetGame().GetChessboard().GetSideModifier(color);
		modifier *= sideModifier;

		modifier = move.type == EMoveType::Castle || move.type == EMoveType::EnPassant ? modifier + modifier : modifier;
		return modifier;
	}
}
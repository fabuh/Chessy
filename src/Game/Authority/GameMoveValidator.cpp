#include "pch.h"
#include "GameMoveValidator.h"

#include "Game/Game.h"
#include "Game/GameState.h"

#include "Game/Actors/Chessboard.h"
#include "Game/Authority/GameMoveCalculator.h"

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	GameMoveValidator::GameMoveValidator(std::shared_ptr<GameState> state, std::shared_ptr<GameMoveCalculator> calculator) : m_state(state), m_calculator(calculator)
	{

	}

	//////////////////////////////////////////////////////////////////////////////////////////
	GameMoveValidator::~GameMoveValidator()
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::CanMove(const EColor player, const Position& position) const
	{
		AssertReturnUnless(position.IsValid(), false);//pos valid
		auto& piece = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		ReturnUnless(piece, false);//tile has piece
		ReturnIf(piece->GetColor() != player, false);//piece is player's
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::IsPossibleMove(const Position& selected, const Position& position) const
	{
		auto& fromTile = GetGame().GetChessboard().GetTile(selected);
		auto chessPieceType = fromTile.GetChessPiece()->GetType();

		const auto& hits = m_state->GetMoves(selected);
		auto it = std::find_if(hits.begin(), hits.end(), [&position](const GameMove& hit) {
			return hit.pos == position;
			});

		if (it != hits.end())
		{
			if (chessPieceType == EChessPieceType::King)
			{
				return IsPossibleKing(selected, *it);
			}

			if (chessPieceType == EChessPieceType::Pawn)
			{
				return IsPossiblePawn(selected, *it);
			}

			return IsPossibleChessPiece(selected, *it);
		}

		//unhandled error
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::HasPossibleMoves(const Position& position) const
	{
		const auto& chessPiece = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		ReturnUnless(chessPiece, false);

		for (const auto& move : m_state->GetMoves(position))
		{
			ContinueUnless(IsPossibleMove(position, move.pos));

			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::HasPossibleMoves(const EColor color) const
	{
		for (int8_t i = 0; i < sChessboardSize; ++i)
		{
			Position pos(i);
			const auto& chessPiece = GetGame().GetChessboard().GetTile(pos).GetChessPiece();
			ContinueUnless(chessPiece);

			if (chessPiece->GetColor() == color)
			{
				for (const auto& move : m_state->GetMoves(pos))
				{
					ContinueUnless(IsPossibleMove(pos, move.pos));

					return true;
				}
			}
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::IsPossibleKing(const Position& position, const GameMove& move) const
	{
		AssertReturnUnless(position.IsValid(), false);

		ReturnIf(move.move.type == Castle, CastleCheck(position, move));

		const auto& king = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		auto& posChessPiece = GetGame().GetChessboard().GetTile(move.pos).GetChessPiece();
		ReturnIf(posChessPiece && posChessPiece->GetColor() == king->GetColor(), false);

		for (const auto& hit : m_state->GetHits(move.pos))
		{
			auto& hitChessPiece = GetGame().GetChessboard().GetTile(hit.pos).GetChessPiece();
			ContinueUnless(hitChessPiece);
			ContinueIf(hit.move.type == EMoveType::PawnMove || hit.move.type == EMoveType::EnPassant);

			ReturnIf(hitChessPiece->GetColor() != king->GetColor(), false);
		}

		if (GetGame().GetState() == EGameState::Check)
		{
			for(const auto& checkMove : m_state->GetCheckCache())
			{
				if (checkMove.move.type == EMoveType::Multiple)
				{
					ReturnIf(GetOpositeDirection(checkMove.move.direction) == move.move.direction, false);
				}
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::IsPossiblePawn(const Position& position, const GameMove& hit) const
	{
		AssertReturnUnless(position.IsValid(), false);

		const auto& pawn = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		auto& posChessPiece = GetGame().GetChessboard().GetTile(hit.pos).GetChessPiece();

		const auto& cache = m_state->GetEnPassantCache();
		auto CanHitEnPassant = [&cache](const Position& position, EColor color)->bool {
			ReturnUnless(cache, false);
			ReturnUnless((cache->takePosition == position), false);

			const auto& enPassantChessPiece = GetGame().GetChessboard().GetTile(cache->piecePosition).GetChessPiece();
			ReturnUnless(enPassantChessPiece, false);

			return enPassantChessPiece->GetColor() != color;
		};

		ReturnUnless(CanMoveInCheck(hit.pos), false);

		switch (hit.move.type)
		{
		case chessy::PawnMove:
			return !posChessPiece;//move if no piece
		case chessy::EnPassant:
			{
				auto increment = position.GetRow() > hit.pos.GetRow() ? -1 : 1;
				auto& frontTilePiece = GetGame().GetChessboard().GetTile(Position(position.GetRow() + increment, position.GetCol())).GetChessPiece();
				return !posChessPiece && !frontTilePiece && !pawn->HasMoved();
			}
		case chessy::PawnHit:
			return posChessPiece && posChessPiece->GetColor() != pawn->GetColor() || CanHitEnPassant(hit.pos, pawn->GetColor());
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::IsPossibleChessPiece(const Position& position, const GameMove& hit) const
	{
		AssertReturnUnless(position.IsValid(), false);

		const auto& chessPiece = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		auto& posChessPiece = GetGame().GetChessboard().GetTile(hit.pos).GetChessPiece();

		ReturnUnless(CanMoveInCheck(hit.pos), false);

		ReturnIf(posChessPiece && posChessPiece->GetColor() == chessPiece->GetColor(), false);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::CastleCheck(const Position& position, const GameMove& hit) const
	{
		ReturnIf(GetGame().GetState() == EGameState::Check, false);

		auto increment = position.GetCol() > hit.pos.GetCol() ? -1 : 1;
		Position rookPos(position.GetRow(), increment > 0 ? 7 : 0);

		const auto& king = GetGame().GetChessboard().GetTile(position).GetChessPiece();
		const auto& rook = GetGame().GetChessboard().GetTile(rookPos).GetChessPiece();

		if (rook && king
			&& rook->GetType() == EChessPieceType::Rook
			&& rook->HasMoved() == false
			&& king->GetType() == EChessPieceType::King
			&& king->HasMoved() == false)
		{
			int8_t tilesToCheckSize = position.GetCol() - rookPos.GetCol();
			tilesToCheckSize = tilesToCheckSize > 0 ? tilesToCheckSize : -tilesToCheckSize;
			for (auto posToCheck = position.GetCol() + increment; posToCheck != rookPos.GetCol(); posToCheck += increment)
			{
				Position pos(position.GetRow(), posToCheck);
				ReturnIf(GetGame().GetChessboard().GetTile(pos).GetChessPiece(), false);

				for (const auto& hit : m_state->GetHits(pos))
				{
					const auto& hitChessPiece = GetGame().GetChessboard().GetTile(hit.pos).GetChessPiece();
					ContinueUnless(hitChessPiece);

 					ReturnIf(hit.move.type == EMoveType::PawnMove || hit.move.type == EMoveType::EnPassant, true);
					ReturnIf(hitChessPiece->GetColor() != king->GetColor(), false);
				}
			}

			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool GameMoveValidator::CanMoveInCheck(const Position& position) const
	{
		ReturnUnless(GetGame().GetState() == EGameState::Check, true);

		ReturnUnless(m_state->GetCheckCache().size() == 1, false);

		auto IsInPath = [&position](const std::vector<Position> path) {
			return std::find_if(path.begin(), path.end(), [&position](const Position& pos) {
				return pos == position;
			}) != path.end();
		};

		auto checkMove = m_state->GetCheckCache().front();
		return position == checkMove.pos
			|| m_state->GetCheckCache().front().move.type == EMoveType::Multiple && IsInPath(m_calculator->GetPath(checkMove.pos, checkMove.move));
	}
}
#include "pch.h"
#include "Chessboard.h"

#include "Game/Actors/ChessPieces/Pawn.h"
#include "Game/Actors/ChessPieces/Knight.h"
#include "Game/Actors/ChessPieces/Bishop.h"
#include "Game/Actors/ChessPieces/Rook.h"
#include "Game/Actors/ChessPieces/Queen.h"
#include "Game/Actors/ChessPieces/King.h"

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	Chessboard::Chessboard()
	{
		for (std::int8_t i = 0; i < sChessboardSize; ++i)
		{
			m_board[i].Initialize((i / sChessboardSide % 2) * 1 - i % 2 ? Black : White);//division is slow
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessboard::Initialize(EColor friendly)
	{
		m_friendly = friendly;
		EColor enemy = friendly == White ? Black : White;
		for (int i = 0; i < sChessboardSide; ++i)
		{
			InitializePiece(EChessPieceType::Pawn, enemy, Position(1 * sChessboardSide + i));
			InitializePiece(EChessPieceType::Pawn, friendly, Position((sChessboardSide - 2) * sChessboardSide + i));
		}

		auto lastRow = (sChessboardSide - 1) * sChessboardSide;
		InitializePiece(EChessPieceType::Rook, enemy, Position(0));
		InitializePiece(EChessPieceType::Rook, enemy, Position(sChessboardSide - 1));
		InitializePiece(EChessPieceType::Rook, friendly, Position(lastRow + 0));
		InitializePiece(EChessPieceType::Rook, friendly, Position(lastRow + sChessboardSide - 1));

		InitializePiece(EChessPieceType::Knight, enemy, Position(1));
		InitializePiece(EChessPieceType::Knight, enemy, Position(sChessboardSide - 2));
		InitializePiece(EChessPieceType::Knight, friendly, Position(lastRow + 1));
		InitializePiece(EChessPieceType::Knight, friendly, Position(lastRow + sChessboardSide - 2));

		InitializePiece(EChessPieceType::Bishop, enemy, Position(2));
		InitializePiece(EChessPieceType::Bishop, enemy, Position(sChessboardSide - 3));
		InitializePiece(EChessPieceType::Bishop, friendly, Position(lastRow + 2));
		InitializePiece(EChessPieceType::Bishop, friendly, Position(lastRow + sChessboardSide - 3));

		if (enemy == White)
		{
			InitializePiece(EChessPieceType::Queen, enemy, Position(4));
			InitializePiece(EChessPieceType::Queen, friendly, Position(lastRow + 4));

			InitializePiece(EChessPieceType::King, enemy, Position(3));
			InitializePiece(EChessPieceType::King, friendly, Position(lastRow + 3));
		}
		else
		{
			InitializePiece(EChessPieceType::Queen, enemy, Position(3));
			InitializePiece(EChessPieceType::Queen, friendly, Position(lastRow + 3));

			InitializePiece(EChessPieceType::King, enemy, Position(4));
			InitializePiece(EChessPieceType::King, friendly, Position(lastRow + 4));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessboard::InitializePiece(EChessPieceType type, EColor color, const Position& position)
	{
		auto* piece = Chessboard::CreateChessPiece(type, color);
		AssertReturnUnless(piece);

		m_board[position.AsIndex()].ClearChessPiece();
		m_board[position.AsIndex()].SetChessPiece(std::move(*piece));
	};

	//////////////////////////////////////////////////////////////////////////////////////////
	const ChessboardTile& Chessboard::GetTile(const Position& position) const
	{
		return m_board[position.AsIndex()];
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessboardTile& Chessboard::GetMutableTile(const Position& position)
	{
		return m_board[position.AsIndex()];
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const ChessboardTiles& Chessboard::GetTiles() const
	{
		return m_board;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const int8_t Chessboard::GetSideModifier(EColor color) const
	{
		return color == m_friendly ? -1 : 1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPiece* Chessboard::CreateChessPiece(EChessPieceType type, EColor color)
	{
		switch (type)
		{
		case chessy::EChessPieceType::Pawn:
			return new Pawn(color);
		case chessy::EChessPieceType::Knight:
			return new Knight(color);
		case chessy::EChessPieceType::Bishop:
			return new Bishop(color);
		case chessy::EChessPieceType::Rook:
			return new Rook(color);
		case chessy::EChessPieceType::Queen:
			return new Queen(color);
		case chessy::EChessPieceType::King:
			return new King(color);
		}
		return nullptr;
	};
}
#pragma once

#include "Game/Utils/Enums.h"
#include "Game/Utils/Position.h"

namespace chessy
{

	constexpr std::int8_t sChessboardSide = 8;
	constexpr std::int8_t sChessboardSize = sChessboardSide * sChessboardSide;
	constexpr std::int8_t sPawnPromoteRow = 3;

	constexpr int SCREEN_WIDTH = 640;
	constexpr int SCREEN_HEIGHT = 640;
	constexpr int TILE_SIZE = 80;

	struct ChessPieceMove
	{
		const EDirection direction = EDirection::NoDirection;
		const EMoveType type = EMoveType::Error;

		bool operator==(const ChessPieceMove& other) const { return direction == other.direction && type == other.type; }
		ChessPieceMove(const ChessPieceMove& other) : direction(other.direction), type(other.type) { }
		ChessPieceMove(ChessPieceMove&& other) noexcept : direction(other.direction), type(other.type) { }
		ChessPieceMove(EDirection _direction, EMoveType _type) : direction(_direction), type(_type) { }
	};

	struct GameMove
	{
		const ChessPieceMove move;
		const Position pos;

		bool operator==(const GameMove& other) const { return move == other.move && pos == other.pos; }
		GameMove(const GameMove& other) : move(other.move), pos(other.pos) { }
		GameMove(const ChessPieceMove& _move, const Position& _pos) : move(_move), pos(_pos) { }
	};

	struct EnPassantCache
	{
		Position takePosition;
		Position piecePosition;
	};

	EDirection GetOpositeDirection(EDirection direction);

	#define ReturnIf(x, ...) if(x) {return __VA_ARGS__; }
	#define ReturnUnless(x, ...) if(!(x)) {return __VA_ARGS__;}
	#define AssertReturnIf(x, ...) if(x) {assert(false); return __VA_ARGS__;}
	#define AssertReturnUnless(x, ...) if(!(x)) {assert(false); return __VA_ARGS__;}
	
	#define BreakIf(x) if(x) break; 
	#define BreakUnless(x) if(!(x)) break;
	#define AssertBreakIf(x) if(x) {assert(false); break;}
	#define AssertBreakUnless(x) if(!(x)) {assert(false); break;}
	
	#define ContinueIf(x) if(x) continue; 
	#define ContinueUnless(x) if(!(x)) continue;
	#define AssertContinueIf(x) if(x) {assert(false); continue;}
	#define AssertContinueUnless(x) if(!(x)) {assert(false); continue;}
}
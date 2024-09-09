#pragma once

#include "Game/Authority/GameMoveValidator.h"
#include "Game/Authority/GameMoveCalculator.h"
namespace chessy
{
	class GameState;
	class GameWorld;
	class GameMoveValidator;
	class GameMoveCalculator;

	class GameMode
	{
	public:
		GameMode(std::shared_ptr<GameWorld> world);
		~GameMode();

		void Initialize(EColor friendly);

		void MoveChessPiece(const Position& from, const Position& to);
		void PromotePawn(const Position& input);
		void OnEndTurn();

		const GameState& GetGameState() const;
		const GameMoveValidator& GetValidator() const;
		const EColor GetTurn() const;
		EGameState GetState() const;

	private:
		void MovePiece(const Position& from, const Position& to);
		void HandleCastling(const Position& from, const Position& to);

		void HandleEnPassant(const Position& from, const Position& to);
		void TakeEnPassant(const Position& position);

		bool IsPawnPromotion(const Position& position) const;

		bool IsInCheck(const Position& kingPos);
		void CheckmateCheck(const Position& kingPos);
		void DrawCheck();

	private:
		static EColor m_turn;
		EGameState m_matchState = EGameState::Initialize;
		std::shared_ptr<GameWorld> m_world = nullptr;
		std::shared_ptr<GameState> m_state = nullptr;

		//game instance subsystems
		//should be in game?
		std::shared_ptr<GameMoveCalculator> m_calculator = nullptr;
		std::unique_ptr<GameMoveValidator> m_validator = nullptr;
	};
}
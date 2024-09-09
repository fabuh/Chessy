#pragma once

namespace chessy
{
	class Chessboard;
	class GameWorld;
	class GameMode;
	class GameState;
	class GameView;
	class PlayerController;

	class Game
	{
	public:
		Game();
		~Game();

		void Initialize(EColor friendly);

		void Play(const Position& input);

		const Chessboard& GetChessboard() const;
		const GameState& GetGameState() const;
		const GameView& GetView() const;
		const EColor GetTurn() const;
		const EGameState GetState() const;

	private:
		void FillView();

	private:
		std::shared_ptr<GameWorld> m_world;
		std::shared_ptr<GameMode> m_gameMode;
		std::unique_ptr<PlayerController> m_controller;
		std::unique_ptr<GameView> m_view;
	};
}
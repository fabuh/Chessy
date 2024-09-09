#pragma once

namespace chessy
{
	class Game;
	class Input;
	class Renderer;
	class FPSCounter;

	class Chessy
	{
	protected:
		static std::shared_ptr<Chessy> GetInstance();

		Chessy();

		void Start();
		void Update();

	private:
		std::unique_ptr<Game> m_game;
		std::unique_ptr<Input> m_input;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<FPSCounter> m_fpsCounter;

	private:
		friend static void StartGame();
		friend static const Game& GetGame();
	};

	static void StartGame()
	{
		return chessy::Chessy::GetInstance()->Start();
	}
	static const Game& GetGame() //give const access to avoid unwanted changes
	{
		return *chessy::Chessy::GetInstance()->m_game;
	}
}


#pragma once

namespace chessy
{
	class GameMode;
	class View;

	class PlayerController
	{
	public:
		PlayerController(std::shared_ptr<GameMode> gameMode);
		~PlayerController() = default;

		void Play(const Position& input);

		const std::optional<Position>& GetSelected() const;
		
	private:
		bool Turn(const EColor player, const Position& input);

	private:
		std::shared_ptr<GameMode> m_gameMode = nullptr;
		std::optional<Position> m_select;
	};
}
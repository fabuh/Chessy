#pragma once

namespace chessy
{
	class Input
	{
	public:
		Input();

		void Update();

		std::optional<int8_t> GetValue() const;
		bool HasQuit() const;
		
	private:
		void HandleEvents();

	private:
		std::optional<int8_t> m_input;
		bool m_quit = false;
	};
}


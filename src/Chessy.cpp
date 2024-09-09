#include "pch.h"
#include "Chessy.h"

#include "Services/Input.h"
#include "Services/Renderer.h"
#include "Services/FPSCounter.h"
#include "Game/Game.h"

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	std::shared_ptr<Chessy> Chessy::GetInstance()
	{
		static std::shared_ptr<Chessy> instance;
		if (!instance)
		{
			instance = std::shared_ptr<Chessy>(new Chessy());
		}
		return instance;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	Chessy::Chessy() :
		m_game(std::make_unique<Game>()),
		m_input(std::make_unique<Input>()),
		m_renderer(std::make_unique<Renderer>()),
		m_fpsCounter(std::make_unique<FPSCounter>())
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::Start()
	{
		//Initialize the renderer
		m_renderer->Initialize();

		//Initialize the game
		m_game->Initialize(EColor::White);

		//Initial Render
		m_renderer->Render();

		//Game Loop
		while (m_input->HasQuit() == false)
		{
			Update();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::Update()
	{
		//Input
		m_input->Update();

		if (m_input->GetValue())
		{
			//Game
			m_game->Play(*m_input->GetValue());

			m_renderer->Render();
		}

		//FPS Counter
		m_fpsCounter->Update();
	}
}
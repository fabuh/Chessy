#include "pch.h"
#include "Input.h"

#include "Game/Game.h"

#include <SDL.h>

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	Input::Input()
	{
	}

	//	//////////////////////////////////////////////////////////////////////////////////////////
    void Input::Update()
    {   
		m_input.reset();
		HandleEvents();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    std::optional<int8_t> Input::GetValue() const
    {
        return m_input;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    bool Input::HasQuit() const
    {
        return m_quit;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Input::HandleEvents()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                SDL_Quit();
				m_quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                m_input = y / TILE_SIZE * sChessboardSide + x / TILE_SIZE;
            }
        }
    }
}
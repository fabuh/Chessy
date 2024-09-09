#include "pch.h"
#include "Renderer.h"

#include "Game/Game.h"
#include "Game/GameState.h"
#include "Game/GameView.h"

#include <SDL.h>
#include <SDL_image.h>

namespace chessy
{
    //////////////////////////////////////////////////////////////////////////////////////////
    static SDL_HitTestResult HitCallback(SDL_Window* win, const SDL_Point* area, void* data)
    {
		return SDL_HITTEST_NORMAL;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    Renderer::Renderer() : m_window(nullptr), m_renderer(nullptr)
    {
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    Renderer::~Renderer()
    {
        for (auto& texture : m_textures)
        {
            SDL_DestroyTexture(texture.second);
            texture.second = nullptr;
        }

        // Destroy window and renderer
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;

        SDL_DestroyWindow(m_window);
        m_window = nullptr;

        // Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    bool Renderer::Initialize()
	{
        AssertReturnUnless(static_cast<bool>(SDL_Init(SDL_INIT_VIDEO) >= 0), false);

        m_window = SDL_CreateWindow("Chessboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        AssertReturnUnless(m_window, false);

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        AssertReturnUnless(m_renderer, false);

        int imgFlags = IMG_INIT_PNG;
        AssertReturnUnless((IMG_Init(imgFlags) & imgFlags), false);

        void* callback_data = nullptr;
        SDL_SetWindowHitTest(m_window, HitCallback, callback_data);

        std::filesystem::current_path(std::filesystem::path(SDL_GetBasePath()));
        LoadResources();

        return true;
	}

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::Render()
    {
        switch (GetGame().GetState())
        {
        case EGameState::Gameplay:
        case EGameState::Check:
            RenderGameplay();
            break;
        case EGameState::Promote:
            RenderPromoteMenu();
            break;
        case EGameState::WhiteWin:
        case EGameState::BlackWin:
		case EGameState::Draw:
			RenderGameOver();
			break;
        default:
            assert(false);
            break;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::RenderGameplay()
    {
        SDL_RenderClear(m_renderer);

        const auto& view = GetGame().GetView();
        for (int8_t i = 0; i < sChessboardSize; ++i)
        {
            Position pos(i);
            SDL_Rect tileRect{ pos.GetCol() * TILE_SIZE, pos.GetRow() * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            const auto& posView = view.Get(i);
            if (posView.tileColor != EColor::Colorless)
            {
                RenderTile(tileRect, posView.tileColor, posView.selected);
            }

            if (posView.pieceColor != EColor::Colorless && posView.pieceType != EChessPieceType::Error)
            {
                RenderChessPiece(tileRect, posView.pieceType, posView.pieceColor);
            }

            if (posView.moveType != EMoveType::Error)
            {
                if (posView.pieceColor != EColor::Colorless && posView.pieceType != EChessPieceType::Error)
                {
                    RenderChessPieceAttack(tileRect, posView.pieceType, posView.pieceColor);
                }
                else
                {
                    RenderMove(tileRect, posView.moveType);
                }
            }
        }

        SDL_RenderPresent(m_renderer);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::RenderPromoteMenu()
    {
        //Do not clear. Renders on top of the game view so you can see the board.
        //Maybe rewrite this under/above the board
        //Service line as big as a single row of tiles. It handles state, promotion, moves etc.
        auto color = GetGame().GetTurn();
        for (int8_t i = 2; i < sChessboardSide - 2; ++i)//knight, bishop, rook, queen
        {
            Position pos(sPawnPromoteRow * sChessboardSide + i);
            SDL_Rect tileRect{ pos.GetCol() * TILE_SIZE, pos.GetRow() * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            std::string texture = color == EColor::White ? "w_" : "b_";
            texture += (std::to_string(i) + ".png");

            SDL_RenderCopy(m_renderer, m_textures[color == EColor::Black ? "white_tile_warning.png" : "blue_tile_warning.png"], nullptr, &tileRect);
            SDL_RenderCopy(m_renderer, m_textures[texture], nullptr, &tileRect);
        }

        SDL_RenderPresent(m_renderer);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::RenderGameOver()
	{
		SDL_RenderClear(m_renderer);

		auto HasWon = [](EColor color) -> bool {
            return GetGame().GetState() == EGameState::WhiteWin && color == White
                || GetGame().GetState() == EGameState::BlackWin && color == Black;
		};

		const auto& view = GetGame().GetView();
		for (int8_t i = 0; i < sChessboardSize; ++i)
		{
			Position pos(i);
			SDL_Rect tileRect{ pos.GetCol() * TILE_SIZE, pos.GetRow() * TILE_SIZE, TILE_SIZE, TILE_SIZE };

			const auto& posView = view.Get(i);
			if (posView.tileColor != EColor::Colorless)
			{
				RenderTile(tileRect, posView.tileColor, posView.selected);
			}

			if (posView.pieceColor != EColor::Colorless && posView.pieceType != EChessPieceType::Error)
			{
				RenderChessPiece(tileRect, posView.pieceType, posView.pieceColor);
			}

			if (posView.pieceType == EChessPieceType::King)
			{
                ContinueIf(HasWon(posView.pieceColor));

				RenderChessPieceAttack(tileRect, posView.pieceType, posView.pieceColor);
			}
		}

		SDL_RenderPresent(m_renderer);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::RenderTile(const SDL_Rect& tileRect, EColor color, bool selected)
    {
		std::string texture = color == EColor::White ? "white_tile" : "blue_tile";
        if (selected)
        {
            texture += "_warning";
        }
        texture += ".png";

        SDL_RenderCopy(m_renderer, m_textures[texture], nullptr, &tileRect);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::RenderChessPiece(const SDL_Rect& tileRect, EChessPieceType type, EColor color)
    {
        switch (type)
        {
        case chessy::EChessPieceType::Pawn:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_1.png" : "b_1.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::Knight:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_2.png" : "b_2.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::Bishop:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_3.png" : "b_3.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::Rook:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_4.png" : "b_4.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::Queen:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_5.png" : "b_5.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::King:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_6.png" : "b_6.png"], nullptr, &tileRect);
            break;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::RenderChessPieceAttack(const SDL_Rect& tileRect, EChessPieceType type, EColor color)
    {
        switch (type)
        {
        case chessy::EChessPieceType::Pawn:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_1_attack.png" : "b_1_attack.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::Knight:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_2_attack.png" : "b_2_attack.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::Bishop:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_3_attack.png" : "b_3_attack.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::Rook:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_4_attack.png" : "b_4_attack.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::Queen:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_5_attack.png" : "b_5_attack.png"], nullptr, &tileRect);
            break;
        case chessy::EChessPieceType::King:
            SDL_RenderCopy(m_renderer, m_textures[color == EColor::White ? "w_6_attack.png" : "b_6_attack.png"], nullptr, &tileRect);
            break;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::RenderMove(const SDL_Rect& tileRect, EMoveType type)
    {
        auto specialMoves = { EnPassant, Castle };

        if(std::find(specialMoves.begin(), specialMoves.end(), type) != specialMoves.end())
		{
			SDL_RenderCopy(m_renderer, m_textures["special_move_dot.png"], nullptr, &tileRect);
		}
        else
        {
            SDL_RenderCopy(m_renderer, m_textures["move_dot.png"], nullptr, &tileRect);
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    bool Renderer::LoadResources()
    {
        // Load textures
        //LoadTextures("./res/menu");
        LoadTextures("./res/chessbois");

        return true;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::LoadTextures(const std::string path)
    {
        for (const auto& file : std::filesystem::directory_iterator(path))
        {
            std::string filePath = file.path().string();
            std::string fileName = std::filesystem::path(filePath).filename().string();

            AssertContinueUnless(std::filesystem::exists(filePath));

            SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
            AssertContinueUnless(loadedSurface);

            m_textures[fileName] = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);

            AssertContinueUnless(m_textures[fileName]);
        }
    }
}

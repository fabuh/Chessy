#pragma once	

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace chessy
{
	class View;
	class MenuView;
	class GameView;
	class PromotionView;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		bool Initialize();

		void Render();

	private:
		void RenderGameplay();
		void RenderPromoteMenu();
		void RenderGameOver();

	private:
		void RenderTile(const SDL_Rect& tileRect, EColor color, bool selected);
		void RenderChessPiece(const SDL_Rect& tileRect, EChessPieceType type, EColor color);
		void RenderChessPieceAttack(const SDL_Rect& tileRect, EChessPieceType type, EColor color);
		void RenderMove(const SDL_Rect& tileRect, EMoveType type);

	private:
		bool LoadResources();
		void LoadTextures(const std::string path);

	private:
		//Textures
		std::unordered_map<std::string, SDL_Texture*> m_textures;

		// The window we'll be rendering to
		SDL_Window* m_window;

		// The window renderer
		SDL_Renderer* m_renderer;
	};
}
#pragma once

namespace chessy
{
	struct ChessPieceMove;
	typedef std::vector<ChessPieceMove> ChessPieceMoves;
	
	class ChessPiece
	{
	public:
		ChessPiece(EColor color, EChessPieceType type);
		ChessPiece(ChessPiece&& other) noexcept;
		virtual ~ChessPiece() = default;

		void SetHasMoved(bool hasMoved);
		bool HasMoved() const;

		const ChessPieceMoves& GetChessPieceMoves() const;
		EChessPieceType GetType() const;
		EColor GetColor() const;

	private:
		//prevent copying
		ChessPiece(const ChessPiece&) = delete;
		ChessPiece& operator=(const ChessPiece&) = delete;

	protected:
		ChessPieceMoves m_chessMoves;

	private:
		const EColor m_color = EColor::Colorless;
		const EChessPieceType m_type = EChessPieceType::Error;
		bool m_hasMoved = false;
	};
}    

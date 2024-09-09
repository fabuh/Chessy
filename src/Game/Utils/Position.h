#pragma once

namespace chessy
{
	class Position
	{
	public:
		Position(int8_t index = -1);//default is invalid
		Position(int8_t _row, int8_t _col);

		const Position& operator=(const Position& other);

		bool operator==(const Position& other) const;
		const Position& operator+=(const Position& other);
		const Position& operator*=(int8_t modifier);
		const Position operator+(const Position& other);//creates new object

		bool IsValid() const;

		std::int8_t GetRow() const;
		std::int8_t GetCol() const;
		std::int8_t AsIndex() const;

	private:
		std::int8_t m_row;
		std::int8_t m_col;
	};

}
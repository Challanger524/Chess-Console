#include "Pices.h"

const bool ChessPice::BeatAllies(const Coord from, const Coord to, const grid_t &grid) {
	if (grid[to.y][to.x] == nullptr) return false;
	return grid[from.y][from.x]->GetColr() == grid[to.y][to.x]->GetColr(); //if try to beat allies
}

const bool ChessPice::Rule(const Coord from, const Coord to, const grid_t &grid)
{
	if (BeatAllies(from, to, grid)) return false;

	//if only one cell jump - no point to check for units on the way
	if (dist(from.x, to.x) == 1 || dist(from.y, to.y) == 1) return true;

	//if something on the way(vertically)
	if (from.x == to.x) {
		if (from.y < to.y) {
			for (int i = from.y + 1; i < to.y; i++) if (grid[i][from.x] != nullptr) return false;//check down
		}
		else for (int i = from.y - 1; i > to.y; i--) if (grid[i][from.x] != nullptr) return false;//check up
	}

	//if something on the way(horizontally)
	if (from.y == to.y) {
		if (from.x < to.x) {
			for (int i = from.x + 1; i < to.x; i++) if (grid[from.y][i] != nullptr) return false;//check  right
		}
		else for (int i = from.x - 1; i > to.x; i--) if (grid[from.y][i] != nullptr) return false;//chek left
	}

	//if something on the way(diagonally)
	if (dist(from.x, to.x) == dist(from.y, to.y))
	{
		if (from.y > to.y) {//if first and second quater move
			if (from.x < to.x) { //first quater
				for (int i = from.x + 1, j = from.y - 1; j > to.y; i++, j--)
					if (grid[j][i] != nullptr) return false;
			}
			else //second quater
				for (int i = from.x - 1, j = from.y - 1; j > to.y; i--, j--)
					if (grid[j][i] != nullptr)	return false;
		}
		else {// third and fourth quater
			if (from.x > to.x) { //third quater
				for (int i = from.x - 1, j = from.y + 1; j < to.y; i--, j++)
					if (grid[j][i] != nullptr) return false;
			}
			else //furth quater
				for (int i = from.x + 1, j = from.y + 1; j < to.y; i++, j++)
					if (grid[j][i] != nullptr) return false;
		}
	}

	return true; //no ramming detected
}

const bool Pawn::Rule(const Coord from, const Coord to, const grid_t &grid)
{
	if (from.x == to.x && grid[to.y][to.x] != nullptr) return false;//cannot beat forwards |bad|

	if (!ChessPice::Rule(from, to, grid)) return false;//if try to overjump something |bad|

	if (dist(from.x, to.x) == 0 && dist(from.y, to.y) == 2)//if two cells movement is first
	{
		if (grid[from.y][from.x]->GetColr() == L'W' && from.y == 1) return true;
		if (grid[from.y][from.x]->GetColr() == L'B' && from.y == 6) return true;
		return false;
	}

	if (dist(from.x, to.x) == 1 && dist(from.y, to.y) == 1)
		if (grid[to.y][to.x] != nullptr)
			if (grid[from.y][from.x]->GetColr() != grid[to.y][to.x]->GetColr()) return true;//if diagnoal enemy beating |good|

	if (from.x == to.x && dist(from.y, to.y) == 1) return true;// if simple move |good|

	return false;//if invalid for pawns movment |bad|
}

const bool Queen::Rule(const Coord from, const Coord to, const grid_t &grid)
{
	if (!ChessPice::Rule(from, to, grid)) return false;//if try to overjump something |bad|

	if (dist(from.x, to.x) == dist(from.y, to.y)) return true;//diagnoal movment |good|

	if (from.x == to.x && from.y != to.y) return true;//vertical movement |good|

	if (from.y == to.y && from.x != to.x) return true;//horizontal movement |good|

	return false;//if invalid for Queens movment |bad|
}

const bool King::Rule(const Coord from, const Coord to, const grid_t &grid)
{
	if (!ChessPice::Rule(from, to, grid)) return false;//if try to overjump something |bad|

	if (from.y == to.y && dist(from.x, to.x) == 2) //if Castling
	{
		Coord rook{-1,-1};

		if (static_cast<King*>(grid[from.y][from.x])->IfMoved()) return false;

		if (from.x < to.x) rook = Coord{from.y, 7};
		else rook = Coord{from.y, 0};

		if (grid[rook.y][rook.x] == nullptr || static_cast<Rook*>(grid[rook.y][rook.x])->IfMoved()) return false;

		if (rook.x == 7) {
			for (int i = from.x + 1; i < 7; i++) if (grid[from.y][i] != nullptr) return false;
		}
		else for (int i = 0 + 1; i < from.x; i++) if (grid[from.y][i] != nullptr) return false;

		return true;
	}

	if (dist(from.y, to.y) < 2 && dist(from.x, to.x) < 2) return true;

	return false;//if invalid for Rooks movment |bad|
}

const bool Rook::Rule(const Coord from, const Coord to, const grid_t &grid)
{
	if (!ChessPice::Rule(from, to, grid)) return false;//if try to overjump something |bad|

	if (from.x == to.x && from.y != to.y) return true;//vertical movement |good|

	if (from.y == to.y && from.x != to.x) return true;//horizontal movement |good|

	return false;//if invalid for Rooks movment |bad|
}

const bool Bishop::Rule(const Coord from, const Coord to, const grid_t &grid)
{
	if (!ChessPice::Rule(from, to, grid)) return false;//if try to overjump something |bad|

	if (dist(from.x, to.x) == dist(from.y, to.y)) return true;//diagnoal movment |good|

	return false;
}

const bool kNight::Rule(const Coord from, const Coord to, const grid_t &grid)
{
	if (BeatAllies(from, to, grid)) return false;

	if (dist(from.x, to.x) == 1 && dist(from.y, to.y) == 2)return true;
	if (dist(from.x, to.x) == 2 && dist(from.y, to.y) == 1)return true;

	return false;
}

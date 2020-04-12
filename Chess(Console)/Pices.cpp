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

		if (grid[rook.y][rook.x] == nullptr) return false;//if nothing
		if (grid[rook.y][rook.x]->GetName() != L'R') return false;//if there are not a Rook
		if (grid[rook.y][rook.x]->GetColr() != grid[from.y][from.x]->GetColr()) return false;//if enemy there are enemy rook
		if (static_cast<Rook*>(grid[rook.y][rook.x])->IfMoved()) return false;// if rook already moved

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


//attention to the position -1 -1. It must appear as late as possible(as return position)(validates as 'false').
Coord Walker::GoKing(const Coord pos) {
	static int it = 0;
	Coord move{-1,-1};

	if (!pos) it = -1;

	else if (it == 0) move = Coord{pos.y,     pos.x + 2};//castling
	else if (it == 1) move = Coord{pos.y,     pos.x - 2};//castling

	else if (it == 2) move = Coord{pos.y, pos.x - 1};//left
	else if (it == 3) move = Coord{pos.y + 1, pos.x - 1};
	else if (it == 4) move = Coord{pos.y + 1, pos.x};
	else if (it == 5) move = Coord{pos.y + 1, pos.x + 1};
	else if (it == 6) move = Coord{pos.y    , pos.x + 1};
	else if (it == 7) move = Coord{pos.y - 1, pos.x + 1};
	else if (it == 8) move = Coord{pos.y - 1, pos.x};
	else if (it == 9) move = Coord{pos.y - 1, pos.x - 1};

	else it = -1;//variations end

	it++;

	return move;
}
Coord Walker::GoPawn(const Coord pos) {
	static int it = 0;
	Coord move{-1,-1};

	if (!pos) it = -1;

	else if (it == 0) move = Coord{pos.y + 1, pos.x + 1};
	else if (it == 1) move = Coord{pos.y + 1, pos.x};
	else if (it == 2) move = Coord{pos.y + 2, pos.x};
	else if (it == 3) move = Coord{pos.y + 1, pos.x - 1};

	else if (it == 4) move = Coord{pos.y - 1, pos.x - 1};
	else if (it == 5) move = Coord{pos.y - 1, pos.x};
	else if (it == 4) move = Coord{pos.y - 2, pos.x};
	else if (it == 4) move = Coord{pos.y - 1, pos.x + 1};

	else it = -1;//variations end

	it++;

	return move;
}
Coord Walker::GokNight(const Coord pos) {
	static int it = 0;
	Coord move{-1,-1};

	if (!pos) it = -1;

	else if (it == 0) move = Coord{pos.y - 2, pos.x + 1};//1st sq
	else if (it == 1) move = Coord{pos.y - 1, pos.x + 2};

	else if (it == 2) move = Coord{pos.y + 1, pos.x + 2};//4th sq
	else if (it == 3) move = Coord{pos.y + 2, pos.x + 1};

	else if (it == 4) move = Coord{pos.y + 2, pos.x - 1};//3rd sq
	else if (it == 5) move = Coord{pos.y + 1, pos.x - 2};

	else if (it == 6) move = Coord{pos.y - 1, pos.x - 2};//2nd sq
	else if (it == 7) move = Coord{pos.y - 2, pos.x - 1};

	else it = -1;//variations end

	it++;

	return move;
}
Coord Walker::GoRook(const Coord pos) {
	static int V = -7;
	static int H = -7;
	Coord move{-1,-1};

	if (!pos) {
		V = -7;
		H = -7;
	}
	else if (V < 8) {
		move = Coord{pos.y + V, pos.x};
		V++;
	}
	else if (H < 8){
		move = Coord{pos.y, pos.x + H};
		H++;
	}
	else {//variations end
		V = -7;
		H = -7;
	}

	return move;
}
Coord Walker::GoBishop(const Coord pos) {
	static int S = 7;//slash // '+7' be careful here!
	static int B = -7;//back slash
	Coord move{-1,-1};

	if (!pos) {
		S = 7;
		B = -7;
	}
	else if (B < 8) {
		move = Coord{pos.y + B, pos.x - B};
		B++;
	}
	else if (S > -8) {
		move = Coord{pos.y + S, pos.x + S};
		S--;
	}
	else {//variations end
		S = 7;
		B = -7;
	}

	return move;
}
Coord Walker::GoQueen(const Coord pos) {
	static int V = -7;
	static int H = -7;

	static int S = 7;//slash
	static int B = -7;//back slash

	Coord move{-1,-1};

	if (!pos) {
		V = -7;
		H = -7;
		S = -7;
		B = -7;
	}
	else if (V < 8) {
		move = Coord{pos.y + V, pos.x};
		V++;
	}
	else if (H < 8) {
		move = Coord{pos.y, pos.x + H};
		H++;
	}
	else if (B < 8) {
		move = Coord{pos.y + B, pos.x - B};
		B++;
	}
	else if (S > -8) {
		move = Coord{pos.y + S, pos.x + S};
		S--;
	}
	else {//variations end
		V = -7;
		H = -7;
		S = -7;
		B = -7;
	}

	return move;
}
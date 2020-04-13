#pragma once
#include "Header.h"
#include "Board.h"

void Desk::PreciseUpdate(const Coord from, const Coord to)
{
	//assert((from != to && grid[to.y][to.x] != nullptr) && L"cell for piece write must not be empty");//always triggering

	size_t cell = (FIRST_CELL_Y + from.y * 2) * WIDTH + FIRST_CELL_X + from.x * 3;//Update 'from' cell
	if ((from.x + from.y) % 2 == 0) { //if cell should be painted - paint
		map[cell] = L'█';
		map[cell + 1] = L'█';
	}
	else map[cell] = map[cell + 1] = L' ';

	if (from == to) return;

	cell = (FIRST_CELL_Y + to.y * 2) * WIDTH + FIRST_CELL_X + to.x * 3;//update 'to' cell
	map[cell] = grid[to.y][to.x]->GetColr();
	map[cell + 1] = grid[to.y][to.x]->GetName();
}

bool Desk::IfEnPassant(const Coord from, const Coord to)
{
	bool enpass = false;

	if (grid[from.y][from.x]->GetName() != L'P')return false;
	if (dist(from.y, to.y) != 2) return false;

	if (to.x > 0 && grid[to.y][to.x - 1] != nullptr) {//if something could be left 
		if (grid[from.y][from.x]->GetColr() != grid[to.y][to.x - 1]->GetColr())// if enemys to each other
			if (grid[to.y][to.x - 1]->GetName() == L'P') enpass = true; // if that enemy is also a Pawn
	}
	else if (to.x < 9 && grid[to.y][to.x + 1] != nullptr) { //if something could be right 
		if (grid[from.y][from.x]->GetColr() != grid[to.y][to.x + 1]->GetColr())// if enemys to each other
			if (grid[to.y][to.x + 1]->GetName() == L'P') enpass = true; // if that enemy is also a Pawn
	}

	if (enpass) {//if EnPassant can occur - leve a mark

		if (en_passant) {//if second enp in row after expired first
			if (grid[en_passant.y][en_passant.x]->GetName() == L'p') {
				ErasePiece(en_passant);
				PreciseUpdate(en_passant, en_passant);
			}
		}

		en_passant = Coord{(from.y + to.y) / 2, from.x};

		SetPiece(to, grid[from.y][from.x]);
		SetPiece(en_passant, new EnPawn(grid[from.y][from.x]->GetColr()));
		grid[from.y][from.x] = nullptr;


		PreciseUpdate(from, to);//for Pawn under passant
		PreciseUpdate(from, en_passant);//for tooltip show
	}

	return enpass;
}

void Desk::ErasePiece(const Coord pos) {
	assert(pos.x >= 0 && pos.y >= 0 && L"Coord must be positive");
	assert(pos.x < 9 && pos.y < 9 && L"Coord must be less then 9");
	assert(grid[pos.y][pos.x] != nullptr && L"Piece for erase must exist");

	delete grid[pos.y][pos.x];
	grid[pos.y][pos.x] = nullptr;
}

void Desk::SetPiece(const Coord pos, ChessPiece *piece) {
	assert(pos.x >= 0 && pos.y >= 0 && L"Coord must be positive");
	assert(pos.x < 9 && pos.y < 9 && L"Coord must be less then 9");
	assert(piece != nullptr && L"Piece for insert must exist");

	if (piece->GetName() == L'K') {
		if (piece->GetColr() == L'W') wking = pos;
		else bking = pos;
	}

	if (grid[pos.y][pos.x] != nullptr) delete grid[pos.y][pos.x];
	grid[pos.y][pos.x] = move(piece);
	//piece = nullptr;//!!! make 'piece' invalid after SetPiece() call!!!
}

bool Desk::Move(const Coord from, const Coord to)
{
	assert(grid[from.y][from.x] != nullptr);

	if (Stalemate(grid[from.y][from.x]->GetColr())) {//if Stalemate
		if (grid[from.y][from.x]->GetColr() == L'W') {
			if (Stalemate(L'B')) wcout << L"Both teams suffering Stalemate\n";
		}
		else if (Stalemate(L'W')) wcout << L"Both teams suffering Stalemate\n";
		else wcout << L"You are under a Stalemate\n";

		return false;
	}	

	if (!grid[from.y][from.x]->Rule(from, to, grid)) return false;//rule violation check

	if (Check(from, to, grid)) {//if this move will put you under a Check(or wont put out of)
		wcout << L"You are under a Check\n";
		return false;
	}

	if (grid[from.y][from.x]->GetColr() == L'W' && wking) {//if CheckMate
		if (Checkmate(wking)) {
			wcout << L"CheckMate, Black won!\n";
			return false;
		}
	}
	else if (bking) if (Checkmate(bking)) {
		wcout << L"CheckMate, White won!\n";
		return false;
	}

	if (IfEnPassant(from, to)) return true;//for intercepted
	else if (grid[from.y][from.x]->GetName() == L'P' && to == en_passant)//if interceptor
	{
		Coord intercepted{from.y, en_passant.x};

		SetPiece(to, grid[from.y][from.x]);
		grid[from.y][from.x] = nullptr;

		ErasePiece(intercepted);

		PreciseUpdate(from, to);
		PreciseUpdate(intercepted, intercepted);//for intercepted's cell painting

		return true;
	}
	else if (grid[from.y][from.x]->GetName() == L'P' && (to.y == 0 || to.y == 7))//if Pawn gets promotion.
	{
		switch (GetInput())//!! > INPUT from user !!
		{
		case L'1':
			if (grid[from.y][from.x]->GetColr() == L'W') SetPiece(to, new WkNight);
			else SetPiece(to, new BkNight);
			break;

		case L'2':
			if (grid[from.y][from.x]->GetColr() == L'W') SetPiece(to, new WRook);
			else SetPiece(to, new BRook);
			break;

		case L'3':
			if (grid[from.y][from.x]->GetColr() == L'W') SetPiece(to, new WBishop);
			else SetPiece(to, new BBishop);
			break;

		default://Queen
			if (grid[from.y][from.x]->GetColr() == L'W') SetPiece(to, new WQueen);
			else SetPiece(to, new BQueen);
			break;
		}

		ErasePiece(from);
		PreciseUpdate(from, to);
	}
	else if (grid[from.y][from.x]->GetName() == L'K')
	{
		if (dist(from.x, to.x) == 2)//if castling
		{
			Coord rook_from, rook_to;

			static_cast<King*>(grid[from.y][from.x])->YesMoved();
			SetPiece(to, grid[from.y][from.x]);//move the King
			grid[from.y][from.x] = nullptr;

			//define Rook's movement
			if (from.x < to.x) {
				rook_from = Coord{from.y, 7};
				rook_to = Coord{from.y, to.x - 1};
			}
			else {
				rook_from = Coord{from.y, 0};
				rook_to = Coord{from.y, to.x + 1};
			}

			//move the Rook
			static_cast<Rook*>(grid[rook_from.y][rook_from.x])->YesMoved();
			SetPiece(rook_to, grid[rook_from.y][rook_from.x]);
			grid[rook_from.y][rook_from.x] = nullptr;
			PreciseUpdate(rook_from, rook_to);
		}
		else {//King just moves
			static_cast<King*>(grid[from.y][from.x])->YesMoved();
			SetPiece(to, grid[from.y][from.x]);
			grid[from.y][from.x] = nullptr;
		}

		PreciseUpdate(from, to);

		if (grid[to.y][to.x]->GetColr() == L'W') wking = Coord(to);
		else bking = Coord(to);
	}
	else//default move
	{
		if (grid[from.y][from.x]->GetName() == L'R') //if Rook moves
			static_cast<Rook*>(grid[from.y][from.x])->YesMoved();

		SetPiece(to, grid[from.y][from.x]);
		grid[from.y][from.x] = nullptr;

		PreciseUpdate(from, to);
	}

	if (en_passant) {//if ability expired
		if (grid[en_passant.y][en_passant.x]->GetName() == L'p') {
			ErasePiece(en_passant);
			PreciseUpdate(en_passant, en_passant);
		}
		en_passant = {-1, -1};
	}

	return true;
}

unsigned short Desk::GetInput() {
	wcout << L"Promotion: which piece do you choose:\n"
		"0 - Queen, 1 - Knight, 2 - Rook, 3 - Bishop: ";
	return wcin.get();
}

const void Desk::Print() {
	system("cls");
	wcout << map << endl;
}

void Desk::Refresh() {
	for (size_t y = 0; y < CELLS; y++) //cells
		for (size_t x = 0; x < CELLS; x++)
		{
			size_t cell = (FIRST_CELL_Y + y * CELL_ITER_Y) * WIDTH + FIRST_CELL_X + x * CELL_ITER_X;

			if (grid[y][x] != nullptr) {
				map[cell] = grid[y][x]->GetColr();
				map[cell + 1] = grid[y][x]->GetName();
			}
			else if ((y + x) % 2 != 0) map[cell + 1] = map[cell] = L' ';
			else map[cell + 1] = map[cell] = L'█';
		}
}

void Desk::ClearBoard()
{
	for (size_t y = 0; y < CELLS; y++) //cells
		for (size_t x = 0; x < CELLS; x++) {
			if (grid[y][x] != nullptr) {
				delete grid[y][x];
				grid[y][x] = nullptr;
			}
		}

	Refresh();
}

void Desk::DefaultPlacement()
{
	for (size_t i = 0; i < CELLS; i++)
		grid[1][i] = new WPawn;

	grid[0][0] = new WRook;
	grid[0][7] = new WRook;

	grid[0][3] = new WQueen;
	grid[0][4] = new WKing;

	grid[0][2] = new WBishop;
	grid[0][5] = new WBishop;

	grid[0][1] = new WkNight;
	grid[0][6] = new WkNight;

	wking = Coord{0,4};

	for (size_t i = 0; i < CELLS; i++)
		grid[6][i] = new BPawn;

	grid[7][0] = new BRook;
	grid[7][7] = new BRook;

	grid[7][4] = new BKing;
	grid[7][3] = new BQueen;

	grid[7][2] = new BBishop;
	grid[7][5] = new BBishop;

	grid[7][1] = new BkNight;
	grid[7][6] = new BkNight;

	bking = Coord{7,4};
}

void Desk::Restart() {
	ClearBoard();
	DefaultPlacement();
	Refresh();
}

const bool Desk::Save() {
	wofstream save("Save.txt");
	if (!save) return false;

	for (size_t y = 0; y < CELLS; y++)
		for (size_t x = 0; x < CELLS; x++)
		{
			if (grid[y][x] != nullptr)
			{
				save << grid[y][x]->GetColr();
				save << grid[y][x]->GetName();

				if (grid[y][x]->GetName() == L'K')
					save << static_cast<King*>(grid[y][x])->IfMoved();
				else if (grid[y][x]->GetName() == L'R')
					save << static_cast<Rook*>(grid[y][x])->IfMoved();
			}
			else save << L'#';
		}

	return true;
}

bool Desk::Load() {
	wifstream save("Save.txt");
	if (!save) return false;
	if (save.eof()) return false;

	wchar_t red;
	ClearBoard();

	for (int y = 0; y < CELLS; y++)
		for (int x = 0; x < CELLS; x++)
		{
			if (save.eof()) return false;

			save >> red;

			if (red == L'#') grid[y][x] = nullptr;
			else if (red == L'W')
			{
				if (save.eof()) return false;
				save >> red;

				if (red == L'R') {
					bool moved;
					SetPiece(Coord{y,x}, new WRook);

					if (save.eof()) return false;
					save >> moved;

					if (moved) static_cast<Rook*>(grid[y][x])->YesMoved();
				}
				else if (red == L'K') {
					bool moved;
					SetPiece(Coord{y,x}, new WKing);
					wking = Coord{y,x};

					if (save.eof()) return false;
					save >> moved;

					if (moved) static_cast<King*>(grid[y][x])->YesMoved();
				}
				else if (red == L'p') SetPiece(Coord{y,x}, new EnPawn(L'W'));
				else if (red == L'P') SetPiece(Coord{y,x}, new WPawn);
				else if (red == L'B') SetPiece(Coord{y,x}, new WBishop);
				else if (red == L'N') SetPiece(Coord{y,x}, new WkNight);
				else if (red == L'Q') SetPiece(Coord{y,x}, new WQueen);
				else return false;
			}
			else if (red == L'B')
			{
				if (save.eof()) return false;
				save >> red;

				if (red == L'R') {
					bool moved;
					SetPiece(Coord{y,x}, new BRook);

					if (save.eof()) return false;
					save >> moved;

					if (moved) static_cast<Rook*>(grid[y][x])->YesMoved();
				}
				else if (red == L'K') {
					bool moved;
					SetPiece(Coord{y,x}, new BKing);
					wking = Coord{y,x};

					if (save.eof()) return false;
					save >> moved;

					if (moved) static_cast<King*>(grid[y][x])->YesMoved();
				}
				else if (red == L'p') SetPiece(Coord{y,x}, new EnPawn(L'B'));
				else if (red == L'P') SetPiece(Coord{y,x}, new BPawn);
				else if (red == L'B') SetPiece(Coord{y,x}, new BBishop);
				else if (red == L'N') SetPiece(Coord{y,x}, new BkNight);
				else if (red == L'Q') SetPiece(Coord{y,x}, new BQueen);
				else return false;
			}
			else return false;
		}

	Refresh();

	return true;
}


const bool Desk::Check(const Coord from, const Coord to, const grid_t ngrid)
{//if any opponent's piece can strike the king
	Coord king;
	assert(ngrid[from.y][from.x] != nullptr);

	wchar_t side = ngrid[from.y][from.x]->GetColr();

	if (side == L'W') king = wking;
	else king = bking;

	if (!king) return false;//can trigger only within unit testing

	if (from == to) {//if you are under the Check(static)
		king = from;
		auto& f_grid = ngrid;


		for (int y = 0; y < CELLS; y++)
			for (int x = 0; x < CELLS; x++)
				if (f_grid[y][x] != nullptr && f_grid[y][x]->GetColr() != side)
					if (f_grid[y][x]->Rule(Coord{y,x}, king, f_grid)) return true;
	}
	else {//if check will occur(move)
		if (ngrid[from.y][from.x]->GetName() == L'K') king = to;
		auto f_grid = ngrid;

		f_grid[to.y][to.x] = move(f_grid[from.y][from.x]);
		f_grid[from.y][from.x] = nullptr;

		for (int y = 0; y < CELLS; y++)
			for (int x = 0; x < CELLS; x++)
				if (f_grid[y][x] != nullptr && f_grid[y][x]->GetColr() != side)
					if (f_grid[y][x]->Rule(Coord{y,x}, king, f_grid)) return true;
	}

	return false;
}

const bool Desk::Checkmate(const Coord king)
{
	if (!Check(king, king, grid))return false;

	assert(king && (king == wking || king == bking) && L"King Coord must be valod");

	wchar_t side = grid[king.y][king.x]->GetColr();

	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
		{
			Coord mov{0,0}, pre_mov{y,x};

			//returns true if check can be avoided
			auto CheckDodge = [&] (Coord(*GoWrapper)(const Coord pos), grid_t &sub_grid, Coord &mov, Coord &pre_mov) -> bool
			{
				mov = GoWrapper(Coord{y,x});

				if (mov.y > 7 || mov.y < 0 || mov.x > 7 || mov.x < 0) return false;
				if (!grid[y][x]->Rule(Coord{y,x}, mov, grid)) return false;


				sub_grid[mov.y][mov.x] = move(sub_grid[pre_mov.y][pre_mov.x]);
				sub_grid[pre_mov.y][pre_mov.x] = nullptr;

				if (GoWrapper == Walker::GoKing) {
					if (!Check(mov, mov, sub_grid)) {
						mov = GoWrapper(Coord{-1,-1});//reset
						return true;
					}
				}
				else if (!Check(king, king, sub_grid)) {
					mov = GoWrapper(Coord{-1,-1});//reset
					return true;
				}

				pre_mov = mov;
				return false;
			};

			if (grid[y][x] == nullptr) continue;
			if (grid[y][x]->GetColr() != side) continue;

			auto sub_grid = grid;

			if (grid[y][x]->GetName() == L'P') {
				while (mov) if (CheckDodge(Walker::GoPawn, sub_grid, mov, pre_mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'K') {
				while (mov) if (CheckDodge(Walker::GoKing, sub_grid, mov, pre_mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'N') {
				while (mov)
					if (CheckDodge(Walker::GokNight, sub_grid, mov, pre_mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'R') {
				while (mov)
					if (CheckDodge(Walker::GoRook, sub_grid, mov, pre_mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'B') {
				while (mov)
					if (CheckDodge(Walker::GoBishop, sub_grid, mov, pre_mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'Q') {
				while (mov)
					if (CheckDodge(Walker::GoQueen, sub_grid, mov, pre_mov)) return false;
			}
		}

	return true;
}

const bool Desk::Stalemate(const wchar_t side)
{
	assert(side == L'W' || side == L'B');

	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
		{
			auto CanMove = [&] (Coord(*GoWrapper)(const Coord pos), Coord &mov) -> bool
			{
				mov = GoWrapper(Coord{y,x});

				if (mov.y > 7 || mov.y < 0 || mov.x > 7 || mov.x < 0) return false;

				if (grid[y][x]->Rule(Coord{y,x}, mov, grid)) {
					mov = GoWrapper(Coord{-1,-1});//reset
					return true;
				}

				return false;
			};

			Coord mov{0,0};
			if (grid[y][x] == nullptr) continue;
			if (grid[y][x]->GetColr() != side) continue;

			if (grid[y][x]->GetName() == L'K') {
				while (mov) if (CanMove(Walker::GoKing, mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'P') {
				while (mov) if (CanMove(Walker::GoPawn, mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'R') {
				while (mov) if (CanMove(Walker::GoRook, mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'B') {
				while (mov) if (CanMove(Walker::GoBishop, mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'N') {
				while (mov) if (CanMove(Walker::GokNight, mov)) return false;
			}
			else if (grid[y][x]->GetName() == L'Q') {
				while (mov) if (CanMove(Walker::GoQueen, mov)) return false;
			}
		}

	return true;
}
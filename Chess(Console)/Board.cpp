#pragma once
#include "Header.h"
#include "Board.h"

void Desk::PreciseUpdate(const Coord from, const Coord to)
{
	//assert((from != to && grid[to.y][to.x] != nullptr) && L"cell for pice write must not be empty");

	size_t cell = (FIRST_CELL_Y + from.y * 2) * WIDTH + FIRST_CELL_X + from.x * 3;//Update 'from' cell
	if ((from.x + from.y) % 2 == 1) { //if cell should be painted - paint
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
		en_passant = Coord{(from.y + to.y) / 2, from.x};

		SetPice(to, grid[from.y][from.x]);
		SetPice(en_passant, new EnPawn(grid[from.y][from.x]->GetColr()));
		grid[from.y][from.x] = nullptr;


		PreciseUpdate(from, to);//for Pawn under passant
		PreciseUpdate(from, en_passant);//for tooltip show
	}

	return enpass;
}

void Desk::ErasePice(const Coord pos) {
	assert(pos.x >= 0 && pos.y >= 0 && L"Coord must be positive");
	assert(pos.x < 9 && pos.y < 9 && L"Coord must be less then 9");
	assert(grid[pos.y][pos.x] != nullptr && L"Pice for erase must exist");

	delete grid[pos.y][pos.x];
	grid[pos.y][pos.x] = nullptr;
}

void Desk::SetPice(const Coord pos, ChessPice *pice) {
	assert(pos.x >= 0 && pos.y >= 0 && L"Coord must be positive");
	assert(pos.x < 9 && pos.y < 9 && L"Coord must be less then 9");
	assert(pice != nullptr && L"Pice for insert must exist");

	if (grid[pos.y][pos.x] != nullptr) delete grid[pos.y][pos.x];
	grid[pos.y][pos.x] = move(pice);
	//pice = nullptr;//!!! make 'pice' invalid after SetPice() call!!!
}

bool Desk::Move(const Coord from, const Coord to)
{
	if (!grid[from.y][from.x]->Rule(from, to, grid)) return false;//rule violation check

	if (IfEnPassant(from, to)) return true;//for intercepted
	else if (grid[from.y][from.x]->GetName() == L'P' && to == en_passant)//if interceptor
	{
		Coord intercepted{from.y, en_passant.x};

		SetPice(to, grid[from.y][from.x]);
		grid[from.y][from.x] = nullptr;

		ErasePice(intercepted);

		PreciseUpdate(from, to);
		PreciseUpdate(intercepted, intercepted);//for intercepted's cell painting

		return true;
	}
	else if (grid[from.y][from.x]->GetName() == L'P' && (to.y == 0 || to.y == 7))//if Pawn gets promotion.
	{
		switch (GetInput())
		{
		case L'1':
			if (grid[from.y][from.x]->GetColr() == L'W') SetPice(to, new WkNight);
			else SetPice(to, new BkNight);
			break;

		case L'2':
			if (grid[from.y][from.x]->GetColr() == L'W') SetPice(to, new WRook);
			else SetPice(to, new BRook);
			break;

		case L'3':
			if (grid[from.y][from.x]->GetColr() == L'W') SetPice(to, new WBishop);
			else SetPice(to, new BBishop);
			break;

		default://Queen
			if (grid[from.y][from.x]->GetColr() == L'W') SetPice(to, new WQueen);
			else SetPice(to, new BQueen);
			break;
		}

		ErasePice(from);
		PreciseUpdate(from, to);
	}
	else if (grid[from.y][from.x]->GetName() == L'K')
	{
		if (dist(from.x, to.x) == 2)//if castling
		{
			Coord rook_from, rook_to;

			static_cast<King*>(grid[from.y][from.x])->YesMoved();
			SetPice(to, grid[from.y][from.x]);//move the King
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
			SetPice(rook_to, grid[rook_from.y][rook_from.x]);
			grid[rook_from.y][rook_from.x] = nullptr;
			PreciseUpdate(rook_from, rook_to);
		}
		else {//King just moves
			static_cast<King*>(grid[from.y][from.x])->YesMoved();
			SetPice(to, grid[from.y][from.x]);
			grid[from.y][from.x] = nullptr;
		}

		PreciseUpdate(from, to);

		if (grid[to.y][to.x]->GetColr() == L'W') wking = Coord(to);
		else bking = Coord(to);
	}
	else //default situation
	{
		if (grid[from.y][from.x]->GetName() == L'R') //if Rook moves
			static_cast<Rook*>(grid[from.y][from.x])->YesMoved();

		SetPice(to, grid[from.y][from.x]);
		grid[from.y][from.x] = nullptr;

		PreciseUpdate(from, to);
	}

	if (en_passant) {//if ability expired
		if (grid[en_passant.y][en_passant.x]->GetName() == L'p') {
			ErasePice(en_passant);
			PreciseUpdate(en_passant, en_passant);
		}
		en_passant = {-1, -1};
	}
	return true;
}

unsigned short Desk::GetInput() {
	wcout << L"Promotion: which pice do you choose:\n"
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
			else if ((y + x) % 2 == 0) map[cell + 1] = map[cell] = L' ';
			else map[cell + 1] = map[cell] = L'█';
		}
}

void Desk::Clear()
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
}

void Desk::Restart() {
	Clear();
	DefaultPlacement();
	Refresh();
}

const bool Desk::Save() {
	
}
bool Desk::Load() {

}

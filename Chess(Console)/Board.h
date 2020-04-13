#pragma once
#include "Header.h"
#include "Pieces.h"

class Desk {
	wstring map;
	grid_t grid{};
	Coord en_passant{-1,-1};
	Coord bking{-1,-1}, wking{-1,-1};

private:

	void DefaultPlacement();

	void PreciseUpdate(const Coord from, const Coord to);//of the map

	bool IfEnPassant(const Coord from, const Coord to);

	unsigned short GetInput();
public://main logic

	void ErasePiece(const Coord pos);
	void SetPiece(const Coord pos, ChessPiece *piece);
	bool Move(const Coord from, const Coord to);

	const bool Check(const Coord from, const Coord to, const grid_t ngrid);
	const bool Checkmate(const Coord king);//Hooray!
	const bool Stalemate(const wchar_t side);
public:

	const bool Save();
	bool Load();

	const auto& Layout() { return grid; }
	const auto& Mapout() { return map; }
	
	const void Print();

	void ClearBoard();
	void Refresh();
	void Start() { Restart(); }
	void Restart();

public:
	Desk() : map(MAKE_BOARD().data()) {}
};
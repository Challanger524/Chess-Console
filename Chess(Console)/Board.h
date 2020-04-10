#pragma once
#include "Header.h"
#include "Pices.h"

class Desk {
	wstring map;
	grid_t grid{};
	Coord en_passant{-1,-1};
	Coord bking, wking;

private:

	void DefaultPlacement();

	void PreciseUpdate(const Coord from, const Coord to);//of the map

	bool IfEnPassant(const Coord from, const Coord to);

	unsigned short GetInput();
public://main logic

	void ErasePice(const Coord pos);
	void SetPice(const Coord pos, ChessPice *pice);
	bool Move(const Coord from, const Coord to);

	const bool Check();//
	const bool Checkmate();//
public:

	const bool Save();//
	bool Load();//

	const auto& Layout() { return grid; }
	const auto& Mapout() { return map; }
	
	const void Print();

	void Clear();
	void Refresh();
	void Start() { Restart(); }
	void Restart();

public:
	Desk() : map(MAKE_BOARD().data()) {}
	//Desk(const Desk&) = delete;
};
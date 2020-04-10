#pragma once
#include "Header.h"


class ChessPice {

protected:
	using grid_t = array<array<ChessPice*, CELLS>, CELLS>;

	wchar_t name{};
	wchar_t color{};

	const bool BeatAllies(const Coord from, const Coord to, const grid_t &grid);
public:
	const wchar_t GetName() { return name; }
	const wchar_t GetColr() { return color; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) = 0;
};

using grid_t = array<array<ChessPice*, CELLS>, CELLS>;


class Pawn : public ChessPice {
public:
	Pawn() { name = L'P'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override = 0;
};

class EnPawn : public Pawn {
public:
	EnPawn() = delete;

	const bool virtual Rule(const Coord from[[maybe_unused]], const Coord to[[maybe_unused]], const grid_t &grid[[maybe_unused]]) override { return false; }

	EnPawn(const wchar_t side) {
		assert((side == L'W' || side == L'B') && L"Invalid color of EnPawn");
		name = L'p';
		color = side;
	}
};
class WPawn : public Pawn {
public:
	WPawn() { color = L'W'; }

	const bool Rule(const Coord from, const Coord to, const grid_t &grid) override final {
		if (from.y > to.y) return false;//if backwards(upwards)
		if (!Pawn::Rule(from, to, grid)) return false; //default rules for all Pawns (and Pices)
		return true;
	}
};
class BPawn : public Pawn {
public:
	BPawn() { color = L'B'; }

	const bool Rule(const Coord from, const Coord to, const grid_t &grid) override final {
		if (from.y < to.y) return false;//if backwards(downwards)
		if (!Pawn::Rule(from, to, grid)) return false; //default rules for all Pawns
		return true;
	}
};


class Queen : public ChessPice {
public:
	Queen() { name = L'Q'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override = 0;
};

class WQueen : public Queen {
public:
	WQueen() { color = L'W'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final { return Queen::Rule(from, to, grid); }
};
class BQueen : public Queen {
public:
	BQueen() { color = L'B'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final { return Queen::Rule(from, to, grid); }
};


class King : public ChessPice {
protected:
	bool moved = false;
public:
	King() { name = L'K'; }

	const bool IfMoved() { return moved; }
	void YesMoved() { moved = true; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override = 0;
};

class WKing : public King {
public:
	WKing() { color = L'W'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final { return King::Rule(from, to, grid); }
};
class BKing : public King {
public:
	BKing() { color = L'B'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final { return King::Rule(from, to, grid); }
};


class Rook : public ChessPice {
	bool moved = false;
public:
	Rook() { name = L'R'; }

	const bool IfMoved() { return moved; }
	void YesMoved() { moved = true; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override = 0;
};

class WRook : public Rook {
public:
	WRook() { color = L'W'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final { return Rook::Rule(from, to, grid); }
};
class BRook : public Rook {
public:
	BRook() { color = L'B'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final { return Rook::Rule(from, to, grid); }
};
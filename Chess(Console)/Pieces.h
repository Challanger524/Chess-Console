#pragma once
#include "Header.h"


class ChessPiece {

protected:
	using grid_t = array<array<ChessPiece*, CELLS>, CELLS>;

	wchar_t name{};
	wchar_t color{};

	const bool BeatAllies(const Coord from, const Coord to, const grid_t &grid);
public:
	const wchar_t GetName() { return name; }
	const wchar_t GetColr() { return color; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) = 0;
};

using grid_t = array<array<ChessPiece*, CELLS>, CELLS>;


class Pawn : public ChessPiece {
public:
	Pawn() { name = L'P'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override = 0;
};

class EnPawn : public Pawn {
public:
	EnPawn() = delete;

	const bool virtual Rule(const Coord from[[maybe_unused]], const Coord to[[maybe_unused]], const grid_t &grid[[maybe_unused]]) override final{ return false; }

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
		if (!Pawn::Rule(from, to, grid)) return false; //default rules for all Pawns (and Pieces)
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


class Queen : public ChessPiece {
public:
	Queen() { name = L'Q'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final;
};

class WQueen : public Queen {
public:
	WQueen() { color = L'W'; }
};
class BQueen : public Queen {
public:
	BQueen() { color = L'B'; }
};


class King : public ChessPiece {
protected:
	bool moved = false;
public:
	King() { name = L'K'; }

	const bool IfMoved() { return moved; }
	void YesMoved() { moved = true; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final;
};

class WKing : public King {
public:
	WKing() { color = L'W'; }
};
class BKing : public King {
public:
	BKing() { color = L'B'; }
};


class Rook : public ChessPiece {
	bool moved = false;
public:
	Rook() { name = L'R'; }

	const bool IfMoved() { return moved; }
	void YesMoved() { moved = true; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final;
};

class WRook : public Rook {
public:
	WRook() { color = L'W'; }
};
class BRook : public Rook {
public:
	BRook() { color = L'B'; }
};


class Bishop : public ChessPiece {
public :
	Bishop() { name = L'B'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final;
};

class WBishop : public Bishop {
public:
	WBishop() { color = L'W'; }
};
class BBishop : public Bishop {
public:
	BBishop() { color = L'B'; }
};


class kNight : public ChessPiece {
public:
	kNight() { name = L'N'; }

	const bool virtual Rule(const Coord from, const Coord to, const grid_t &grid) override final;
};

class WkNight : public kNight {
public:
	WkNight() { color = L'W'; }
};
class BkNight : public kNight {
public:
	BkNight() { color = L'B'; }
};

struct Walker {
	static Coord GoKing(const Coord pos);
	static Coord GoPawn(const Coord pos);
	static Coord GokNight(const Coord pos);
	static Coord GoRook(const Coord pos);
	static Coord GoBishop(const Coord pos);
	static Coord GoQueen(const Coord pos);
};
#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <functional>

#include <fcntl.h>
#include <io.h>
#include <cassert>
#include <cmath>
using namespace std;

constexpr unsigned short MARKS = 2, BOARDERS = 9, CELLS = 8, COLORS = 2, NEWLINE = 1, NULLTERMINATOR = 1;
constexpr size_t WIDTH = MARKS + BOARDERS + CELLS * 2 + NEWLINE;
constexpr size_t HEIGHT = COLORS + MARKS + BOARDERS + CELLS;
constexpr size_t LENGTH = HEIGHT * WIDTH + NULLTERMINATOR;

constexpr size_t CELL_ITER_Y = 1 + BOARDERS / 9;
constexpr size_t CELL_ITER_X = 2 + BOARDERS / 9;


constexpr size_t FIRST_CELL_Y = COLORS / 2 + MARKS / 2 + BOARDERS / 9;
constexpr size_t FIRST_CELL_X = MARKS / 2 + BOARDERS / 9;

constexpr auto MAKE_BOARD(void) {
	array<wchar_t, LENGTH> grid = {
		L"           White           \n"
		L"   a  b  c  d  e  f  g  h  \n"
		L" ╔══╤══╤══╤══╤══╤══╤══╤══╗ \n"
		L"1║  │  │  │  │  │  │  │  ║1\n"
		L" ╟──┼──┼──┼──┼──┼──┼──┼──╢ \n"
		L"2║  │  │  │  │  │  │  │  ║2\n"
		L" ╟──┼──┼──┼──┼──┼──┼──┼──╢ \n"
		L"3║  │  │  │  │  │  │  │  ║3\n"
		L" ╟──┼──┼──┼──┼──┼──┼──┼──╢ \n"
		L"4║  │  │  │  │  │  │  │  ║4\n"
		L" ╟──┼──┼──┼──┼──┼──┼──┼──╢ \n"
		L"5║  │  │  │  │  │  │  │  ║5\n"
		L" ╟──┼──┼──┼──┼──┼──┼──┼──╢ \n"
		L"6║  │  │  │  │  │  │  │  ║6\n"
		L" ╟──┼──┼──┼──┼──┼──┼──┼──╢ \n"
		L"7║  │  │  │  │  │  │  │  ║7\n"
		L" ╟──┼──┼──┼──┼──┼──┼──┼──╢ \n"
		L"8║  │  │  │  │  │  │  │  ║8\n"
		L" ╚══╧══╧══╧══╧══╧══╧══╧══╝ \n"
		L"   a  b  c  d  e  f  g  h  \n"
		L"           Black           \n"
	};

	for (size_t y = 0; y < CELLS; y++) //painting cells
		for (size_t x = 0; x < CELLS; x++) {
			if ((y + x) % 2 == 0) continue;
			size_t cell = (FIRST_CELL_Y + y * 2) * WIDTH + FIRST_CELL_X + x * 3;
			grid[cell] = L'█';
			grid[cell + 1] = L'█';
		}

	return grid;
}

class Timer {
	std::chrono::time_point<std::chrono::steady_clock> start;
public:
	Timer() : start{std::chrono::steady_clock::now()} {}

	Timer(const Timer&) = delete;

	Timer operator = (const Timer&) = delete;

	~Timer() {
		wcout << L"\nTimer : " << static_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start).count() * 1000 << "ms\n";
	}

	std::chrono::duration<float> get() {
		return std::chrono::steady_clock::now() - start;
	}//sec

	operator std::chrono::duration<float>() const {
		return std::chrono::steady_clock::now() - start;
	}

	void Lap() {
		wcout << L"\nLap : " << static_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start).count() * 1000 << "ms\n";
	}
};

//Coord {y, x};
struct Coord {
	int y;
	int x;
	friend bool operator== (const Coord &lhs, const Coord &rhs)
	{ return (lhs.x == rhs.x) && (lhs.y == rhs.y); }
	operator bool() { return x != -1 && y != -1; }
};

//get distance btween
int dist(const int fir, const int sec);

//void Refresh(wstring& map);

class Player {
protected:
	wchar_t side{};
public:
	const wchar_t GetSide() { return side; }
	void virtual Turn() = 0;
};

class WPlayer : public Player {
public:
	WPlayer() { side = L'W'; }
	void Turn() override { wcout << L"White turn: "; }
};

class BPlayer : public Player {
public:
	BPlayer() { side = L'B'; }
	void Turn() override { wcout << L"Black turn: "; }
};

void swap(Player * const &lhs, Player * const &rhs);
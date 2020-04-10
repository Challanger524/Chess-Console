#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Chess(console)\Header.h"
//#include "..\Chess(console)\Source.cpp"
#include "..\Chess(console)\UInterface.cpp"
#include "..\Chess(console)\Pices.cpp"
#include "..\Chess(console)\Board.h"
#include "..\Chess(console)\Board.cpp"
#include "..\Chess(console)\Main.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_for_Chess
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(CorrectMoveReqest)
		{
			Assert::IsTrue(CorrectMoveReq(wstring(L"c1c2")), L"[ASSERT]: Move input must be correct");
			Assert::IsTrue(CorrectMoveReq(wstring(L"a6c3")), L"[ASSERT]: Move input must be correct");
			Assert::IsTrue(CorrectMoveReq(wstring(L"c7c8")), L"[ASSERT]: Move input must be correct");

			Assert::IsFalse(CorrectMoveReq(wstring(L"a1c")), L"[ASSERT]: Move input must be too short");
			Assert::IsFalse(CorrectMoveReq(wstring(L"a1c10")), L"[ASSERT]: Move input must be too long");
			Assert::IsFalse(CorrectMoveReq(wstring(L"j1c1")), L"[ASSERT]: Move input must be out of bounds");
			Assert::IsFalse(CorrectMoveReq(wstring(L"c9c1")), L"[ASSERT]: Move input must be out of bounds");

			Assert::IsFalse(CorrectMoveReq(wstring(L"!9c1")), L"[ASSERT]: Move input must be incorrect");
			Assert::IsFalse(CorrectMoveReq(wstring(L"ccc1")), L"[ASSERT]: Move input must be incorrect");
			Assert::IsFalse(CorrectMoveReq(wstring(L"11c1")), L"[ASSERT]: Move input must be incorrect");
		}

		TEST_METHOD(GameInput)
		{
			wstring test(L" a3 b4 ");

			for (size_t i = 0; i < test.size(); i++) if (test[i] == L' ') test.erase(i, 1);

			Assert::AreEqual(wstring(L"a3b4"), test, L"[ASSERT]: Function had to erase 'spacebar'");
		}

		TEST_METHOD(Distance_Test)//int dist();
		{
			Assert::AreEqual(2, dist(2, 4));
			Assert::AreEqual(2, dist(2, 0));
			Assert::AreEqual(2, dist(-2, -4));
			Assert::AreEqual(2, dist(4, 2));
		}
	};

	TEST_CLASS(Pawn_Movement_Vlidation)
	{
	public:
		TEST_METHOD(Valid_Movement)
		{
			Desk board;
			board.Start();

			/*board.SetPice(Coord{1,1}, new WPawn);
			board.SetPice(Coord{2,2}, new WPawn);
			board.SetPice(Coord{2,1}, new WPawn);*/

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{1,1}, Coord{3,1}));//try to beat front
			//Assert::IsTrue(board.Move(Coord{1,1}, Coord{3,1}));//try to overjump

			Logger::WriteMessage(board.Mapout().c_str());

		}

		TEST_METHOD(Front_Enemy)
		{
			Desk board;

			board.SetPice(Coord{1,1}, new WPawn);
			board.SetPice(Coord{2,1}, new BPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsFalse(board.Move(Coord{1,1}, Coord{2,1}), L"Pawns cannot beat frontally");//try to beat front
			Assert::IsFalse(board.Move(Coord{1,1}, Coord{3,1}), L"Pawns cannot overjump");//try to overjump

			//Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Front_Allies)
		{
			Desk board;

			board.SetPice(Coord{1,1}, new WPawn);
			board.SetPice(Coord{2,2}, new WPawn);
			board.SetPice(Coord{2,1}, new WPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsFalse(board.Move(Coord{1,1}, Coord{2,2}), L"Pices cannot beat allies");//try to beat front
			Assert::IsFalse(board.Move(Coord{1,1}, Coord{3,1}), L"Pawns cannot overjump");//try to overjump

			//Logger::WriteMessage(board.Mapout().c_str());
		}
	};

	TEST_CLASS(EnPassan_Mechanics)
	{
	public:
		TEST_METHOD(EnPassan_Mark_Creation)
		{
			Desk board;

			board.SetPice(Coord{1,1}, new WPawn);
			board.SetPice(Coord{3,2}, new BPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{1,1}, Coord{3,1}));

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(EnPassan_Intercepting)
		{
			Desk board;

			board.SetPice(Coord{1,1}, new WPawn);
			board.SetPice(Coord{3,2}, new BPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{1,1}, Coord{3,1}));
			Logger::WriteMessage(board.Mapout().c_str());
			Assert::IsTrue(board.Move(Coord{3,2}, Coord{2,1}));

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(EnPassan_Bypassing)
		{
			Desk board;

			board.SetPice(Coord{1,1}, new WPawn);
			board.SetPice(Coord{3,2}, new BPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{1,1}, Coord{3,1}));
			Logger::WriteMessage(board.Mapout().c_str());
			Assert::IsTrue(board.Move(Coord{3,2}, Coord{2,2}));

			//board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());
		}
	};

	unsigned short GetInput() {//Fake input
		return L'0';
	}
	TEST_CLASS(Promotion_Mechanics)//reqires input from the user(originally)
	{
		wstring map = MAKE_BOARD().data();
		grid_t grid{};
		Coord en_passant{-1,-1};

		unsigned short GetInput() {
			//wcout << L"Promotion: which pice do you choose:\n"
			//	"0 - Queen";
			return L'b';//  <-- L'<for change>'
		}
		void PreciseUpdate(const Coord from, const Coord to)
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
		bool IfEnPassant(const Coord from, const Coord to)
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
		void ErasePice(const Coord pos) {
			assert(pos.x >= 0 && pos.y >= 0 && L"Coord must be positive");
			assert(pos.x < 9 && pos.y < 9 && L"Coord must be less then 9");
			assert(grid[pos.y][pos.x] != nullptr && L"Pice for erase must exist");

			delete grid[pos.y][pos.x];
			grid[pos.y][pos.x] = nullptr;
		}
		void SetPice(const Coord pos, ChessPice *pice) {
			assert(pos.x >= 0 && pos.y >= 0 && L"Coord must be positive");
			assert(pos.x < 9 && pos.y < 9 && L"Coord must be less then 9");
			assert(pice != nullptr && L"Pice for insert must exist");

			if (grid[pos.y][pos.x] != nullptr) delete grid[pos.y][pos.x];
			grid[pos.y][pos.x] = move(pice);
			//pice = nullptr;//!!! make 'pice' invalid after SetPice() call!!!
		}
		bool Move(const Coord from, const Coord to)
		{
			if (!grid[from.y][from.x]->Rule(from, to, grid)) return false;// rule violation check

			if (IfEnPassant(from, to)) return true;//for intercepted
			else if (grid[from.y][from.x]->GetName() == L'P' && to == en_passant)//for interceptors
			{
				Coord intercepted{from.y, en_passant.x};

				SetPice(to, grid[from.y][from.x]);
				grid[from.y][from.x] = nullptr;

				ErasePice(intercepted);

				PreciseUpdate(from, to);
				PreciseUpdate(intercepted, intercepted);//for intercepted's cell painting
				return true;
			}
			else if (grid[from.y][from.x]->GetName() == L'P' && to.y == 0 || to.y == 7) // if Pawn gets promotion.
			{
				switch (GetInput()) {
				default :
					if (grid[from.y][from.x]->GetColr() == L'W') SetPice(to, new WQueen);
					else SetPice(to, new BQueen);
					break;
				}

				ErasePice(from);
				PreciseUpdate(from, to);
			}
			else {//default situation
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
		void Refresh() {
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
	
public:
		TEST_METHOD(Pawn_Promotion)
		{
			grid[6][4] = new WPawn;

			Refresh();

			Logger::WriteMessage(map.c_str());

			Assert::IsTrue(Move(Coord{6,4}, Coord{7,4}));;
			Logger::WriteMessage(map.c_str());

			Assert::IsTrue(Move(Coord{7,4}, Coord{5,2}));
			Logger::WriteMessage(map.c_str());

			Assert::IsTrue(Move(Coord{5,2}, Coord{5,6}));
			Logger::WriteMessage(map.c_str());
		}
	};
}
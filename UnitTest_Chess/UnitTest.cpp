﻿#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Chess(console)\Header.h"
#include "..\Chess(console)\UInterface.cpp"
#include "..\Chess(console)\Pieces.cpp"
#include "..\Chess(console)\Board.h"
#include "..\Chess(console)\Board.cpp"
#include "..\Chess(console)\Main.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_for_Chess
{
	TEST_CLASS(InputAndDistance)
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

		TEST_METHOD(Dispatch_Test)
		{
			Assert::AreEqual(static_cast<int>(Invalid), Dispatch(L"wrong command"), L"Dispatch must be correct");
			Assert::AreEqual(static_cast<int>(Exit), Dispatch(L" exit"), L"Dispatch must be correct");
			Assert::AreEqual(static_cast<int>(Empty), Dispatch(L"    "), L"Dispatch must be correct");
			Assert::AreEqual(static_cast<int>(Help), Dispatch(L"  help"), L"Dispatch must be correct");
			Assert::AreEqual(static_cast<int>(Start), Dispatch(L"start  "), L"Dispatch must be correct");
			Assert::AreEqual(static_cast<int>(Restart), Dispatch(L"   restart"), L"Dispatch must be correct");
			Assert::AreEqual(static_cast<int>(Save), Dispatch(L"save   "), L"Dispatch must be correct");
			Assert::AreEqual(static_cast<int>(Load), Dispatch(L"  load  "), L"Dispatch must be correct");
			Assert::AreEqual(static_cast<int>(Move), Dispatch(L" abcd "), L"Dispatch must be correct(dispite it is invalid move)");
			Assert::AreEqual(static_cast<int>(Move), Dispatch(L"b1b3 "), L"Dispatch must be correct");
		}

		TEST_METHOD(Distance_Test)//int dist();
		{
			Assert::AreEqual(2, dist(2, 4));
			Assert::AreEqual(2, dist(2, 0));
			Assert::AreEqual(2, dist(-2, -4));
			Assert::AreEqual(2, dist(4, 2));
		}
	};

	
	TEST_CLASS(Pawn_Movement)
	{
	public:
		TEST_METHOD(Valid_Movement)
		{
			Desk board;

			board.SetPiece(Coord{1,1}, new WPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{1,1}, Coord{3,1}), L"First double move");
			Assert::IsTrue(board.Move(Coord{3,1}, Coord{4,1}), L"Just move");
			Assert::IsFalse(board.Move(Coord{4,1}, Coord{3,1}), L"Just wrong move backwards");
			Assert::IsFalse(board.Move(Coord{4,1}, Coord{4,2}), L"Just wrong move aside");
			Assert::IsFalse(board.Move(Coord{4,1}, Coord{5,2}), L"Just wrong move diagonally");

			//Logger::WriteMessage(board.Mapout().c_str());

		}

		TEST_METHOD(Front_Enemy)
		{
			Desk board;

			board.SetPiece(Coord{1,1}, new WPawn);
			board.SetPiece(Coord{2,1}, new BPawn);
			board.SetPiece(Coord{2,2}, new BPawn);


			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsFalse(board.Move(Coord{1,1}, Coord{2,1}), L"Pawns cannot beat frontally");//try to beat front
			Assert::IsFalse(board.Move(Coord{1,1}, Coord{3,1}), L"Pawns cannot overjump");//try to overjump
			Assert::IsTrue(board.Move(Coord{1,1}, Coord{2,2}), L"Pawns can beat deagonally");

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Front_Allies)
		{
			Desk board;

			board.SetPiece(Coord{1,1}, new WPawn);
			board.SetPiece(Coord{2,2}, new WPawn);
			board.SetPiece(Coord{2,1}, new WPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsFalse(board.Move(Coord{1,1}, Coord{2,2}), L"Pieces cannot beat allies");//try to beat front
			Assert::IsFalse(board.Move(Coord{1,1}, Coord{3,1}), L"Pawns cannot overjump");//try to overjump allies

			//Logger::WriteMessage(board.Mapout().c_str());
		}
	};

	TEST_CLASS(Rook_Movement)
	{
	public:
		TEST_METHOD(Valid_Movement)
		{
			Desk board;

			board.SetPiece(Coord{3,4}, new BRook);

			board.Refresh();
			//Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{3,4}, Coord{3,6}), L"Just move");
			Assert::IsTrue(board.Move(Coord{3,6}, Coord{1,6}), L"Just move");
			Assert::IsFalse(board.Move(Coord{1,6}, Coord{2,5}), L"Just wrong move diagonally");
			Assert::IsFalse(board.Move(Coord{1,6}, Coord{4,2}), L"Just wrong move ");

			//Logger::WriteMessage(board.Mapout().c_str());

		}

		TEST_METHOD(Front_Enemy)
		{
			Desk board;

			board.SetPiece(Coord{1,1}, new WRook);
			board.SetPiece(Coord{2,1}, new BPawn);
			board.SetPiece(Coord{2,2}, new BPawn);


			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsFalse(board.Move(Coord{1,1}, Coord{2,2}), L"Rooks cannot beat diagonally");
			Assert::IsFalse(board.Move(Coord{1,1}, Coord{3,1}), L"Rook cannot overjump");
			Assert::IsTrue(board.Move(Coord{1,1}, Coord{2,1}), L"Rook can beat vertically");

			Logger::WriteMessage(board.Mapout().c_str());
		}
	};

	TEST_CLASS(Bishop_Movement)
	{
	public:
		TEST_METHOD(Valid_Movement)
		{
			Desk board;

			board.SetPiece(Coord{0,0}, new WBishop);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{0,0}, Coord{5,5}), L"Just move");
			Assert::IsTrue(board.Move(Coord{5,5}, Coord{4,6}), L"Just move");
			Assert::IsFalse(board.Move(Coord{4,6}, Coord{4,5}), L"Just wrong move ");
			Assert::IsFalse(board.Move(Coord{4,6}, Coord{3,4}), L"Just wrong move ");

			//Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Front_Enemy)
		{
			Desk board;

			board.SetPiece(Coord{1,1}, new WBishop);
			board.SetPiece(Coord{2,1}, new BPawn);
			board.SetPiece(Coord{2,2}, new BPawn);


			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsFalse(board.Move(Coord{1,1}, Coord{2,1}), L"Bishop cannot beat aside");
			Assert::IsFalse(board.Move(Coord{1,1}, Coord{3,3}), L"Bishop cannot overjump");
			Assert::IsTrue(board.Move(Coord{1,1}, Coord{2,2}), L"Bishop can beat diagonally");

			Logger::WriteMessage(board.Mapout().c_str());
		}
	};

	TEST_CLASS(kNight_Movement)
	{
	public:
		TEST_METHOD(Valid_Movement)
		{
			Desk board;

			board.SetPiece(Coord{0,0}, new WkNight);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{0,0}, Coord{2,1}), L"Just move");

			Assert::IsTrue(board.Move(Coord{2,1}, Coord{1,3}), L"Just move");
			Assert::IsFalse(board.Move(Coord{1,3}, Coord{4,5}), L"Just wrong move ");
			Assert::IsFalse(board.Move(Coord{1,3}, Coord{3,7}), L"Just wrong move ");

			//Logger::WriteMessage(board.Mapout().c_str());

		}

		TEST_METHOD(Front_Enemy)
		{
			Desk board;

			board.SetPiece(Coord{0,0}, new WkNight);
			board.SetPiece(Coord{0,1}, new WBishop);
			board.SetPiece(Coord{1,0}, new WPawn);
			board.SetPiece(Coord{1,1}, new WPawn);

			board.SetPiece(Coord{2,1}, new BPawn);
			board.SetPiece(Coord{2,2}, new BPawn);


			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{0,0}, Coord{2,1}), L"Knight can overjump and beat right down down");
			Assert::IsFalse(board.Move(Coord{2,1}, Coord{2,2}), L"Knight cannot beat aside");

			Logger::WriteMessage(board.Mapout().c_str());
		}
	};

	TEST_CLASS(Queen_Movement)//!
	{
	public:
		TEST_METHOD(Valid_Movement)
		{
			Desk board;

			board.SetPiece(Coord{3,3}, new WQueen);
			board.SetPiece(Coord{5,5}, new BQueen);
			board.SetPiece(Coord{3,5}, new BRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{3,3}, Coord{5,5}), L"Queen can move & beat diagonally");
			Assert::IsTrue(board.Move(Coord{5,5}, Coord{3,5}), L"Queen can move & beat vertically");
			Assert::IsFalse(board.Move(Coord{3,5}, Coord{2,2}), L"Queen cannot move that way");

			Logger::WriteMessage(board.Mapout().c_str());

		}
	};

	TEST_CLASS(King_Movement_and_Castling)//!
	{
	public:
		TEST_METHOD(Castling_Test)
		{
			Desk board;

			board.SetPiece(Coord{7,4}, new BKing);
			board.SetPiece(Coord{7,0}, new BRook);
			board.SetPiece(Coord{7,7}, new WRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsFalse(board.Move(Coord{7,4}, Coord{7,6}), L"Cannot castle with enemy Rook");

			board.ErasePiece(Coord{7,7});//erase White Rook to avoid Check
			board.Refresh();

			Assert::IsTrue(board.Move(Coord{7,4}, Coord{7,2}), L"Just Castle");

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Valid_Movement)
		{
			Desk board;

			board.SetPiece(Coord{0,4}, new WKing);
			board.SetPiece(Coord{0,7}, new WRook);
			board.SetPiece(Coord{1,3}, new BPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{0,4}, Coord{1,3}), L"King can move diagonally & beat");
			Assert::IsTrue(board.Move(Coord{1,3}, Coord{0,3}), L"King can move vertically");
			Assert::IsTrue(board.Move(Coord{0,3}, Coord{0,4}), L"King can move horizonally");

			Assert::IsFalse(board.Move(Coord{0,4}, Coord{0,6}), L"King cannot Castle after movement");

			board.SetPiece(Coord{2,4}, new BPawn);//for Check triggering
			board.Refresh();

			Assert::IsFalse(board.Move(Coord{0,4}, Coord{1,5}), L"King cannot move to a Check position");

			Logger::WriteMessage(board.Mapout().c_str());
		}
	};


	TEST_CLASS(EnPassan_Mechanics)
	{
	public:
		TEST_METHOD(EnPassan_Mark_Creation)
		{
			Desk board;

			board.SetPiece(Coord{1,1}, new WPawn);
			board.SetPiece(Coord{3,2}, new BPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{1,1}, Coord{3,1}));

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(EnPassan_Intercepting)
		{
			Desk board;

			board.SetPiece(Coord{1,1}, new WPawn);
			board.SetPiece(Coord{3,2}, new BPawn);

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

			board.SetPiece(Coord{1,1}, new WPawn);
			board.SetPiece(Coord{3,2}, new BPawn);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Move(Coord{1,1}, Coord{3,1}));
			Logger::WriteMessage(board.Mapout().c_str());
			Assert::IsTrue(board.Move(Coord{3,2}, Coord{2,2}));

			//board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());
		}
	};

	//need update, maybe
	TEST_CLASS(Promotion_Mechanics)//reqires input from the user(originally)
	{
		wstring map = MAKE_BOARD().data();
		grid_t grid{};
		Coord en_passant{-1,-1};
		Coord bking, wking;

		unsigned short GetInput() {//fake input
			//wcout << L"Promotion: which piece do you choose:\n"
			//	"0 - Queen";
			return L'b';//  <-- L'<for change>'
		}
		void PreciseUpdate(const Coord from, const Coord to)
		{
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

				SetPiece(to, grid[from.y][from.x]);
				SetPiece(en_passant, new EnPawn(grid[from.y][from.x]->GetColr()));
				grid[from.y][from.x] = nullptr;


				PreciseUpdate(from, to);//for Pawn under passant
				PreciseUpdate(from, en_passant);//for tooltip show
			}

			return enpass;
		}
		void ErasePiece(const Coord pos) {
			assert(pos.x >= 0 && pos.y >= 0 && L"Coord must be positive");
			assert(pos.x < 9 && pos.y < 9 && L"Coord must be less then 9");
			assert(grid[pos.y][pos.x] != nullptr && L"Piece for erase must exist");

			delete grid[pos.y][pos.x];
			grid[pos.y][pos.x] = nullptr;
		}
		void SetPiece(const Coord pos, ChessPiece *piece) {
			assert(pos.x >= 0 && pos.y >= 0 && L"Coord must be positive");
			assert(pos.x < 9 && pos.y < 9 && L"Coord must be less then 9");
			assert(piece != nullptr && L"Piece for insert must exist");

			if (grid[pos.y][pos.x] != nullptr) delete grid[pos.y][pos.x];
			grid[pos.y][pos.x] = move(piece);
			//piece = nullptr;//!!! make 'piece' invalid after SetPiece() call!!!
		}
		bool Move(const Coord from, const Coord to)
		{
			if (!grid[from.y][from.x]->Rule(from, to, grid)) return false;//rule violation check

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
				switch (GetInput())
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
			else //default situation
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
		void Refresh() {
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

	TEST_CLASS(Check_Mechanic_Test)
	{
		TEST_METHOD(Test_1) {
			Desk board;

			board.SetPiece(Coord{0,4}, new WKing);
			board.SetPiece(Coord{2,3}, new BPawn);
			board.SetPiece(Coord{2,6}, new BkNight);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsFalse(board.Check(Coord{0,4}, Coord{0,4}, board.Layout()), L"There are NO Check");
				
			Assert::IsFalse(board.Move(Coord{0,4}, Coord{1,4}), L"There WILL be a Check");

			Assert::IsTrue(board.Move(Coord{0,4}, Coord{0,3}), L"There WONT be a Check");
		}
	};

	TEST_CLASS(CheckMate_Mechanic_Test)
	{//dont move king, there are function that accepts absolute position that must represent king's current position
		TEST_METHOD(Test_0) {//!!!make king
			Desk board;

			board.SetPiece(Coord{2,2}, new BKing);
			board.SetPiece(Coord{1,4}, new BPawn);

			board.SetPiece(Coord{1,6}, new WRook);
			board.SetPiece(Coord{2,6}, new WRook);
			board.SetPiece(Coord{3,6}, new WRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Check(Coord{2,2}, Coord{2,2}, board.Layout()), L"There are Check");
			Assert::IsFalse(board.Checkmate(Coord{2,2}), L"There are NO CheckMate");//BkNight can cancel attack

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Test_1) {
			Desk board;

			board.SetPiece(Coord{2,2}, new WKing);
			board.SetPiece(Coord{1,3}, new WPawn);

			board.SetPiece(Coord{2,6}, new BRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Check(Coord{2,2}, Coord{2,2}, board.Layout()), L"There are Check");
			Assert::IsFalse(board.Checkmate(Coord{2,2}), L"There are NO CheckMate");//WPawn can attack

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Test_2) {//untill no GOKing()!!!
			Desk board;

			board.SetPiece(Coord{2,2}, new WKing);
			board.SetPiece(Coord{1,3}, new WPawn);

			board.SetPiece(Coord{7,0}, new BQueen);
			board.SetPiece(Coord{2,6}, new BRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Check(Coord{2,2}, Coord{2,2}, board.Layout()), L"There are Check");
			Assert::IsFalse(board.Checkmate(Coord{2,2}), L"There are NO CheckMate");//WPawn can attack

			
			Assert::IsTrue(board.Move(Coord{7,0}, Coord{7,2}), L"BQueen can put a Check on WKing");



			Assert::IsFalse(board.Checkmate(Coord{2,2}), L"King can avoid Check");

			Logger::WriteMessage(board.Mapout().c_str());


			board.SetPiece(Coord{7,0}, new BQueen);
			board.SetPiece(Coord{7,1}, new BQueen);
			board.SetPiece(Coord{7,3}, new BQueen);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Checkmate(Coord{2,2}), L"King cannot avoid CheckMate");

		}

		TEST_METHOD(Test_3) {
			Desk board;

			board.SetPiece(Coord{2,2}, new BKing);
			board.SetPiece(Coord{1,4}, new BkNight);

			board.SetPiece(Coord{1,6}, new WRook);
			board.SetPiece(Coord{2,6}, new WRook);
			board.SetPiece(Coord{3,6}, new WRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Check(Coord{2,2}, Coord{2,2}, board.Layout()), L"There are Check");
			Assert::IsFalse(board.Checkmate(Coord{2,2}), L"There are NO CheckMate");//BkNight can cancel attack

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Test_4) {
			Desk board;

			board.SetPiece(Coord{2,2}, new BKing);
			//board.SetPiece(Coord{1,4}, new BPawn);
			board.SetPiece(Coord{0,4}, new BRook);

			board.SetPiece(Coord{1,6}, new WRook);
			board.SetPiece(Coord{2,6}, new WRook);
			board.SetPiece(Coord{3,6}, new WRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Check(Coord{2,2}, Coord{2,2}, board.Layout()), L"There are Check");
			Assert::IsFalse(board.Checkmate(Coord{2,2}), L"There are NO CheckMate");//BRook can cancel attack

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Test_5) {
			Desk board;

			board.SetPiece(Coord{2,2}, new BKing);
			//board.SetPiece(Coord{0,4}, new BBishop);
			//board.SetPiece(Coord{5,3}, new BBishop);
			board.SetPiece(Coord{1,7}, new BBishop);

			board.SetPiece(Coord{1,6}, new WRook);
			board.SetPiece(Coord{2,6}, new WRook);
			board.SetPiece(Coord{3,6}, new WRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Check(Coord{2,2}, Coord{2,2}, board.Layout()), L"There are Check");
			Assert::IsFalse(board.Checkmate(Coord{2,2}), L"There are NO CheckMate");//BBishop can cancel attack

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Test_6) {
			Desk board;

			board.SetPiece(Coord{2,2}, new BKing);
			//board.SetPiece(Coord{0,4}, new BQueen);
			//board.SetPiece(Coord{5,3}, new BQueen);
			//board.SetPiece(Coord{1,7}, new BQueen);
			board.SetPiece(Coord{2,7}, new BQueen);

			board.SetPiece(Coord{1,6}, new WRook);
			board.SetPiece(Coord{2,6}, new WRook);
			board.SetPiece(Coord{3,6}, new WRook);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Check(Coord{2,2}, Coord{2,2}, board.Layout()), L"There are Check");
			Assert::IsFalse(board.Checkmate(Coord{2,2}), L"There are NO CheckMate");//BBishop can cancel attack

			Logger::WriteMessage(board.Mapout().c_str());
		}

		TEST_METHOD(Full_Board) {
			Desk board;
			board.Start();

			board.SetPiece(Coord{2,3}, new BkNight);

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Check(Coord{0,4}, Coord{0,4}, board.Layout()), L"There are Check");
			Assert::IsFalse(board.Checkmate(Coord{0,4}), L"Pawn can beat the threat");

			Assert::IsFalse(board.Move(Coord{1,2}, Coord{2,2}), L"King is under the Check");

			board.ErasePiece(Coord{1,4});

			board.SetPiece(Coord{5,4}, new BRook);

			Assert::IsTrue(board.Checkmate(Coord{0,4}), L"No Escape");

			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());
		}
	};

	TEST_CLASS(StaleMate_Test)
	{
		TEST_METHOD(Test_1) {
			Desk board;


			board.SetPiece(Coord{1,2}, new WPawn);

			board.SetPiece(Coord{2,2}, new BPawn);


			board.Refresh();
			Logger::WriteMessage(board.Mapout().c_str());

			Assert::IsTrue(board.Stalemate(L'W'), L"White are under a stalemate");

			Assert::IsTrue(board.Stalemate(L'B'), L"Black are under a stalemate");

		}
	};
}
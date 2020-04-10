#include "Header.h"
#include "Board.h"
#include "UInterface.h"

void GameLoop();

int main()
{
	_setmode(_fileno(stdout), _O_U8TEXT);//make console work with Unicode.
	cin.sync_with_stdio(false);
	wcout.precision(2);

	GameLoop();

	system("pause");
	return 0;
}

void GameLoop()
{
	Desk board;
	wstring input;
	Player *current = new WPlayer;
	Player *waiting = new BPlayer;
	bool loop = true;
	bool on_game = false;

	//Refresh(map);

	//wcout << map << endl;
	swap(current, waiting);
	/*wcout << L"The Chess Game\n";
	wcout << L"Message: Type theese commands for interaction:\n";
	PrintHelp();
*/
	while (loop) {
		board.Print();

		if (on_game) {			
			current->Turn();
		}
		else {
			wcout << L"> ";
		}

		getline(wcin, input);
		switch (Dispatch(input))
		{
		case Help:
			system("cls");
			PrintHelp();
			break;

		case Start:
			if (on_game) break;
			board.Start();
			on_game = true;
			break;

		case Restart://
			if (!on_game) break;
			board.Restart();
			on_game = true;
			break;

		case Empty:
			wcout << L"Empty input, try again.\n";
			system("pause");
			break;

		case Invalid:
			wcout << L"Invalid input, try again.\n";
			system("pause");
			break;

		case Exit:
			//system("cls");
			loop = false;
			break;

		default:
		{
			if (!CorrectMoveReq(input)) {
				wcout << L"Message: Incorrect input, try again.\n";
				system("pause");
			}
			else {
				Coord from{input[1] - L'1',input[0] - L'a'};
				Coord to{input[3] - L'1', input[2] - L'a'};

				if (board.Layout()[from.y][from.x] == nullptr) wcout << L"You choosed no pice\n";
				//else if (current->GetSide() != board.Layout()[from.y][from.x]->GetColr()) wcout << L"You tried to move your oponent's pice\n";
				else if (board.Layout()[to.y][to.x] != nullptr && current->GetSide() == board.Layout()[to.y][to.x]->GetColr()) wcout << L"You tried to move onto your own pice\n";
				else {
					if (board.Move(from, to)) {
						//swap(current, waiting);
						continue;
					}
					else wcout << L"Invalid move for the '" << board.Layout()[from.y][from.x]->GetName() << "', try again\n";
				}
				system("pause");
			}
			break;
		}
		}
	}
}

int dist(const int fir, const int sec) { return abs(fir - sec); }

void swap(Player *&lhs, Player *&rhs) {
	Player *temp = move(lhs);
	lhs = move(rhs);
	rhs = move(temp);
}
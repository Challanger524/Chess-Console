#include "UInterface.h"
//#include "Logic.h"

int Dispatch(wstring &input) {
	for (size_t i = 0; i < input.size(); i++)//erase 'spacebars'
		if (input[i] == L' ')
			while (i < input.size() && input[i] == L' ')
				input.erase(i, 1);

	if (input == L"start" || input == L"play") return Start;
	if (input == L"restart") return Restart;
	if (input == L"exit") return Exit;
	if (input == L"help") return Help;

	if (input.empty()) return Empty;
	if (input.size() == 4) return Move;

	return Invalid;
}

bool CorrectMoveReq(const wstring &move) {
	if (move.size() != 4) return false;//move command 4 elements only
	else if (move[0] == move[2] && move[1] == move[3]) return false;//if no movement
	else if (!iswalpha(move[0]) || !iswalpha(move[2]) || !iswdigit(move[1]) || !iswdigit(move[3])) return false;//if wrong symbols
	else if (move[0] < L'a' || move[0] > L'h' || move[1] < L'1' || move[1] > L'8' ||
				move[2] < L'a' || move[2] > L'h' || move[3] < L'1' || move[3] > L'8') return false;//if wrong marks bounds
	return true;
}

void PrintHelp() {
	wcout <<
		L"Manual:\n"
		" help - for help\n"
		" start - to start\n"
		" exit - for exit\n"
		" <from><to> - for move: a3 a4 or a3a4\n"
		"\n"
		;
	system("pause");
	return;
}

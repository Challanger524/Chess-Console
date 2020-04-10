#pragma once
#include "Header.h"

enum Menu : int { Invalid = -1, Exit, Empty, Help, Start, Restart, Save, Load, Move };

bool CorrectMoveReq(const wstring &move);

const int Dispatch(wstring input);

void PrintHelp(void);

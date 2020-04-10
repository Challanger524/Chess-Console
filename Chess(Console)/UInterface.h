#pragma once
#include "Header.h"

enum Menu : int { Invalid = -1, Exit = 0, Empty, Help, Start, Restart, Save, Load, Move };

bool CorrectMoveReq(const wstring &move);

int Dispatch(wstring &input);

void PrintHelp(void);

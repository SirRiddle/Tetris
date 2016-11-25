#include "Tetris.h"

int main()
{
	Game Tetris;
	Tetris.menu();
	while (Tetris.Running)
	{
		Sleep(Tetris.level_speed);
		Tetris.move_down();

		if (GetAsyncKeyState(VK_UP))
			Tetris.rotate();

		if (GetAsyncKeyState(VK_DOWN))
			Tetris.move_down();

		if (GetAsyncKeyState(VK_RIGHT))
			Tetris.move_right();

		if (GetAsyncKeyState(VK_LEFT))
			Tetris.move_left();

		if (GetAsyncKeyState(VK_ESCAPE))
			Tetris.pause();
	}
}
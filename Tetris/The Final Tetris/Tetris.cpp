#include "Tetris.h"

void Block::reset_shape()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tetromino_bar[i][j] = tetromino_bar_reset[i][j];

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			tetromino[i][j] = tetromino_reset[i][j];
}
/// Constructor
Game::Game()
{
	{
		ifstream positions;
		positions.open("InitialPositionsOfBlocks.txt");
		if (!positions.fail())
		{
			for (int k = 0; k < 6; k++)
			{
				memset(Blocks[k].tetromino, ' ', sizeof(Blocks[k].tetromino));
				memset(Blocks[k].tetromino_reset, ' ', sizeof(Blocks[k].tetromino_reset));
				for (int i = 0; i < 4; i++)
				{
					int x, y;
					positions >> x >> y;
					Blocks[k].tetromino[x][y] = C_Block;
					Blocks[k].tetromino_reset[x][y] = C_Block;
				}
				positions >> Blocks[k].Color;
			}
			memset(Blocks[6].tetromino_bar, ' ', sizeof(Blocks[6].tetromino_bar));
			memset(Blocks[6].tetromino_bar_reset, ' ', sizeof(Blocks[6].tetromino_bar_reset));
			for (int i = 0; i < 4; i++)
			{
				int x, y;
				positions >> x >> y;
				Blocks[6].tetromino_bar[x][y] = C_Block;
				Blocks[6].tetromino_bar_reset[x][y] = C_Block;
			}
			positions >> Blocks[6].Color;
			positions.close();
		}
	}
}
/// The menu and save/load
void Game::menu()
{
	system("cls");
	int menu_item = 1, menu_run = 1, x = 25;
	while (menu_run)
	{
		gotoxy(0, 0, 0); logo();
		gotoxy(25, 40, 15); cout << "Play";
		gotoxy(27, 40, 15); cout << "Load last game";
		gotoxy(29, 40, 15); cout << "Instructions";
		gotoxy(31, 40, 15); cout << "High scores";
		gotoxy(33, 40, 15); cout << "Quit";
		gotoxy(22, 15, 15); cout << "Scroll using " << char(30) << " & " << char(31);
		gotoxy(22, 50, 15); cout << "Press Enter to choose";
		gotoxy(x, 38, 12);  cout << char(16);

		system("pause>nul");
		if (GetAsyncKeyState(VK_DOWN) && x != 33)
		{
			gotoxy(x, 36, 12); cout << "   ";
			x += 2;
			menu_item++;
		}
		if (GetAsyncKeyState(VK_UP) && x != 25)
		{
			gotoxy(x, 36, 12); cout << "   ";
			x -= 2;
			menu_item--;
		}
		if (GetAsyncKeyState(VK_RETURN))
		{
			switch (menu_item)
			{
			case 1: {
				system("cls");
				while (player.Name == "")
				{
					system("cls");
					logo();
					cout << "\n\n Enter your Name :  ";
					getline(cin, player.Name);
				}
				cout << endl << "\t\t\t Press any key to Start the game" << endl;;
				system("pause>nul");
				run();
				menu_run = 0;
				break;
			}
			case 2: {
				system("cls");
				run();
				load_last_game();
				gotoxy(0, 0, 15);
				draw_Map();
				next();
				pause();
				menu_run = 0;
				break;
			}
			case 3: {
				system("cls");
				gotoxy(0, 0, 15);
				cout << char(16) << " There are 7 types of blocks which can be moved using the arrow keys left,down and right.\n";
				cout << char(16) << " You can use the up arrow key to rotate any block 90 degree in the clock-wise direction.\n";
				cout << char(16) << " The purpose of this game is to assemble a complete line of block pieces.\n";
				cout << char(16) << " When you reach a certain score you will advance to the next level.\n";
				cout << endl << "\nPress spacebar to return to the menu " << endl;
				system("pause>nul");
				system("cls");
				continue;
			}
			case 4: {
				system("cls");
				HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
				gotoxy(0, 0, 15);
				fstream Hscores;
				Hscores.open("HighScores.txt");
				while (!Hscores.eof())
				{
					string Name;
					int Score;
					getline(Hscores, Name);
					Hscores >> Score;
					if (Name != "")
					{
						SetConsoleTextAttribute(Handle, 12);
						cout << "\t\t\t\t" << char(16) << " " << Name << " " << char(17) << " ";
						SetConsoleTextAttribute(Handle, 9);
						cout << Score << endl << endl;
						SetConsoleTextAttribute(Handle, 15);
						Hscores.ignore();
					}
				}
				Hscores.close();
				cout << "\t\t\t\tPress Spacebar to return to the menu";
				system("pause>nul");
				system("cls");
				continue;
			}
			case 5:
				exit(0);
			}
		}
	}
}
void Game::logo()
{
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	char LOGO[100][100];
	ifstream logo;
	logo.open("Logo.txt");
	if (logo.is_open())
	{
		for (int i = 0; i < 25; i++)
		{
			string line;
			getline(logo, line);
			for (int j = 0; j < line.size(); j++)
			{
				LOGO[i][j] = line[j];
			}
		}
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 86; j++)
			{
				if (LOGO[i][j] == 'R')
					SetConsoleTextAttribute(Handle, 12);
				else if (LOGO[i][j] == 'Y')
					SetConsoleTextAttribute(Handle, 14);
				else if (LOGO[i][j] == 'G')
					SetConsoleTextAttribute(Handle, 10);
				else if (LOGO[i][j] == 'T')
					SetConsoleTextAttribute(Handle, 11);
				else if (LOGO[i][j] == 'P')
					SetConsoleTextAttribute(Handle, 5);

				if (LOGO[i][j] == ' ')
					cout << ' ';
				else
					cout << char(16);
			}
			cout << endl;
		}
		SetConsoleTextAttribute(Handle, 15);
		logo.close();
	}
}
void Game::save_game()
{
	ofstream save;
	save.open("SavedGame.txt");
	if (save.is_open())
	{
		save << player.Name << endl;
		save << player.Score << endl;
		save << score_multi << endl << endl;
		get_positions(Blocks[current_block].curr_positions);
		recover(Blocks[current_block].curr_positions);
		for (int i = 1; i <= MAPHEIGHT; i++)
		{
			for (int j = 1; j <= MAPWIDTH; j++)
			{
				if (Grid[i][j] == ' ')
					save << '.';
				else
					save << Grid[i][j];
			}
			save << endl;
		}
		save.close();
	}
}
void Game::load_last_game()
{
	ifstream load;
	load.open("SavedGame.txt");
	if (load.is_open())
	{
		memset(Grid, ' ', sizeof(Grid));
		getline(load, player.Name);
		if (player.Name == "")
		{
			system("cls");
			gotoxy(0, 0, 15);
			cout << "There is no previous save.\n\n";
			cout << "Press Spacebar to return to the menu";
			system("pause>nul");
			menu();
		}
		load >> player.Score >> score_multi;
		for (int i = 1; i <= MAPHEIGHT; i++)
		{
			for (int j = 1; j <= MAPWIDTH; j++)
			{
				load >> Grid[i][j];
				if (Grid[i][j] == '.')
					Grid[i][j] = ' ';
			}
		}
		load.close();
	}
}
/// Game introduction and start up
void Game::run()
{
	srand(time(NULL));
	current_block = rand() % 7;
	next_block = rand() % 7;
	initiate_Map();
	draw_Map();
	next();
}
void Game::draw_Map()
{
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	gotoxy(0, 0, 15);
	for (int i = 1; i <= MAPHEIGHT; i++)
	{
		for (int j = 1; j <= MAPWIDTH; j++)
		{
			if (Grid[i][j] == char(177))
				SetConsoleTextAttribute(Handle, 17);

			cout << Grid[i][j];
			SetConsoleTextAttribute(Handle, 15);
		}
		cout << endl;
	}
	gotoxy(2, 30, 15);
	cout << "Name : " << player.Name;
	gotoxy(3, 30, 15);
	cout << "Score : " << player.Score;
}
void Game::initiate_Map()
{
	memset(Grid, ' ', sizeof(Grid));
	for (int i = 1; i <= MAPHEIGHT; i++)
	{
		for (int j = 1; j <= MAPWIDTH; j++)
		{
			if (i == 1 || i == MAPHEIGHT)
				Grid[i][j] = C_Grid;
			else if (i > 1 && i < MAPHEIGHT)
			{
				Grid[i][1] = C_Grid;
				Grid[i][MAPWIDTH] = C_Grid;
			}
		}
	}
}
/// Ingame fucntions
void Game::next()
{
	gotoxy(5, 30, 15);
	cout << "Next Block";
	if (next_block != 6)
	{
		for (int i = 7, ti = 0; ti < 3; ti++, i++)
		{
			for (int j = 32, tj = 0; tj < 3; tj++, j++)
			{
				gotoxy(i, j, Blocks[next_block].Color);
				cout << Blocks[next_block].tetromino_reset[ti][tj];
			}
		}
	}
	else
	{
		for (int i = 6, ti = 0; ti < 4; ti++, i++)
		{
			for (int j = 32, tj = 0; tj < 4; tj++, j++)
			{
				gotoxy(i, j, Blocks[next_block].Color);
				cout << Blocks[next_block].tetromino_bar_reset[ti][tj];
			}
		}
	}
}
void Game::pause()
{
	gotoxy(10, 30, 15);
	cout << "Game is now paused !";
	gotoxy(12, 30, 15);
	cout << char(16) << " Press any key to continue the game";
	gotoxy(13, 30, 15);
	cout << char(16) << " Press Spacebar if you want to save the game";
	gotoxy(15, 30, 15);
	cout << "** Note that your last save will be removed **";
	system("Pause>nul");
	if (GetAsyncKeyState(VK_SPACE))
	{
		save_game();
		gotoxy(17, 30, 15);
		cout << "Your game is now saved , Press any key to exit the game";
		system("pause>nul");
		exit(0);
	}
	gotoxy(10, 30, 15);	cout << "\t\t\t\t\t\t\t\t";
	gotoxy(12, 30, 15);	cout << "\t\t\t\t\t\t\t\t";
	gotoxy(13, 30, 15); cout << "\t\t\t\t\t\t\t\t";
	gotoxy(15, 30, 15); cout << "\t\t\t\t\t\t\t\t";
}
void Game::next_Level()
{
	if (player.Score*score_multi == 1000 * score_multi)
	{
		system("cls");
		level_speed -= 50;
		if (level_speed == 0)
		{	
			cout << " \t\t\t You Have Finished ALL Levels !!" << endl;
			cout << " \t\t\t Congratulations !! " << endl;
			system("pause>nul");
			exit(0);
		}
		cout << "\n\n\t\t\t\t Get ready for the next level " << endl;
		Sleep(2500);
		score_multi++;
		gotoxy(0, 0, 15);
		run();
	}
}
void Game::check_end_game()
{
	curr_i = 2;
	curr_j = 10;
	current_block = next_block;
	next_block = rand() % 7;
	check_complete_lines();
	draw_Map();
	if (Grid[curr_i + 1][curr_j + 1] == C_Block)
	{
		Sleep(1500);
		system("cls");
		gotoxy(0, 0, 15);
		cout << "\t\t\t\t GAME OVER !!" << endl;
		cout << "\t\t\t\t Final Score = " << player.Score << endl;
		ofstream Hscores;
		Hscores.open("HighScores.txt", ios::app);
		if (Hscores.is_open() && player.Score != 0)
		{
			Hscores << player.Name << endl << player.Score;
			Hscores.close();
		}
		system("Pause>nul");
		exit(0);
	}
}                                              
void Game::clear_line(int line)
{
	for (int i = line; i > 3; i--)
	{
		for (int j = 2; j < MAPWIDTH; j++)
		{
			Grid[i][j] = Grid[i - 1][j];
		}
	}
}
void Game::check_complete_lines()
{
	for (int i = MAPHEIGHT - 1; i >= 2; i--)
	{
		int counter = 0;
		for (int j = 2; j < MAPWIDTH; j++)
		{
			if (Grid[i][j] == C_Block && Grid[i][j + 1] == C_Block)
			{
				counter++;
				if (counter == MAPWIDTH - 3)
				{
					clear_line(i);
					player.Score += 100 * score_multi;
					next_Level();
					i++;
					break;
				}
			}
		}
	}
}
void Game::gotoxy(int x, int y, int color)
{
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD pos;
	pos.X = y;
	pos.Y = x;

	SetConsoleCursorPosition(Handle, pos);
	SetConsoleTextAttribute(Handle, color);
}
///	 Game Movment of Blocks
void Game::get_positions(int positions[][2])
{
	if (current_block == 6)
	{
		for (int i = curr_i, pi = 0, check_i = 0; i < curr_i + 4; check_i++, i++)
			for (int j = curr_j, pj = 0, check_j = 0; j < curr_j + 4; check_j++, j++)
				if (Blocks[current_block].tetromino_bar[check_i][check_j] == C_Block)
				{
					if (pi == 4)
						break;

					positions[pi][pj] = i;
					pj++;
					positions[pi][pj] = j;
					pi++;
					pj--;
				}
	}
	else
	{
		for (int i = curr_i, pi = 0, check_i = 0; i < curr_i + 3; check_i++, i++)
			for (int j = curr_j, pj = 0, check_j = 0; j < curr_j + 3; check_j++, j++)
				if (Blocks[current_block].tetromino[check_i][check_j] == C_Block)
				{
					if (pi == 4)
						break;

					positions[pi][pj] = i;
					pj++;
					positions[pi][pj] = j;
					pi++;
					pj--;
				}
	}
}
void Game::recover(int positions[][2])
{
	if (current_block == 6)
	{
		for (int i = curr_i, ti = 0; ti < 4; i++, ti++)
		{
			for (int j = curr_j, tj = 0; tj < 4; j++, tj++)
			{
				if (Blocks[current_block].tetromino_bar[ti][tj] == C_Block)
					Grid[i][j] = ' ';
			}
		}
	}
	else
	{
		for (int i = curr_i, ti = 0; ti < 3; i++, ti++)
		{
			for (int j = curr_j, tj = 0; tj < 3; j++, tj++)
			{
				if (Blocks[current_block].tetromino[ti][tj] == C_Block)
					Grid[i][j] = ' ';
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		gotoxy(positions[i][0] - 1, positions[i][1] - 1, Blocks[current_block].Color);
		cout << ' ';
	}
}
void Game::draw_block(int positions[][2], int color)
{
	if (current_block == 6)
	{
		for (int i = curr_i, ti = 0; ti < 4; i++, ti++)
		{
			for (int j = curr_j, tj = 0; tj < 4; j++, tj++)
			{
				if (Blocks[current_block].tetromino_bar[ti][tj] == C_Block)
					Grid[i][j] = Blocks[current_block].tetromino_bar[ti][tj];
			}
		}
	}
	else
	{
		for (int i = curr_i, ti = 0; ti < 3; i++, ti++)
		{
			for (int j = curr_j, tj = 0; tj < 3; j++, tj++)
			{
				if (Blocks[current_block].tetromino[ti][tj] == C_Block)
					Grid[i][j] = Blocks[current_block].tetromino[ti][tj];
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		gotoxy(positions[i][0] - 1, positions[i][1] - 1, color);
		cout << C_Block;
	}
}
///  Game Collisions Test
bool Game::collide_down()
{
	get_positions(Blocks[current_block].curr_positions);
	get_positions(Blocks[current_block].next_positions);

	for (int i = 0; i < 4; i++)
		Blocks[current_block].next_positions[i][0]++;

	for (int i = 0; i < 4; i++)
	{
		if (Grid[Blocks[current_block].next_positions[i][0]][Blocks[current_block].next_positions[i][1]] == C_Block
			|| Grid[Blocks[current_block].next_positions[i][0]][Blocks[current_block].next_positions[i][1]] == C_Grid)
		{
			for (int j = 0; j < 4; j++)
			{
				if ((Blocks[current_block].next_positions[i][0] == Blocks[current_block].curr_positions[j][0])
					&& (Blocks[current_block].next_positions[i][1] == Blocks[current_block].curr_positions[j][1]))
					goto cont;
			}
			return true;
		}
	cont:;
	}
	return false;
}
bool Game::collide_left()
{
	get_positions(Blocks[current_block].curr_positions);
	get_positions(Blocks[current_block].next_positions);

	for (int i = 0; i < 4; i++)
		Blocks[current_block].next_positions[i][1]--;

	for (int i = 0; i < 4; i++)
	{
		if (Grid[Blocks[current_block].next_positions[i][0]][Blocks[current_block].next_positions[i][1]] == C_Block
			|| Grid[Blocks[current_block].next_positions[i][0]][Blocks[current_block].next_positions[i][1]] == C_Grid)
		{
			for (int j = 0; j < 4; j++)
			{
				if ((Blocks[current_block].next_positions[i][0] == Blocks[current_block].curr_positions[j][0])
					&& (Blocks[current_block].next_positions[i][1] == Blocks[current_block].curr_positions[j][1]))
					goto cont;
			}
			return true;
		}
	cont:;
	}
	return false;
}
bool Game::collide_right()
{
	get_positions(Blocks[current_block].curr_positions);
	get_positions(Blocks[current_block].next_positions);

	for (int i = 0; i < 4; i++)
		Blocks[current_block].next_positions[i][1]++;

	for (int i = 0; i < 4; i++)
	{
		if (Grid[Blocks[current_block].next_positions[i][0]][Blocks[current_block].next_positions[i][1]] == C_Block
			|| Grid[Blocks[current_block].next_positions[i][0]][Blocks[current_block].next_positions[i][1]] == C_Grid)
		{
			for (int j = 0; j < 4; j++)
			{
				if ((Blocks[current_block].next_positions[i][0] == Blocks[current_block].curr_positions[j][0])
					&& (Blocks[current_block].next_positions[i][1] == Blocks[current_block].curr_positions[j][1]))
					goto cont;
			}
			return true;
		}
	cont:;
	}
	return false;
}
bool Game::collide_rotate()
{
	if (current_block == 6)
	{
		for (int i = curr_i, check_i = 0; i < curr_i + 4; check_i++, i++)
			for (int j = curr_j, check_j = 0; j < curr_j + 4; check_j++, j++)
			{
				if (Grid[i][j] == C_Block && Blocks[current_block].tetromino_bar[check_i][check_j] != C_Block)
					return true;
				if (Grid[i][j] == C_Grid && Blocks[current_block].tetromino_bar[check_i][check_j] != C_Block)
					return true;
			}
	}
	else
	{
		for (int i = curr_i, check_i = 0; i < curr_i + 3; check_i++, i++)
			for (int j = curr_j, check_j = 0; j < curr_j + 3; check_j++, j++)
			{
				if (Grid[i][j] == C_Block && Blocks[current_block].tetromino[check_i][check_j] != C_Block)
					return true;
				if (Grid[i][j] == C_Grid && Blocks[current_block].tetromino_bar[check_i][check_j] != C_Block)
					return true;
			}
	}
	return false;
}
///  Game Commands
void Game::rotate()
{
	if (!collide_rotate())
	{
		recover(Blocks[current_block].next_positions);
		if (current_block == 6)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = i; j < 4; j++)
				{
					swap(Blocks[current_block].tetromino_bar[i][j], Blocks[current_block].tetromino_bar[j][i]);
				}
			}
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					swap(Blocks[current_block].tetromino_bar[i][j], Blocks[current_block].tetromino_bar[i][j + 2]);
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = i; j < 3; j++)
				{
					swap(Blocks[current_block].tetromino[i][j], Blocks[current_block].tetromino[j][i]);
				}
			}
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 1; j++)
				{
					swap(Blocks[current_block].tetromino[i][j], Blocks[current_block].tetromino[i][j + 2]);
				}
			}
		}
		get_positions(Blocks[current_block].curr_positions);
		draw_block(Blocks[current_block].curr_positions, Blocks[current_block].Color);
	}
}
void Game::move_right()
{
	if (!collide_right())
	{
		recover(Blocks[current_block].curr_positions);
		curr_j++;
		draw_block(Blocks[current_block].next_positions, Blocks[current_block].Color);
	}
}
void Game::move_left()
{
	if (!collide_left())
	{
		recover(Blocks[current_block].curr_positions);
		curr_j--;
		draw_block(Blocks[current_block].next_positions, Blocks[current_block].Color);
	}
}
void Game::move_down()
{
	if (!collide_down())
	{
		recover(Blocks[current_block].curr_positions);
		curr_i++;
		draw_block(Blocks[current_block].next_positions, Blocks[current_block].Color);
	}
	else
	{
		draw_block(Blocks[current_block].curr_positions, 15);
		Blocks[current_block].reset_shape();
		check_end_game();
		next();
	}
}
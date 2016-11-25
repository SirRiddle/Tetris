#include <windows.h>
#include <ctime>
#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
#include <queue>
#include <map>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct Player
{
	string Name;
	int Score = 0;
};
struct Block
{
	char tetromino[3][3];
	char tetromino_reset[3][3];
	char tetromino_bar[4][4];
	char tetromino_bar_reset[4][4];
	int	curr_positions[4][2], next_positions[4][2], Color;
	void reset_shape();
};
struct Game
{
	char Grid[100][100];
	const int MAPWIDTH = 22, MAPHEIGHT = 28;
	const char C_Grid = char(177), C_Block = char(219);
	int Running = 1, level_speed = 200, score_multi = 1, curr_i = 2, curr_j = 10;
	int current_block = 0, next_block = 0;
	Player player;
	Block Blocks[7];
	/// Constructor
	Game();
	/// The menu and save/load
	void menu();
	void logo();
	void save_game();
	void load_last_game();
	/// Game introduction and start up
	void run();
	void draw_Map();
	void initiate_Map();
	/// Ingame fucntions
	void next();
	void pause();
	void next_Level();
	void check_end_game();
	void clear_line(int);
	void check_complete_lines();
	void gotoxy(int,int,int);
	///	Movment of Blocks
	void get_positions(int[][2]);
	void recover(int[][2]);
	void draw_block(int[][2], int);
	/// Collisions Test
	bool collide_down();
	bool collide_left();
	bool collide_right();
	bool collide_rotate();
	/// Commands
	void rotate();
	void move_down();
	void move_left();
	void move_right();
};
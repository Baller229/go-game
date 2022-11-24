#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

struct Brick
{
	size_t row = 0;
	size_t col = 0;
	vector<vector<char>> neighbours; //up, down, left, right neighbour
	char player = '.';
	int isEmpty = 0;

};

void init(int argc, char* argv[]);
void start();
char pickPlayer(bool & playerO);
void checkArgs(int argc, char* argv[]);
void initializeGrid(vector<vector<Brick>> & vG);
vector<string> loadInput(string s);
void makeMove(char player, int row, int col);
void printGrid();
vector<vector<Brick>> createVectorGrid();

int GRID_SIZE;
vector<vector<Brick>> GRID;
vector<string> INPUT;

//====================================================
//
//====================================================

int main(int argc, char* argv[])
{
	init(argc, argv);
	start();
	printGrid();
	return 0;
}

//====================================================
//
//====================================================

void init(int argc, char* argv[])
{
	string s;
	checkArgs(argc, argv);
	GRID = createVectorGrid();
	initializeGrid(GRID);
	getline(std::cin >> std::ws, s);
	INPUT = loadInput(s);
}

//====================================================
//
//====================================================

void start() 
{
	bool playerO = 0;
	char currentPlayer;
	for (size_t i = 0; i < INPUT.size(); i+=2) 
	{
		currentPlayer = pickPlayer(playerO);
		cout << "current player: " << currentPlayer << endl;
		if (INPUT[i] != "pass") 
		{
			makeMove(currentPlayer, stoi(INPUT[i]), stoi(INPUT[i + 1]));
		}
	}
	
}

//====================================================
//
//====================================================

char pickPlayer(bool & playerO)
{
	if (playerO == 0)
	{
		playerO = 1;
		return 'X';
	}
	else 
	{
		playerO = 0;
		return 'O';
	}
}

//====================================================
//
//====================================================

void checkArgs(int argc, char* argv[])
{
	
	if (argc != 4)
	{
		if (argc != 3)
		{
			exit(1);
		}
	}

	if (string(argv[2]) != "--board")
	{
		exit(1);
	}

	GRID_SIZE = stoi(argv[1]);
}

//====================================================
//
//====================================================

vector<vector<Brick>> createVectorGrid()
{
	vector<vector<Brick>> v(GRID_SIZE, vector<Brick>(GRID_SIZE));
	
	return v;
}



void initializeGrid(vector<vector<Brick>> &  vG)
{ 
	for (size_t i = 0; i < vG.size(); i++)
	{
		for (size_t j = 0; j < vG[i].size(); j++)
		{
			vG[i][j].row = i;
			vG[i][j].col = j;
			vG[i][j].player = '.';
		}
	}
	
}

//====================================================
//
//====================================================

vector<string> loadInput(string s)
{
	std::vector<std::string> inputStr;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> tokens(begin, end);
	
	for (auto& str : tokens) 
	{
		if (str == "pass")
		{
			inputStr.push_back(str);
			inputStr.push_back(str);
		}
		else 
		{
			inputStr.push_back(str);
		}
		

	}
	return inputStr;

}

//====================================================
//
//====================================================

void makeMove(char player, int row, int col) 
{
	try
	{
		GRID[row][col].player = player;
	}
	catch (const std::exception&)
	{
		
	}
}

void printGrid()
{
	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			cout << GRID[i][j].player;
		}
		cout << endl;
	}

}
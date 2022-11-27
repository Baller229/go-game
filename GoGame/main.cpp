#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

struct GroupMember {
	int row;
	int col;
};
//map<int, int> GroupMember;

//===================================

struct Neighbour
{
	bool up = 0;
	bool down = 0;
	bool left = 0;
	bool right = 0;
};

//===================================

struct Enemy
{
	bool up = 0;
	bool down = 0;
	bool left = 0;
	bool right = 0;
};

//===================================

struct Freedom 
{
	bool up = 1;
	bool down = 1;
	bool left = 1;
	bool right = 1;
};

//===================================

struct Wall
{
	bool up = 0;
	bool down = 0;
	bool left = 0;
	bool right = 0;
};

//===================================

struct Brick
{
	size_t row = 0;
	size_t col = 0;
	Neighbour neighbours; //up, down, left, right neighbours
	Enemy enemies; //up, down, left, right enemies
	Freedom freedom; //up, down, left, right freedom
	Wall wall;
	char player = '.';
	int groupFreedom = 0;

};

//===================================

//====================================================
//
//====================================================
void printFreedoms();
void setFreedomsForCurrentGroup(vector<GroupMember>& groupMembers, int& count);
bool findMember(vector<GroupMember>& vect, GroupMember& member);
int checkGroupNeighbours(vector<GroupMember> & member, int row, int col);
bool hasFriends(int row, int col);
int countFreedom(vector<GroupMember> & member, size_t r, size_t col);
void checkGroupFreedom();
void updateBrick(char p, int row, int col);
void CurrUP(Neighbour &n, Enemy &e, Freedom &f, Wall& w, char p, int row, int col);
void CurrDOWN(Neighbour& n, Enemy& e, Freedom& f, Wall& w, char p, int row, int col);
void CurrLEFT(Neighbour& n, Enemy& e, Freedom& f, Wall& w, char p, int row, int col);
void CurrRIGHT(Neighbour& n, Enemy& e, Freedom& f, Wall& w, char p, int row, int col);
void update(char p, int row, int col);
bool rowOutOfField(int row);
bool colOutOfField(int col);
bool isOutOfField(int row, int col);
bool isReserved(size_t row, size_t col);
void init(int argc, char* argv[]);
void start();
char pickPlayer(bool & playerO);
void checkArgs(int argc, char* argv[]);
void initializeGrid(vector<vector<Brick>> & vG);
vector<string> loadInput(string s);
void makeMove(char player, int row, int col);
void printGrid();
vector<vector<Brick>> createVectorGrid();

//====================================================
//
//====================================================

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
	cout << "************************" << endl;
	cout << "Final Result"<< endl;
	
	cout << "************************" << endl;


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
		if (INPUT[i] == "pass" && INPUT[i + 1] == "pass")
		{
			currentPlayer = pickPlayer(playerO);
			continue;
		}
		if (isOutOfField(stoi(INPUT[i]), stoi(INPUT[i + 1])))
		{
			continue;
		}
		if (!(isReserved(stoi(INPUT[i]), stoi(INPUT[i+1]))))
		{
			currentPlayer = pickPlayer(playerO);
		}
		else 
		{
			continue;
		}
		
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

//====================================================
//
//====================================================

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
		update(player, row, col);
		checkGroupFreedom();
		
		//HRA DO SAMOVRAZDY -> check if player O or player X is trying to delete himself, replace with '.' update, checkfreedom and continue with same player
		//HRA DO SAMOVRAZDY -> Ale ak by znamenalo odobratie vlastnych a zaroven superovych kamenov, tah je validny a odoberiu sa superove kamene
		//KO -> budem odkladat vsetky predosle konfiguracie a porovnavat s aktualnou konfiguraciou
		//KO -> ak je konfiguracia vypytam tah este raz
		cout << "========================" << endl;
		cout << "Player: " << player << " row: " << row << " col: " << col << endl;
		cout << "========================" << endl;
		printGrid();
		cout << endl;
		printFreedoms();
		cout << "========================" << endl;


		
	}
	catch (const std::exception&)
	{
		cout << "grid out of bounds" << endl;
	}
}

//=================================================
//
//=================================================
void checkGroupFreedom()
{
	int countX = 0;
	int firstX = 0;
	int firstO = 0;
	//int countXTemp = 0;
	
	vector<GroupMember> groupMembersX;
	map<int, int> tempMap;
	
	int countO = 0;
	vector<GroupMember> groupMembersO;

	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			
			if (GRID[i][j].player == 'X' || (i + 1 == GRID.size() && j + 1 == GRID[i].size()))
			{

				if (firstX != 0)
				{
					GroupMember g;
					g.row = (int)i;
					g.col = (int)j;
					if (findMember(groupMembersX, g))
					{
						//cout << "Same group [ " << i << "," << j << " ]" << endl;
					}
					else 
					{
						setFreedomsForCurrentGroup(groupMembersX, countX);
					}
				}
				countX += countFreedom(groupMembersX, i, j);
				if (hasFriends((int)i, (int)j))
				{
					countX += checkGroupNeighbours(groupMembersX, (int)i, (int)j);
					
				}
				
				else 
				{
					//does not have friends so find another group
					//store count in groupFreedom
					/*GRID[i][j].groupFreedom = countX;*/
					setFreedomsForCurrentGroup(groupMembersX, countX);
					//cout << "AAAAA [ " << i << "," << j << " ]" << endl;
				}
			firstX++;
			}
			if (GRID[i][j].player == 'O' || (i + 1 == GRID.size() && j + 1 == GRID[i].size()))
			{
				//cout << "O " << i << " " << j << endl;
				if (firstO != 0)
				{
					GroupMember g;
					g.row = (int)i;
					g.col = (int)j;
					if (findMember(groupMembersO, g))
					{
						if ((i + 1 == GRID.size() && j + 1 == GRID[i].size()))
						{
							setFreedomsForCurrentGroup(groupMembersO, countO);
						}
						//cout << "Same group [ " << i << "," << j << " ]" << endl;
					}
					else
					{
						setFreedomsForCurrentGroup(groupMembersO, countO);
					}
				}
				countO += countFreedom(groupMembersO, i, j);
				if (hasFriends((int)i, (int)j))
				{
					countO += checkGroupNeighbours(groupMembersO, (int)i, (int)j);

				}

				else
				{
					//does not have friends so find another group
					//store count in groupFreedom
					/*GRID[i][j].groupFreedom = countX;*/
					setFreedomsForCurrentGroup(groupMembersO, countO);
					//cout << "AAAAA [ " << i << "," << j << " ]" << endl;
				}
			 firstO++;
			}
		}
		
	}
}

//=================================================
//
//=================================================

void setFreedomsForCurrentGroup(vector<GroupMember>& groupMembers, int& count)
{
	if (!(groupMembers.empty()))
	{
		for (size_t k = 0; k < groupMembers.size(); k++)
		{
			GRID[groupMembers[k].row][groupMembers[k].col].groupFreedom = count;
		}
		groupMembers.clear();

	}
	count = 0;
}

//=================================================
//
//=================================================

bool findMember(vector<GroupMember>& vect, GroupMember& member)
{
	for (size_t i = 0; i < vect.size(); i++)
	{
		if (vect[i].row == member.row && vect[i].col == member.col)
		{
			return true;
		}
	}
	return false;
}
//=================================================
//
//=================================================

int countFreedom(vector<GroupMember> & member, size_t r, size_t c)
{
	int row = (int)r;
	int col = (int)c;
	GroupMember temp;
	temp.row = row;
	temp.col = col;
	int count = 0;
	if (findMember(member, temp))
	{
		return 0;
	}
	
	if (GRID[row][col].freedom.up == 1)
	{
		count++;
	}
	if (GRID[row][col].freedom.down == 1)
	{
		count++;
	}
	if (GRID[row][col].freedom.left == 1)
	{
		count++;
	}

	if (GRID[row][col].freedom.right == 1)
	{
		count++;
	}
	GroupMember gm;
	gm.row = row;
	gm.col = col;
	member.push_back(gm);

	return count;
}

//=================================================
//
//=================================================
bool hasFriends(int row, int col)
{
	if (GRID[row][col].neighbours.up == 1)
	{
		return true;
	}
	if (GRID[row][col].neighbours.down == 1)
	{
		return true;
	}

	if (GRID[row][col].neighbours.left == 1)
	{
		return true;
	}

	if (GRID[row][col].neighbours.right == 1)
	{
		return true;
	}
	else 
	{
		return false;
	}

}

//=================================================
//
//=================================================

int checkGroupNeighbours(vector<GroupMember> & member, int row, int col)
{
	
	int count = 0;
	//UP
	if(GRID[row][col].neighbours.up == 1)
	{
		GroupMember gm;
		gm.row = row - 1;
		gm.col = col;
		if (!(member.empty()))
		{
			if (!(findMember(member, gm)))
			{
				count += countFreedom(member, row - 1, col);
				member.push_back(gm);
			}
		}
		else
		{
			count += countFreedom(member, row - 1, col);
			member.push_back(gm);
		}
	}
	//DOWN
	if (GRID[row][col].neighbours.down == 1)
	{
		GroupMember gm;
		gm.row = row + 1;
		gm.col = col;
		if (!(member.empty()))
		{
			if (!(findMember(member, gm)))
			{
				count += countFreedom(member, row + 1, col);
				member.push_back(gm);
			}
		}
		else
		{
			count += countFreedom(member, row + 1, col);
			member.push_back(gm);
		}
	}
	//LEFT
	if (GRID[row][col].neighbours.left == 1)
	{
		GroupMember gm;
		gm.row = row;
		gm.col = col - 1;
		if (!(member.empty()))
		{
			if (!(findMember(member, gm)))
			{
				count += countFreedom(member, row, col - 1);
				member.push_back(gm);
			}
		}
		else
		{
			count += countFreedom(member, row, col - 1);
			member.push_back(gm);
		}
	}
	//RIGHT
	if (GRID[row][col].neighbours.right == 1)
	{
		GroupMember gm;
		gm.row = row;
		gm.col = col + 1;
		if (!(member.empty()))
		{
			if (!(findMember(member, gm)))
			{
				count += countFreedom(member, row, col + 1);
				member.push_back(gm);
			}
		}
		else
		{
			count += countFreedom(member, row, col + 1);
			member.push_back(gm);
		}
	}
	
	return count;
}

//=================================================
//
//=================================================

void update(char p, int row, int col)
{
	updateBrick(p, row, col);
	if (GRID[row][col].wall.up == 0)
	{
		updateBrick(p, row - 1, col);
	}
	if (GRID[row][col].wall.down == 0)
	{
		updateBrick(p, row + 1, col);
	}
	if (GRID[row][col].wall.left == 0)
	{
		updateBrick(p, row, col - 1);
	}
	if (GRID[row][col].wall.right == 0)
	{
		updateBrick(p, row, col + 1);
	}


}

//=================================================
//
//=================================================

void updateBrick(char p, int row, int col) 
{
	Neighbour n;
	Enemy e;
	Freedom f;
	Wall w;
	p = GRID[row][col].player;
	
	CurrUP(n, e, f, w, p, row , col);
	CurrDOWN(n, e, f, w,  p, row, col);
	CurrLEFT(n, e, f, w,  p, row, col);
	CurrRIGHT(n, e, f, w, p, row, col);
}

//=================================================

void CurrUP(Neighbour &n, Enemy &e, Freedom &f, Wall & w, char p, int row, int col)
{

	if (rowOutOfField(row - 1))
	{
		f.up = 0;
		w.up = 1;
		GRID[row][col].wall = w;
		GRID[row][col].freedom = f;
	}
	else
	{
		if (GRID[row - 1][col].player == p)
		{
			f.up = 0;
			e.up = 0;
			n.up = 1;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
		else if (GRID[row - 1][col].player != '.')
		{
			f.up = 0;
			e.up = 1;
			n.up = 0;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
		else 
		{
			f.up = 1;
			e.up = 0;
			n.up = 0;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
	}
}
void CurrDOWN(Neighbour& n, Enemy& e, Freedom& f, Wall& w, char p, int row, int col)
{

	if (rowOutOfField(row + 1))
	{
		f.down = 0;
		w.down = 1;
		GRID[row][col].wall = w;
		GRID[row][col].freedom = f;
	}
	else
	{
		if (GRID[row + 1][col].player == p)
		{
			f.down = 0;
			e.down = 0;
			n.down = 1;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
		else if (GRID[row + 1][col].player != '.')
		{
			f.down = 0;
			e.down = 1;
			n.down = 0;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
		else
		{
			f.down = 1;
			e.down = 0;
			n.down = 0;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
	}
}
void CurrLEFT(Neighbour& n, Enemy& e, Freedom& f, Wall &w, char p, int row, int col)
{

	if (colOutOfField(col - 1))
	{
		f.left = 0;
		w.left = 1;
		GRID[row][col].wall = w;
		GRID[row][col].freedom = f;
	}
	else
	{
		if (GRID[row][col - 1].player == p)
		{
			f.left = 0;
			e.left = 0;
			n.left = 1;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
		else if (GRID[row][col - 1].player != '.')
		{
			f.left = 0;
			e.left = 1;
			n.left = 0;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
		else
		{
			f.left = 1;
			e.left = 0;
			n.left = 0;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
	}
}
void CurrRIGHT(Neighbour& n, Enemy& e, Freedom& f, Wall& w, char p, int row, int col)
{

	if (colOutOfField(col + 1))
	{
		f.right = 0;
		w.right = 1;
		GRID[row][col].wall = w;
		GRID[row][col].freedom = f;
	}
	else
	{
		if (GRID[row][col + 1].player == p)
		{
			f.right = 0;
			e.right = 0;
			n.right = 1;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
		else if (GRID[row][col + 1].player != '.')
		{
			f.right = 0;
			e.right = 1;
			n.right = 0;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
		else
		{
			f.right = 1;
			e.right = 0;
			n.right = 0;
			GRID[row][col].freedom = f;
			GRID[row][col].enemies = e;
			GRID[row][col].neighbours = n;
		}
	}
}

//====================================================
//
//====================================================

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

//=================================================
//
//=================================================

bool isReserved(size_t row, size_t col)
{
	if (GRID[row][col].player != '.')
	{
		return true;
	}
	else 
	{
		return false;
	}
}

//=================================================
//
//=================================================

bool isOutOfField(int row, int col)
{
	if (row >= 0 && row < GRID_SIZE || col >= 0 && col < GRID_SIZE)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

//=================================================
//
//=================================================

bool rowOutOfField(int row)
{
	if (row >= 0 && row < GRID_SIZE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//=================================================
//
//=================================================

bool colOutOfField(int col)
{
	if (col >= 0 && col < GRID_SIZE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//=================================================
//
//=================================================
void printFreedoms() 
{
	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			if (GRID[i][j].player == 'X' || GRID[i][j].player == 'O')
			{
				cout << "Player: " << GRID[i][j].player << " [" << i << " " << j << "] freedom: " << GRID[i][j].groupFreedom << endl;
			}
		}
	}
}


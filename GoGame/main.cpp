#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include<stack>
#include<iterator>


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
bool checkSameConfiguration(int row, int col);

void checkGroupFreedom();
void printGridWithCurrent(int row, int col);
bool checkDeletionX();
bool checkDeletionO();
void deleteX();
void deleteO();
bool checkSuicide(char currentPlayer, int row, int col);
void printFreedoms();
void setFreedomsForCurrentGroup(vector<GroupMember>& groupMembers, vector<GroupMember>& countFreedom);
bool findMember(vector<GroupMember>& vect, GroupMember& member);
bool findConfig(vector<string>& vect, string s);
GroupMember checkGroupNeighbours(vector<GroupMember>& countFreedoms, vector<GroupMember> & member, int row, int col);
bool hasFriends(vector<GroupMember>& seenMembers, int row, int col);
void countFreedom(vector<GroupMember>& countFreedom, vector<GroupMember> & member, size_t r, size_t col);
//void checkGroupFreedom();
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
char pickPlayer(bool& suicide, bool& isSameConfig, bool& playerO, char againPlayer);
void checkArgs(int argc, char* argv[]);
void initializeGrid(vector<vector<Brick>> & vG);
vector<string> loadInput(string s);
void makeMove(char player, int row, int col, int & countMoves);
void printGrid();
vector<vector<Brick>> createVectorGrid();

//====================================================
//
//====================================================

int GRID_SIZE;
vector<vector<Brick>> GRID;
vector<string> INPUT;
vector<string> CONFIGURATION;
//====================================================
//
//====================================================

int main(int argc, char* argv[])
{
	init(argc, argv);
	start();
	//cout << "************************" << endl;
	//cout << "Final Result"<< endl;
	
	//cout << "************************" << endl;


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
	int moves = 1;
	bool playerO = 0;
	bool isSuicide = 0;
	bool isSameConfig = 0;
	char currentPlayer;
	char againPlayer = '.';
	for (size_t i = 0; i < INPUT.size(); i+=2) 
	{
		if (INPUT[i] != "pass" && INPUT[i + 1] != "pass")
		{
			if ((stoi(INPUT[i]) < 0 || stoi(INPUT[i]) >= GRID_SIZE))
			{
				exit(1);
			}
			if ((stoi(INPUT[i + 1]) < 0 || stoi(INPUT[i + 1]) >= GRID_SIZE))
			{
				exit(1);
			}
			if (INPUT[INPUT.size() - 1] != "pass" && INPUT[INPUT.size() - 2] != "pass")
			{
				if (INPUT[INPUT.size() - 3] != "pass" && INPUT[INPUT.size() - 4] != "pass")
				{
					exit(1);
				}
			}
		}
		if (INPUT[i] == "pass" && INPUT[i + 1] == "pass")
		{
			currentPlayer = pickPlayer(isSuicide, isSameConfig, playerO, againPlayer);
			continue;
		}
		if (isOutOfField(stoi(INPUT[i]), stoi(INPUT[i + 1])))
		{
			continue;
		}
		if (!(isReserved(stoi(INPUT[i]), stoi(INPUT[i+1]))))
		{
			currentPlayer = pickPlayer(isSuicide, isSameConfig, playerO, againPlayer);
		}
		else 
		{
			continue;
		}
		
		if (INPUT[i] != "pass") 
		{
			makeMove(currentPlayer, stoi(INPUT[i]), stoi(INPUT[i + 1]), moves);
			isSameConfig = checkSameConfiguration(stoi(INPUT[i]), stoi(INPUT[i + 1]));
			isSuicide = checkSuicide(currentPlayer, stoi(INPUT[i]), stoi(INPUT[i + 1]));
			againPlayer = currentPlayer;
		}
	}
	
}

//====================================================
//
//====================================================

char pickPlayer(bool & suicide, bool& isSameConfig, bool & playerO, char againPlayer)
{
	if (suicide == 1 || isSameConfig == 1)
	{
		if (againPlayer == 'X')
		{
			playerO = 1;
			return 'X';
		}
		else if (againPlayer == 'O')
		{
			playerO = 0;
			return 'O';
		}
		suicide = 0;
		isSameConfig = 0;
	}
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
	vector<string> inputStr;
	stringstream ss(s);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> tokens(begin, end);
	
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
			/*if (stoi(str) < 0 || stoi(str) > GRID_SIZE)
			{
				exit(1);
			}*/
		}
		

	}
	return inputStr;

}

bool checkSameConfiguration(int row, int col) 
{
	string config = "";
	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			config += GRID[i][j].player;
		}
	}
	if (!(CONFIGURATION.empty())) 
	{
		if (findConfig(CONFIGURATION, config)) 
		{
			GRID[row][col].player = '.';
			update('.', row, col);
			checkGroupFreedom();
			return true;
		}
		return false;
	}
	CONFIGURATION.push_back(config);
	return false;
	
}
//====================================================
//
//====================================================

void makeMove(char player, int row, int col, int & countMoves) 
{
	try
	{
		GRID[row][col].player = player;
		update(player, row, col);
		checkGroupFreedom();
		
		

		
		
		//checkKO();
		
		//HRA DO SAMOVRAZDY -> check if player O or player X is trying to delete himself, replace with '.' update, checkfreedom and continue with same player
		//HRA DO SAMOVRAZDY -> Ale ak by znamenalo odobratie vlastnych a zaroven superovych kamenov, tah je validny a odoberiu sa superove kamene
		//KO -> budem odkladat vsetky predosle konfiguracie a porovnavat s aktualnou konfiguraciou
		//KO -> ak je konfiguracia vypytam tah este raz
		
		//cout << "========================" << endl;
		//cout << countMoves << ": Player: " << player << " row: " << row << " col: " << col << endl;
		//cout << "========================" << endl;
		countMoves++;
		//printGrid();
		//cout << "- - - - - - - - - - " << endl;
		//printGridWithCurrent(row, col);
		//cout << endl;
		//printFreedoms();
		//cout << "========================" << endl;


		
	}
	catch (const std::exception&)
	{
		//cout << "grid out of bounds" << endl;
	}
}

//=================================================
//
//=================================================

bool checkSuicide(char currentPlayer, int row, int col)
{
	
		if (currentPlayer == 'X')
		{
			if (row == 3 && col == 2)
			{
				//cout << "aaaaaaaaa";
			}
			if (GRID[row][col].groupFreedom == 0)
			{
				
				if (checkDeletionO())
				{
					//delete enemies if possible
					deleteO();
					return false;
				}
				else 
				{
					GRID[row][col].player = '.';
					update('.', row, col);
					checkGroupFreedom();
					return true;
				}
				
			}
			else 
			{
				//delete enemies if possible
				deleteO();
				return false;
			}
			
		}
		else if (currentPlayer == 'O')
		{
			if (GRID[row][col].groupFreedom == 0)
			{
				if (checkDeletionX())
				{
					//delete enemies if possible
					deleteX();
					return false;
				}
				else 
				{
					GRID[row][col].player = '.';
					update('.', row, col);
					checkGroupFreedom();
					return true;
				}
			}
			else 
			{
				deleteX();
				return false;
			}
		}
		return false;
}

//=================================================
//
//=================================================

bool checkDeletionX() 
{
	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			if (GRID[i][j].player == 'X')
			{
				if (GRID[i][j].groupFreedom == 0)
				{
					return true;
				}
			}
		}
	}
	return false;
}

//=================================================
//
//=================================================

bool checkDeletionO()
{
	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			if (GRID[i][j].player == 'O')
			{
				if (GRID[i][j].groupFreedom == 0)
				{
					return true;
				}
			}
		}
	}
	return false;
}

//=================================================
//
//=================================================

void deleteX()
{
	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			if (GRID[i][j].player == 'X')
			{
				if (GRID[i][j].groupFreedom == 0)
				{
					GRID[i][j].player = '.';
					update('.', (int)i, (int)j);
					checkGroupFreedom();
				}
			}
		}
	}
	checkGroupFreedom();
}

//=================================================
//
//=================================================


void deleteO()
{
	printGridWithCurrent(18, 18);
	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			if (GRID[i][j].player == 'O')
			{
				if (GRID[i][j].groupFreedom == 0)
				{
					GRID[i][j].player = '.';
					update('.', (int) i, (int) j);
					checkGroupFreedom();
				}
			}
		}
	}
	
	checkGroupFreedom();
}

//=================================================
//
//=================================================

void checkGroupFreedom()
{

	//int countX = 0;
	//int firstX = 0;
	//int firstO = 0;
	//int countXTemp = 0;

	vector<GroupMember> groupMembersXStack;
	vector<GroupMember> countFreedoms;
	stack<GroupMember> stackO;
	stack<GroupMember> stack;
	
	
	vector<GroupMember> groupMembersOStack;
	vector<GroupMember> countFreedomsO;
	
	


	//int countO = 0;
	vector<GroupMember> groupMembersO;

	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{

			if (GRID[i][j].player == 'X')
			{
				GroupMember currentMember;
				currentMember.row = (int)i;
				currentMember.col = (int)j;
				if(!(findMember(groupMembersXStack, currentMember)))
				{
					groupMembersXStack.clear();
					countFreedom(countFreedoms, groupMembersXStack, i, j);
						stack.push(currentMember);
						while (!(stack.empty()))
						{
							if (hasFriends(groupMembersXStack, currentMember.row, currentMember.col))
							{
								//ak ma susedov, najdi prveho, pridaj do stacku a do count pripocitaj neighbours freedom
								currentMember = checkGroupNeighbours(countFreedoms, groupMembersXStack, currentMember.row, currentMember.col);
									stack.push(currentMember);
							}
							else
							{
								stack.pop();
							}
						}
					setFreedomsForCurrentGroup(groupMembersXStack, countFreedoms);
					//countX = 0;
				}
				
				

			}
			if (GRID[i][j].player == 'O')
			{
				GroupMember currentMemberO;
				currentMemberO.row = (int)i;
				currentMemberO.col = (int)j;
				if (!(findMember(groupMembersOStack, currentMemberO)))
				{
					groupMembersOStack.clear();
					countFreedom(countFreedomsO, groupMembersOStack, i, j);
					stackO.push(currentMemberO);
					while (!(stackO.empty()))
					{
						if (hasFriends(groupMembersOStack, currentMemberO.row, currentMemberO.col))
						{
							//ak ma susedov, najdi prveho, pridaj do stacku a do count pripocitaj neighbours freedom
							currentMemberO = checkGroupNeighbours(countFreedomsO, groupMembersOStack, currentMemberO.row, currentMemberO.col);
							stackO.push(currentMemberO);
						}
						else
						{
							stackO.pop();
						}
					}
					setFreedomsForCurrentGroup(groupMembersOStack, countFreedomsO);
					//countX = 0;
				}


			}
		}
	}
	groupMembersXStack.clear();
	groupMembersOStack.clear();
}
//=================================================
//
//=================================================

void setFreedomsForCurrentGroup(vector<GroupMember>& groupMembers, vector<GroupMember>& countFreedom)
{
	if (!(groupMembers.empty()))
	{
		for (size_t k = 0; k < groupMembers.size(); k++)
		{
			GRID[groupMembers[k].row][groupMembers[k].col].groupFreedom = (int) countFreedom.size();
		}
		//groupMembers.clear();

	}
	countFreedom.clear();
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

bool findConfig(vector<string>& vect, string config)
{
	for (size_t i = 0; i < vect.size(); i++)
	{
		if (vect[i] == config)
		{
			CONFIGURATION.push_back(config);
			return true;
		}
	}
	CONFIGURATION.push_back(config);
	return false;
}
//=================================================
//
//=================================================

void countFreedom(vector<GroupMember>& countFreedom, vector<GroupMember> & member, size_t r, size_t c)
{
	int row = (int)r;
	int col = (int)c;
	GroupMember temp;
	temp.row = row;
	temp.col = col;
	/*int count = 0;*/
	if (!(findMember(member, temp)))
	{
		if (GRID[row][col].freedom.up == 1)
		{
			GroupMember tempUP;
			tempUP.row = row - 1;
			tempUP.col = col;
			countFreedom.push_back(tempUP);
		}
		if (GRID[row][col].freedom.down == 1)
		{
			GroupMember tempDOWN;
			tempDOWN.row = row + 1;
			tempDOWN.col = col;
			countFreedom.push_back(tempDOWN);
		}
		if (GRID[row][col].freedom.left == 1)
		{
			GroupMember tempLEFT;
			tempLEFT.row = row;
			tempLEFT.col = col - 1;
			countFreedom.push_back(tempLEFT);
		}

		if (GRID[row][col].freedom.right == 1)
		{
			GroupMember tempRIGHT;
			tempRIGHT.row = row;
			tempRIGHT.col = col + 1;
			countFreedom.push_back(tempRIGHT);
		}
		GroupMember gm;
		gm.row = row;
		gm.col = col;
		member.push_back(gm);
	}	
}

//=================================================
//
//=================================================
bool hasFriends(vector<GroupMember> & seenMembers, int row, int col)
{

	if (GRID[row][col].neighbours.up == 1)
	{
		GroupMember fUP;
		fUP.row = row - 1;
		fUP.col = col;
		if (!(findMember(seenMembers, fUP)))
		{
			return true;
		}
		
	}
	if (GRID[row][col].neighbours.down == 1)
	{
		GroupMember fDOWN;
		fDOWN.row = row + 1;
		fDOWN.col = col;
		if (!(findMember(seenMembers, fDOWN)))
		{
			return true;
		}
	}

	if (GRID[row][col].neighbours.left == 1)
	{
		GroupMember fLEFT;
		fLEFT.row = row;
		fLEFT.col = col - 1;
		if (!(findMember(seenMembers, fLEFT)))
		{
			return true;
		}
	}

	if (GRID[row][col].neighbours.right == 1)
	{
		GroupMember fRIGHT;
		fRIGHT.row = row;
		fRIGHT.col = col + 1;
		if (!(findMember(seenMembers, fRIGHT)))
		{
			return true;
		}
	}
	return false;
	

}

//=================================================
//
//=================================================

GroupMember checkGroupNeighbours(vector<GroupMember>& countFreedoms, vector<GroupMember> & member, int row, int col)
{
	
	//int count = 0;
	//UP
	if(GRID[row][col].neighbours.up == 1)
	{
		GroupMember gm;
		gm.row = row - 1;
		gm.col = col;
		if (!(findMember(member, gm)))
		{
			countFreedom(countFreedoms,member, row - 1, col);
			member.push_back(gm);
			return gm;
		}

	}
	//DOWN
	if (GRID[row][col].neighbours.down == 1)
	{
		GroupMember gm;
		gm.row = row + 1;
		gm.col = col;
		if (!(findMember(member, gm)))
		{
			countFreedom(countFreedoms, member, row + 1, col);
			member.push_back(gm);
			return gm;
		}

	}
	//LEFT
	if (GRID[row][col].neighbours.left == 1)
	{
		GroupMember gm;
		gm.row = row;
		gm.col = col - 1;
		if (!(findMember(member, gm)))
		{
			countFreedom(countFreedoms, member, row, col - 1);
			member.push_back(gm);
			return gm;
		}
	}
	//RIGHT
	if (GRID[row][col].neighbours.right == 1)
	{
		GroupMember gm;
		gm.row = row;
		gm.col = col + 1;
		if (!(findMember(member, gm)))
		{
			countFreedom(countFreedoms, member, row, col + 1);
			member.push_back(gm);
			return gm;
		}
	}
	GroupMember previous;
	previous.row = row;
	previous.col = col;
	return previous;
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

void printGridWithCurrent(int row, int col)
{
	for (size_t i = 0; i < GRID.size(); i++)
	{
		for (size_t j = 0; j < GRID[i].size(); j++)
		{
			if (i == (size_t) row && j == (size_t)col)
			{
				//if (GRID[i][j].player == 'X')
				//{
				//	cout << "C";;
				//}
				//if (GRID[i][j].player == 'O')
				//{
				//	cout << "B";;
				//}
				if (GRID[i][j].player == '.')
				{
					//cout << GRID[i][j].player;
				}

			}
			else 
			{
				if (GRID[i][j].player == '.')
				{
					//cout << GRID[i][j].player;
				}
				else 
				{
					//cout << GRID[i][j].groupFreedom;
				}
				
			}
			
		}
		//cout << endl;
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
				//cout << "Player: " << GRID[i][j].player << " [" << i << " " << j << "] freedom: " << GRID[i][j].groupFreedom << endl;
			}
		}
	}
}


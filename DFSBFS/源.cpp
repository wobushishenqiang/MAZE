#include<iostream>
#include <Windows.h>
#include<vector>
#include<queue>
using namespace std;

#define MAZELENGTH 10
#define MAZEDEPTH 10
#define SLEEP Sleep(500)
#define SLEEPLONG Sleep(1000)

HANDLE hOut;//窗口句柄的变量
COORD pos = { 0,0 };//所要移动的目的地坐标变量
//0 路径 1 墙 2 走过的路径 3 起点 4 终点
int maze[MAZEDEPTH][MAZELENGTH] = {
{ 0,0,0,0,0,1,0,1,1,1 },
{ 1,0,1,1,0,1,0,1,1,1 },
{ 1,0,0,0,1,1,0,0,0,0 },
{ 1,1,1,0,1,1,0,1,1,0 },
{ 1,0,0,0,1,1,0,1,1,0 },
{ 1,0,0,0,0,0,0,1,1,0 },
{ 1,1,1,1,1,1,1,1,1,0 },
{ 1,0,1,1,1,1,0,0,0,0 },
{ 1,0,0,0,0,0,0,1,1,1 },
{ 1,0,1,1,1,1,0,0,0,0 },
};

enum pointtype
{
	ROAD = 0,
	WALL = 1,
	ROADINQUE = 2,
	STARTPOINT = 3,
	ENDPOINT = 4
};

int diretions[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} };
int minstep = MAZELENGTH * MAZEDEPTH + 1;

struct mazepoint
{
	int x;
	int y;
	mazepoint *parent;
	pointtype type;
	bool endreachable = false;
	mazepoint(int i, int j)
	{
		y = i;
		x = j;
	}
	mazepoint(int i, int j, pointtype t)
	{
		y = i;
		x = j;
		type = t;
	}
	mazepoint(int i, int j, mazepoint* parentpoint)
	{
		y = i;
		x = j;
		parent = parentpoint;
	}
	mazepoint(int i, int j, mazepoint* parentpoint, pointtype t)
	{
		y = i;
		x = j;
		parent = parentpoint;
		type = t;
	}
};
vector<mazepoint*> path;
mazepoint startpoint(2, 3, STARTPOINT);
mazepoint endpoint(9, 4 ,ENDPOINT);

void init()
{
	maze[startpoint.y][startpoint.x] = 3;
	maze[endpoint.y][endpoint.x] = 4;
}

void genmaze()
{
}

void printmaze()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	for (int i = 0; i < MAZELENGTH + 2; i++)
		cout << "■";
	cout << endl;
	for (int i = 0; i < MAZEDEPTH; i++)
	{
		
		cout << "■";
		
		for (int j = 0; j < MAZELENGTH; j++)
		{
			if (maze[i][j] == 1)
			{
				cout << "■";
			}
			else if (maze[i][j] == 3)
			{
				cout << "St";
			}
			else if (maze[i][j] == 4)
			{
				cout << "Ed";
			}
			else
			{
				cout << "  ";
			}
		}
		
		cout << "■";
		
		cout << endl;
	}
	for (int i = 0; i < MAZELENGTH + 2; i++)
		cout << "■";
	cout << endl;
}

void finish()
{
	for (auto i : path)
	{
		if (i != &startpoint && i != &endpoint)
			delete i;

	}
	path.clear();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void reset()
{
	minstep = MAZELENGTH * MAZEDEPTH + 1;
	for (int i = 0; i < MAZEDEPTH; i++)
	{
		for (int j = 0; j < MAZELENGTH; j++)
		{
			if (maze[i][j] == 2)
				maze[i][j] = 0;
		}
	}

	for (auto i : path)
	{
		if (i != &startpoint && i != &endpoint)
			delete i;

	}
	path.clear();
	pos.X = 0;//对相应坐标的赋值
	pos.Y = 0;
	SetConsoleCursorPosition(hOut, pos);//移动光标函数 
	printmaze();
}


void printres()
{

	int step = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	if (endpoint.endreachable)
	{
		mazepoint* tmp = endpoint.parent;
		while (tmp->type != STARTPOINT)
		{
			pos.X = (tmp->x + 1) * 2;//对相应坐标的赋值
			pos.Y = tmp->y + 1;
			SetConsoleCursorPosition(hOut, pos);//移动光标函数 
			if (tmp == &startpoint)
				break;
			cout << "**";
			tmp = tmp->parent;
			step++;
		}
		minstep = minstep < step ? minstep : step;
		pos.X = 0;//对相应坐标的赋值
		pos.Y = MAZEDEPTH + 3;
		SetConsoleCursorPosition(hOut, pos);//移动光标函数 
		cout << "MIN MAZE STEP: " << minstep << endl;
	}
	else
	{
		pos.X = 0;//对相应坐标的赋值
		pos.Y = MAZEDEPTH + 3;
		SetConsoleCursorPosition(hOut, pos);//移动光标函数 
		cout << "The end is unreachable!" << endl;
	}

}

void dfswalk(mazepoint *parentpoint, int steplen)
{
	if (endpoint.endreachable == true)
		return;
	int x = parentpoint->x;
	int y = parentpoint->y;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);//获取本运行窗口句柄
	pos.X = (x + 1) * 2;//对相应坐标的赋值
	pos.Y = y + 1;
	SetConsoleCursorPosition(hOut, pos);//移动光标函数 
	if(parentpoint != &startpoint)
		cout << "**";

	
	for (int k = 0; k < 4; k++)
	{
		int newy = y + diretions[k][0];
		int newx = x + diretions[k][1];
		if (newy < 0 || newx < 0 || newy >= MAZEDEPTH || newx >= MAZELENGTH)
			continue;
		if (maze[newy][newx] == 4)
		{
			endpoint.parent = parentpoint;
			endpoint.endreachable = true;
			minstep = steplen;
			return;
		}

		if (maze[newy][newx] == 0)
		{
			maze[y][x] = 2;
			SLEEP;
			mazepoint *newpoint = new mazepoint(newy, newx, parentpoint);
			path.push_back(newpoint);
			dfswalk(newpoint, steplen + 1);
			if (endpoint.endreachable == true)
				return;
			maze[y][x] = 0;
			
		}
	}
	
	pos.X = (x + 1) * 2;//对相应坐标的赋值
	pos.Y = y + 1;
	SetConsoleCursorPosition(hOut, pos);//移动光标函数 
	cout << "  ";
}

void dfs()
{
	dfswalk(&startpoint, 0);
}

void bfs()
{
	/*
	vector<vector<int>> tempres;
	for (int i = 0; i < MAZEDEPTH; i++)
	{
		vector<int> temp(maze[i], maze[i] + MAZELENGTH);
		tempres.push_back(temp);
	}
	*/

	int tempstep = 0;
	queue<mazepoint*> que;
	que.push(&startpoint);
	

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);//获取本运行窗口句柄
	while (!que.empty())
	{

		mazepoint *toppoint = que.front();

		//tempres[toppoint.y][toppoint.x] = 2;


		pos.X = (toppoint->x + 1) * 2;//对相应坐标的赋值
		pos.Y = toppoint->y + 1;
		SetConsoleCursorPosition(hOut, pos);//移动光标函数 


		if (toppoint->x == endpoint.x && toppoint->y == endpoint.y)
		{
			//res = tempres;
			endpoint.endreachable = true;
			endpoint.parent = toppoint->parent;
			break;
		}
		if (toppoint != &startpoint)
		{
			maze[toppoint->y][toppoint->x] = 2;
			cout << "**";
		}

		int needsleep = 0;
		for (int k = 0; k < 4; k++)
		{
			int newi = toppoint->y + diretions[k][0];
			int newj = toppoint->x + diretions[k][1];
			if (newi < 0 || newj < 0 || newi >= MAZEDEPTH || newj >= MAZELENGTH)
				continue;
			if (maze[newi][newj] == 0 || maze[newi][newj] == 4)
			{
				mazepoint* newpoint = new mazepoint(newi, newj, toppoint);
				path.push_back(newpoint);
				que.push(newpoint);
				needsleep = 1;
			}
		}
		que.pop();
		if (needsleep)
			SLEEP;

	}

}

void directionChange(mazepoint startpoint, mazepoint endpoint)
{
	int dify = endpoint.y - startpoint.y;
	int difx = endpoint.x - startpoint.x;
	mazepoint y[2] = { mazepoint(-1,0),mazepoint(1,0) };
	mazepoint x[2] = { mazepoint(0,-1),mazepoint(0,1) };
	if (abs(difx) > abs(dify))
	{
		if (difx >= 0)
		{
			mazepoint temp = x[0];
			x[0] = x[1];
			x[1] = temp;
		}
		if (dify >= 0)
		{
			mazepoint temp = y[0];
			y[0] = y[1];
			y[1] = temp;
		}
		diretions[0][0] = x[0].y;
		diretions[0][1] = x[0].x;
		diretions[1][0] = y[0].y;
		diretions[1][1] = y[0].x;
		diretions[2][0] = y[1].y;
		diretions[2][1] = y[1].x;
		diretions[3][0] = x[1].y;
		diretions[3][1] = x[1].x;
	}
	else 
	{
		if (difx >= 0)
		{
			mazepoint temp = x[0];
			x[0] = x[1];
			x[1] = temp;
		}
		if (dify >= 0)
		{
			mazepoint temp = y[0];
			y[0] = y[1];
			y[1] = temp;
		}
		diretions[0][0] = y[0].y;
		diretions[0][1] = y[0].x;
		diretions[1][0] = x[0].y;
		diretions[1][1] = x[0].x;
		diretions[2][0] = x[1].y;
		diretions[2][1] = x[1].x;
		diretions[3][0] = y[1].y;
		diretions[3][1] = y[1].x;
	}
}

void axwalk(mazepoint *parentpoint, int steplen)
{
	if (endpoint.endreachable == true)
		return;
	int x = parentpoint->x;
	int y = parentpoint->y;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);//获取本运行窗口句柄
	pos.X = (x + 1) * 2;//对相应坐标的赋值
	pos.Y = y + 1;
	SetConsoleCursorPosition(hOut, pos);//移动光标函数 
	if (parentpoint != &startpoint)
		cout << "**";
	directionChange(*parentpoint, endpoint);
	for (int k = 0; k < 4; k++)
	{
		int newy = y + diretions[k][0];
		int newx = x + diretions[k][1];
		if (newy < 0 || newx < 0 || newy >= MAZEDEPTH || newx >= MAZELENGTH)
			continue;
		if (maze[newy][newx] == 4)
		{
			endpoint.parent = parentpoint;
			endpoint.endreachable = true;
			minstep = steplen;
			return;
		}

		if (maze[newy][newx] == 0)
		{
			maze[y][x] = 2;
			SLEEP;
			mazepoint *newpoint = new mazepoint(newy, newx, parentpoint);
			path.push_back(newpoint);
			dfswalk(newpoint, steplen + 1);
			if (endpoint.endreachable == true)
				return;
			maze[y][x] = 0;

		}
	}

	pos.X = (x + 1) * 2;//对相应坐标的赋值
	pos.Y = y + 1;
	SetConsoleCursorPosition(hOut, pos);//移动光标函数 
	cout << "  ";
}

void ax()
{
	
	axwalk(&startpoint, 0);
}

int main()
{

	init();
	printmaze();
	SLEEPLONG;
	//dfs();
	//bfs();
	ax();
	cout << endl;
	printres();
	pos.X = 0;//对相应坐标的赋值
	pos.Y = MAZEDEPTH + 6;
	SetConsoleCursorPosition(hOut, pos);//移动光标函数 
	//reset();
	finish();
	system("pause");
	return 0;
}

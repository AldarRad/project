#include <iostream>
#include <windows.h>
#include <vector>
#include <ctime>
#include <stack>
#include <conio.h>
using namespace std;
//Hello
void gotoxy(int x, int y)// установка курсора в нужную позицию на консоли 
{
	COORD p = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
enum ConsoleColor 
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GRAY = 7,
	DARKGRAY = 8,
	LIGHT_GREEN = 9,
	LIGHT_BLUE = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};
void setColor(int background, int text) // для цвета текста 
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
	
const int BORDER = 100; // граница поля 	
const int EMPTY_CELL = 0; // пустая ячейка 
const int MINE = 10; // мина

class Map // игровое поле
{
private:
	vector <vector<int>> map; // который скрыт от пользователя
	vector <vector<int>> mask;// который показан для пользователя
public:
	int count;
	int countmine;
	int size;// размер поля включая границы
	Map()
	{
		count = 0;
		size = 10;
		mask.resize(size);
		for (int i = 0; i < size; ++i) {
			mask[i].resize(size, 0);
		}
	}
	int openCell(int x, int y) // открывает клетки 
	{
		int result = 1;
		mask[x][y] = 1;
		if (map[x][y] == MINE)
		{
			result = MINE;
		}
		else if (map[x][y] == EMPTY_CELL)
		{
			result = EMPTY_CELL;
		}
		show();
		return result;
	}
	bool isBorder(int x, int y) // проверка на границу
	{
		if (x < 0 || x >= size)
			return false;
		if (y < 0 || y >= size)
			return false;
		if (map[x][y] == BORDER)
		{
			return true;
		}
		return false;
	}
	void initVec(vector <vector<int>>& vec)// установка границ поля
	{
		for (int i = 0; i < size; i++)
		{
			vector <int> temp;
			for (int j = 0; j < size; j++)
			{
				if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
					temp.push_back(BORDER);
				else
					temp.push_back(EMPTY_CELL);
			}
			vec.push_back(temp);
		}
	}
	void initMap()
	{
		initVec(map);
	}
	void initMask()
	{
		mask.clear();
		for (int i = 0; i < size; i++)
		{
			mask.push_back(map[i]);
		}
	}
	void coutColor(char ch, int color)
	{
		setColor(WHITE, color);
		cout << ch;
		setColor(WHITE, BLACK);
	}
	void fill(int px, int py)// рекурсивно открывает все пустые клетки 
	{
		stack <int> stk;
		stk.push(px);
		stk.push(py);
		int x = 0, y = 0;
		while (true)
		{
			y = stk.top();
			stk.pop();
			x = stk.top();
			stk.pop();
			if (map[x][y + 1] == EMPTY_CELL && mask[x][y + 1] == 0)
			{
				stk.push(x);
				stk.push(y + 1);
			}
			mask[x][y + 1] = 1;
			if (map[x][y - 1] == EMPTY_CELL && mask[x][y - 1] == 0)
			{
				stk.push(x);
				stk.push(y - 1);
			}
			mask[x][y - 1] = 1;
			if (map[x + 1][y + 1] == EMPTY_CELL && mask[x + 1][y + 1] == 0)
			{
				stk.push(x + 1);
				stk.push(y + 1);
			}
			mask[x + 1][y + 1] = 1;
			if (map[x + 1][y - 1] == EMPTY_CELL && mask[x + 1][y - 1] == 0)
			{
				stk.push(x + 1);
				stk.push(y - 1);
			}
			mask[x + 1][y - 1] = 1;
			if (map[x - 1][y + 1] == EMPTY_CELL && mask[x - 1][y + 1] == 0)
			{
				stk.push(x - 1);
				stk.push(y + 1);
			}
			mask[x - 1][y + 1] = 1;
			if (map[x - 1][y - 1] == EMPTY_CELL && mask[x - 1][y - 1] == 0)
			{
				stk.push(x - 1);
				stk.push(y - 1);
			}
			mask[x - 1][y - 1] = 1;
			if (map[x - 1][y] == EMPTY_CELL && mask[x - 1][y] == 0)
			{
				stk.push(x - 1);
				stk.push(y);
			}
			mask[x - 1][y] = 1;
			if (map[x + 1][y] == EMPTY_CELL && mask[x + 1][y] == 0)
			{
				stk.push(x + 1);
				stk.push(y);
			}
			mask[x + 1][y] = 1;
			if (stk.empty())
			{
				break;
			}
		}
	}
	void show() // выводит игровое поле на консоль 
	{
		gotoxy(0, 0);
		count = 0;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (mask[j][i] == EMPTY_CELL)
				{
					cout << ".";
					continue;
				}
				if (map[j][i] == BORDER)
				{
					coutColor('#', LIGHT_BLUE);
				}
				else if (map[j][i] == EMPTY_CELL) {
					cout << " ";
					count++;
				}
				else if (map[j][i] == MINE) {
					cout << "*";
					count++;
				}
				else if (map[j][i] == 1) {
					coutColor('1', BLUE);
					count++;
				}
				else if (map[j][i] == 2) {
					coutColor('2', GREEN);
					count++;
				}
				else if (map[j][i] == 3) {
					coutColor('3', RED);
					count++;
				}
				else if (map[j][i] == 4) {
					coutColor('4', MAGENTA);
					count++;
				}
				else if (map[j][i] == 5) {
					coutColor('5', BROWN);
					count++;
				}
				else if (map[j][i] == 6) {
					coutColor('6', DARKGRAY);
					count++;
				}
				else if (map[j][i] == 7) {
					coutColor('7', LIGHT_GREEN);
					count++;
				}
				else if (map[j][i] == 8) {
					coutColor('8', LIGHT_CYAN);
					count++;
				}

			}
			cout << endl;
		}
	}
	void setRandMines(int numMines)// случайная расстановка мин
	{
		countmine = numMines;
		if (numMines >= (size - 2) * (size - 2))
		{
			cout << "Too many mines" << endl;
			return;
		}
		for (int i = 0; i < numMines; i++)
		{
			int x = 0;
			int y = 0;
			// поиск пустой ячейки, не занятой миной
			do {
				srand(time(0));
				x = rand() % (size - 2) + 1;	
				y = rand() % (size - 2) + 1;
			} while (map[x][y] == MINE);// MINE = 10
			map[x][y] = MINE;
		}
	}
	void setDigits()// расстановка чисел на игровом поле(количество мин вокруг каждой клетки)
	{
		int d = 0;
		for (int i = 1; i < size - 1; i++)
		{
			for (int j = 1; j < size - 1; j++)
			{
				if (map[j][i] == MINE)
					continue;
				if (map[j][i + 1] == MINE)
					d++;
				if (map[j][i - 1] == MINE)
					d++;
				if (map[j + 1][i + 1] == MINE)
					d++;
				if (map[j + 1][i - 1] == MINE)
					d++;
				if (map[j - 1][i + 1] == MINE)
					d++;
				if (map[j - 1][i - 1] == MINE)
					d++;
				if (map[j + 1][i] == MINE)
					d++;
				if (map[j - 1][i] == MINE)
					d++;
				map[j][i] = d;
				d = 0;
			}
		}
	}
};
class Keyboard
{
private:
	int ch = 0;
public:
	Keyboard()
	{
		ch = 0;
	}
	void waitKey()// ждем начало клавиши	

	{
		ch = _getch();
	}
	int getKey()
	{
		return ch;
	}

};
class Cursor
{
private:
	int x = 1, y = 1;
	int tx = 1, ty = 1;
public:
	void save()
	{
		tx = x;
		ty = y;
	}
	void undo()
	{
		x = tx;
		y = ty;
	}
	void incX()
	{
		x++;
	}
	void decX()
	{
		x--;
	}
	void incY()
	{
		y++;
	}
	void decY()
	{
		y--;
	}
	int getX()
	{
		return x;
	}
	int getY()
	{
		return y;
	}
	void move()
	{
		gotoxy(x, y);
	}
};

class Game
{
private:
	void showLogo()// гототип игры 
	{
		gotoxy(40, 9);
		cout << "Saper" << endl;
		Sleep(2000);//сколько секунд надпись будет отображаться  
		system("cls");
	}
public:
	void gameOver()
	{
		gotoxy(40, 9);
		cout << "Game over";
		Sleep(2000);
		gotoxy(0, 15);
		system("pause");
	}
	void win_game()
	{
		gotoxy(40, 9);
		cout << "You win, well done";
		Sleep(2000);
		gotoxy(0, 15);
		system("pause");
	}
	void run() // запускает игру 
	{
		showLogo();
		Map map;
		map.initMap();
		map.initMask();
		map.setRandMines(11);
		map.setDigits();
		map.show();
		Keyboard kb;
		Cursor cs;

		cs.move();
		bool exit = false;
		while (!exit)
		{
			kb.waitKey();
			cs.save();
			switch (kb.getKey())
			{
			case 77: cs.incX(); break; // вправо 
			case 80: cs.incY(); break; // вниз
			case 75: cs.decX(); break; // влево
			case 72: cs.decY(); break; // вверх
			case 13:
				int result = map.openCell(cs.getX(), cs.getY());
				if (((map.size - 2) * (map.size - 2)) - map.countmine == map.count)
				{
					win_game();
					exit = true;
				}
				if (result == MINE)
				{
					gameOver();
					exit = true;
				}
				else
					if (result == EMPTY_CELL)
					{
						map.fill(cs.getX(), cs.getY());
						map.show();
					}
				break;
			}
			if (map.isBorder(cs.getX(), cs.getY()))//чтобы не выходить за пределы поля
			{
				cs.undo();
			}
			cs.move();
		}
	}
};
int main()
{
	Game game;
	game.run();
	cout << "\n\n\n\n\n\n\n\n";
}
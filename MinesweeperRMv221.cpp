#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#ifdef __gnu_linux__
#include <pthread.h>
#include <termios.h>
#elif _WIN32
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#endif

using namespace std;

//Variabile
struct display{
	int d;
	int v;
} a[2][20][40];
const int dx[] = {-1, 0, 1, 0,-1,-1,1,1};
const int dy[] = {0, 1, 0, -1,1,-1,1,-1};
char p1[31], p2[31];
char r;
struct coord{
	int x;
	int y;
} al[2], target[2];
int mines[2], miness, bonus[2], bonuss, ailvl[2] = {0, 0};
vector <coord> array;
coord u[2];
int sel = 1;
bool abonus[2][7];
int b[2][8] = {{1,1,1,1,1,1,1,-1},{0,15,25,40,55,60,80,100}};
int won;
bool LuckyGuess[2] = {true,true};
bool play[2] = {true,true};
char Controls[2][6] = {{'s','a','d','w','c','z'}, {'5','4','6','8','0','+'}};
int bonustime[2][7];
int series = 2;

//Functii

//Meniuri
void Start();
void InGameMenu();
void MainMenu();
void Begin();
void Options();
void StartGame();
void Tutorial();
//Gameplay related
bool Check(int i, int j, int r, int q, int z);
void Clear (int z);
void Generate (int x, int y, int z);
void Test (int x, int y, int z);
void Show(int x, int y, int z);
void Calculate (int z);
void Interface(int u1x, int u1y, int u2x, int u2y);
bool Conditions (int x, int y, int z);
//AI related
void AI(int z);
void TargetS1(int x, int y, int z); //1.0 Series
bool TargetS2(int x, int y, int z); //2.0 Series
bool TFree(int x, int y, int z); // 2.0 Series Target selector
//Bonusuri
void StartBonus(int x, int y, int z, int w);
void ExpireBonus();
void Expand (int z);
void Joker (int x, int y, int z);
void Stomp (int x, int y, int z);
void StompCheck (int i, int j, int z);
//Afisari (Dual system)
void fout (int disp, int val, int z);
void fin();
void clrscr();
void LampProtocol(int val,int disp, int z);
//Linux specific
#ifdef __gnu_linux__
void Tada()
{
	// Black magic to prevent Linux from buffering keystrokes.
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
#endif
//Windows specific
#ifdef _WIN32 // Black magic to generate colors for windows
WORD GetConsoleTextAttribute (HANDLE hCon)
{
  CONSOLE_SCREEN_BUFFER_INFO con_info;
  GetConsoleScreenBufferInfo(hCon, &con_info);
  return con_info.wAttributes;
}
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int saved_colors = GetConsoleTextAttribute(hConsole);
#endif

//Functii dual-system
int main ()
{
	Begin();
	#ifdef __gnu_linux__
	Tada();
	#endif
	MainMenu();
	clrscr();
	cout << "Thanks for playing! For feedback & others go to cronologium.wordpress.com\n";
	cout << "Press any key to continue...\n";
	return 0;
}

void fin()
{
	#ifdef _WIN32
	r = getch();
	#elif __gnu_linux__
	cin >> r;
	#endif
}

void clrscr()
{
	#ifdef _WIN32
	system ("CLS");
	#elif __gnu_linux__
	system ("clear");
	#endif
}

void fout (int disp, int val, int z)
{
	if (abonus[z][4] == false) // Freeze
	{
		#ifdef _WIN32
		if (disp % 10 == 1)
		{
			if (abonus[z][5] == false) //White
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN| 8);
			if (disp == 101)
				cout << "D";
			if (disp == 111)
				cout << "T";
			if (disp == 121)
				cout << "E";
			if (disp == 131)
				cout << "J";
			if (disp == 141)
				cout << "F";
			if (disp == 151)
				cout << "W";
			if (disp == 161)
				cout << "L";
			if (disp == 1)
			{
				if (!val)
				cout << ' ';
				else
				{
				if (!abonus[z][5]) //White
					{
					if (val == 1)
						SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE| 1);
					if (val == 2)
						SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN| 1);
					if (val == 3)
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 4);
					if (val == 4)
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 9);
					if (val == 5)
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 10);
					if (val == 6)
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 4);
					if (val == 7)
						SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE| 9);
					if (val == 8)
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 3);
					}
                cout << val;
				}
			}
		}
		else
			if (disp % 10 == 0)
				cout << "?";
		else
			if (disp % 10 == 2)
			{
				if (!abonus[z][5]) //White
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 8);
				cout << "#";
			}
		SetConsoleTextAttribute(hConsole, saved_colors);
		#endif

		#ifdef __gnu_linux__
		if (disp % 10 == 1)
		{
			if (abonus[z][5] == false) // White
				cout << "\033[0;31m";
			if (disp == 101)
					cout << "D";
			else
				if (disp == 111)
					cout << "T";
			else
				if (disp == 121)
					cout << "E";
			else
				if (disp == 131)
					cout << "J";
			else
				if (disp == 141)
					cout << "F";
			else
				if (disp == 151)
					cout << "W";
			else
				if (disp == 161)
					cout << "L";
			else
			if (disp == 1)
			{
				if (!val)
					cout << ' ';
				else
				{
					if (abonus[z][5] == false) // White
					{
						if (val == 1)
							cout << "\033[0;34m";
						if (val == 2)
							cout << "\033[0;35m";
						if (val == 3)
							cout << "\033[0;36m";
						if (val == 4)
							cout << "\033[0;33m";
						if (val == 5)
							cout << "\033[0;34m";
						if (val == 6)
							cout << "\033[0;35m";
						if (val == 7)
							cout << "\033[0;36m";
						if (val == 8)
							cout << "\033[0;33m";
					}
					cout << val;
				}
			}
		}
		if (disp % 10 == 0)
			cout << "?";
		if (disp % 10 == 2)
		{
			if (abonus[z][5] == false) // White
				cout << "\033[0;31m";
			cout << "#";
		}
		cout << "\033[0m";
		#endif
	}
	else
	{
		#ifdef _WIN32
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE| 14);
		cout << "*";
		SetConsoleTextAttribute(hConsole, saved_colors);
		#elif __gnu_linux__
		cout << "\033[0;34m*\033[0m";
		#endif
	}
}

//Functii standard
void Begin()
{
	clrscr();
	cout << "Rival Minfields v221 Beta @Cronologium\n\n";
	cout << "For feedback and updates go to cronologium.wordpress.com\n\n";
	cout << "Input names: \n";
	cout << "Player 1: ";
	cin.getline (p1, 31);
	cout << "Player 2: ";
	cin.getline (p2, 31);
	al[0].x = al[1].x = 16;
	al[0].y = al[1].y = 18;
	array.resize (1600);
}

void MainMenu()
{
	clrscr();
	cout << "Rival Minefields v221 Beta @Cronologium\n\n";
	cout << "Main Menu\n";
	if (sel == 1)
		cout << " >> ";
	else
		cout << "    ";
	cout << "  New Game  ";
	if (sel == 1)
		cout << " <<\n";
	else
		cout << "\n";
	if (sel == 2)
		cout << " >> ";
	else
		cout << "    ";
	cout << "Game Options";
	if (sel == 2)
		cout << " <<\n";
	else
		cout << "\n";
	if (sel == 3)
		cout << " >> ";
	else
		cout << "    ";
	cout << "  Tutorial  ";
	if (sel == 3)
		cout << " <<\n";
	else
		cout << "\n";
	if (sel == 0)
		cout << " >> ";
	else
		cout << "    ";
	cout << "    Exit    ";
	if (sel == 0)
		cout << " <<\n";
	else
		cout << "\n";
	cout << "8 - Up, 2 - Down, 5 - Select";
	fin();
	if (r == '8')
		sel = (sel + 3) % 4;
	if (r == '2')
		sel = (sel + 1) % 4;
	if (r == '5')
	{
		if (sel == 1)
			StartGame();
		else
		if (sel == 2)
		{
			sel = 0;
			Options();
			MainMenu();
		}
		else
		if (sel == 3)
			Tutorial();
		else
		if (sel == 0)
			return;
		else
		{
			MainMenu();
			return;
		}
	}
	else
	{
		MainMenu();
		return;
	}
}

void Options ()
{
	clrscr();
	cout << "Rival Minefields v221 Beta @Cronologium\n\n";
	cout << " Options\n";
	if (!sel)
		cout << " >> ";
	else
		cout << "    ";
	cout << "AI Version: " << series << ".0 Series";
	if (sel == 1)
		cout << "\n >> ";
	else
		cout << "\n    ";
	cout << "Player 1: ";
	if (play[0] == false && ailvl[0] < 3)
		cout << "AI level " << ailvl[0] + 1;
	else
	if (play[0])
		cout << p1;
	else
		cout << "Chuck Norris";
	if (sel == 2)
		cout << "\n >> ";
	else
		cout << "\n    ";
	cout << "Player 2: ";
	if (play[1] == false && ailvl[1] < 3)
		cout << "AI level " << ailvl[1] + 1;
	else
	if (play[1])
		cout << p2;
	else
		cout << "Chuck Norris";
	if (sel == 3)
		cout << "\n >> ";
	else
		cout << "\n    ";
	cout << "Bonuses: ";
	if (!bonuss)
		cout << "None";
	else
		cout << bonuss;
	for (int i = 0; i < 7; ++i)
	{
		if (sel == i + 4)
			cout << "\n >> ";
		else
			cout << "\n    ";
		if (!i)
			cout << "Dig: ";
		if (i == 1)
			cout << "Throw: ";
		if (i == 2)
			cout << "Expansion: ";
		if (i == 3)
			cout << "Joker: ";
		if (i == 4)
			cout << "Freeze: ";
		if (i == 5)
			cout << "White: ";
		if (i == 6)
			cout << "Lamp: ";
		if (b[0][i] == 1)
			cout << "On";
		else
			cout << "Off";
	}
	if (sel == 11)
		cout << "\n >> ";
	else
		cout << "\n    ";
	cout << "Back to main menu";
	cout << "\n8 - Up, 5 - Select, 2 - Down";
	fin();
	if (r == '2')
		sel = (sel + 1) % 12;
	if (r == '8')
		sel = (sel + 11) % 12;
	if (r == '5')
	{
		if (!sel)
			series = series % 2 + 1;
		if (sel < 3 && sel > 0)
		{
			if (play[sel-1] == false && ailvl[sel-1] < 3)
				ailvl[sel-1]++;
			else
				if (play[sel-1])
				{
					play[sel-1] = false;
					ailvl[sel-1] = 0;
				}
			else
				play[sel-1] = true;
		}
		else
		if (sel == 3)
			bonuss = (bonuss + 5) % 25;
		else
		if (sel < 11 && b[0][sel-4] == false)
			b[0][sel-4]++;
		else
			if (sel < 11 && b[0][sel-4])
				b[0][sel-4]--;
		else
		if (sel == 11)
			return;
	}
	Options();
	return;
}

void StartGame()
{
	clrscr();
	cout << "(1) Standard   (15% mines)\n";
	cout << "(2) Medium     (20% mines)\n";
	cout << "(3) Brainstorm (25% mines)\n";
	fin();
	if (r == '1')
		miness = 44;
	else
	if (r == '2')
		miness = 58;
	else
	if (r == '3')
		miness = 72;
	else
	{
		StartGame();
		return ;
	}
	InGameMenu();
	return;
}

void InGameMenu()
{
	clrscr();
	for (int z = 0; z < 2; ++z)
	{
		mines[z] = miness;
		bonus[z] = bonuss;
		Clear(z);
		target[z].x = 0;
		target[z].y = 0;
		for (int i = 0; i < 7; ++i)
		{
			abonus[z][i] = 0;
			mines[z] = miness;
			bonus[z] = bonuss;
			al[z].x = 16;
			al[z].y = 18;
		}
	}
	u[0].x = u[0].y = u[1].x = u[1].y = 0;
	time_t go = time(0);
	double stop = go + 2;
	while (go <= stop)
	{
		go = time (0);
		if (stop - go == 1 || stop - go == 2 || stop - go == 3 || stop - go == 4 || stop - go == 5)
		{
			clrscr();
			cout << p1 << " vs. " << p2 << "\n";
			cout << "Your game starts in " << stop - go << " seconds...\n";
		}
	}
	time_t st = time (0);
	LuckyGuess[0] = LuckyGuess[1] = true;
	won = 0;
	while (!won)
	{
		for (int z = 0; z < 2; ++z)
		{
			int unk = 0;
			for (int i = 0; i < al[z].x; ++i)
				for (int j = 0; j < al[z].y; ++j)
					if (a[z][i][j].d % 10 == 1)
						unk++;
			if (unk == al[z].x * al[z].y - mines[z])
			{
				won = z + 1;
				break;
			}
		}
		Interface(u[0].x, u[0].y, u[1].x, u[1].y);
		ExpireBonus();
		if (play[0] || play[1])
			fin();
		else
			usleep(200000);
		ExpireBonus();
		//Players
		for (int z = 0; z < 2; ++z)
		if (play[z] == true)
		{
			//Movement
			if ((r == Controls[z][0]) && u[z].x + 1 < al[z].x)
				u[z].x++;
			if ((r == Controls[z][1]) && u[z].y - 1 >= 0)
				u[z].y--;
			if ((r == Controls[z][2]) && u[z].y + 1 < al[z].y)
				u[z].y++;
			if ((r == Controls[z][3]) && u[z].x - 1 >= 0)
				u[z].x--;
			//Marking
			if ((r == Controls[z][4]) && a[z][u[z].x][u[z].y].d % 10 == 0)
				a[z][u[z].x][u[z].y].d += 2;
			else
				if ((r == Controls[z][4]) && a[z][u[z].x][u[z].y].d % 10 == 2)
					a[z][u[z].x][u[z].y].d -= 2;
			//Selecting
			if ((r == Controls[z][5]) && a[z][u[z].x][u[z].y].d != 1 && a[z][u[z].x][u[z].y].d % 10 != 2 && !abonus[z][4] && !abonus[z][0] && !abonus[z][1] && !abonus[z][3])
			{
				if (!a[z][u[z].x][u[z].y].d)
				{
					if (LuckyGuess[z])
						Generate (u[z].x, u[z].y, z);
					Test (u[z].x, u[z].y, z);
				}
				else
				if (a[z][u[z].x][u[z].y].d % 10 == 1)
				{
					if ((a[z][u[z].x][u[z].y].d - 101) != 20 || (a[z][u[z].x][u[z].y].d - 101 == 20 && !LuckyGuess[(z+1)%2]))
					{
						StartBonus (u[z].x, u[z].y, z, ((a[z][u[z].x][u[z].y].d - 101) / 10));
						a[z][u[z].x][u[z].y].d = 1;
					}
				}
				else
				if (a[z][u[z].x][u[z].y].d % 10 == 0)
					a[z][u[z].x][u[z].y].d++;
			}
			else
			if (r == Controls[z][5])
			{
				if (abonus[z][0])
					Stomp (u[z].x, u[z].y, z);
				if (abonus[z][1])
				{
					if (LuckyGuess[(z+1)%2])
						Generate (u[z].x, u[z].y, (z+1)%2);
					Test (u[z].x, u[z].y, ((z+1)%2));
					abonus[z][1] = false;
				}
				if (abonus[z][3])
					Joker (u[z].x, u[z].y, z);
			}
		}
		else
			AI(z);
	}
	clrscr();
	time_t en = time(0);
	cout << "Game ended. \n";
	cout << "Time passed: " << en - st << " seconds\n";
		if (won == 1)
			cout << p1 << " won!";
		else
			cout << p2 << " won!";
	if (ailvl[1] == -1)
		ailvl[1] = 0;
	fin();
	MainMenu();
}

void Clear(int z)
{
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 40; ++j)
			a[z][i][j].v = 0, a[z][i][j].d = 0;
}

void Generate (int x, int y, int z)
{
	srand((unsigned)time(0));
	int t;
	int aux = mines[z];
	while (aux)
	{
		t = rand() % (al[z].x * al[z].y);
		if (!a[z][t/al[z].y][t%al[z].y].v && (t / al[z].y) != x && (t % al[z].y) != y && Check(t / al[z].y, t % al[z].y, x , y, z))
			a[z][t/al[z].y][t %  al[z].y].v = -1, aux--;
	}
	//Warning! Neoptim!
	int i;
	for (i = 0; !b[0][i] && i < 7;++i);
	if (i < 7)
		aux = bonus[z];
	while (aux)
	{
		t = rand() % (al[z].x * al[z].y);
		if (!a[z][t/al[z].y][t%al[z].y].v && (t / al[z].y) != x && (t % al[z].y) != y && Check(t / al[z].y, t % al[z].y, x , y, z) && !a[z][t/al[z].y][t%al[z].y].d)
		{
			int w;
			w = rand() % 100;
			for (int bb = 0; bb < 7 && w > b[1][bb]; ++bb)
				if (b[0][bb])
					a[z][t/al[z].y][t %  al[z].y].d = 100 + bb * 10;
				else
					a[z][t/al[z].y][t %  al[z].y].d = 0;
			if (a[z][t/al[z].y][t %  al[z].y].d)
				aux--;
		}
	}
	Calculate(z);
	LuckyGuess[z] = false;
}

bool Check(int i, int j, int r, int q, int z)
{
	for (int k = 0; k < 8; ++k)
	{
		int ii = r + dx[k];
		int jj = q + dy[k];
		if (ii >= 0 && jj >= 0 && ii < al[z].x && jj < al[z].y && ii == i && jj == j)
			return false;
	}
	return true;
}

void Calculate(int z)
{
	for (int i = 0; i < al[z].x; ++i)
		for (int j = 0; j < al[z].y; ++j)
			if (a[z][i][j].v != -3 && a[z][i][j].v != -1)
				a[z][i][j].v = 0;
			else
				a[z][i][j].v = -1;
	for (int i = 0; i < al[z].x; ++i)
		for (int j = 0; j < al[z].y; ++j)
			if (a[z][i][j].v == -1)
			{
				a[z][i][j].v = -1;
				for (int k = 0; k < 8; ++k)
				{
					int ii = i + dx[k];
					int jj = j + dy[k];
					if (ii >= 0 && jj >= 0 && ii < al[z].x && jj < al[z].y && a[z][ii][jj].v != -1)
					{
						a[z][ii][jj].v++;
					}
				}
			}
}

void Test (int x, int y, int z)
{
	if (a[z][x][y].v == -1)
	{
		Clear(z);
		LuckyGuess[z] = true;
		return;
	}
	if (a[z][x][y].v > 0)
		a[z][x][y].d = 1;
	else
	if (!a[z][x][y].v)
		Show(x, y, z);
}

void Show (int x, int y, int z)
{
	int st, dr;
	st = dr = 0;
	array[dr].x = x;
	array[dr++].y = y;
	a[z][x][y].d = 1;
	int i, j;
	while (st < dr)
	{
		i = array[st].x;
		j = array[st].y;
		for (int k = 0; k < 4; ++k)
		{
			int ii = i + dx[k];
			int jj = j + dy[k];
			if (ii < al[z].x && jj < al[z].y && ii >= 0 && jj >= 0 && a[z][ii][jj].v == 0 && a[z][ii][jj].d == 0)
			{
				array[dr].x = ii;
				array[dr++].y = jj;
					a[z][ii][jj].d = 1;
			}
		}
		for (int k = 0; k < 8; ++k)
		{
			int ii = i + dx[k];
			int jj = j + dy[k];
			if (ii < al[z].x && jj < al[z].y && ii >= 0 && jj >= 0 && a[z][ii][jj].v > 0 && a[z][ii][jj].d < 100)
				a[z][ii][jj].d = 1;
		}
		st++;
	}
}

void StartBonus (int x, int y, int z, int w)
{
	if (w == 1) // Hit
	{
		abonus[z][w] = true;
		time_t expire = time(0) + 5;
		bonustime[z][w] = expire;
	}
	else
	if (w == 0 || w == 3) //Stomp & Joker
	{
		abonus[z][w] = true;
		if (!w)
		{
			time_t expire = time(0) + 5;
			bonustime[z][0] = expire;
		}
	}
	else
	if (w == 2 && al[z].y > 5) // Expand
	{
			a[z][u[z].x][u[z].y].d = 1;
			Expand (z);
	}
	else
	if (w == 4 || w == 5 || w == 6) //Freeze & White & Lamp
	{
		if (w < 6)
			abonus[(z + 1) % 2][w] = true;
		else
			if (w == 6)
				abonus[z][w] = true;
		if (w < 6)
		bonustime[(z+1) % 2][w] = time(0) + (w - 3) * 3;
		else
			if (w == 6)
				bonustime[z][w] = time(0) + 5;
	}
	return;
}

void Interface (int u1x, int u1y, int u2x, int u2y) //Neoptim
{
	clrscr();
	for (int i = 0; i < (al[0].y + al[1].y) * 2 + 5; ++i)
		cout << "-";
	cout << "\n";
	for (int i = 0; i < al[0].x; ++i)
	{
		cout << "|";
		for (int j = 0; j < al[0].y; ++j)
		{
			if (u1x == i && j == u1y)
			{
				if (!abonus[0][0] && !abonus[0][1] && !abonus[0][3])
					cout << ">";
				else
					cout << "[";
			}
			else
				if (u1x == i && j == u1y + 1)
				{
					if (!abonus[0][0] && !abonus[0][1] && !abonus[0][3])
						cout << "<";
					else
						cout << "]";
				}
			else
				cout << ' ';
			if (!abonus[0][6] || (abonus[0][6] && !(max (i - u1x, u1x -i) <= 1 && max (j - u1y, u1y - j) <= 1)) || abonus[0][4])
				fout (a[0][i][j].d, a[0][i][j].v, 0);
			else
				LampProtocol(a[0][i][j].v, a[0][i][j].d, 0);
			if (u1y == al[0].y - 1 && j == al[0].y - 1 && u1x == i)
			{
				if (!abonus[0][0] && !abonus[0][1] && !abonus[0][3])
					cout << "<";
				else
					cout << "]";
			}
				if (j == al[0].y - 1 && (u1y != al[0].y - 1 || u1x != i))
					cout << " ";
		}
		cout << "|";
		for (int j = 0; j < al[1].y; ++j)
		{
			if (u2x == i && j == u2y)
			{
				if (!abonus[1][0] && !abonus[1][1] && !abonus[1][3])
					cout << ">";
				else
					cout << "[";
			}
			else
				if (u2x == i && j == u2y + 1)
				{
					if (!abonus[1][0] && !abonus[1][1] && !abonus[1][3])
						cout << "<";
					else
					cout << "]";
				}
			else
				cout << ' ';
            if (!abonus[1][6] || (abonus[1][6] && !(max (i - u2x, u2x -i) <= 1 && max (j - u2y, u2y - j) <= 1)) || abonus[1][4])
				fout (a[1][i][j].d, a[1][i][j].v, 1);
			else
				LampProtocol(a[1][i][j].v, a[1][i][j].d, 1);
			if (u2y == al[1].y - 1 && j == al[1].y - 1 && u2x == i)
			{
				if (!abonus[1][0] && !abonus[1][1] && !abonus[1][3])
					cout << "<";
				else
					cout << "]";
			}
			if (j == al[1].y - 1 && (u2y != al[1].y - 1 || u2x != i))
				cout << " ";
		}
		cout << "|\n";
	}
	for (int i = 0; i < (al[0].y+al[1].y) * 2 + 5; ++i)
		cout << "-";
	cout << "\n  ";
	for (int z = 0; z < 2; ++z)
	{
		for (int i = 0; i < 7; ++i)
			if (i != 2)
			{
				if (abonus[z][i])
					cout << "[";
				else
					cout << " ";
				if (!i)
					cout << "D";
				if (i == 1)
					cout << "T";
				if (i == 3)
					cout << "J";
				if (i == 4)
					cout << "F";
				if (i == 5)
					cout << "W";
				if (i == 6)
					cout << "L";
				if (abonus[z][i])
					cout << "]";
				else
					cout << " ";
				if (bonustime[z][i] - time(0) >= 0)
				{
					#ifdef __gnu_linux__
					if (abonus[z][i])
					cout << "\033[0;31m";
					else
					cout << "\033[0;32m";
					#elif _WIN32
					//
					#endif
					cout << "(" << bonustime[z][i] - time(0) << ")";
					#ifdef __gnu_linux__
					cout << "\033[0m";
					#elif _WIN32
					//
					#endif
				}
				else
					cout << "( )";
			}
		cout << "   ";
	}
	cout << "\n";
}

void ExpireBonus()
{
	time_t expire = time(0);
	for (int z = 0; z < 2; ++z)
		for (int i = 0; i < 7; ++i)
		if (i == 1 || i > 3)
			if (bonustime[z][i] < expire && abonus[z][i])
				abonus[z][i] = false;
}

void Expand (int z) // Optim
{
	if (!z)
	{
		for (int i = 0; i < al[1].x; ++i)
			for (int j = al[1].y - 1; j >= 0; --j)
			{
				if (a[1][i][j].v >= 0)
					a[1][i][j].v = 0;
				else
					a[1][i][j+1].v = -1, a[1][i][j].v = 0;
				a[1][i][j+1].d = a[1][i][j].d;
			}
		for (int i = 0; i < al[0].x; ++i)
		{
			a[1][i][0].d = 0;
			if (a[0][i][al[0].y-1].v == -1)
				a[1][i][0].v = -1, a[0][i][al[0].y-1].v = 0, mines[1]++, mines[0]--;
		}
		al[0].y--;
		al[1].y++;
		u[1].y++;
	}
	else
	{
		for (int i = 0; i < al[0].x; ++i)
		{
			a[0][i][al[0].y].d = 0;
			if (a[1][i][0].v == -1)
				a[0][i][al[0].y].v = -1, a[1][i][0].v = 0, mines[0]++, mines[1]--;
		}
		for (int i = 0; i < al[1].x; ++i)
			for (int j = 1; j <= al[1].y; ++j)
			{
				a[1][i][j-1].d = a[1][i][j].d;
				if (a[1][i][j].v >= 0)
					a[1][i][j].v = 0;
				else
					a[1][i][j-1].v = -1, a[1][i][j].v = 0;
			}
		al[0].y++;
		al[1].y--;
		u[1].y--;
	}
	for (z = 0; z < 2; ++z)
		Calculate(z);
}

void Joker (int x, int y, int z)
{
	if (x > 2)
		for (int i = x - 2; i <= x + 2 && i < al[(z+1) % 2].x; ++i)
		{
			if (y > 2)
			for (int j = y - 2; j <= y + 2 && j < al[(z+1) % 2].y; ++j)
				a[(z+1)%2][i][j].d = 0;
			else
				for (int j = 0; j <= y + 2 && j < al[(z+1) % 2].y; ++j)
					a[(z+1) % 2][i][j].d = 0;
		}
	else
		for (int i = 0; i <= x + 2 && i < al[(z+1) % 2].x; ++i)
		{
			if (y > 2)
			for (int j = y - 2; j <= y + 2 && j < al[(z+1) % 2].y; ++j)
				a[(z+1)%2][i][j].d = 0;
			else
				for (int j = 0; j <= y + 2; ++j)
					a[(z+1) % 2][i][j].d = 0;
		}
	abonus[z][3] = false;
}

void Stomp (int x, int y, int z)
{
	if (x > 2)
		for (int i = x - 2; i <= x + 2 && i < al[z].x; ++i)
		{
			if (y > 2)
			for (int j = y - 2; j <= y + 2 && j < al[z].y; ++j)
				StompCheck(i, j, z);
			else
				for (int j = 0; j <= y + 2 && j < al[z].y; ++j)
					StompCheck (i, j, z);
		}
	else
		for (int i = 0; i <= x + 2 && i < al[z].x; ++i)
		{
			if (y > 2)
			for (int j = y - 2; j <= y + 2 && j < al[z].y; ++j)
				StompCheck (i, j, z);
			else
				for (int j = 0; j <= y + 2; ++j)
					StompCheck (i, j, z);
		}
	abonus[z][0] = false;
}

void LampProtocol(int val, int disp, int z)
{
	if (!abonus[z][4])
	{
		#ifdef _WIN32
		if (disp % 10 == 1 && disp > 100)
		{
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN| 8);
			if (disp == 101)
				cout << "D";
			if (disp == 111)
				cout << "T";
			if (disp == 121)
				cout << "E";
			if (disp == 131)
				cout << "J";
			if (disp == 141)
				cout << "F";
			if (disp == 151)
				cout << "W";
			if (disp == 161)
				cout << "L";
		}
		else
		{
			if (val)
			{
			    if (val == 1)
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE| 1);
				if (val == 2)
					SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN| 1);
				if (val == 3)
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 4);
				if (val == 4)
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 9);
				if (val == 5)
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 10);
				if (val == 6)
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 4);
				if (val == 7)
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE| 9);
				if (val == 8)
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 3);
				if (val > 0)
				cout << val;
				else
				{
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED| 8);
					cout << "X";
				}
			}
			else
			cout << ' ';
		}
		SetConsoleTextAttribute(hConsole, saved_colors);
		#elif __gnu_linux__
		if (disp % 10 == 1 && disp > 100)
		{
			cout << "\033[0;31m";
			if (disp == 101)
					cout << "D";
			else
				if (disp == 111)
					cout << "T";
			else
				if (disp == 121)
					cout << "E";
			else
				if (disp == 131)
					cout << "J";
			else
				if (disp == 141)
					cout << "F";
			else
				if (disp == 151)
					cout << "W";
			else
				if (disp == 161)
					cout << "L";
		}
		else
		{
			if (val)
			{
				if (val == 1)
					cout << "\033[0;34m";
				if (val == 2)
					cout << "\033[0;35m";
				if (val == 3)
					cout << "\033[0;36m";
				if (val == 4)
					cout << "\033[0;33m";
				if (val == 5)
					cout << "\033[0;34m";
			if (val == 6)
				cout << "\033[0;35m";
			if (val == 7)
				cout << "\033[0;36m";
			if (val == 8)
				cout << "\033[0;33m";
				if (val > 0)
				cout << val;
				else
					cout << "\033[0;31mX";
			}
			else
				cout << ' ';
		}
		cout << "\033[0m";
		#endif
	}
}

void StompCheck(int i, int j, int z)
{
	if (a[z][i][j].v > 0 && a[z][i][j].d % 10 == 0)
		a[z][i][j].d++;
	else
	if (!a[z][i][j].v && !a[z][i][j].d)
		Show(i, j, z);
	else
	if (a[z][i][j].v == -1)
		a[z][i][j].d = 2;
}

void AI (int z)
{
	if (u[z].x == target[z].x && u[z].y == target[z].y)
	{
		int t = rand() % 100;
		if (LuckyGuess[z] == true && !target[z].x && !target[z].y)
		{
			target[z].x = 8;
			target[z].y = 9;
		}
		else
		if (LuckyGuess[z] == true)
		{
			Generate(u[z].x, u[z].y, z);
			Test(u[z].x, u[z].y, z);
		}
		if (!abonus[z][0] && !abonus[z][1] && !abonus[z][3] && !abonus[z][4])
		{
			if (a[z][u[z].x][u[z].y].v == -1)
			{
				if (ailvl[z] == -1 && t < 10)
					a[z][u[z].x][u[z].y].d = 2;
				else
				if (ailvl[z] == 0 && t < 20)
					a[z][u[z].x][u[z].y].d = 2;
				else
				if (ailvl[z] == 1 && t < 30)
					a[z][u[z].x][u[z].y].d = 2;
				else
				if (ailvl[z] == 2 && t < 50)
					a[z][u[z].x][u[z].y].d = 2;
				else
				if (ailvl[z] == 3 && t < 99)
					a[z][u[z].x][u[z].y].d = 2;
			}
			else
			if (a[z][u[z].x][u[z].y].d % 10 == 0 && a[z][u[z].x][u[z].y].d > 100)
				a[z][u[z].x][u[z].y].d++;
			else
			if (a[z][u[z].x][u[z].y].d % 10 == 1 && a[z][u[z].x][u[z].y].d > 100)
			{
				StartBonus(u[z].x, u[z].y, z, ((a[z][u[z].x][u[z].y].d - 101) / 10));
				a[z][u[z].x][u[z].y].d = 1;
			}
			else
			if (a[z][u[z].x][u[z].y].v > 0)
			{
				if (ailvl[z] == -1 && t < 10)
					a[z][u[z].x][u[z].y].d = 1;
				else
				if (ailvl[z] == 0 && t < 20)
					a[z][u[z].x][u[z].y].d = 1;
				else
				if (ailvl[z] == 1 && t < 30)
				{
					a[z][u[z].x][u[z].y].d = 1;
				}
				else
				if (ailvl[z] == 2 && t < 50)
					a[z][u[z].x][u[z].y].d = 1;
				else
				if (ailvl[z] == 3 && t < 99)
					a[z][u[z].x][u[z].y].d = 1;
			}
			else
			if (!a[z][u[z].x][u[z].y].v && !LuckyGuess[z])
			{
				if (ailvl[z] == -1 && t < 20)
					Show(u[z].x, u[z].y, z);
				if (ailvl[z] == 0 && t < 30)
					Show(u[z].x, u[z].y, z);
				if (ailvl[z] == 1 && t < 40)
					Show(u[z].x, u[z].y, z);
				if (ailvl[z] == 2 && t < 60)
					Show(u[z].x, u[z].y, z);
				if (ailvl[z] == 3)
					Show(u[z].x, u[z].y, z);
			}
			if (!LuckyGuess[z])
			{
				cout << z << ' ' << "\n";
				if (series == 1)
					TargetS1 (u[z].x, u[z].y, z);
				else
					if (series == 2)
						if (!TargetS2 (u[z].x, u[z].y, z))
							TargetS1 (u[z].x, u[z].y, z);
				Calculate(z);
			}
		}
		else
		if (abonus[z][0])
			Stomp(u[z].x, u[z].y, z);
		else
		if (abonus[z][1])
		{
			if (LuckyGuess[(z+1)%2] == true)
				Generate (u[z].x, u[z].y, (z+1)%2);
			Test (u[z].x, u[z].y, (z+1)%2);
			abonus[z][1] = false;
		}
		else
		if (abonus[z][3])
			Joker (u[z].x, u[z].y, z);
	}
	else
	{
		if (u[z].x < target[z].x)
			u[z].x++;
		else
		if (u[z].y > target[z].y)
			u[z].y--;
		else
		if (u[z].y < target[z].y)
			u[z].y++;
		else
		if (u[z].x > target[z].x)
			u[z].x--;
	}
}

void Start()
{
	// Invisible awesomeness.
}

void TargetS1 (int x, int y, int z) // 1.0 Series
{
	if (ailvl[z] == -1)
	{
		int t;
		t = rand () % 4;
		u[z].x = u[z].x + dx[t];
		u[z].y = u[z].y + dy[t];
		if (u[z].x < 0)
			u[z].x = 0;
		if (u[z].x >= al[z].x)
			u[z].x = al[z].x - 1;
		if (u[z].y < 0)
			u[z].y = 0;
		if (u[z].y >= al[z].y)
			u[z].y = al[z].y - 1;
		target[z].x = u[z].x;
		target[z].y = u[z].y;
	}
	else
	{
		int st, dr;
		bool stai = true;
		st = dr = 0;
		if ((ailvl[z] > 1 && a[z][x][y].d >= 100) || (ailvl[z] == 3 && !a[z][x][y].d))
		{
			target[z].x = x, target[z].y = y;
			return;
		}
		if (a[z][x][y].v > -1)
			a[z][x][y].v = -2;
		else
			if (a[z][x][y].v == -1)
				a[z][x][y].v = -3;
		array[dr].x = x;
		array[dr++].y = y;
		while (stai && st < dr)
		{
			int i = array[st].x;
			int j = array[st].y;
			for (int k = 0; k < 8; ++k)
			{
				int ii = i + dx[k];
				int jj = j + dy[k];
				if (ii >= 0 && jj >= 0 && ii < al[z].x && jj < al[z].y && a[z][ii][jj].v >= -1)
				{
					if (a[z][ii][jj].v > -1)
						a[z][ii][jj].v = -2;
					else
						if (a[z][ii][jj].v == -1)
							a[z][ii][jj].v = -3;
					if (a[z][ii][jj].d == 1 || a[z][ii][jj].d == 2 || (!a[z][ii][jj].d && !Conditions (ii, jj, z)))
					{
						array[dr].x = ii;
						array[dr++].y = jj;
					}
					else
					if (Conditions (ii, jj, z))
					{
						target[z].x = ii;
						target[z].y = jj;
						return;
					}
				}
			}
			st++;
		}
	}
}

bool TargetS2 (int x, int y, int z) //2.0 Series
{
	int st, dr;
	st = dr = 0;
	bool stai = true;
	if (a[z][x][y].d > 100)
	{
		target[z].x = x;
		target[z].y = y;
		return true;
	}
	for (int k = 0; k < 8; ++k)
	{
		int ii = x + dx[k];
		int jj = y + dy[k];
		if (ii >= 0 && jj >= 0 && ii < al[z].x && jj < al[z].y)
		{
			array[dr].x = ii;
			array[dr++].y = jj;
		if (a[z][ii][jj].v > -1)
			a[z][ii][jj].v = -2;
		else
			if (a[z][ii][jj].v == -1)
				a[z][ii][jj].v = -3;
		}
	}
	while (stai && st < dr)
	{
		int i = array[st].x;
		int j = array[st].y;
		if (a[z][i][j].d == 1 && TFree (i,j,z))
			return true;
		for (int k = 0; k < 8; ++k)
		{
			int ii = i + dx[k];
			int jj = j + dy[k];
			if (ii >= 0 && jj >= 0 && ii < al[z].x && jj < al[z].y && a[z][ii][jj].v >= -1)
			{
				array[dr].x = ii;
				array[dr++].y = jj;
			if (a[z][ii][jj].v > -1)
				a[z][ii][jj].v = -2;
			else
			if (a[z][ii][jj].v == -1)
				a[z][ii][jj].v = -3;
			}
		}
		st++;
	}
	return false;
}

bool TFree (int x, int y, int z)
{
	int lol2 = 0, lol = 0;
	for (int k = 0; k < 8; ++k)
	{
		int ii = x + dx[k];
		int jj = y + dy[k];
		if (ii >= 0 && jj >= 0 && ii < al[z].x && jj < al[z].y)
		{
			if (a[z][ii][jj].d == 2)
				lol2++;
			else
				if (a[z][ii][jj].d % 10 == 0)
					lol++;
		}
	}
	if (a[z][x][y].v - lol2 < 2 && lol)
		for (int k = 0; k < 8; ++k)
		{
			int ii = x + dx[k];
			int jj = y + dy[k];
			if (ii >= 0 && jj >= 0 && ii < al[z].x && jj < al[z].y && a[z][ii][jj].d % 10 == 0)
			{
				target[z].x = ii;
				target[z].y = jj;
				return true;
			}
		}
	return false;
}

bool Conditions (int x, int y, int z)
{
	int lol = 0;
	for (int k = 0; k < 8; ++k)
	{
		int ii = x + dx[k];
		int jj = y + dy[k];
		if (ii >= 0 && jj >= 0 && ii < al[z].x && jj < al[z].y && a[z][ii][jj].d % 10)
			lol++;
	}
	if (lol < 3)
		return false;
	else
		return true;
}

void Tutorial ()
{
	clrscr();
	cout << "Rival Minefields v221 Beta @Cronologium\n\n";
	cout << "Tutorial:\n\n";
	cout << "You will play against an level 0 AI.\n";
	cout << "Bonus rate is increased. There are less mines than in a normal game.\n";
	cout << "Your chance to lose is less than 1%. Warning! Game options will be slightly modified\n";
	cout << "\nPress any key to continue...";
	fin();
	bonuss = 30;
	miness = 30;
	play[0] = true;
	play[1] = false;
	ailvl[1] = -1;
	InGameMenu();
}

// ConsoleApplication1.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include<iostream>
#include<Windows.h> 
#include<string.h>
#include<deque>
#include<vector>
#include<time.h>
#define CONS_WD 60
#define CONS_HT 22
//specify the color
#define red 0x0c
#define green 0x0a
#define blue 0x09
#define syan 0x0b
#define white 0x0f
//0:blue 1:green 2:red 3:black 4:white
const int color[5] = { 0x09, 0x0a, 0x0c ,0x00,0x0f};
using namespace std;
typedef pair<int, int> vec2;
//----------------------------------------------------------
//----------------------------------------------------------
//extern variance
struct character
{
	vec2 position;
	int color;
	int HP;
	char* shape[2];
	vec2 speed;
	vector <vec2> hitpoint;
};
character boss;
//----------------------------------------------------------
//----------------------------------------------------------
//vector operation function
vec2 positionLimit(vec2 pos, int width, int height)
{
	if (pos.first > width)pos.first = width;
	if (pos.first < 0)pos.first = 0;
	if (pos.second > height)pos.second = height;
	if (pos.second < 0)pos.second = 0;
	return pos;
}
vec2 euqal(vec2 v1, vec2 v2)
{
	vec2 v3;
	v3.first = v1.first + v2.first;
	v3.second = v1.second + v2.second;
	return v3;
}
vec2 euqal(vec2 v1)
{
	vec2 v2;
	v2.first = v1.first;
	v2.second = v1.second;
	return v2;
}
bool ifequal(vec2 v1, vec2 v2)
{
	if (v1.first == v2.first&&v1.second == v2.second)return 1;
	else return 0;
}
//----------------------------------------------------------
//-----------------------------------------------------------
//game function
void setCursorpos(vec2 position)
{
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = position.first;
	pos.Y = position.second;
	SetConsoleCursorPosition(hCons, pos);
}

void setColor(int col)
{
	HANDLE hCons=GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = 0;
	switch (col)
	{
	case 0:attr |= FOREGROUND_BLUE; break;
	case 1:attr |= FOREGROUND_GREEN; break;
	case 2:attr |= FOREGROUND_RED; break;
	case 3:attr |= FOREGROUND_BLUE&FOREGROUND_GREEN; break;
	case 4:attr |= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; break;

	default:
		break;
	}
	SetConsoleTextAttribute(hCons, attr);
}

bool isKeyPress(int vkey)
{
	return (GetAsyncKeyState(vkey) & 0x8000 )!= 0;
}

void print_field()
{
	setColor(3);
	setCursorpos(vec2(0, 1));
	for (int i1 = 0; i1 < CONS_HT + 5; i1++)
	{
		for (int i2 = 0; i2 < CONS_WD + 5; i2++)
		{
			cout << ".";
		}
		cout << "\n";
	}
	setColor(4);
	setCursorpos(vec2(0, 0));
	for (int i3 = 0; i3 <= CONS_WD+5; i3++)
	{
		cout << "-";
	}
	

	setCursorpos(vec2(0, CONS_HT+1));
	for (int i4 = 0; i4 <= CONS_WD + 5; i4++)

	{
		cout << "-";
	}
	for (int i5 = 1; i5 <= CONS_HT; i5++)
	{
		setCursorpos(vec2(0, i5));
		cout << "|";
		setCursorpos(vec2(CONS_WD + 5, i5));
		cout << "|";
	}

	
	}     


void print_player(vec2 pos,int col,char* shape)
{
	vec2 p;
	p.first = pos.first - 2;
	p.second = pos.second;
	setCursorpos(p);
	setColor(col);
	cout << shape;
	
}


void print_bul(vec2 pos, int col, char* shape)
{
	vec2 p;
	p.first = pos.first;
	p.second = pos.second;
	setCursorpos(p);
	setColor(col);
	cout << shape;

}

void isCrossboundry(vec2 pos,int* HP)
{
	if (pos.first<0 || pos.first>CONS_WD || pos.second<0 || pos.second>CONS_HT) *HP -= 10;
}

void isBulletHit(vec2 pos_b, vector <vec2>hitpoint, int* HP_b, int* HP_c)
{
	for (int i = 0; i < hitpoint.size();i++)
	if (ifequal(pos_b,hitpoint[i]) == 1)
	{
		*HP_b -= 5;
		*HP_c -= 5;
		
	}
}
void isBulletHit(vec2 pos_b, vec2 pos_c, int* HP_b, int* HP_c)
{
	bool isHit = ifequal(pos_b, pos_c);
	if (isHit == 1)
	{
		*HP_b -= 10;
		*HP_c -= 10;

	}
}

void timer(int count)
{
	vec2 pos = vec2(CONS_WD + 6, 1);
	setColor(1);
	setCursorpos(pos);
	cout << "time:"<<count;
}

vector<character> generate_enemy(int count,vector<character> enemy)
{
	vector<character>newenemy = enemy;
	//define  varieties of enemy
	//enemyA
	character enemyA;
	enemyA.position = vec2(0, CONS_HT / 2);
	enemyA.color = 2;
	enemyA.HP = 3;
	enemyA.shape[0] = "**";
	enemyA.shape[1] = "O";
	enemyA.speed = vec2(1, 0);
	enemyA.hitpoint.push_back(enemyA.position);
	//enemyB
	character enemyB;
	enemyB.position = vec2(CONS_WD-1, CONS_HT / 2);
	enemyB.color = 2;
	enemyB.HP = 3;
	enemyB.shape[0] = "**";
	enemyB.shape[1] = "O";
	enemyB.speed = vec2(-1, 0);
	enemyB.hitpoint.push_back(enemyB.position);
	//enemyC
	character enemyC;
	enemyC.position = vec2(20, 2);
	enemyC.color = 2;
	enemyC.HP = 3;
	enemyC.shape[0] = "**";
	enemyC.shape[1] = "O";
	enemyC.speed = vec2(1, 1);
	enemyC.hitpoint.push_back(enemyC.position);
	//enemyD
	character enemyD;
	enemyD.position = vec2(50, 2);
	enemyD.color = 2;
	enemyD.HP = 3;
	enemyD.shape[0] = "**";
	enemyD.shape[1] = "O";
	enemyD.speed = vec2(-1, 1);
	enemyD.hitpoint.push_back(enemyD.position);
	//boss
	character boss;
	boss.position = vec2(CONS_WD / 2, 2);
	boss.color = 2;
	boss.HP = 15;
	boss.shape[0] = "**";
	boss.shape[1] = "B";
	boss.speed = vec2(0, 0);
	switch (count%150)
	{
	case 10:newenemy.push_back(enemyA); break;
	case 30:newenemy.push_back(enemyB); break;
	case 55:newenemy.push_back(enemyA); break;
	case 60:newenemy.push_back(enemyA); break;
	case 65:newenemy.push_back(enemyA); break;
	case 70:newenemy.push_back(enemyA); break;
	case 100:newenemy.push_back(enemyC); break;
	case 105:newenemy.push_back(enemyD); break;
	case 110:newenemy.push_back(enemyC); break;
	case 112:newenemy.push_back(enemyC); break;
	case 114:newenemy.push_back(enemyC); break;
	case 116:newenemy.push_back(enemyC); break;
	case 130:newenemy.push_back(enemyD); break;
	case 132:newenemy.push_back(enemyD); break;
	case 134:newenemy.push_back(enemyD); break;
	case 136:newenemy.push_back(enemyD); break;
	case 149:newenemy.push_back(boss); break;
	default:
		break;
	}
	return newenemy;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

int main()
{

	//application code

	//create structions to manage the factors
	
	struct bullet
	{
		vec2 pos;
		int HP;
		vec2 speed;
		char* shape[2];
		int col;

	};
	
	

	//display the character
    RETRY:
	for (;;)
	{
		//set up parameters
		int dx = 0, dy = 0, erase = 0, HP_b = 1, count = 0,shotdown=0;
		char *status_p, *status_bp, *status_e, *status_ep;
		vec2 pos_p;
		vector <character> enemy, livingenemy;
		vector <bullet> bulletmanager_p, livingbullets_p, bulletmanager_e, livingbullets_e;

		//to initial the characters and bullets
		//to define different characters and bullets
		character player;
		player.position = vec2(CONS_WD / 2, CONS_HT);
		player.color = 1;
		player.shape[0] = "(X_X)";
		player.shape[1] = "('_')";
		pos_p = player.position;
		player.HP = 1;
		player.hitpoint.push_back(player.position);

		bullet bullet_p;
		bullet_p.shape[0] = "*";
		bullet_p.shape[1] = "A";
		bullet_p.col = 1;
		bullet_p.speed = vec2(0, 1);
		bullet_p.HP = 1;

		bullet bullet_e;
		bullet_e.shape[0] = "*";
		bullet_e.shape[1] = "V";
		bullet_e.col = 2;
		bullet_e.speed = vec2(0, 1);
		bullet_e.HP = 1;

		bullet p;
		p.pos = vec2(CONS_WD / 2, CONS_HT / 2);
		p.shape[0] = "*";
        p.shape[1] = "p";
		p.col = 4;
		p.speed = vec2(0,1);
		p.HP = 1;

		bullet u;
		u.pos = vec2(CONS_WD / 2+1, CONS_HT / 2);
		u.shape[0] = "*";
		u.shape[1] = "u";
		u.col = 4;
		u.speed = vec2(0,1);
		u.HP = 1;

		bullet s;
		s.pos = vec2(CONS_WD / 2+2, CONS_HT / 2);
		s.shape[0] = "*";
		s.shape[1] = "s";
	    s.col = 4;
		s.speed = vec2(0,1);
		s.HP = 1;

		bullet h;
		h.pos = vec2(CONS_WD / 2+3, CONS_HT / 2);
		h.shape[0] = "*";
		h.shape[1] = "h";
		h.col = 4;
		h.speed = vec2(0,1);
		h.HP = 1;

		bullet E;
		E.pos = vec2(CONS_WD / 2 + 5, CONS_HT / 2);
		E.shape[0] = "*";
		E.shape[1] = "E";
		E.col = 4;
		E.speed = vec2(0, 1);
		E.HP = 1;

		bullet n;
		n.pos = vec2(CONS_WD / 2 + 6, CONS_HT / 2);
		n.shape[0] = "*";
		n.shape[1] = "n";
		n.col = 4;
		n.speed = vec2(0, 1);
		n.HP = 1;

		bullet t;
		t.pos = vec2(CONS_WD / 2 + 7, CONS_HT / 2);
		t.shape[0] = "*";
		t.shape[1] = "t";
		t.col = 4;
		t.speed = vec2(0, 1);
		t.HP = 1;

		bullet e;
		e.pos = vec2(CONS_WD / 2 + 8, CONS_HT / 2);
		e.shape[0] = "*";
		e.shape[1] = "e";
		e.col = 4;
		e.speed = vec2(0, 1);
		e.HP = 1;

		bullet r;
		r.pos = vec2(CONS_WD / 2 + 9, CONS_HT / 2);
		r.shape[0] = "*";
		r.shape[1] = "r";
		r.col = 4;
		r.speed = vec2(0, 1);
		r.HP = 1;

		//print the bg at first
		print_field();
		bulletmanager_e.push_back(p);
		bulletmanager_e.push_back(u);
		bulletmanager_e.push_back(s);
		bulletmanager_e.push_back(h);
		bulletmanager_e.push_back(E);
		bulletmanager_e.push_back(n);
		bulletmanager_e.push_back(t);
		bulletmanager_e.push_back(e);
		bulletmanager_e.push_back(r);

		for (int j3 = 0; j3<bulletmanager_e.size(); j3++)
		{
		    print_bul(bulletmanager_e[j3].pos, bulletmanager_e[j3].col, bulletmanager_e[j3].shape[1]);
		}
		getchar();
		//game begin
		//check if the HP of the player 0 or time over
		if ((player.HP > 0)&&shotdown<10)
		{
			for (int cnt = 0; player.HP > 0&&shotdown<20; ++cnt)
			{
				//initialize livingenemy and living bullet
				livingbullets_p.clear();
				livingenemy.clear();
				livingbullets_e.clear();
				//initial dx and dy ,earse every frame
				//keyboard position control
				dx = 0; dy = 0; erase = 0;
				if (isKeyPress(VK_LEFT)){ dx = -1; dy = 0; }
				else if (isKeyPress(VK_DOWN)){ dx = 0; dy = 1; }
				else if (isKeyPress(VK_RIGHT)){ dx = 1; dy = 0; }
				else if (isKeyPress(VK_UP)){ dx = 0; dy = -1; }
				//to decide whether update the background or not 
				//if (dx != 0 || dy != 0)erase = 1;
				//if (erase != 0)print_field();
				print_field();
				pos_p.first += dx;
				pos_p.second += dy;
				pos_p = positionLimit(pos_p, CONS_WD, CONS_HT);
				player.hitpoint[0] = euqal(player.position);
				//print the player
				status_p = player.shape[player.HP > 0 ? 1 : 0]  ;
				print_player(pos_p, player.color, status_p);
				//generate a enemy
				enemy=generate_enemy(count, enemy);
				//enemy shot
				if (count % 4 == 0)
				{
					for (int i8 = 0; i8 < enemy.size(); i8+=2)
					{
						bullet_e.pos = euqal(enemy[i8].position);
						bulletmanager_e.push_back(bullet_e);
					}
				}
				else if (count%4==2)
				{

					for (int i8 = 1; i8 < enemy.size(); i8 += 2)
					{
						bullet_e.pos = euqal(enemy[i8].position);
						bulletmanager_e.push_back(bullet_e);
					}
				}

				//check if the enemy bullet hit player or cross the boundry
				for (int i6 = 0; i6 < bulletmanager_e.size(); i6++)
				{
					isBulletHit(bulletmanager_e[i6].pos, pos_p, &bulletmanager_e[i6].HP, &player.HP);
					isCrossboundry(bulletmanager_e[i6].pos, &bulletmanager_e[i6].HP);

					if (bulletmanager_e[i6].HP>0)livingbullets_e.push_back(bulletmanager_e[i6]);
				}
				bulletmanager_e = livingbullets_e;

				//print enemy bullets
				for (int i7 = 0; i7 < bulletmanager_e.size(); i7++)
				{
					bulletmanager_e[i7].pos = euqal(bulletmanager_e[i7].pos, bulletmanager_e[i7].speed);
					print_bul(bulletmanager_e[i7].pos, bulletmanager_e[i7].col, bulletmanager_e[i7].shape[1]);
				}


				//player shots
				bullet_p.pos.first = pos_p.first;
				bullet_p.pos.second = pos_p.second;
				bulletmanager_p.push_back(bullet_p);

				//clean the dead player bullet
				for (int i1 = 0; i1 < bulletmanager_p.size(); i1++)
				{
					//check if the bullet cross the boundry or hit something,if not ,put it into the vector livingbullet
					isCrossboundry(bulletmanager_p[i1].pos, &bulletmanager_p[i1].HP);
					for (int i3 = 0; i3 < enemy.size(); i3++)
					{
						isBulletHit(bulletmanager_p[i1].pos, enemy[i3].position, &bulletmanager_p[i1].HP, &enemy[i3].HP);
					}

					if (bulletmanager_p[i1].HP > 0) livingbullets_p.push_back(bulletmanager_p[i1]);
				}
				bulletmanager_p = livingbullets_p;
				//print the player bullets
				for (int i4 = 0; i4<bulletmanager_p.size(); i4++)
				{
					//bullet flies forwards
					bulletmanager_p[i4].pos.first -= bulletmanager_p[i4].speed.first;
					bulletmanager_p[i4].pos.second -= bulletmanager_p[i4].speed.second;
					//print the bullet
					status_bp = bulletmanager_p[i4].shape[(bulletmanager_p[i4].HP > 0 ? 1 : 0)];
					print_bul(bulletmanager_p[i4].pos, bulletmanager_p[i4].col, status_bp);


				}

				//check all the enemies and put the living enemies into vector liveingenemy
				for (int i2 = 0; i2 < enemy.size(); i2++)
				{
					if (enemy[i2].HP <= 0){ print_player(enemy[i2].position, enemy[i2].color, enemy[i2].shape[0]); shotdown++; }
					isCrossboundry(enemy[i2].position, &enemy[i2].HP);
					if (enemy[i2].HP > 0)livingenemy.push_back(enemy[i2]);

				}
				enemy = livingenemy;
				//print the enemy
				for (int i5 = 0; i5 < enemy.size(); i5++)
				{
					//enemy move
					enemy[i5].position = euqal(enemy[i5].position, enemy[i5].speed);
					print_player(enemy[i5].position, enemy[i5].color, enemy[i5].shape[1]);
					//cout << enemy[i5].HP;

				}
				timer(++count);
				setCursorpos(vec2(CONS_WD + 6, 2));
				cout << "shotdown:"<<shotdown;
				Sleep(20);
			}
		}
			//when the HP of player < 0 
		if (player.HP < 0)
		{
			int retry=-1;
			print_field();
			print_player(player.position, player.color, player.shape[0]);
			setColor(2);
			setCursorpos(vec2(CONS_WD / 2 - 4, CONS_HT / 2));
			cout << "GAME OVER"<<"  retry?(Yes:1/No:other)\n";
			setCursorpos(vec2(0, 0));
			 cin >> retry; 
			if (retry==1) goto RETRY;
			else goto END;
		}
		else
		{
			int retry =-1;
			print_field();
			setColor(1);
			setCursorpos(vec2(CONS_WD / 2 - 4, CONS_HT / 2));
			cout << "Congratulations ! " << "  retry?(Yes:1/No:other)\n";
			while (retry == -1){ cin >> retry; }
			if (retry==1) goto RETRY;
			else goto END;


		}
		}
		
		END:
		getchar();
		return 0;
	}

#ifndef _MYSUPPORT
#define _MYSUPPORT

#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<algorithm>
#include<cstdio>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<cstring>
#include<random>
#include<time.h> 
#include<assert.h>
#include<fstream>

#define BOARDSIZE 13
#define INF 100000000
using namespace std;

// 位置
struct Pos {
public:
	int r;
	int c;
	Pos() {}
	Pos(int _r, int _c) { r = _r; c = _c; }
	Pos operator+(const Pos &p)const { return Pos(r + p.r, c + p.c); }
};


// 单步记录
struct Step {
	int player;	// 玩家
	int hand;	// 手数
	Pos p;		// 落子位置
	Step() {}
	Step(int _player, int _hand, Pos _p) {
		player = _player;
		hand = _hand;
		p = _p;
	}
};


class Situation {
public:
	int win6 = 0;		// 长连
	int win5 = 0;	// 5连珠
	int live4 = 0;	// 活4
	int dead4 = 0;		//死4
	int jump4 = 0;
	int live3 = 0;		//活3
	int jump3 = 0;		//跳3
	int dead3 = 0;		// 死3
	int live2 = 0;		// 活2	
	int dead2 = 0;			// 死2
	int jump2 = 0;
	Situation operator + (const Situation s1) const {
		Situation s2;
		s2.win6 = win6 + s1.win6;
		s2.win5 = win5 + s1.win5;
		s2.live4 = live4 + s1.live4;
		s2.dead4 = dead4 + s1.dead4;
		s2.live3 = live3 + s1.live3;
		s2.jump3 = jump3 + s1.jump3;
		s2.dead3 = dead3 + s1.dead3;
		s2.live2 = live2 + s1.live2;
		s2.dead2 = dead2 + s1.dead2;
		s2.jump2 = jump2 + s1.jump2;
		return s2;
	}
};

class Score {
public:
	const static int win6 = 100000;		// 长连
	const static int win5 = 1000000;	// 5连珠
	const static int live4 = 100000;	// 活4
	const static int dead4 = 10000;		//死4
	const static int jump4 = 5000;
	const static int live3 = 1000;		//活3
	const static int jump3 = 500;		//跳3
	const static int dead3 = 100;		// 死3
	const static int live2 = 30;		// 活2	
	const static int dead2 = 10;	    // 死2
	const static int jump2 = 5;			// 跳2
	const static int nothreat = 0;		// 没有威胁
};

string pos_transform(Pos p);

bool is_in(Pos p);

int reverse_role(int role);

void print_board(int board[][BOARDSIZE]);

#endif // _SUPPORT



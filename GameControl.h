#ifndef _GAMECONTROL
#define _GAMECONTROL


#include"Support.h"
#include"Player.h"

class GameControl {
public:
	int board[BOARDSIZE][BOARDSIZE];
	static const int black_chess = 1;
	static const int white_chess = 2;
	static const int nothing = 0;
	GameControl() {
		memset(this->board, 0, sizeof(this->board));
	}
	vector<Step>steps;				// 棋谱记录

	Pos last_pos;
	void mainloop();				// 游戏主循环
	void board_display();
	bool five_con(Pos p0, Pos dir);
	bool six_con(Pos p0, Pos dir);
	Pos find_edge(Pos p0, Pos dir);
	// 打印棋盘
	int win_detect(Pos p);			// 从(r,c)处检测是否形成5连、6连，若有人胜利，返回玩家编号，否则返回0

	// 辅助功能
	void backward();				// 悔棋
	void forward();					// 前进
	string save_steps();

};

#endif // !_GAMECONTROL
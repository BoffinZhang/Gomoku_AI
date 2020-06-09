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
	vector<Step>steps;				// ���׼�¼

	Pos last_pos;
	void mainloop();				// ��Ϸ��ѭ��
	void board_display();
	bool five_con(Pos p0, Pos dir);
	bool six_con(Pos p0, Pos dir);
	Pos find_edge(Pos p0, Pos dir);
	// ��ӡ����
	int win_detect(Pos p);			// ��(r,c)������Ƿ��γ�5����6����������ʤ����������ұ�ţ����򷵻�0

	// ��������
	void backward();				// ����
	void forward();					// ǰ��
	string save_steps();

};

#endif // !_GAMECONTROL
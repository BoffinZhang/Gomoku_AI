#ifndef _PLAYER
#define _PLAYER


#include"Support.h"

class Player {
protected:
	int chess;
	int(*board)[BOARDSIZE];
public:
	virtual Pos get_position() = 0;
	void change_chess(int _chess);
};


class HumanPlayer :public Player {
public:
	HumanPlayer() {}
	HumanPlayer(int c, int _board[BOARDSIZE][BOARDSIZE]) { chess = c; board = _board; }

	virtual Pos get_position();
};

class AIPlayer :public Player {

private:

	int chess_score[BOARDSIZE][BOARDSIZE] = { 0 };	// ��������

	int search_depth = 2;

	int board_evaluate(int chess);	// ���Ƕ��������̾��Ƶ����������յõ�����

	// �µķ���
	Situation line_situation(const vector<int>& line, int chess);
	Situation single_situation(Pos p, int chess);
	int board_situation_evaluate(int chess);
	void single_score_update(Pos p);
	void put_chess(Pos p, int chess);				// ���壬ͬʱ������������
	void remove_chess(Pos p, int chess);			// �Ƴ����ӣ�ͬʱ��������
	int get_board_score();				    // ��ȡ�������������ӵ��ܷ�

	int line_evaluate(const vector<int>& line, int chess);
	// int line_evaluate(const vector<int>& line, int chess, bool one_color);
	int single_evaluate(Pos p, int chess);	// ��ĳһ��λ�õ�����
	void get_line(vector<int> &line,Pos start,int len,Pos dir);
	void effect_pos_gen(vector<Pos> &ps);	// ������Ч�㼯
	
public:
	AIPlayer() {}
	AIPlayer(int c, int _board[BOARDSIZE][BOARDSIZE]) { chess = c; board = _board; }
	
	int min_max_search(int chess, Pos step, int beta, int alpha, int deep, bool maxl);

	virtual Pos get_position();

	void TEST();

};

#endif
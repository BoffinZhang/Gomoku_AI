#include"Support.h"

// λ��ת�ַ������
string pos_transform(Pos p) {
	string s = "";
	s = "(" + to_string(13 - p.r) + ", " + char(p.c + 'A') + ")";
	return s;
}

// �߽��ж�
bool is_in(Pos p) {
	if (p.r >= 0 && p.r < BOARDSIZE && p.c >= 0 && p.c < BOARDSIZE)
		return true;
	else return false;
}

// ������ɫ���������ֽ���
int reverse_role(int role) {
	if (role == 1) return 2;
	return 1;
}

void print_board(int board[][BOARDSIZE])
{
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

#include"Support.h"

// 位置转字符串输出
string pos_transform(Pos p) {
	string s = "";
	s = "(" + to_string(13 - p.r) + ", " + char(p.c + 'A') + ")";
	return s;
}

// 边界判断
bool is_in(Pos p) {
	if (p.r >= 0 && p.r < BOARDSIZE && p.c >= 0 && p.c < BOARDSIZE)
		return true;
	else return false;
}

// 交换角色，用于三手交换
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

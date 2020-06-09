#include "GameControl.h"

// 游戏主循环
void GameControl::mainloop()
{
	// 对局设定：
	// 1. 人机模式or人人模式or机机对战模式
	// 2. 玩家先手or玩家后手
	int game_mode;
	int is_human_first;
	Player *player1, *player2;

	cout << "选择游戏模式：人机对弈-0，人人对弈-1，机机对弈" << endl;
	cin >> game_mode;

	if (game_mode == 3) {
		player1 = new AIPlayer(black_chess, board);
		player2 = new AIPlayer(white_chess, board);
	}
	else {
		cout << "选择先手：人类先手-0，AI先手-1" << endl;
		cin >> is_human_first;
		is_human_first = 1 - is_human_first;

		if (game_mode == 1) {
			cout << "人人模式尚在开发中" << endl;
			return;
		}
		// 人机模式初始化
		if (is_human_first) {
			player1 = new HumanPlayer(black_chess, board);
			player2 = new AIPlayer(white_chess, board);
		}
		else {
			player1 = new AIPlayer(black_chess, board);
			player2 = new HumanPlayer(white_chess, board);
		}
	}
	
	// 游戏状态
	// 1为玩家1回合，2为玩家2回合
	int game_state = 1;

	board_display();

	while (game_state >= 0) {
		// 玩家1回合
		if (game_state == 1) {
			Pos p = player1->get_position();
			last_pos = p;
			if (steps.empty()) {
				steps.push_back(Step(1, 1, p));
			}
			else {
				steps.push_back(Step(1, steps.back().hand + 1, p));
			}
			if (p.r < 0 && p.c < 0) {
				// 悔棋
				backward();
				continue;
			}
			board_display();
			int result = win_detect(p);
			if (result == 1) {
				cout << "玩家1胜利！！" << endl << "Game Over" << endl;
				game_state = -1;
			}
			else if (result == 2) {
				cout << "玩家2胜利！！" << endl << "Game Over" << endl;
				game_state = -2;
			}
			else {
				// 切换回玩家2
				game_state = 2;
				continue;
			}
		}

		// 玩家2回合
		if (game_state == 2) {
			Pos p = player2->get_position();
			last_pos = p;
			if (p.r < 0 && p.c < 0) {
				// 悔棋
				backward();
				continue;
			}
			if (steps.empty()) {
				steps.push_back(Step(1, 1, p));
			}
			else {
				steps.push_back(Step(1, steps.back().hand + 1, p));
			}

			board_display();

			int result = win_detect(p);
			if (result == 1) {
				cout << "玩家1胜利" << endl;
				game_state = -1;
			}
			else if (result == 2) {
				cout << "玩家2胜利" << endl;
				game_state = -2;
			}
			// 切换回玩家1
			else game_state = 1;
			continue;
		}
	}
	save_steps();
	system("pause");
}

// 棋盘显示
void GameControl::board_display()
{
	/**********特殊符号**********
			 ┏┳┓
			 ┣╋┫
		     ┗┻┛
			 ○●☆★
	****************************/

	system("cls");

	// 打印第一行
	cout << "   ";
	for (int i = 0; i < BOARDSIZE; i++) {
		cout << char('A' + i) << " ";
	}
	cout << endl;
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			if (j == 0) {
				printf("%*d", 3, 13 - i);
				// printf("%*d", 3, i+1);
			}
			if (board[i][j]>0) {
				if (i == last_pos.r && j == last_pos.c && board[i][j] == black_chess) cout << "☆";
				else if (i == last_pos.r && j == last_pos.c && board[i][j] == white_chess) cout << "★";
				else if (board[i][j] == black_chess) cout << "○";
				else if (board[i][j] == white_chess) cout << "●";
				if (j == BOARDSIZE - 1)cout << endl;
			}
			else {
				if (i == 0 && j == 0)                   cout << "┏-";
				else if (i == 0 && j == BOARDSIZE - 1)  cout << "┓" << endl;
				else if (i == 0)						cout << "┳-";
				else if (i == BOARDSIZE - 1 && j == 0)			cout << "┗-";
				else if (i == BOARDSIZE - 1 && j == BOARDSIZE - 1)  cout << "┛" << endl;
				else if (j == 0) cout << "┣-";
				else if (i == BOARDSIZE - 1) cout << "┻-";
				else if (j == BOARDSIZE - 1) cout << "┫" << endl;
				else cout << "╋-";
			}

		}
	}
	// 为了方便，打印AI的最后一步
	if (!steps.empty()) {
		Pos p = steps.back().p;
		cout << "最后一步" << pos_transform(p) << endl;
	}
}



// 五连检测
bool GameControl::five_con(Pos p0, Pos dir) {
	Pos p1 = p0 + dir;
	Pos p2 = p1 + dir;
	Pos p3 = p2 + dir;
	Pos p4 = p3 + dir;
	if (is_in(p0) && is_in(p1) && is_in(p2) && is_in(p3) && is_in(p4) &&
		board[p1.r][p1.c] == board[p0.r][p0.c] && board[p2.r][p2.c] == board[p0.r][p0.c] &&
		board[p3.r][p3.c] == board[p0.r][p0.c] && board[p4.r][p4.c] == board[p0.r][p0.c]) {
		return true;
	}
	return false;
}

// 六连检测
bool GameControl::six_con(Pos p0, Pos dir) {
	Pos p1 = p0 + dir;
	Pos p2 = p1 + dir;
	Pos p3 = p2 + dir;
	Pos p4 = p3 + dir;
	Pos p5 = p4 + dir;
	if (is_in(p0) && is_in(p1) && is_in(p2) && is_in(p3) && is_in(p4) && is_in(p5) &&
		board[p1.r][p1.c] == board[p0.r][p0.c] && board[p2.r][p2.c] == board[p0.r][p0.c] &&
		board[p3.r][p3.c] == board[p0.r][p0.c] && board[p4.r][p4.c] == board[p0.r][p0.c] &&
		board[p5.r][p5.c] == board[p0.r][p0.c])
	{
		return true;
	}

	return false;
}

// 返回与棋子相连的、某个方向上的边缘棋子
Pos GameControl::find_edge(Pos p0, Pos dir) {
	Pos p1 = p0 + dir;
	while (is_in(p1) && board[p1.r][p1.c] == board[p0.r][p0.c]) {
		p0 = p1;
		p1 = p1 + dir;
	}
	return p0;
}

// 胜利检测
int GameControl::win_detect(Pos p)
{
	// 从p开始的检测五连
	char chess = board[p.r][p.c];

	// 找到最左、上、左上、左下端
	Pos p_left = find_edge(p, Pos(0, -1));
	Pos p_up = find_edge(p, Pos(-1, 0));
	Pos p_upleft = find_edge(p, Pos(-1, -1));
	Pos p_downleft = find_edge(p, Pos(1, -1));

	// p位置的棋子胜利标志	
	bool win_flag = five_con(p_left, Pos(0, 1)) || five_con(p_up, Pos(1, 0)) ||
		five_con(p_upleft, Pos(1, 1)) || five_con(p_downleft, Pos(-1, 1));
	if (chess == this->black_chess) {
		// 如果玩家1胜利，返回1
		if (win_flag) return 1;
	}
	else {
		// 如果玩家2胜利，返回2
		if (win_flag) return 2;
	}
	return 0;
}

// 悔棋
// 因为只有在玩家回合时才能悔棋，所以要撤销两步操作
void GameControl::backward()
{
	Pos pos1 = steps.back().p;
	steps.pop_back();
	Pos pos2 = steps.back().p;
	steps.pop_back();
	board[pos1.r][pos1.c] = 0;
	board[pos2.r][pos2.c] = 0;
}

// 前进
// 撤销悔棋
void GameControl::forward() {

}

string GameControl::save_steps()
{
	time_t t = time(0);
	char ch[64];
	strftime(ch, sizeof(ch), "%Y-%m-%d %H-%M-%S", localtime(&t)); //年-月-日 时-分-秒
	cout << ch << endl;
	ofstream location_out;
	string ss(ch);
	ss = "record/" + ss;
	ss += ".txt";
	location_out.open(ss, std::ios::out | std::ios::app);  //以写入和在文件末尾添加的方式打开.txt文件，没有的话就创建该文件。
	if (!location_out.is_open())
		return 0;
	for (int i = 0; i < steps.size(); i++) {
		location_out << pos_transform(steps[i].p);
	}
	cout << "棋谱保存为" << ss << endl;
	return string();
}


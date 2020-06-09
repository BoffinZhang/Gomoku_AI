#include "GameControl.h"

// ��Ϸ��ѭ��
void GameControl::mainloop()
{
	// �Ծ��趨��
	// 1. �˻�ģʽor����ģʽor������սģʽ
	// 2. �������or��Һ���
	int game_mode;
	int is_human_first;
	Player *player1, *player2;

	cout << "ѡ����Ϸģʽ���˻�����-0�����˶���-1����������" << endl;
	cin >> game_mode;

	if (game_mode == 3) {
		player1 = new AIPlayer(black_chess, board);
		player2 = new AIPlayer(white_chess, board);
	}
	else {
		cout << "ѡ�����֣���������-0��AI����-1" << endl;
		cin >> is_human_first;
		is_human_first = 1 - is_human_first;

		if (game_mode == 1) {
			cout << "����ģʽ���ڿ�����" << endl;
			return;
		}
		// �˻�ģʽ��ʼ��
		if (is_human_first) {
			player1 = new HumanPlayer(black_chess, board);
			player2 = new AIPlayer(white_chess, board);
		}
		else {
			player1 = new AIPlayer(black_chess, board);
			player2 = new HumanPlayer(white_chess, board);
		}
	}
	
	// ��Ϸ״̬
	// 1Ϊ���1�غϣ�2Ϊ���2�غ�
	int game_state = 1;

	board_display();

	while (game_state >= 0) {
		// ���1�غ�
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
				// ����
				backward();
				continue;
			}
			board_display();
			int result = win_detect(p);
			if (result == 1) {
				cout << "���1ʤ������" << endl << "Game Over" << endl;
				game_state = -1;
			}
			else if (result == 2) {
				cout << "���2ʤ������" << endl << "Game Over" << endl;
				game_state = -2;
			}
			else {
				// �л������2
				game_state = 2;
				continue;
			}
		}

		// ���2�غ�
		if (game_state == 2) {
			Pos p = player2->get_position();
			last_pos = p;
			if (p.r < 0 && p.c < 0) {
				// ����
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
				cout << "���1ʤ��" << endl;
				game_state = -1;
			}
			else if (result == 2) {
				cout << "���2ʤ��" << endl;
				game_state = -2;
			}
			// �л������1
			else game_state = 1;
			continue;
		}
	}
	save_steps();
	system("pause");
}

// ������ʾ
void GameControl::board_display()
{
	/**********�������**********
			 ���ש�
			 �ǩ��
		     ���ߩ�
			 �����
	****************************/

	system("cls");

	// ��ӡ��һ��
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
				if (i == last_pos.r && j == last_pos.c && board[i][j] == black_chess) cout << "��";
				else if (i == last_pos.r && j == last_pos.c && board[i][j] == white_chess) cout << "��";
				else if (board[i][j] == black_chess) cout << "��";
				else if (board[i][j] == white_chess) cout << "��";
				if (j == BOARDSIZE - 1)cout << endl;
			}
			else {
				if (i == 0 && j == 0)                   cout << "��-";
				else if (i == 0 && j == BOARDSIZE - 1)  cout << "��" << endl;
				else if (i == 0)						cout << "��-";
				else if (i == BOARDSIZE - 1 && j == 0)			cout << "��-";
				else if (i == BOARDSIZE - 1 && j == BOARDSIZE - 1)  cout << "��" << endl;
				else if (j == 0) cout << "��-";
				else if (i == BOARDSIZE - 1) cout << "��-";
				else if (j == BOARDSIZE - 1) cout << "��" << endl;
				else cout << "��-";
			}

		}
	}
	// Ϊ�˷��㣬��ӡAI�����һ��
	if (!steps.empty()) {
		Pos p = steps.back().p;
		cout << "���һ��" << pos_transform(p) << endl;
	}
}



// �������
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

// �������
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

// ���������������ġ�ĳ�������ϵı�Ե����
Pos GameControl::find_edge(Pos p0, Pos dir) {
	Pos p1 = p0 + dir;
	while (is_in(p1) && board[p1.r][p1.c] == board[p0.r][p0.c]) {
		p0 = p1;
		p1 = p1 + dir;
	}
	return p0;
}

// ʤ�����
int GameControl::win_detect(Pos p)
{
	// ��p��ʼ�ļ������
	char chess = board[p.r][p.c];

	// �ҵ������ϡ����ϡ����¶�
	Pos p_left = find_edge(p, Pos(0, -1));
	Pos p_up = find_edge(p, Pos(-1, 0));
	Pos p_upleft = find_edge(p, Pos(-1, -1));
	Pos p_downleft = find_edge(p, Pos(1, -1));

	// pλ�õ�����ʤ����־	
	bool win_flag = five_con(p_left, Pos(0, 1)) || five_con(p_up, Pos(1, 0)) ||
		five_con(p_upleft, Pos(1, 1)) || five_con(p_downleft, Pos(-1, 1));
	if (chess == this->black_chess) {
		// ������1ʤ��������1
		if (win_flag) return 1;
	}
	else {
		// ������2ʤ��������2
		if (win_flag) return 2;
	}
	return 0;
}

// ����
// ��Ϊֻ������һغ�ʱ���ܻ��壬����Ҫ������������
void GameControl::backward()
{
	Pos pos1 = steps.back().p;
	steps.pop_back();
	Pos pos2 = steps.back().p;
	steps.pop_back();
	board[pos1.r][pos1.c] = 0;
	board[pos2.r][pos2.c] = 0;
}

// ǰ��
// ��������
void GameControl::forward() {

}

string GameControl::save_steps()
{
	time_t t = time(0);
	char ch[64];
	strftime(ch, sizeof(ch), "%Y-%m-%d %H-%M-%S", localtime(&t)); //��-��-�� ʱ-��-��
	cout << ch << endl;
	ofstream location_out;
	string ss(ch);
	ss = "record/" + ss;
	ss += ".txt";
	location_out.open(ss, std::ios::out | std::ios::app);  //��д������ļ�ĩβ��ӵķ�ʽ��.txt�ļ���û�еĻ��ʹ������ļ���
	if (!location_out.is_open())
		return 0;
	for (int i = 0; i < steps.size(); i++) {
		location_out << pos_transform(steps[i].p);
	}
	cout << "���ױ���Ϊ" << ss << endl;
	return string();
}


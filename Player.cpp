#include "Player.h"

void Player::change_chess(int _chess)
{
	this->chess = chess;
}


Pos HumanPlayer::get_position()
{
	int r, c;	// 行列编号
	char temp;  // 读入的列字符
	cout << "人类回合" << endl;
	cout << "请输入行列坐标：（-1悔棋）" << endl;
	while (true) {
		cin >> r;
		if (r == -1) {
			// 悔棋
			return Pos(-1, -1);
		}
		cin >> temp;
		r = 13 - r; c = temp - 'A';
		if (!is_in(Pos(r, c))) {
			cout << "位置无效" << endl;
			continue;
		}
		if (board[r][c]) {
			cout << "格子被占用，请重新输入" << endl;
			continue;
		}
		break;
	}
	system("pause");
	board[r][c] = chess;
	return Pos(r, c);
}


int AIPlayer::board_evaluate(int chess)
{
	// 获取棋盘上每条线上的棋子排布，对于N*N的棋盘来说，有4N-1条线需要检测
	vector<int> line;
	int total_score = 0;
	//cout << endl;
	//print_board(board);
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			if (board[i][j]==chess) {
				total_score += single_evaluate(Pos(i, j), chess);
			}
			else if (board[i][j] == reverse_role(chess)) {
				total_score -= single_evaluate(Pos(i, j), reverse_role(chess));
			}
		}
	}
	return total_score;
}

const vector<int> win5 = { 1,1,1,1,1 };
const vector<int> live4 = { 0,1,1,1,1,0 };
const vector<int> jump4 = { 1,1,1,0,1 };
const vector<int> dead4 = { 1,1,1,1,0 };
const vector<int> live3 = { 0,1,1,1,0 };
const vector<int> jump3 = { 0,1,1,0,1,0 };
const vector<int> dead3 = { 1,1,1,0 };
const vector<int> live2 = { 0,1,1,0 };
const vector<int> jump2 = {0,1,0,1,0};
// 检测模式之间是否匹配
bool inline match(const vector<int> &shape, const vector<int> &line, int start, int chess) {
	if (line.size() - start < shape.size()) return false;
	vector<int> shape_rev(shape.size());
	reverse_copy(shape.begin(), shape.end(), shape_rev.begin());
	bool flag1 = true;
	bool flag2 = true;
	for (int i = 0; i < int(shape.size()); i++) {
		if (line[start + i] != chess && shape[i] == 1 ||
			line[start + i] != 0 && shape[i] == 0) {
			flag1 = false;
			break;
		}
	}
	for (int i = 0; i < int(shape.size()); i++) {
		if (line[start + i] != chess && shape_rev[i] == 1 ||
			line[start + i] != 0 && shape_rev[i] == 0) {
			flag2 = false;
			break;
		}
	}
	return flag1 || flag2;
}

Situation AIPlayer::line_situation(const vector<int>& line, int chess)
{
	Situation situation;

	for (int i = 0; i < int(line.size()); i++) {
		if (match(win5,  line, i, chess)) situation.win5++;
		if (match(live4, line, i, chess)) situation.live4++;
		if (match(jump4, line, i, chess)) situation.jump4++;
		if (match(dead4, line, i, chess)) situation.dead4++;
		if (match(live3, line, i, chess)) situation.live3++;
		if (match(jump3, line, i, chess)) situation.jump3++;
		if (match(dead3, line, i, chess)) situation.dead3++;
		if (match(live2, line, i, chess)) situation.live2++;
		if (match(jump2, line, i, chess)) situation.jump2++;
	}

	return situation;
}

Situation AIPlayer::single_situation(Pos p, int chess)
{
	int score = 0;
	// 找到以p为中心9个方格内所有点
	int b_left = max(0, p.c - 4);
	int b_right = min(BOARDSIZE, p.c + 4);
	int b_up = max(0, p.r - 4);
	int b_down = min(BOARDSIZE, p.r + 4);
	vector<int>line;
	Situation situation;
	// 取行
	get_line(line, Pos(p.r, b_left), b_right - b_left + 1, Pos(0, 1));
	situation = situation + line_situation(line, chess);
	// 取列
	get_line(line, Pos(b_up, p.c), b_down - b_up + 1, Pos(1, 0));
	situation = situation + line_situation(line, chess);

	int a = min(p.c - b_left, p.r - b_up);
	int b = min(b_right - p.c, p.r - b_up);
	int c = min(b_down - b_up, b_right - b_left);
	// 取主对角线
	get_line(line, Pos(p.r - a, p.c - a), c, Pos(1, 1));
	situation = situation + line_situation(line, chess);
	// 取次对角线
	get_line(line, Pos(p.r - a, p.c + a), c, Pos(1, -1));
	situation = situation + line_situation(line, chess);

	return situation;
}

void AIPlayer::single_score_update(Pos p)
{
	if (board[p.r][p.c] == 0) {
		chess_score[p.r][p.c] = 0;
		return;
	}

	int score = 0;
	// 找到以p为中心9个方格内所有点
	int b_left = max(0, p.c - 4);
	int b_right = min(BOARDSIZE, p.c + 4);
	int b_up = max(0, p.r - 4);
	int b_down = min(BOARDSIZE, p.r + 4);
	vector<int>line;
	// 取行
	get_line(line, Pos(p.r, b_left), b_right - b_left + 1, Pos(0, 1));
	score += line_evaluate(line, chess);
	// 取列
	get_line(line, Pos(b_up, p.c), b_down - b_up + 1, Pos(1, 0));
	score += line_evaluate(line, chess);

	int a = min(p.c - b_left, p.r - b_up);
	int b = min(b_right - p.c, p.r - b_up);
	int c = min(b_down - b_up, b_right - b_left);
	// 取主对角线
	get_line(line, Pos(p.r - a, p.c - a), c, Pos(1, 1));
	score += line_evaluate(line, chess);
	// 取次对角线
	get_line(line, Pos(p.r - a, p.c + a), c, Pos(1, -1));
	score += line_evaluate(line, chess);

	if (board[p.r][p.c] == chess) {
		chess_score[p.r][p.c] = score;
	}
	else if(board[p.r][p.c] == reverse_role(chess)){
		chess_score[p.r][p.c] = -score;
	}
	else {
		chess_score[p.r][p.c] = 0;
	}
	
}

int AIPlayer::get_board_score()
{
	int total_score = 0;
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			total_score += board[i][j] == chess ? chess_score[i][j] : -chess_score[i][j];
		}
	}
	return total_score;
}

int AIPlayer::line_evaluate(const vector<int> &line, int chess)
{
	int score = 0;
	int len = line.size();
	// 五连：连续5个相同颜色棋子
	// 活四：连续4个相同颜色棋子，且两边没有阻挡
	// 跳四：3+1
	// 死四：连续4个相同颜色棋子，一边被阻挡
	// 活三：连续3个相同颜色棋子
	// 跳三：2+1，且周围没有阻挡
	// 死三：连续3个，一边被阻挡
	// 活2：连续2个，无阻挡

	Situation my_situation;
	Situation enemy_situation;
	
	my_situation = line_situation(line, chess);
	enemy_situation = line_situation(line, reverse_role(chess));
	score += Score::win5*(my_situation.win5);
	score += -Score::win5 / 2 * (enemy_situation.win5);
	score += Score::live4*(my_situation.live4 - enemy_situation.live4);
	score += Score::jump4*(my_situation.jump4 - enemy_situation.jump4);
	score += Score::dead4*(my_situation.dead4 - enemy_situation.dead4);
	score += Score::live3*(my_situation.live3 - enemy_situation.live3);
	score += Score::jump3*(my_situation.jump3 - enemy_situation.jump3);
	score += Score::dead3*(my_situation.dead3 - enemy_situation.dead3);
	score += Score::live2*(my_situation.live2 - enemy_situation.live2);
	return score;
}

int AIPlayer::single_evaluate(Pos p, int chess)
{
	int score = 0;
	// 找到以p为中心9个方格内所有点
	int b_left = max(0, p.c - 4);
	int b_right = min(BOARDSIZE, p.c + 4);
	int b_up = max(0, p.r - 4);
	int b_down = min(BOARDSIZE, p.r + 4);
	vector<int>line;
	// 取行
	get_line(line, Pos(p.r, b_left), b_right - b_left+1, Pos(0, 1));
	score += line_evaluate(line, chess);
	// 取列
	get_line(line, Pos(b_up, p.c), b_down - b_up+1, Pos(1, 0));
	score += line_evaluate(line, chess);

	int a = min(p.c - b_left, p.r - b_up);
	int b = min(b_right - p.c, p.r - b_up);
	int c = min(b_down - b_up, b_right - b_left);
	// 取主对角线
	get_line(line, Pos(p.r-a, p.c-a), c, Pos(1, 1));
	score += line_evaluate(line, chess);
	// 取次对角线
	get_line(line, Pos(p.r-a, p.c+a), c, Pos(1, -1));
	score += line_evaluate(line, chess);

	return score;
}

// 剪枝次数统计
int ab_cut = 0;

// 极大极小搜索
// alpha剪枝：若在MAX层的前若干节点中，计算出的最小值为alpha；若MAX层后面节点的某个子节点的返回值大于alpha，则剪掉MAX层节点其余子节点
// beta剪枝：若在MIN层的前若干节点中，计算出的最大值为beta；若MIN层后面节点的某个子节点的返回值小于beta，则剪掉MIN层节点其余子节点
// 显然，alpha的初始值为INF，beta的初始值为-INF
int AIPlayer::min_max_search(int chess, Pos step, int alpha, int beta, int deep, bool maxl)
{
	if (deep <= 1) {
		board[step.r][step.c] = chess;
		int score = board_evaluate(reverse_role(chess));
		board[step.r][step.c] = 0;
		return score;
	}

	board[step.r][step.c] = chess;

	vector<Pos>effect_pos;
	effect_pos_gen(effect_pos);

	int best_score;

	if (maxl) best_score = -INF;
	else	  best_score = INF;

	for (auto iter = effect_pos.begin(); iter != effect_pos.end(); iter++) {
		Pos p = *iter;
		int score = min_max_search(reverse_role(chess), p , alpha, beta, deep - 1, !maxl);
		if (maxl) {	// 当前是MAX层
			if (score >= alpha) { // alpha剪枝
				// cout << "alpha剪枝" << endl; 
				board[step.r][step.c] = 0; 
				ab_cut++;
				return score; 
			}	
			best_score = max(best_score, score);
			beta = max(beta, score);
		}
		else {	// 当前是MIN层
			if (score <= beta) { // beta剪枝
				// cout << "beta剪枝" << endl; 
				board[step.r][step.c] = 0; 
				ab_cut++;
				return score; 
			}	
			best_score = min(best_score, score);
			alpha = min(alpha, score);
		}
	}
	
	board[step.r][step.c] = 0;
	return best_score;
}

Pos AIPlayer::get_position()
{
	cout << "AI回合，计算中..." << endl;
	vector<Pos>effect_pos;
	effect_pos_gen(effect_pos);
	int best_score = -INF;
	Pos best_pos = Pos(0,0);

	cout << "生成位置" <<effect_pos.size()<<"个"<< endl;
	// 第一手永远下在天元
	bool is_board_empty = true;
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			if (board[i][j]) {
				is_board_empty = false;
				break;
			}
		}
		if (!is_board_empty) break;
	}
	if (is_board_empty) {
		board[BOARDSIZE / 2][BOARDSIZE / 2] = chess;
		return Pos(BOARDSIZE / 2, BOARDSIZE / 2);
	}
	int alpha = INF;
	int beta = -INF;
	// 剪枝次数统计
	ab_cut = 0;

	int count = 0;
	// 极大极小搜索
	for (auto iter = effect_pos.begin(); iter != effect_pos.end(); iter++) {

		int score = min_max_search(chess, *iter,alpha, beta, search_depth,false);
		cout << pos_transform(*iter) << " : " << score << endl;
		if (score > best_score) {
			best_score = score;
			best_pos = *iter;
		}
		if (score > beta) {
			beta = score;
		}
	}
	cout << "本次搜索剪枝次数" << ab_cut << endl;
	board[best_pos.r][best_pos.c] = chess;
	return best_pos;
}

void AIPlayer::TEST()
{
	int bo[13][13] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,1,0,0,0,0,0,0,0,
		0,0,0,0,0,0,1,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
	};
	board = bo;
	cout<<  board_evaluate(1);
	cout << board_evaluate(2);

}


void inline AIPlayer::get_line(vector<int>& line, Pos start, int len, Pos dir)
{
	line.clear();
	Pos p = start;
	for (int i = 0; i < len; i++) {
		line.push_back(board[p.r][p.c]);
		p = p + dir;
	}
}

bool visit[BOARDSIZE][BOARDSIZE];

void get_neighbor(vector<Pos> &ps,Pos p) {
	ps.clear();
	for (int i = max(p.r - 2, 0); i <= min(p.r + 2, BOARDSIZE-1); i++) {
		for (int j = max(p.c - 2, 0); j <= min(p.c + 2, BOARDSIZE-1); j++) {
			if (!visit[i][j]) { 
				ps.push_back(Pos(i, j)); 
				visit[i][j] = 1;
			}
		}
	}
}

void AIPlayer::effect_pos_gen(vector<Pos> &ps) {
	memset(visit, 0, sizeof(visit));
	// 启发式搜索优化：
	vector<Pos> v_win5;
	vector<Pos> v_live4;
	vector<Pos> v_dead4;
	vector<Pos> v_jump4;
	vector<Pos> v_live3;
	vector<Pos> v_dead3;
	vector<Pos> v_jump3;
	vector<Pos> v_live2;
	vector<Pos> v_dead2;
	vector<Pos> v_jump2;
	vector<Pos> v_others;

	Situation situation;

	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			if (board[i][j])	 {
				visit[i][j] = 1;
				vector<Pos> neighbor;
				get_neighbor(neighbor, Pos(i, j));
				for (auto it = neighbor.begin(); it != neighbor.end(); it++) {
					if (board[it->r][it->c]) continue;
					board[it->r][it->c] = chess;
					Situation s1 = single_situation(Pos(it->r, it->c), chess);
					board[it->r][it->c] = reverse_role(chess);
					// print_board(board);
					Situation s2 = single_situation(Pos(it->r, it->c), reverse_role(chess));
					board[it->r][it->c] = 0;
					if (s1.win5 || s2.win5) v_win5.push_back(Pos(it->r, it->c));
					else if (s1.live4 || s2.live4) v_live4.push_back(Pos(it->r, it->c));
					else if (s1.dead4 || s2.dead4) v_dead4.push_back(Pos(it->r, it->c));
					else if (s1.jump4 || s2.jump4) v_jump4.push_back(Pos(it->r, it->c));
					else if (s1.live3 || s2.live3) v_live3.push_back(Pos(it->r, it->c));
					else if (s1.dead3 || s2.dead3) v_dead3.push_back(Pos(it->r, it->c));
					else if (s1.jump3 || s2.jump3) v_jump3.push_back(Pos(it->r, it->c));
					else if (s1.live2 || s2.live2) v_live2.push_back(Pos(it->r, it->c));
					else if (s1.dead2 || s2.live2) v_dead2.push_back(Pos(it->r, it->c));
					else if (s1.jump2 || s2.jump2) v_jump2.push_back(Pos(it->r, it->c));
				}
				
			}
		}
	}
	bool higher_situation = false;     // 如果有五连、活四，死四、跳四、活三，则不考虑低级棋型
	bool lower_situation = false;	   // 死三、跳三、活二、跳二
	bool mid_situation = false;
	higher_situation = !v_win5.empty() || !v_live4.empty() || !v_dead4.empty() ||
		               !v_jump4.empty() || !v_live3.empty();
	lower_situation = !v_dead3.empty() || !v_jump3.empty() || !v_live2.empty() ||
		              !v_dead2.empty() || !v_jump2.empty();

	if (!v_win5.empty()) {
		ps.insert(ps.end(), v_win5.begin(), v_win5.end());
		return;
	}
	if (!v_live4.empty()) {
		ps.insert(ps.end(), v_live4.begin(), v_live4.end());
		return;
	}
	
	ps.insert(ps.end(), v_dead4.begin(), v_dead4.end());
	ps.insert(ps.end(), v_jump4.begin(), v_jump4.end());
	ps.insert(ps.end(), v_live3.begin(), v_live3.end());

	if (!higher_situation && lower_situation) {
		ps.insert(ps.end(), v_dead3.begin(), v_dead3.end());
		ps.insert(ps.end(), v_jump3.begin(), v_jump3.end());
		if (v_dead3.empty() && v_jump3.empty()) {
			ps.insert(ps.end(), v_live2.begin(), v_live2.end());
		}
		if (v_dead3.empty() && v_jump3.empty() && v_live2.empty()) {
			ps.insert(ps.end(), v_jump2.begin(), v_jump2.end());
		}
		if (v_dead3.empty() && v_jump3.empty() && v_live2.empty() && v_jump2.empty()) {
			ps.insert(ps.end(), v_dead2.begin(), v_dead2.end());
		}
	}
	else if(!higher_situation && !lower_situation) {
		ps.insert(ps.end(), v_others.begin(), v_others.end());
	}
	
}







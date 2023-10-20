#include <iostream>
#include <set>
#include <utility>
#include <vector>
#include <map>

const int DIRECTIONS_CNT = 4;
const int INF = 1e9;
const std::vector<std::pair<int, int> > directions = {std::make_pair(-1, 0), std::make_pair(0, 1),
													  std::make_pair(1, 0), std::make_pair(0, -1)};

long long total_time = 0;
int A, B, C, K;
std::set<std::pair<int, int> > used;
std::set<std::pair<int, int> > definitely_bad;
std::pair<int, int> direction;
bool my_flag = true;

void change_direction(const std::pair<int, int>& target_direction) {
	if (direction == target_direction)
		return;
	if (direction.first == -target_direction.first) {
		std::cout << 2 << " " << 0 << std::endl;
		int res;
		std::cin >> res;
		std::cout << 2 << " " << 0 << std::endl;
		std::cin >> res;
		total_time += 2 * B;
	}
	else {
		if ((direction == std::make_pair(-1, 0) && target_direction == std::make_pair(0, -1)) ||
		    (direction == std::make_pair(0, -1) && target_direction == std::make_pair(1, 0)) ||
			(direction == std::make_pair(1, 0) && target_direction == std::make_pair(0, 1)) ||
			(direction == std::make_pair(0, 1) && target_direction == std::make_pair(-1, 0))) {
			std::cout << 2 << " " << 0 << std::endl;
		} 
		else {
			std::cout << 2 << " " << 1 << std::endl;
		}
		int res;
		std::cin >> res;
		total_time += B;
	}
	direction = target_direction;
}


void dfs(int x_cur, int y_cur, int x_prev, int y_prev, bool start_point) {
	used.insert({x_cur, y_cur});
	std::vector<std::pair<int, int> > directions_sequence = {direction};
	for (size_t i = 0; i < DIRECTIONS_CNT - 1; i++) {
		directions_sequence.push_back(directions_sequence.back());
		std::swap(directions_sequence.back().first, directions_sequence.back().second);
		directions_sequence.back().second *= (-1);
	}
	for (size_t i = 0; i < DIRECTIONS_CNT; i++) {
		int x_next = x_cur + direction.first;
		int y_next = y_cur + direction.second;
		if (used.find({x_next, y_next}) != used.end()) {
			if (i != DIRECTIONS_CNT - 1) {
				change_direction(directions_sequence[i + 1]);
			}
			continue;
		}
		if (definitely_bad.find({x_next, y_next}) != definitely_bad.end()) {
			if (i != DIRECTIONS_CNT - 1) {
				change_direction(directions_sequence[i + 1]);
			}
			continue;
		}
		total_time += A;
		std::cout << 1 << std::endl;
		int result;
		std::cin >> result;
		if (!result) {
			definitely_bad.insert({x_next, y_next});
			if (i != DIRECTIONS_CNT - 1) {
				change_direction(directions_sequence[i + 1]);
			}
			continue;
		}
		dfs(x_next, y_next, x_cur, y_cur, false);
		if (i != DIRECTIONS_CNT - 1) {
			change_direction(directions_sequence[i + 1]);
		}
	}
	if (!start_point) {
		change_direction(std::make_pair(x_prev - x_cur, y_prev - y_cur));
		std::cout << 1 << std::endl;
		total_time += A;
		int res;
		std::cin >> res;
	}
}


struct Action {
	int type;
	int turn;
	int cost;
	Action() {}

	Action(int my_type, int my_turn, int my_cost) : type(my_type), turn(my_turn), cost(my_cost) {}
};

std::pair<std::vector<int>, std::vector<std::pair<int, Action> > > Dijkstra(const std::vector<std::vector<std::pair<int, Action> > >& graph, int v) {
	std::vector<int> used(graph.size()), cost(graph.size(), INF);
	std::vector<std::pair<int, Action> > parent(graph.size());
	cost[v] = 0;
	used[v] = 1;
	while (true) {
		used[v] = 1;
		for (auto u : graph[v]) {
			if (cost[u.first] > cost[v] + u.second.cost) {
				cost[u.first] = cost[v] + u.second.cost;
				parent[u.first] = std::make_pair(v, u.second);
			}
		}
		int min_val = INF, index = -1;
		for (int i = 0; i < used.size(); i++) {
			if (!used[i] && cost[i] < min_val) {
				min_val = cost[i];
				index = i;
			}
		}
		if (min_val == INF) {
			break;
		}
		v = index;
	}
	for (auto el : cost) std::cout << el << " ";
	std::cout << "cost" << std::endl;
	return std::make_pair(cost, parent);
}

void solution_with_dijkstra(int x, int y) {
	std::set<std::pair<int, int> > bad_points;
	std::set<std::pair<int, int> > good_points;
	std::set<std::pair<int, int> > used_points;
	good_points.insert(std::make_pair(x, y));
	while (true) {
		int cnt_of_walls_around = 0;
		for (int i = 0; i < DIRECTIONS_CNT; i++) {
			if (bad_points.find(std::make_pair(x + directions[i].first, y + directions[i].second)) != bad_points.end()) {
				cnt_of_walls_around++;
			}
		}
		if (cnt_of_walls_around != 3) {
			total_time += C;
			std::cout << 3 << std::endl;
			char surround[2 * K + 1][2 * K + 1];
			for (int i = 0; i < 2 * K + 1; i++) {
				for (int j = 0; j < 2 * K + 1; j++) {
					std::cin >> surround[i][j];
					if (surround[i][j] == '#') {
						bad_points.insert(std::make_pair(x + (j - K), y + (K - i)));
					}
					else {
						good_points.insert(std::make_pair(x + (j - K), y + (K - i)));
					}
				}
			}
		}
		if (used_points.size() == good_points.size()) {
			std::cout << 4 << " " << total_time << std::endl;
			break;
		}
		std::vector<std::vector<std::pair<int, Action> > > graph(4 * good_points.size());
		std::map<std::pair<int, int>, int> start_of_block;
		std::vector<std::pair<int, int> > all_vertices;
		int cur_pos = 0;
		for (auto el : good_points) {
			start_of_block[el] = cur_pos;
			cur_pos++;
			for (int i = 0; i < DIRECTIONS_CNT; i++) {
				all_vertices.push_back(el);
			}
		}
		cur_pos = 0;
		int start_index = 0;
		for (auto el : good_points) {
			for (int i = 0; i < DIRECTIONS_CNT; i++) {
				graph[cur_pos + i].push_back(std::make_pair((cur_pos + i + 1) % DIRECTIONS_CNT, Action(2, 1, B)));
				if (el == std::make_pair(x, y) && direction == directions[i]) {
					start_index = cur_pos + i;
				}
			}
			for (int i = 0; i < DIRECTIONS_CNT; i++) {
				graph[cur_pos + i].push_back(std::make_pair((cur_pos + i - 1 + DIRECTIONS_CNT) % DIRECTIONS_CNT, Action(2, 0, B)));
			}
			for (int i = 0; i < DIRECTIONS_CNT; i++) {
				std::pair<int, int> nxt = std::make_pair(x + directions[i].first, y + directions[i].second);
				if (good_points.find(nxt) != good_points.end()) {
					graph[cur_pos + i].push_back(std::make_pair(start_of_block[nxt] * 4 + i, Action(1, -1, A)));
				}
			}
			cur_pos += 4;
		}
		/*for (auto el : good_points) {
			std::cout << el.first << " " << el.second << std::endl;
		}
		std::cout << "good points" << std::endl;*/

		auto res = Dijkstra(graph, start_index);
		auto cost = res.first;
		auto parent = res.second;
		int best = INF, finish = -1;
		for (int i = 0; i < cost.size(); i++) {
			if (used_points.find(all_vertices[i]) == used_points.end()) {
				if (cost[i] < best) {
					best = cost[i];
					finish = i;
				}
			}
		}
		int finish_backup = finish;
		std::vector<Action> path;
		while (finish != start_index) {
			path.push_back(parent[finish].second);
			finish = parent[finish].first;
		}
		for (int i = path.size() - 1; i >= 0; i--) {
			if (path[i].type == 1) {
				std::cout << 1 << std::endl;
				total_time += A;
				int res;
				std::cin >> res;
			}
			else {
				std::cout << 2 << " " << path[i].turn << std::endl;
				total_time += B;
				int res;
				std::cin >> res;
			}
		}
		used_points.insert(std::make_pair(x, y));
		x = all_vertices[finish_backup].first;
		y = all_vertices[finish_backup].second;
		direction = directions[finish_backup % DIRECTIONS_CNT];
	}
}


void solve() {
	int x, y, x1, y1;
	std::cin >> x >> y >> x1 >> y1 >> A >> B >> C >> K;
	direction = {x1 - x, y1 - y};
	if (C > 4 * A + 4 * B || my_flag) {
		dfs(x, y, 0, 0, true);
		std::cout << 4 << " " << total_time << std::endl;
	}
	else {
		// std::cout << "here" << std::endl;
		solution_with_dijkstra(x, y);
	}
}


int main() {
	solve();
}
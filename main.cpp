#include <iostream>
#include <set>
#include <utility>
#include <vector>

const int DIRECTIONS_CNT = 4;


/*
struct Point {
	int x, y;

	Point(int x, int y): x(x), y(y) {}

	bool operator==(const Point& other) const {
		return other.x == x && other.y == y;
	}

	bool operator<(const Point& other) const {
		return x < other.x || (x == other.x && y < other.y);
	}

	bool operator>(const Point& other) const {
		return other < *this;
	}

	Point operator+(const Point& other) const {
		new_point = Point(x + other.x, y + other.y);
	}
};*/

long long total_time = 0;
int A, B, C, K;
std::set<std::pair<int, int> > used;
std::set<std::pair<int, int> > definitely_bad;
std::pair<int, int> direction;

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
		// std::cout << i << " i" << std::endl;
		// std::cout << direction.first << " " << direction.second << std::endl;
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
		int res;
		std::cin >> res;
	}
}

void solve() {
	int x, y, x1, y1;
	std::cin >> x >> y >> x1 >> y1 >> A >> B >> C >> K;
	direction = {x1 - x, y1 - y};
	dfs(x, y, 0, 0, true);
	std::cout << 4 << " " << total_time << std::endl;
}

int main() {
	solve();
}
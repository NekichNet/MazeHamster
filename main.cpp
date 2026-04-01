#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <Windows.h> 

#define point std::pair<unsigned short, unsigned short>

const std::string FILEPATH = "input.txt"; // Путь к файлу, содержащему лабиринт
const char FLOOR = '.'; // Символ, обозначающий клетку, по которой можно ходить
const char CHECKED = ','; // Символ, обозначающий клетку, по которой уже пробежала одна из копий хомячка
const char PATH = '*'; // Символ, обозначающий кратчайший найденный путь
const char START = 'S'; // Символ, обозначающий точку старта
const char FINISH = 'F'; // Символ, обозначающий точку финиша

point read_file(std::string filepath, std::vector<std::vector<char>>& maze);
std::vector<point> check_near_cells(point cell, std::vector<std::vector<char>>& maze);
void print_maze(std::vector<std::vector<char>>& maze);

int main() {
	std::vector<std::vector<char>> maze; // К клеткам лабиринта обращаться maze[y][x]!
	point start_point = read_file(FILEPATH, maze); // Читаем лабиринт из файла и ищем старт

	std::queue<std::vector<point>> hamsters; // Очередь всех бегущих копий хомяка
	hamsters.push(std::vector<point> { start_point }); // Среди яйца и курицы первее появился хомяк

	while (!hamsters.empty()) {
		for (point available_cell : check_near_cells(hamsters.front().back(), maze)) {
			if (maze[available_cell.second][available_cell.first] == FINISH) { // Если найден финиш
				for (point waypoint : hamsters.front()) // То отмечаем символами его путь в лабиринте
					maze[waypoint.second][waypoint.first] =
						maze[waypoint.second][waypoint.first] != START ? PATH : START;
				print_maze(maze); // Итог выводится в консоль
				Sleep(10000); // Консоль закроется через 10 секунд после вывода сообщения
				return 0;
			}
			// Создаём копию хомяка на свободной клетке и добавляем в очередь
			std::vector<point> hamster_copy = std::vector<point>(hamsters.front());
			hamster_copy.push_back(available_cell);
			hamsters.push(hamster_copy);
			maze[available_cell.second][available_cell.first] = CHECKED;
		}
		hamsters.pop();
	}

	// Если все копии хомяка скончались, значит найти финиш невозможно
	std::cout << "No possible ways to pass this labyrinth.";
	Sleep(10000); // Консоль закроется через 10 секунд после вывода сообщения

	return 0;
}

// Чтение файла по пути, последовательное сохранение ячеек в переданный maze.
// Возвращает координаты точки старта.
point read_file(std::string filepath, std::vector<std::vector<char>>& maze)
{
	point start_point = point(0, 0);

	std::ifstream stream(filepath);
	if (stream.is_open())
	{
		maze.clear(); // Очищаем вектора

		char cell;
		unsigned short y = 0;
		unsigned short x = 0;
		while (stream.get(cell)) {
			maze.resize(y + 1, std::vector<char>());
			if (cell == '\n') { // Если строка закончилась, то переходим к следующему vector
				y++;
				x = 0;
			}
			else {
				if (cell == START) // Если нашли точку старта, сохраняем
					start_point = point(x, y);
				maze[y].push_back(cell);
				x++;
			}
		}
	}

	return start_point;
}

void print_maze(std::vector<std::vector<char>>& maze)
{
	for (std::vector<char> line : maze) {
		for (char cell : line)
			std::cout << (cell != CHECKED ? cell : FLOOR); // Подчищаем следы проверенных клеток
		std::cout << std::endl;
	}
}

bool is_cell_available(point cell, std::vector<std::vector<char>>& maze) {
	return maze[cell.second][cell.first] == FLOOR || maze[cell.second][cell.first] == FINISH;
}

// Принимает координаты клетки и объект лабиринта.
// Возвращает только клетки, доступные для перехода, из 4-ёх клеток вокруг.
std::vector<point> check_near_cells(point cell, std::vector<std::vector<char>>& maze)
{
	std::vector<point> cells;
	if (cell.first != 0) {
		point checking_cell = point{ cell.first - 1, cell.second };
		if (is_cell_available(checking_cell, maze))
			cells.push_back(checking_cell);
	}
	if (cell.second != 0) {
		point checking_cell = point{ cell.first, cell.second - 1 };
		if (is_cell_available(checking_cell, maze))
			cells.push_back(checking_cell);
	}
	if (cell.first != maze[cell.second].size() - 1) {
		point checking_cell = point{ cell.first + 1, cell.second };
		if (is_cell_available(checking_cell, maze))
			cells.push_back(checking_cell);
	}
	if (cell.second != maze.size() - 1) {
		point checking_cell = point{ cell.first, cell.second + 1 };
		if (is_cell_available(checking_cell, maze))
			cells.push_back(checking_cell);
	}
	return cells;
}

/*
 * Daniel Vázquez, Kiran Vasudev, Patrick Nagel
 * Aritifiacial Intelligence for Robots
 * MAS SS 2016
 * Assignment 3
 *
 * agent.cpp
 * */

#include "agent.hpp"

#include <iostream>
#include <stack>
#include <queue>

#include <chrono>
#include <thread>

#include <ctime>

using namespace std;
using namespace std::chrono;

Agent::Agent(vector<vector<string> > map, const pair<int, int> initial_pos,
		int number_of_goals, int search_option) :
		map(map), initial_pos(initial_pos), number_of_goals(number_of_goals), search_option(
				search_option), number_of_stored_nodes(0), number_of_visited_nodes(
				0){

}

Agent::~Agent() {

}

void Agent::run() {

	if (search_option == 1) {
		cout << "Running BFS " << endl;
		cout << "Number of goals " << number_of_goals << endl;
		bfs();
	} else {
		cout << "Running DFS " << endl;
		cout << "Number of goals " << number_of_goals << endl;
		dfs();
	}

}

void Agent::print_map() {
	system("clear");

	for (int row = 0; row < map_rows; row++) {
		for (int col = 0; col < map_cols; col++) {
			cout << map[row][col];
		}
		cout << endl;
	}

	this_thread::sleep_for(chrono::milliseconds(10));

}

// Here is something to do
void Agent::bfs() {

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	//queue stores a pair in the form (row, col)
	queue<pair<int, int> > nodes_queue;
	pair<int, int> current_node;

	std::string right_child;
	std::string left_child;
	std::string up_child;
	std::string down_child;

	int row;
	int col;

	nodes_queue.push(make_pair(initial_pos.first, initial_pos.second));
	number_of_stored_nodes++;

	while (goal_positions.size() < number_of_goals && nodes_queue.size() > 0) {

		current_node = nodes_queue.front();
		number_of_visited_nodes++;
		nodes_queue.pop();

		row = current_node.first;
		col = current_node.second;

		right_child = map[row][col + 1];
		left_child = map[row][col - 1];
		up_child = map[row + 1][col];
		down_child = map[row - 1][col];

		if (map[row][col] == "*") {
			goal_positions.push_back(current_node);

		}

		if (right_child == " " || right_child == "*") {
			nodes_queue.push(
					make_pair(current_node.first, current_node.second + 1));
			if (right_child != "*") {
				map[row][col + 1] = ".";
			}
			number_of_stored_nodes++;
		}

		if (left_child == " " || left_child == "*") {
			nodes_queue.push(
					make_pair(current_node.first, current_node.second - 1));
			if (left_child != "*") {
				map[row][col - 1] = ".";
			}
			number_of_stored_nodes++;
		}

		if (up_child == " " || up_child == "*") {
			nodes_queue.push(
					make_pair(current_node.first + 1, current_node.second));
			if (up_child != "*") {
				map[row + 1][col] = ".";
			}
			number_of_stored_nodes++;
		}

		if (down_child == " " || down_child == "*") {
			nodes_queue.push(
					make_pair(current_node.first - 1, current_node.second));
			if (down_child != "*") {
				map[row - 1][col] = ".";
			}
			number_of_stored_nodes++;
		}

		map[row][col] = "~";

		print_map();
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<seconds>( t2 - t1 ).count();

	print_evaluation_metrics("queue");
	cout << "Execution time: " << duration << "sec"<< endl;
}

void Agent::dfs() {
	//the stack stores a pair in the form (row, col)
	stack<pair<int, int> > nodes_stack;
	pair<int, int> current_node;
	//Note: Elements stored in a pair can be accessed by calling the .first and .second attributes. E.g:
	//int row = current_node.first
	//int col = current_node.second

	//Add the initial node to the stack
	//Note: To add a new pair to the stack you can use the std::make_pair method e.g:
	nodes_stack.push(make_pair(initial_pos.first, initial_pos.second));
	number_of_stored_nodes++;

	while (nodes_stack.size() > 0) {

		current_node = nodes_stack.top();
		nodes_stack.pop();

		//TODO
		/*Note: If the current node contains a dirt you can
		 * store the position in the goal_positions vector
		 * by calling the push method e.g goal_positions.push(current_node)
		 */

		print_map();

	}

	print_evaluation_metrics("stack");

}

void Agent::print_evaluation_metrics(string data_structure_name) {
	print_map();
	cout << "All the possible nodes have been explored " << endl;
	cout << "Found " << goal_positions.size() << " of " << number_of_goals
			<< endl;
	cout << "Maximum size of the " << data_structure_name << " : "
			<< number_of_stored_nodes << endl;
	cout << "Number of stored nodes: " << number_of_stored_nodes << endl;
	cout << "Total of visited nodes: " << number_of_visited_nodes << endl;
	print_goal_positions();
}

void Agent::print_goal_positions() {
	cout << "Goals: " << endl;
	for (int i = 0; i < goal_positions.size(); i++) {
		cout << i + 1 << ": (" << goal_positions[i].first << ", "
				<< goal_positions[i].second << ")" << endl;
	}
}


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
#include <stdlib.h>
#include <chrono>
#include <thread>

#include <ctime>

using namespace std;
using namespace std::chrono;

Agent::Agent(vector<vector<string> > map, const pair<int, int> initial_pos,
		int number_of_goals, int search_option) :
		map(map), initial_pos(initial_pos), number_of_goals(number_of_goals), search_option(
				search_option), number_of_stored_nodes(0), number_of_visited_nodes(
				0) {

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

	//get the start of exectution time
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	//the queue stores a pair in the form (row, col)
	queue<pair<int, int> > nodes_queue;
	pair<int, int> current_node;
	//Note: Elements stored in a pair can be accessed by calling the .first and .second attributes. E.g:
	//int row = current_node.first
	//int col = current_node.second

	//variables for the child nodes
	std::string right_child;
	std::string left_child;
	std::string up_child;
	std::string down_child;

	//variables for the position (row, column) of a node
	int row;
	int col;

	//Add the initial node to the queue
	//Note: To add a new pair to the queue you can use the std::make_pair method e.g:
	nodes_queue.push(make_pair(initial_pos.first, initial_pos.second));
	number_of_stored_nodes++;

	while (goal_positions.size() < number_of_goals && nodes_queue.size() > 0) {

		current_node = nodes_queue.front();
		number_of_visited_nodes++;
		nodes_queue.pop();

		//get the (row, column) pair of the current node
		row = current_node.first;
		col = current_node.second;

		//get the children nodes of the current node
		right_child = map[row][col + 1];
		left_child = map[row][col - 1];
		up_child = map[row + 1][col];
		down_child = map[row - 1][col];

		//check for goal state and add position to "goal_positions"
		if (map[row][col] == "*" or map[row][col] == "2") {
			goal_positions.push_back(current_node);

		}

		//checks the neightbors of the current node if they are marked or visited, if not push them to the queue and mark them
		if (right_child == " " || right_child == "*") {
			nodes_queue.push(
					make_pair(current_node.first, current_node.second + 1));
			if (right_child != "*") {
				map[row][col + 1] = "1";
			}
			if (right_child == "*") {
				map[row][col + 1] = "2";
			}
			number_of_stored_nodes++;
		}

		if (left_child == " " || left_child == "*") {
			nodes_queue.push(
					make_pair(current_node.first, current_node.second - 1));
			if (left_child != "*") {
				map[row][col - 1] = "1";
			}
			if (left_child == "*") {
				map[row][col - 1] = "2";
			}
			number_of_stored_nodes++;
		}

		if (up_child == " " || up_child == "*") {
			nodes_queue.push(
					make_pair(current_node.first + 1, current_node.second));
			if (up_child != "*") {
				map[row + 1][col] = "1";
			}
			if (up_child == "*") {
				map[row + 1][col] = "2";
			}
			number_of_stored_nodes++;
		}

		if (down_child == " " || down_child == "*") {
			nodes_queue.push(
					make_pair(current_node.first - 1, current_node.second));
			if (down_child != "*") {
				map[row - 1][col] = "1";
			}
			if (down_child == "*") {
				map[row - 1][col] = "2";
			}
			number_of_stored_nodes++;
		}

		map[row][col] = "0";

		print_map();
	}

	//get the end of execution time
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	//the difference between start and end time is the duration of the search
	auto duration = duration_cast < seconds > (t2 - t1).count();

	print_evaluation_metrics("queue");

	//print the execution time of the search
	cout << "Execution time: " << duration << "sec" << endl;
}

void Agent::dfs() {

	//get the start of exectution time
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

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

	while (nodes_stack.size() > 0 && goal_positions.size() < number_of_goals) {

		current_node = nodes_stack.top();
		number_of_visited_nodes++;
		nodes_stack.pop();

		//get the (row, column) pair of the current node
		int row = current_node.first;
		int col = current_node.second;

		//check for goal state and add position to "goal_positions"
		if (map[row][col] == "*") {
			goal_positions.push_back(make_pair(row, col));
		}

		//checks the neightbors of the current node if it is unvisited or not, pushes it to the stack and marks it visited
		if (map[row][col + 1] == " " || map[row][col + 1] == "*") {
			nodes_stack.push(make_pair(row, col + 1));
			if (map[row][col + 1] != "*") {
				map[row][col + 1] = "1";
			}
			number_of_stored_nodes++;
		}

		if (map[row][col - 1] == " " || map[row][col - 1] == "*") {
			nodes_stack.push(make_pair(row, col - 1));
			if (map[row][col - 1] != "*") {
				map[row][col - 1] = "1";
			}
			number_of_stored_nodes++;
		}

		if (map[row + 1][col] == " " || map[row + 1][col] == "*") {
			nodes_stack.push(make_pair(row + 1, col));
			if (map[row + 1][col] != "*") {
				map[row + 1][col] = "1";
			}
			number_of_stored_nodes++;
		}

		if (map[row - 1][col] == " " || map[row - 1][col] == "*") {
			nodes_stack.push(make_pair(row - 1, col));
			if (map[row - 1][col] != "*") {
				map[row - 1][col] = "1";
			}
			number_of_stored_nodes++;
		}

		map[row][col] = "0";
		//TODO
		/*Note: If the current node contains a dirt you can
		 * store the position in the goal_positions vector
		 * by calling the push method e.g goal_positions.push(current_node)
		 */

		print_map();
	}

	//get the end of execution time
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	//the difference between start and end time is the duration of the search
	auto duration = duration_cast < seconds > (t2 - t1).count();

	print_evaluation_metrics("stack");

	//print the execution time of the search
	cout << "Execution time: " << duration << "sec" << endl;

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


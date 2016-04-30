/*
 * Daniel Vázquez
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

using namespace std;

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

void Agent::bfs() {
	//queue stores a pair in the form (row, col)
	queue<pair<int, int> > nodes_queue;
	pair<int, int> current_node;
	//Note: Elements stored in a pair can be accessed by calling the .first and .second attributes. E.g:
	//int row = current_node.first
	//int col = current_node.second

	//Add the initial node to the queue
	//Note: To add a new pair to the queue you can use the std::make_pair method e.g:

	nodes_queue.push(make_pair(initial_pos.first, initial_pos.second));
	number_of_stored_nodes++;

	while (nodes_queue.size() > 0) {

		current_node = nodes_queue.front();
		nodes_queue.pop();

		//TODO
		/*Note: If the current node contains a dirt you can
		 * store the position in the goal_positions vector 
		 * by calling the push method e.g goal_positions.push(current_node)
		 */

		print_map();

	}

	print_evaluation_metrics("queue");
}

void Agent::dfs() {

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

	while (nodes_stack.size() > 0) {

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

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<seconds>( t2 - t1 ).count();

	print_evaluation_metrics("stack");
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


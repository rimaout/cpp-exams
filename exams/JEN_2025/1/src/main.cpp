#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "markov.hpp"

using namespace std;

#define INPUT_FILE "parameters.txt"
#define OUTPUT_FILE "results.txt"

#define STUDENTE_INFO "NAME-SURNAME_UNIID"
#define EXAM_DATE "2025-01-09"

#define NUM_SIMULATIONS 1000
#define START_NODE 0
#define END_NODE (num_of_nodes - 1)

int read_input_file(const string filename, int& num_of_nodes, vector<Transition>& transition_list);
int write_output_file(const string filename, double result);

int main() {

    // - READ INPUT FILE AND PARSE DATA
    int num_of_nodes = 0;
    vector<Transition> transition_list;
    
    if (read_input_file("parameters.txt", num_of_nodes, transition_list) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    // - INITIALIZE MARKOV CHAIN
    MarkovChain markovChain = MarkovChain(transition_list);

    // - RUN SIMULATION
    double total_cost = 0.0;

    for (int i = 0; i < NUM_SIMULATIONS; ++i) {
        int current_node = START_NODE;

        while (current_node != END_NODE) {
            int next_node = markovChain.get_next_node(current_node);
            total_cost += markovChain.get_transition_cost(current_node, next_node);
            current_node = next_node;
        }
    }
    
    double average_cost = total_cost / NUM_SIMULATIONS;

    // - SAVE OUTPUT TO FILE
    if (write_output_file("results.txt", average_cost) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;   
}

int read_input_file(const string filename, int& num_of_nodes, vector<Transition>& transition_list) {
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char lineType;
    while (inputFile >> lineType) {
        if (lineType == 'N') {
            inputFile >> num_of_nodes;
        } 
        else if (lineType == 'A') {
            Transition t;
            inputFile >> t.from >> t.to >> t.probability >> t.cost;
            transition_list.push_back(t);
        }
    }
    inputFile.close();
    return EXIT_SUCCESS;
}

int write_output_file(const string filename, double result) {
    ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    outputFile << EXAM_DATE << "-" << STUDENTE_INFO << endl;
    outputFile << "C " << result << endl;
    outputFile.close();
    return EXIT_SUCCESS;
}
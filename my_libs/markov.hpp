#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>

#include "my_random.hpp"

using namespace std;

/**
 * @brief Represents a single definition of a transition between two states.
 * * This struct is used to initialize the Markov Chain.
 */
struct Transition {
    int from;           ///< The ID of the starting node
    int to;             ///< The ID of the destination node
    double probability; ///< The weight/probability of this transition (0.0 to 1.0)
    double cost;        ///< The cost associated with this transition
};

/**
 * @brief A Markov Chain engine that manages states and probabilistic transitions.
 * * This class builds a directed graph where edges have probabilities and costs.
 * It allows for traversing the graph based on weighted random selection.
 */
class MarkovChain {
private:
    /**
     * @brief Internal edge structure stored in the adjacency list.
     */
    struct Edge {
        int to;
        double probability;
        double cost;
    };

    /// Adjacency list mapping: Node ID -> List of outgoing Edges
    map<int, vector<Edge>> adjacent_list; 
    
public:
    
    /**
     * @brief Constructs the Markov Chain from a list of transitions.
     * * Builds the internal adjacency list and seeds the random number engine.
     * * @param transitions A vector of Transition structs defining the graph topology.
     */
    MarkovChain(const vector<Transition>& transitions) {
        for (const Transition& t : transitions) {
            Edge edge {t.to, t.probability, t.cost};
            adjacent_list[t.from].push_back(edge);
        }
    }

    /**
     * @brief Selects the next node based on the current node's transition probabilities.
     * * This function uses a weighted random selection. Transitions with higher 
     * probability values are more likely to be chosen.
     * * @param current_node The ID of the node you are currently at.
     * @return int The ID of the next node.
     * @throws std::runtime_error If the current_node does not exist or has no outgoing edges.
     */
    int get_next_node(int current_node) {
        auto it = adjacent_list.find(current_node); // note: we use auto because the real type is (to long) std::map<int, std::vector<Edge>>::iterator
        
        /**
         * Note:
         *  - `it` is a map iterator, `it->first` is the key, `it->second` is the value (vector of edges)
         *  - `adjacent_list.end()` is the iterator that points to one past the last element, so if `it == adjacent_list.end()`, it means `current_node` is not in the map
         *  - Additionally, we check if the vector of edges is empty, which would mean there are no outgoing transitions from this node
         */

        if (it == adjacent_list.end() || it->second.empty()) {
            throw runtime_error("Current node has no outgoing transitions.");
        }

        const vector<Edge>& edges = it->second;
        vector<double> weights;         // Create a vector of weights for discrete_distribution
        weights.reserve(edges.size());  // reserve capacity to avoid multiple allocations

        for (const Edge& edge : edges) {
            weights.push_back(edge.probability);
        }

        int index = Random::getDiscrete(weights); // Get a random index based on the weights
        return edges[index].to;
    }

    /**
     * @brief Retrieves the cost of a specific transition.
     * * @param from The starting node ID.
     * * @param to The destination node ID.
     * @return double The cost associated with the transition.
     * @throws std::runtime_error If the start node is not found or the transition does not exist.
     */
    double get_transition_cost(int from, int to) {
        auto it = adjacent_list.find(from);
        if (it == adjacent_list.end()) {
            throw runtime_error("Start node not found.");
        }

        for (const Edge& edge : it->second) {
            if (edge.to == to) {
                return edge.cost;
            }
        }

        throw runtime_error("Transition not found between specified nodes.");
    }
};
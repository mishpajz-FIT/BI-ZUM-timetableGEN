/**
 * @file cv02.cpp
 * @author Michal Dobes
 * @date 2023-03-02
 *
 * @brief Use Dijkstra to solve homework cv02/1
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <bitset>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <queue>

struct Node {

    // positions of persons
    // 0 means in right, 1 means in left
    std::bitset<5> positions;

    // position of battery
    // 0 means in left, 1 means in right
    bool battery;

    Node(): positions(), battery(false) { }

    void print() { // print node to stream
        std::cout << "node: { ";
        for (size_t i = 0; i < 5; i++) {
            if (positions[i]) {
                std::cout << 1 << " ";
            } else {
                std::cout << 0 << " ";
            }
        }
        std::cout << " } " << battery << std::endl;
    }

};

std::vector<unsigned long> timeForPerson = { 1, 3, 5, 8, 12 }; // timeForPerson[i] = time to cross for person with index i

inline bool operator==(const Node & lhs, const Node & rhs) {
    return (lhs.positions == rhs.positions) && (lhs.battery == rhs.battery);
}

// Hash for Node
struct NodeHasher {
    size_t operator() (const Node & node) const {
        unsigned long value = node.positions.to_ulong() << 1; // convert positions to number
        if (node.battery) { // if battery in right, set last bit in number to true
            value += 1;
        }
        return std::hash<unsigned long>{}(value);
    }
};

int main() {
    std::unordered_map<Node, unsigned long, NodeHasher> distances; // best distances for each node
    std::unordered_map<Node, size_t, NodeHasher> states; // states of nodes (0 = fresh, 1 = open, 2 = closed)
    std::unordered_map<Node, Node, NodeHasher> previous; // previous node for each node (to reconstruct path)


    using NodeDistance = std::pair<Node, unsigned long>;

    // priority queue for dijkstra
    auto compare = [ ](const NodeDistance & lhs, const NodeDistance & rhs) { return rhs.second < lhs.second; };
    std::priority_queue<NodeDistance, std::vector<NodeDistance>, decltype(compare)> queue(compare);

    // initial position
    Node start;
    start.positions.reset();
    start.battery = false;
    distances[start] = 0;
    states[start] = 1;
    queue.emplace(start, 0);

    while (!queue.empty()) {
        // retrieve node with smallest distance from queue
        Node top = queue.top().first;
        unsigned long topDistance = queue.top().second;
        queue.pop();

        if (states[top] == 2) { // check if node was already closed
            continue;
        }
        states[top] = 2; // close node

        if (top.positions.all()) { // check if reached goal
            std::cout << topDistance << std::endl; // print distance
            Node it(top);
            while (!(it == start)) { // print path
                it.print();
                it = previous[it];
            }
            start.print();
            return 0;
        }

        // try to move each person to other side (generate new nodes)
        for (size_t i = 0; i < 5; i++) {
            if (top.positions[i] != top.battery) { // check they are on the same side as battery
                continue;
            }

            // move them
            Node copy(top);
            copy.positions[i] = !copy.positions[i];
            copy.battery = !copy.battery;
            unsigned long newDistance = topDistance + timeForPerson[i];

            // relax as in dijkstra
            if ((distances.count(copy) == 0) || (distances[copy] > newDistance)) {

                if (states[copy] == 2) {
                    continue;
                }

                states[copy] = 1; // open node
                distances[copy] = newDistance;
                previous[copy] = top;

                queue.emplace(copy, newDistance); // put node into queue
            }
        }

        // try to move each two persons to other side
        for (size_t i = 0; i < 4; i++) {
            for (size_t j = i + 1; j < 5; j++) {
                if (top.positions[i] != top.battery || top.positions[j] != top.battery) { // check they are on the same side as battery
                    continue;
                }

                // move them
                Node copy(top);
                copy.positions[i] = !copy.positions[i];
                copy.positions[j] = !copy.positions[j];
                copy.battery = !copy.battery;

                // relax as in dijkstra
                unsigned long newDistance = topDistance + timeForPerson[j];
                if ((distances.count(copy) == 0) || (distances[copy] > newDistance)) {

                    if (states[copy] == 2) {
                        continue;
                    }

                    states[copy] = 1; // open node
                    distances[copy] = newDistance;
                    previous[copy] = top;

                    queue.emplace(copy, newDistance); // put node into queue
                }
            }
        }
    }

    return 1;
}
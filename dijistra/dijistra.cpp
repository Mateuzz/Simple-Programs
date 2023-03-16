#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>

struct Edge;

struct Node {
    int ch;
    std::vector<Edge> edges;
};

struct Edge {
    Node* nodeTo;
    int weight;
};

struct NodePath {
    Node* parent;
    int weigthSum;
};

using Path = std::unordered_map<Node*, NodePath>;
using NodeList = std::vector<Node*>;

Path findNode(Node* start, Node* end) {
    Path map;
    std::queue<std::pair<Node*, int>> nodesToCheck;
    int currentWeight = 0;
    Node* currentNode = start;

    nodesToCheck.emplace(start, currentWeight);

    while (!nodesToCheck.empty()) {
        auto& pair = nodesToCheck.front();
        currentNode = pair.first;
        currentWeight = pair.second;

        for (Edge& edge : pair.first->edges) {
            if (edge.nodeTo == start) {
                continue;
            }

            auto& NodePathTo = map[edge.nodeTo];
            int weightSum = currentWeight + edge.weight;

            if (!NodePathTo.parent || NodePathTo.weigthSum > weightSum) {
                NodePathTo.weigthSum = weightSum;
                NodePathTo.parent = currentNode;
                if (edge.nodeTo != end) {
                    nodesToCheck.push({edge.nodeTo, weightSum});
                }
            }
        }

        nodesToCheck.pop();
    }

    return map;
}

void addTwoWayEdge(Node* n1, Node* n2) {
    int distance = rand() % 20 + 1;
    n1->edges.push_back({n2, distance});
    n2->edges.push_back({n1, distance});
}

NodeList generateNodeList() {
    srand(time(nullptr));

    NodeList list;

    for (int i = 0; i < 8; ++i) {
        list.emplace_back(new Node {'a' + i, {}});
    }

    for (int i = 0; i < 6; ++i) {
        addTwoWayEdge(list[i], list[i + 1]);
        addTwoWayEdge(list[i], list[i + 2]);
    }

    addTwoWayEdge(list[6], list[7]);

    return list;
}

void printNodelist(const NodeList& list) {
    for (auto node : list) {
        for (const auto& edge : node->edges) {
            std::cout << '\'' << char(node->ch) << "' => '" << char(edge.nodeTo->ch)
                      << "'  distance = " << edge.weight << '\n';
        }
        std::cout << '\n';
    }
}

void printNodePath(Node* start, Path& path) {
    auto node = start;
    while (node) {
        std::cout << "Current = " << char(node->ch)
                  << " -- Distance remaining = " << path[node].weigthSum << '\n';
        node = path[node].parent;
    }
}

int main() {
    auto nodeList = generateNodeList();
    printNodelist(nodeList);

    auto path = findNode(nodeList.back(), nodeList.front());
    printNodePath(nodeList.front(), path);

    for (auto node : nodeList) {
        delete node;
    }
}

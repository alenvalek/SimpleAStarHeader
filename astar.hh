#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <functional>

using namespace std;

struct Node {
    int x, y;
    int gCost;
    int hCost;
    Node* parent;

    Node(int x, int y, int gCost, int hCost, Node* parent)
        : x(x), y(y), gCost(gCost), hCost(hCost), parent(parent) {}

    int calculateFCost() const {
        return gCost + hCost;
    }

    bool operator>(const Node& comparisonNode) const {
        return calculateFCost() > comparisonNode.calculateFCost();
    }
};

int getDistanceOnXAxis(const Node& currentNode, const Node& goalNode) {
    return abs(currentNode.x - goalNode.x);
}

int getDistanceOnYAxis(const Node& currentNode, const Node& goalNode) {
    return abs(currentNode.y - goalNode.y);
}

int calculateManhattanDistance(const Node& currentNode, const Node& goalNode) {
    int xDistance = getDistanceOnXAxis(currentNode, goalNode);
    int yDistance = getDistanceOnYAxis(currentNode, goalNode);
    return xDistance + yDistance;
}

bool isValidPositionOnGrid(int rowCount, int colCount, int x, int y) {
    return x >= 0 && x < rowCount && y >= 0 && y < colCount;
}

bool validateInput(int rowCount, int colCount, pair<int, int> startPosition, pair<int, int> goalPosition) {
    return isValidPositionOnGrid(rowCount, colCount, startPosition.first, startPosition.second) &&
           isValidPositionOnGrid(rowCount, colCount, goalPosition.first, goalPosition.second);
}

vector<pair<int, int>> processAStarPathfinding(vector<vector<int>>& grid, pair<int, int> startPosition, pair<int, int> goalPosition) {
    int rows = grid.size();
    int cols = grid[0].size();

    if(!validateInput(rows, cols, startPosition, goalPosition)) {
        return vector<int,int>>();
    }

    vector<pair<int, int>> possibleDirections = {
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}
    };

    priority_queue<Node, vector<Node>, greater<Node>> prioQueue;

    Node startNode(startPosition.first, startPosition.second, 0, calculateManhattanDistance(startPosition, goalPosition), nullptr);
    Node goalNode(goalPosition.first, goalPosition.second, 0, 0, nullptr);

    map<pair<int, int>, bool> visitedNodes;

    prioQueue.push(startNode);

    while (!prioQueue.empty()) {
        Node currentNode = prioQueue.top();
        prioQueue.pop();

        if (currentNode.x == goalNode.x && currentNode.y == goalNode.y) {
            vector<pair<int, int>> traversalPath;

            while (currentNode.parent != nullptr) {
                traversalPath.emplace_back(currentNode.x, currentNode.y);
                currentNode = *currentNode.parent;
            }

            traversalPath.emplace_back(startPosition.first, startPosition.second);
            reverse(traversalPath.begin(), traversalPath.end());

            return traversalPath;
        }

        visitedNodes[{currentNode.x, currentNode.y}] = true;

        for (const auto& possibleDirection : possibleDirections) {
            int newPositionX = currentNode.x + possibleDirection.first;
            int newPositionY = currentNode.y + possibleDirection.second;

            if (isValidPositionOnGrid(rows, cols, newPositionX, newPositionY)) {
                if (grid[newPositionX][newPositionY] != 1) {
                    int calculatedGCost = currentNode.gCost + 1;

                    if (!visitedNodes[{newPositionX, newPositionY}] || calculatedGCost < calculatedGCost) {
                        Node neighboringNode(newPositionX, newPositionY, calculatedGCost, calculateManhattanDistance({newPositionX, newPositionY}, goalNode), &currentNode);

                        prioQueue.push(neighboringNode);
                    }
                }
            }
        }
    }

    return vector<pair<int, int>>();
}

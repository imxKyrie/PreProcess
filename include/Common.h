#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

struct Node
{
    int index;
    float x, y;
    Node(const int& index, const double& x, const double& y)
        : index(index), x(x), y(y) {}
};

struct Face
{
    int index;
    int num;
    int type;
    std::vector<int> nodes;
    std::vector<int> adjacentcells;
    Face(const int& index, const int& num, const int& type, const int& node1, const int& node2, const int& cell1, const int& cell2)
        : index(index), num(num), type(type), nodes{node1, node2}, adjacentcells{cell1, cell2} {}
};

struct Cell
{
    int index;
    int type;
    std::vector<int> nodes;
    std::vector<Face> faces;
    Cell(const int& index, const int& type, const std::vector<int>& nodes, const std::vector<Face>& faces)
        : index(index), type(type), nodes(nodes), faces(faces) {}
};

#endif // COMMON_H
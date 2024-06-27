#include "Common.h"

void ProcessFile(std::string& filename,
                 std::vector<Node>& nodes,
                 std::vector<int>& cellstype,
                 std::vector<Face>& faces,
                 int& numberofnodes,
                 int& numberoffaces,
                 int& boundaryfaces,
                 int& interiorfaces,
                 int& numberofcells,
                 int& tricells,
                 int& quadcells);

void InputDataToCells(const int& numberofcells, const std::vector<Face>& faces, const std::vector<int>& cellstype, std::vector<Cell>& cells);
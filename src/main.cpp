#include "Common.h"
#include "ProcessFile.h"
#include "CallMetis.h"
#include "OutputVTK.h"

int main()
{
    std::vector<Node> nodes;
    std::vector<int> cellstype;
    std::vector<Face> faces;
    std::vector<Cell> cells;
    std::vector<int> boundaryfaces;

    int numberofnodes = 0;
    int numberoffaces = 0;
    int boundaryface = 0;
    int interiorface = 0;
    int numberofcells = 0;
    int tricells = 0;
    int quadcells = 0;

    std::string filename = "hybridmesh.cas";
    //std::cout << "Now begin to reading mesh...\n";
    //std::cout << "Please enter the name of CAS file: ";
    //std::cin >> filename;

    ProcessFile(filename, nodes, cellstype, faces,
                numberofnodes, numberoffaces, boundaryface,
                interiorface, numberofcells, tricells, quadcells);

    cells.reserve(numberofcells);
    InputDataToCells(numberofcells, faces, cellstype, cells);
    
    std::vector<idx_t> part = CallMetis(numberofcells, numberofnodes, cells);

    for (int i = 0; i < numberofcells; ++i)
    {
        std::cout << "Cell " << i+1 << " belongs to partition " << part[i] << std::endl;
    }

    std::unordered_map<int, std::vector<Cell>> partitionedCells;

    for (int i = 0; i < part.size(); ++i)
    {
        partitionedCells[part[i]].emplace_back(cells[i]);
    }
    
    OutputVTK(nodes, partitionedCells);
}
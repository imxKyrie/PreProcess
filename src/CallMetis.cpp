#include "CallMetis.h"

std::vector<idx_t> CallMetis(const int numberofcells,
                             const int numberofnodes,
                             const std::vector<Cell>& cells)
{
    idx_t ne = numberofcells;
    idx_t nn = numberofnodes;
    idx_t nparts;
    idx_t ncommon = 2;
    idx_t objval;

    std::vector<idx_t> eptr, eind;
    std::vector<idx_t> epart(ne, 0);
    std::vector<idx_t> npart(nn, 0);

    std::cout << "Number of cells: " << ne << "\n";
    std::cout << "Calling METIS to partition the mesh...\n";
    std::cout << "Please enter the number of partitions(>=2): ";
    std::cin >> nparts;

    for (int i = 0; i < cells.size(); ++i)
    {
        eptr.push_back(eind.size());
        for (int j = 0; j < cells[i].nodes.size(); ++j)
        {
            idx_t a = cells[i].nodes[j] - 1;
            eind.push_back(a);
        }
    }
    eptr.push_back(eind.size());

    int status = METIS_PartMeshDual(&ne, &nn, eptr.data(), eind.data(), NULL, NULL, 
                                    &ncommon, &nparts, NULL, NULL, &objval, epart.data(), npart.data());
    if (status != METIS_OK)
    {
        std::cout << "METIS returned with error code " << status << std::endl;
        exit(1);
    }

    return epart;
}
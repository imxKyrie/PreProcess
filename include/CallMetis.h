#include "Common.h"
#include "metis.h"

std::vector<idx_t> CallMetis(const int numberofcells,
                             const int numberofnodes,
                             const std::vector<Cell>& cells);
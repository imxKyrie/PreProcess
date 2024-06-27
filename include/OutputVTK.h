#include <Common.h>
#include <vtkSmartPointer.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkQuad.h>
#include <vtkXMLMultiBlockDataWriter.h>
#include <vtkIntArray.h>
#include <vtkCellData.h>

void OutputVTK(const std::vector<Node>& nodes,
               const std::unordered_map<int, std::vector<Cell>> partitionedCells);
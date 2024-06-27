#include <OutputVTK.h>

void CreatePartitionedMesh(vtkSmartPointer<vtkUnstructuredGrid>& mesh,
                           const std::vector<Node>& nodes,
                           const std::vector<Cell>& cells,
                           const int& partitionId)
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkIntArray> partitionIds = vtkSmartPointer<vtkIntArray>::New();
    partitionIds->SetName("PartitionIds");

    // Create points
    std::unordered_map<int, int> globalToLocalNodeIds;
    int localId = 0;

    for (const auto& cell : cells)
    {
        for (const auto& node : cell.nodes)
        {
            if (globalToLocalNodeIds.find(node) == globalToLocalNodeIds.end())
            {
                globalToLocalNodeIds[node] = localId++;
                points->InsertNextPoint(nodes[node-1].x, nodes[node-1].y, 0.0);
            }
        }
    }

    mesh->SetPoints(points);

    // Create cells
    for (const auto& cell : cells)
    {
        if (cell.faces.size() == 3)
        {
            // Triangular
            vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
            triangle->GetPointIds()->SetId(0, globalToLocalNodeIds[cell.nodes[0]]);
            triangle->GetPointIds()->SetId(1, globalToLocalNodeIds[cell.nodes[1]]);
            triangle->GetPointIds()->SetId(2, globalToLocalNodeIds[cell.nodes[2]]);
            mesh->InsertNextCell(triangle->GetCellType(), triangle->GetPointIds());
            partitionIds->InsertNextValue(partitionId);
        }
        else if (cell.faces.size() == 4)
        {
            // Quadrilateral
            vtkSmartPointer<vtkQuad> quad = vtkSmartPointer<vtkQuad>::New();
            quad->GetPointIds()->SetId(0, globalToLocalNodeIds[cell.nodes[0]]);
            quad->GetPointIds()->SetId(1, globalToLocalNodeIds[cell.nodes[1]]);
            quad->GetPointIds()->SetId(2, globalToLocalNodeIds[cell.nodes[2]]);
            quad->GetPointIds()->SetId(3, globalToLocalNodeIds[cell.nodes[3]]);
            mesh->InsertNextCell(quad->GetCellType(), quad->GetPointIds());
            partitionIds->InsertNextValue(partitionId);
        }
    }

    mesh->GetCellData()->AddArray(partitionIds);
}

void OutputVTK(const std::vector<Node>& nodes,
               const std::unordered_map<int, std::vector<Cell>> partitionedCells)
{
    vtkSmartPointer<vtkMultiBlockDataSet> multiBlock = vtkSmartPointer<vtkMultiBlockDataSet>::New();
    multiBlock->SetNumberOfBlocks(partitionedCells.size());

    int blockId = 0;
    for (const auto& partition : partitionedCells)
    {
        vtkSmartPointer<vtkUnstructuredGrid> mesh = vtkSmartPointer<vtkUnstructuredGrid>::New();
        CreatePartitionedMesh(mesh, nodes, partition.second, partition.first);
        multiBlock->SetBlock(partition.first, mesh);
    }
    
    vtkSmartPointer<vtkXMLMultiBlockDataWriter> writer = vtkSmartPointer<vtkXMLMultiBlockDataWriter>::New();
    writer->SetFileName("partitioned_mesh.vtm");
    writer->SetInputData(multiBlock);
    writer->Write();
}

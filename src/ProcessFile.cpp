#include "ProcessFile.h"
#include <regex>

bool OpenFileWithPrompt(std::ifstream& file, std::string& filename)
{
    while (true)
    {
        file.open(filename);
        if (file.is_open()) return true;
        else
        {
            std::cerr << "Fail to open the file, please ensure that the file exists and the filename is correct!" << std::endl;
            std::cout << "Do you want to exit the program? (y/n): ";
            std::string choice;
            std::cin >> choice;
            if (choice == "y" || choice == "Y") return false;
            else
            {
                std::cout << "Please enter the filename again: ";
                std::cin >> filename;
            }
        }
    }
}

int JudgeFaceType(const std::string& line)
{
    std::regex re("\\d+");
    std::sregex_iterator it(line.begin(), line.end(), re);
    std::sregex_iterator end;
    std::vector<int> nums;
    while (it!= end)
    {
        nums.push_back(std::stoi(it->str()));
        ++it;
    }

    if (nums.size() >= 2) return nums[nums.size() - 2];
    else return -1;
}

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
                 int& quadcells)
{
    std::ifstream file;

    if (!OpenFileWithPrompt(file, filename))
    {
        std::cerr << "Exiting program." << std::endl;
        std::exit(1);
    }

    std::cout << "Successfully found the file, now start processing..." << std::endl;

    std::string line;
    bool captureNodes, captureCellstype, captureInteriorFaces, captureWallFaces,
         captureInletFaces, captureOutletFaces, captureSymmetryFaces;

    while (getline(file, line))
    {
        if (line.find("(0 \"Number of Nodes") != std::string::npos)
        {
            int start = line.find_first_of("0123456789", line.find("Number of Nodes"));
            if (start != std::string::npos)
            {
                int end = line.find_first_not_of("0123456789", start);
                std::string numberstr = line.substr(start, end - start);
                std::istringstream(numberstr) >> numberofnodes;
                nodes.reserve(numberofnodes);
                std::cout << "Number of Nodes: " << numberofnodes << std::endl;
            }
            continue;
        }

        if (line.find("(0 \"Total Number of Faces") != std::string::npos)
        {
            int start = line.find_first_of("0123456789", line.find("Total Number of Faces"));
            if (start != std::string::npos)
            {
                int end = line.find_first_not_of("0123456789", start);
                std::string numberstr = line.substr(start, end - start);
                std::istringstream(numberstr) >> numberoffaces;
                std::cout << "Number of Faces: " << numberoffaces <<std::endl;
            }
            continue;
        }

        if (line.find("Boundary Faces") != std::string::npos)
        {
            int start = line.find_first_of("0123456789", line.find("Boundary Faces"));
            if (start != std::string::npos)
            {
                int end = line.find_first_not_of("0123456789", start);
                std::string numberstr = line.substr(start, end - start);
                std::istringstream(numberstr) >> boundaryfaces;
                std::cout << "Boundary Faces: " << boundaryfaces <<std::endl;
            }
            continue;
        }

        if (line.find("Interior Faces") != std::string::npos)
        {
            int start = line.find_first_of("0123456789", line.find("Interior Faces"));
            if (start != std::string::npos)
            {
                int end = line.find_first_not_of("0123456789", start);
                std::string numberstr = line.substr(start, end - start);
                std::istringstream(numberstr) >> interiorfaces;
                std::cout << "Interior Faces: " << interiorfaces <<std::endl;
            }
            continue;
        }

        if (line.find("Total Number of Cells") != std::string::npos)
        {
            int start = line.find_first_of("0123456789", line.find("Total Number of Cells"));
            if (start != std::string::npos)
            {
                int end = line.find_first_not_of("0123456789", start);
                std::string numberstr = line.substr(start, end - start);
                std::istringstream(numberstr) >> numberofcells;
                cellstype.reserve(numberofcells);
                std::cout << "Number of Cells: " << numberofcells <<std::endl;
            }
            continue;
        }

        if (line.find("Tri cells") != std::string::npos)
        {
            int start = line.find_first_of("0123456789", line.find("Tri cells"));
            if (start != std::string::npos)
            {
                int end = line.find_first_not_of("0123456789", start);
                std::string numberstr = line.substr(start, end - start);
                std::istringstream(numberstr) >> tricells;
                std::cout << "Tri cells: " << tricells <<std::endl;
            }
            continue;
        }

        if (line.find("Quad cells") != std::string::npos)
        {
            int start = line.find_first_of("0123456789", line.find("Quad cells"));
            if (start != std::string::npos)
            {
                int end = line.find_first_not_of("0123456789", start);
                std::string numberstr = line.substr(start, end - start);
                std::istringstream(numberstr) >> quadcells;
                std::cout << "Quad cells: " << quadcells <<std::endl;
            }
            continue;
        }

        if (line.find("(10 (1 1") != std::string::npos)
        {
            captureNodes = true;
            continue;
        }

        if (captureNodes)
        {
            std::istringstream ss(line);
            int i = nodes.size() + 1;
            float x, y;
            while(ss >> x >> y) nodes.emplace_back(i, x, y);
            if (nodes.size() >= numberofnodes) captureNodes = false;
            continue;
        }

        if (line.find("(12 (2 1") !=std::string::npos)
        {
            captureCellstype = true;
            continue;
        }

        if (captureCellstype)
        {
            std::istringstream ss(line);
            int type;
            while (ss >> type) cellstype.emplace_back(type);
            if (cellstype.size() >= numberofcells) captureCellstype = false;
            continue;
        }

        if (line.find("(13 (") != std::string::npos && line.find("(13 (0") == std::string::npos)
        {
            if (JudgeFaceType(line) == 2)
            {
                captureInteriorFaces = true;
                captureWallFaces = false;
                captureInletFaces = false;
                captureOutletFaces = false;
                captureSymmetryFaces = false;
            }
            else if (JudgeFaceType(line) == 3)
            {
                captureInteriorFaces = false;
                captureWallFaces = true;
                captureInletFaces = false;
                captureOutletFaces = false;
                captureSymmetryFaces = false;
            }
            else if (JudgeFaceType(line) == 4)
            {
                captureInteriorFaces = false;
                captureWallFaces = false;
                captureInletFaces = true;
                captureOutletFaces = false;
                captureSymmetryFaces = false;
            }
            else if (JudgeFaceType(line) == 5)
            {
                captureInteriorFaces = false;
                captureWallFaces = false;
                captureInletFaces = false;
                captureOutletFaces = true;
                captureSymmetryFaces = false;
            }
            else if (JudgeFaceType(line) == 7)
            {
                captureInteriorFaces = false;
                captureWallFaces = false;
                captureInletFaces = false;
                captureOutletFaces = false;
                captureSymmetryFaces = true;
            }
            continue;
        }

        if (captureInteriorFaces)
        {
            std::istringstream ss(line);
            int i = faces.size() + 1;
            int num;
            std::string node1, node2, cell1, cell2;
            while (ss >> num >> node1 >> node2 >> cell1 >> cell2)
            {
                faces.emplace_back(i, num, 2, std::stoi(node1, nullptr, 16), std::stoi(node2, nullptr, 16), std::stoi(cell1, nullptr, 16), std::stoi(cell2, nullptr, 16));
            }
            if (faces.size() >= interiorfaces) captureInteriorFaces = false;
            continue;
        }

        if (captureWallFaces)
        {
            std::istringstream ss(line);
            int i = faces.size() + 1;
            int num;
            std::string node1, node2, cell1, cell2;
            while (ss >> num >> node1 >> node2 >> cell1 >> cell2)
            {
                faces.emplace_back(i, num, 3, std::stoi(node1, nullptr, 16), std::stoi(node2, nullptr, 16), std::stoi(cell1, nullptr, 16), std::stoi(cell2, nullptr, 16));
            }
            continue;
        }

        if (captureInletFaces)
        {
            std::istringstream ss(line);
            int i = faces.size() + 1;
            int num;
            std::string node1, node2, cell1, cell2;
            while (ss >> num >> node1 >> node2 >> cell1 >> cell2)
            {
                faces.emplace_back(i, num, 4, std::stoi(node1, nullptr, 16), std::stoi(node2, nullptr, 16), std::stoi(cell1, nullptr, 16), std::stoi(cell2, nullptr, 16));
            }
            continue;
        }

        if (captureOutletFaces)
        {
            std::istringstream ss(line);
            int i = faces.size() + 1;
            int num;
            std::string node1, node2, cell1, cell2;
            while (ss >> num >> node1 >> node2 >> cell1 >> cell2)
            {
                faces.emplace_back(i, num, 5, std::stoi(node1, nullptr, 16), std::stoi(node2, nullptr, 16), std::stoi(cell1, nullptr, 16), std::stoi(cell2, nullptr, 16));
            }
            continue;
        }

        if (captureSymmetryFaces)
        {
            std::istringstream ss(line);
            int i = faces.size() + 1;
            int num;
            std::string node1, node2, cell1, cell2;
            while (ss >> num >> node1 >> node2 >> cell1 >> cell2)
            {
                faces.emplace_back(i, num, 7, std::stoi(node1, nullptr, 16), std::stoi(node2, nullptr, 16), std::stoi(cell1, nullptr, 16), std::stoi(cell2, nullptr, 16));
            }
            continue;
        }
    }
    
    file.close();
}

void InputDataToCells(const int& numberofcells, const std::vector<Face>& faces, const std::vector<int>& cellstype, std::vector<Cell>& cells)
{
    std::unordered_map<int, std::vector<Face>> FacesofCells;

    cells.reserve(numberofcells);
    FacesofCells.reserve(numberofcells+1);
    
    for (const auto& face : faces)
    {
        for (const auto& cell : face.adjacentcells)
        {
            FacesofCells[cell].emplace_back(face);
        }
    }

    for (int i = 1; i < FacesofCells.size(); ++i)
    {
        std::unordered_set<int> uniqueNodesSet, visited;
        std::unordered_map<int, std::vector<int>> adjacentNodes;
        std::vector<int> uniqueNodes;
        std::vector<Face> uniqueFaces;
        int current;

        if (FacesofCells.at(i)[0].adjacentcells[0] == i) current = FacesofCells.at(i)[0].nodes[0];
        else current = FacesofCells.at(i)[0].nodes[1];

        for (const auto& face : FacesofCells.at(i))
        {
            uniqueNodesSet.insert(face.nodes[0]);
            uniqueNodesSet.insert(face.nodes[1]);
            adjacentNodes[face.nodes[0]].push_back(face.nodes[1]);
            adjacentNodes[face.nodes[1]].push_back(face.nodes[0]);
        }

        uniqueNodes.push_back(current);
        visited.insert(current);

        while (uniqueNodes.size() < uniqueNodesSet.size())
        {
            for (const int& neighbor : adjacentNodes[current])
            {
                if (visited.find(neighbor) == visited.end())
                {
                    visited.insert(neighbor);
                    uniqueNodes.push_back(neighbor);
                    current = neighbor;
                    break;
                }
            }
        }

        uniqueFaces.assign(FacesofCells[i].begin(), FacesofCells[i].end());
        cells.emplace_back(i, cellstype[i-1], uniqueNodes, uniqueFaces);
    }
}
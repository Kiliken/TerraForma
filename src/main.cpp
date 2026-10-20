#include <iostream>
#include <string>
#include "TerrainController.h"

int main(int argc, char* argv[]) {
    // Default values if no arguments are passed
    std::string filename = "test.obj";
    int seed = 0;
    int width = 256;
    int depth = 256;
    float scale = 1.0f;

    // Parse command-line arguments
    if (argc > 1) filename = argv[1];
    if (argc > 2) seed = std::stoi(argv[2]);
    if (argc > 3) width = std::stoi(argv[2]);
    if (argc > 4) depth = std::stoi(argv[3]);
    if (argc > 5) scale = std::stof(argv[4]);

    std::cout << "Generating terrain with parameters:\n";
    std::cout << " - Output OBJ: " << filename << "\n";
    std::cout << " - Seed: " << seed << "\n";
    std::cout << " - Width: " << width << "\n";
    std::cout << " - Depth: " << depth << "\n";
    std::cout << " - Scale: " << scale << "\n";

    TerrainController controller;
    controller.GenerateTerrainMesh(filename, seed, width, depth, scale);

    std::cout << "Terrain mesh and texture generated successfully!\n";

    return 0;
}
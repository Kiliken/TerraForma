
#include <glad/gl.h>

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include "TerrainController.h"

int main(int argc, char *argv[])
{
    // Default values if no arguments are passed
    std::string filename = "test.obj";
    int seed = 0;
    int width = 512;
    int depth = 512;
    float scale = 1.0f;

    if (argc > 1)
    {
        // Parse command-line arguments
        if (argc > 1)
            filename = argv[1];
        if (argc > 2)
            seed = std::stoi(argv[2]);
        if (argc > 3)
            width = std::stoi(argv[3]);
        if (argc > 4)
            depth = std::stoi(argv[4]);
        if (argc > 5)
            scale = std::stof(argv[5]);
    }
    else
    {
        std::ifstream f("config.json");
        if (f.is_open())
        {
            json data = json::parse(f);
            std::cout << "Loading Configs From config.json"<< std::endl;

            filename = data.value("name", "test.obj");
            seed = data.value("seed", 0);
            width = data.value("width", 512);
            depth = data.value("height", 512);
            scale = data.value("scale", 1);
        }
        else
        {
            // Handle error: file doesn't exist or couldn't be opened
        }
    }

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
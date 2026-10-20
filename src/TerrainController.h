
#ifndef __TERRAIN_CONTROLLER_H__
#define __TERRAIN_CONTROLLER_H__

#include <fstream>
#include <sstream>
#include <time.h>
#include "Noise.h"

class TerrainType
{
public:
    float minHeight;
    float maxHeight;
    std::array<int, 3> minColor;
    std::array<int, 3> maxColor;
    float lerpAdjustment;

    TerrainType(float minH, float maxH,
                std::array<int, 3> minC,
                std::array<int, 3> maxC,
                float lerpAdj = 0.0f)
        : minHeight(minH), maxHeight(maxH),
          minColor(minC), maxColor(maxC),
          lerpAdjustment(lerpAdj) {}
};

class TerrainController
{
public:
    TerrainController() = default;
    ~TerrainController() = default;

    void GenerateTerrainMesh(const std::string& filename, int width, int depth, float scale);

private:
    void saveBMP(const std::string& filename, int width, int height, const std::vector<std::vector<std::array<int, 3>>>& colorMap);

    std::vector<std::vector<float>> generateNoiseMap(
        int mapWidth, int mapHeight,
        Noise &noise, float scale,
        int octaves, float persistence,
        float lacunarity, float offY, float offX);
    std::array<int, 3> lerpColor(
        const std::array<int, 3> &colorA,
        const std::array<int, 3> &colorB,
        float t);
    std::array<int, 3> getTerrainColor(float noiseValue, const TerrainType &terrain);

    static std::vector<std::vector<float>> heightMap;
    static float terrainScale;
};

#endif // __TERRAIN_CONTROLLER_H__
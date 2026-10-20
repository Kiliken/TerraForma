#include <iostream>

#include "TerrainController.h"

int main(){

    TerrainController controller;
    controller.GenerateTerrainMesh("test.obj",1000,1000,1);

    return 0;
}
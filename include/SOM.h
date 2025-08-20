#ifndef SOM_H
#define SOM_H

#include <glm/glm.hpp>
#include <texture.h>
#include "ReadvoxelModel.h"

#define PI 3.1415926
using namespace std; 
enum latType{
    PLANE,
    CYLINDER,
    DONUT,
    BALL,
    THREED,
    COUNT
};

enum texType
{
    TWODTEX,
    THREEDTEX
};

// 3D
typedef struct LatData_t{

    glm::fvec3 ***lattice;

    int width;
    int height;
    int depth;

    int iter = 0;
    int max_iter = 120000;

    double learning_rate = 0.005;
    double learning_rate_now = 0.005;

    double radius = width / 2.0;
    double radius_now = width / 2.0;

    int type = 1; // 0: plane, 1: cylinder, 2: donut, 3: ball, 4: 3D lattice
    int typeNum[5] = {1, 1, 1, 6, 10};

    // control
    bool go = false;

}LatData_t;

typedef struct InputData_t{
    glm::fvec3 *input;
    // int dataExtraPoint[4];
    int num = 0;
}InputData_t;

typedef struct texData_t{

    glm::fvec3 *input;
    // int dataExtraPoint[4];
    int num = 0;
}texData_t;

class som_cls
{
public:
    som_cls();
    ~som_cls();

    // SOM for lattice
    LatData_t latticeData;
    InputData_t inputData;
    // SOM for voxel
    LatData_t v_latticeData;
    InputData_t v_inputData;
    // SOM for texture
    LatData_t t_latticeData;
    texData_t t_inputData;

    // function
    void create_SOM(std::vector<surfaceVoxel> voxelData, int voxelNum, glm::ivec3 max, glm::ivec3 min, int type);
    void destory_SOM();
    void renew_SOM(int *max, int lat_mode);
    void SOM_lattice();
    void destory_dataset();
    void destory_texture();
    void SOM_voxel(); // do texture mapping with specific layer
    void SOM_texture();
    void FreeTexLat();
    glm::fvec3 ***createTexLat();

private:

    glm::ivec3 latticeAnchorPoints[14];
    glm::ivec2 textureAnchorPoints[5];
    int idx[5];
    LatData_t init_SOM(LatData_t data);
    LatData_t init_texLat(LatData_t data);
    glm::fvec3 ***createLattice(LatData_t data, std::vector<surfaceVoxel> voxelData, int voxelNum, glm::ivec3 max, glm::ivec3 min);
    glm::fvec3 *createDataset(InputData_t data, std::vector<surfaceVoxel> voxelData, int voxelNum);
    glm::fvec3 *createTexture();
    const glm::fvec3 getInput(glm::fvec3 *dataset, int datasetNum);
    const glm::fvec3 getTexInput();

    glm::ivec3 computeNeiborhood(glm::ivec3 node, glm::ivec3 bmu);
    glm::ivec3 computeHalfballDist(glm::ivec3 p0);
    double compute_rate(int iter, int max_iter, double learning_rate);
    double compute_radius(int iter, int max_iter, double radius);
    double compute_influence(double radius, double squareDist);
    bool is_in_neighborhood(double squareDist, double radius);
    void update_node(glm::fvec3 nowInput, glm::ivec3 nodeId, double radius, double learning_rate);
    void update_texture(glm::fvec3 nowInput, glm::ivec2 nodeId, double radius, double learning_rate);
    void update_node_voxel(glm::fvec3 *dataset, glm::fvec3 nowInput, int bmuId, int nodeId, double radius, double learning_rate);
    void destory_lattice();
    bool isTextureAnchor(int w, int h, glm::ivec2 archorPoint[5]);
    bool isLatticeAnchor(int w, int h, int d, glm::ivec3 archorPoint[14]);
    glm::fvec3 findNearestVoxel(glm::fvec3 init_anchor, std::vector<surfaceVoxel> voxelData, int voxelNum);
};
extern som_cls SOM;
#endif
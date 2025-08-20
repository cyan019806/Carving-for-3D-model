#ifndef READVOXELMODEL_H
#define READVOXELMODEL_H

#include <glm/glm.hpp>
#include <string.h>
#include <vector>
#include <iostream>

#define MAX_DIST 1.0e10

enum voxelType {
    BOUNDARY,
    INNER,
    SURFACE,
};

struct myVector2D {
    int x, y;
};

struct myVector3D {
    float x, y, z;
};

typedef struct RawData_t
{
    double layer;
    bool air;
    glm::fvec3 color;
    bool draw;
} RawData_t;

struct inf {
    int resolution[3];
    char sample_type[15];
    float voxel_size[3];
    char endian[10];
    int type;            // 0:unsigned char, 1: float, 2: double
};

// bounder voxel (Done voxel)
struct bounderVoxel{
    glm::fvec3 locate = {0, 0, 0};
    glm::fvec3 color = {0, 0, 0};
    bool faceAir[6] = {true, true, true, true, true, true}; // back, front, left, right, bottom, up
};

// surface voxel
struct surfaceVoxel{
    glm::fvec3 locate = {0, 0, 0};
    glm::fvec3 color = {0, 0, 0};
    glm::fvec3 normal = {0, 0, 0};
    bool faceAir[6] = {true, true, true, true, true, true}; // back, front, left, right, bottom, up
    glm::fvec3 texcoord = {0.0, 0.0, 0.0};
    int textype = 0; // 3 texture
    bool draw = false;
};

// k layers voxel
struct LayerSurfaceData {

    // int choiceLayerNum = 1; // Max 8 layers
    // int initLayer = 1;
    int blockNum = 1; // Max 5 blocks
    std::vector<int> blockLocate; // Use vector for dynamic size
    std::vector<char> blockOrientation;

    std::vector<surfaceVoxel> surfaceVoxels; // Surface voxels for layers(layer > 0 && layer < initLayer + layerNum)
    std::vector<surfaceVoxel> oneSurfaceVoxels; //  Surface voxels for layers
    std::vector<surfaceVoxel> innerVoxels; // Inner voxels for layers (layer = initLayer + choiceLayerNum)
    std::vector<surfaceVoxel> boundaryVoxels; // Adjacent to one layer SurfaceVoxels (face adjacent, edge adjacent, point adjacent)
    std::vector<surfaceVoxel> boundaryVoxels2; // Adjacent to Surfacevoxels (face adjacent, edge adjacent, point adjacent)

    std::vector<std::vector<int>> surfaceVoxelNum; // 2D array as vector
    std::vector<std::vector<int>> oneSurfaceVoxelNum;   // 2D array as vector
    std::vector<std::vector<int>> innerVoxelNum;   // 2D array as vector
    std::vector<std::vector<int>> boundaryVoxelNum;   // 2D array as vector

    std::vector<glm::ivec3> maxsize; // Use vector to simplify memory management
    std::vector<glm::ivec3> minsize;
};


class rawModel_cls{
public:
    // func
    rawModel_cls();
    ~rawModel_cls();
    // void load_file(const char* infFileName, const char* rawFileName);
    void LoadFile(const char *infFileName, const char *rawFileName, const char *o_inf);
    bool isFaceAir(int x, int y, int z);
    void set_surface_voxel_face_air(int y, int x, int z, surfaceVoxel &surfaceVoxelData);
    void findSurfaceVoxel(int y, int x, int z, int num, int layer, int voxelType);
    void set_max_bounder(int i, int j, int k);
    glm::fvec3 findGradient(int x, int y, int z);
    // var
    inf infData;
    RawData_t*** rawData; // distance value of model (0: air, not 0: model) and show or not show
    double*** smoothData; // store the rawData after Laplacian smoothing to draw iso-surface

    double*** voxelData; // distance value of triangle model (0: air, not 0: model)
    double*** voxelData2; // distance value of triangle model (0: air, not 0: model) for copy in smoothing
    double*** voxelData3; // distance value of iso-surface (0: air, not 0: model)
    double*** weightData; // weight for smoothing
    int*** voteData; // inner or outer voxel at six direction
    // voxel index range
    myVector3D max = {0, 0, 0};
    myVector3D min = {0, 0, 0};
    // voxel center
    myVector3D center = {0, 0, 0};
    // distance
    double minDist = 999999;
    double maxDist = 0;
    int centerIdx = 0; // index

    int choiceLayerNum = 5; // max: 8
    int initLayer = 1;

    int minLayer = 1;
    int maxLayer = 1;

    std::vector<LayerSurfaceData> layerSurfaces; // Supports multiple layers
    std::vector<int> layerVoxelNum; // Dynamic size

    unsigned char* uc_voxelData;
    // unsigned char* new_uc_voxelData;
    float* f_voxelData;
    double* d_voxelData;

private:
    // bool load_inf_file(const char* infFileName);
    bool LoadInfFile(const char* infFileName, const char* o_inf);
    // bool load_raw_file(const char* rawFileName);
    bool LoadRawFile(const char* rawFileName);
    // void create_voxel();
    void CreateRawData();
    bool set_sample_type(const char* type);
    bool read_raw_file(FILE* file);
    bool ReadRawFile(FILE *file);
    void set_voxel_data();
    void set_raw_data();
    void check_computer_endian();
    void findNeighbor(int x, int y, int z);
    bool is_surface_voxel(int x, int y, int z);
    void laplacianSmoothing(int times);


};
extern rawModel_cls rawModel;
#endif
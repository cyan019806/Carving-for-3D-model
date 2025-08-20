#ifndef TWO_STEP_MAPPING
#define TWO_STEP_MAPPING
#include "SOM.h"
#include "ReadVoxelModel.h"
#include "texture.h"

enum texFilter{
    MINIFICATION,
    MAGNIFICATON
};
enum latFilter{
    LINEAR,
    NEAREST
};

class Mapping{
public:
    Mapping();
    ~Mapping();
    bool latticeTextureFilter = LINEAR;
    bool latticeVoxelFilter = LINEAR;
    int texfilter = MINIFICATION;
    bool map = false;
    // set carving range
    // // 花瓶
    // float y_max = 320.0f;
    // float y_min = 50.0f;
    // float z_max = 205.0f;
    // float z_min = 0.0f;
    // float x_max = 205.0f;
    // float x_min = 0.0f;
    // // 兔子
    // float y_max = 120.0f;
    // float y_min = 60.0f;
    // float z_max = 192.0f;
    // float z_min = 70.0f;
    // float x_max = 157.0f;
    // float x_min = 0.0f;
    // 茶壺
    float y_max = 145.0f;
    float y_min = 10.0f;
    float z_max = 290.0f;
    float z_min = 90.0f;
    float x_max = 217.0f;
    float x_min = 0.0f;
    // // 茶壺(蓋子)
    // float y_max = 160.0f;
    // float y_min = 145.0f;
    // float z_max = 280.0f;
    // float z_min = 80.0f;
    // float x_max = 217.0f;
    // float x_min = 0.0f;
    void textureMapping(bool* checkFlags);
    // void unitVoxelTextureMapping(int layern);
    void unitVoxelTextureMapping(std::vector<surfaceVoxel>& voxel);
    void computeTexel(int layern, int centerIdx);
    glm::fvec3 latticeCoord_to_textureCoord_2D(glm::fvec2 latticeCoord);

private:
    bool tri = false;
    glm::vec3 findBMU(double v_x, double v_y, double v_z, double *minDist);
    float dir[5][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 0}};
    float innerProduct(glm::vec3 a, glm::vec3 b);
    glm::fvec3 crossProduct(glm::vec3 a, glm::vec3 b);
    bool inTriangle(glm::fvec3 bmu, glm::fvec3 a, glm::fvec3 b, glm::fvec3 projp, glm::fvec3 n);
    glm::vec3 outerProjp(glm::vec3 bmu, glm::vec3 a, glm::vec3 b, glm::vec3 projp, glm::vec3 p);
    glm::fvec3 point_to_triangle(glm::fvec3 o, glm::fvec3 a, glm::fvec3 b, glm::fvec3 projp, double *minDist);
    glm::fvec3 barycentric_coord(glm::fvec3 o, glm::fvec3 a, glm::fvec3 b, glm::fvec3 projp, glm::fvec3 vector_n);
    glm::fvec3 computeTexel(glm::vec2 minPindex);
    glm::fvec3 magFilter(glm::fvec2 imageRate1, int texType);
    glm::fvec3 minFilter(glm::fvec2 imageRate0, glm::fvec2 imageRate2, int textType);
    glm::fvec2 surroundTriangles(double min, glm::fvec3 voxelPos, glm::ivec3 minLatticeCoord);
};
extern Mapping mapping;

#endif
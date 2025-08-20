#ifndef TEXTURE_H
#define TEXTURE_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "SOM.h"
#include "MatrixStack.h"
#include "twoStepMapping.h"
#include <iostream>

enum wrapType{
    REPEAT,
    BORDER
};

typedef struct textureMap_t
{
    int angle_hw = 0;
    int angle_dh = 0;
    int angle_dw = 0;
    glm::fvec2 translate = {0.0, 0.0};
    glm::fvec2 scale = {0.0, 0.0};
    glm::fvec2 resolution_w = {0.0, 1.0};
    glm::fvec2 resolution_h = {0.0, 1.0};
    glm::fvec2 resolution_d = {0.0, 1.0};
    int wrapType = 0;
    MatrixStack texture_m;
}textureMap_t;

typedef struct image_Tex_t
{
    int width, height;
    // glm::ivec3** image;
    glm::fvec3 **image;
    unsigned int texture;
    unsigned char *data;
    bool tri;    // if in triangle
}image_Tex_t;

typedef struct shadowTex_t{
    const unsigned int width = 1024, height = 1024;
    unsigned int depthCubemap;
    unsigned int depthFBO;
}shadowTex_t;

typedef struct intensityTex_t{
    int x,y,z;
    unsigned int texture3D;
    float *data;
}intensityTex_t;

typedef struct colormapTex_t
{
    int color = 255;
    unsigned int texture1D;
    float *data;
}colormapTex_t;

typedef struct threeDTex_t{
    int width = 1024, height = 1024, depth = 10;
    unsigned int texture;
    float *data;
}threeDTex_t;

class texture_cls
{
public:
    texture_cls();
    ~texture_cls();
    void create_texture();
    void bindTexture(int bind);
    void updataColorMap(std::vector<float> newdata);
    void updateIntensityMap();
    glm::fvec3 compute_voxel_texture(glm::fvec4 texCoord);
    glm::fvec3 lattice_to_texture(glm::fvec4 texCoord, int degree, int resolution);
    void free_image_memory();

    int texTypeNum = 3;
    image_Tex_t imageTex[3];
    threeDTex_t threeDTex;
    shadowTex_t shadowTex;
    intensityTex_t intensityTex;
    colormapTex_t colormapTex;

    textureMap_t texMatrix2D;
    textureMap_t texMatrix3D;

    image_Tex_t img;

    int num;
private:
    void create_img_tex();
    void create_3D_tex();
    void create_depth_tex();
    void create_ray_tex();
    const char* texName[3] = {"texture/vase_pattern34.png", "texture/pattern.png", "texture/wwb.png"};
    // const char* texName[3] = {"texture/pattern.png", "texture/pattern.png", "texture/wwb.png"};
    void texture1Dto2D(int nrChannels, int t);
};
extern texture_cls tex;

#endif
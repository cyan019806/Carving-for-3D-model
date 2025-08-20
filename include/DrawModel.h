#ifndef DRAWMODEL_H
#define DRAWMODEL_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <algorithm>

#include "MatrixStack.h"
#include "background.h"
#include "Object.h"
#include "Camera.h"
#include "Shader.h"
#include "Math.h"



class draw_model_cls
{
public:
    draw_model_cls();
    ~draw_model_cls();

    void Shader_Create();
    void Shader_Use(GLFWwindow *window);

    void Lattice_renew();
    void Center_Voxel_renew();
    void Surface_renew();
    void InnerSurface_renew();
    void IsoSurface_renew();
    void Layer_Voxel_renew();
    void Inner_Voxel_renew();
    void Shader_init(int n, bool settex);
    void depthShader_model(GLFWwindow *window);
    bool is_inside(int x, int y, int z);

    Camera camera;
    glm::vec3 lightPos;
    glm::vec3 modelsize = {0.4, 0.4, 0.4};
    int SCR_WIDTH = 800, SCR_HEIGHT = 600;
    float rate;

    bool preciousShowVoxel[7] = {true, true, true, true, true, true, true};
    bool showVoxel[7] = {true, true, true, true, true, true, true};

private:

    void Modify_position(int x, int y, int z);
    void ViewProjection_Create(int n);
    // voxel model
    void model_voxel_create(Shader shader);
    void model_innervoxel_create(Shader shader);
    // triangle model
    void model_surface_create(Shader shader);
    void model_innerSurface_create(Shader shader);
    // iso-surface
    void model_IsoSurface_create(Shader shader);

    void model_floor_create(Shader shader);
    void model_lattice_create(Shader shader);
    void model_bound_create(Shader shader);
    void model_axis_create(Shader shader);
    void model_lightCube_create(Shader shader);

    void rayShader_model(GLFWwindow *window);
    void shader_model();
    glm::fvec3 vector_matrix(glm::mat4 matrix, glm::fvec4 vec);

    MatrixStack model;
    MatrixStack view;
    MatrixStack projection;
    std::vector<glm::mat4> shadowTransforms;

    Shader rayShader;
    Shader shader;
    Shader depthShader;

    Object Boundingbox;
    // Object Axis;
    Object P_axis;
    Object Lightcube;
    Object Floor;
    // voxel model
    Object Voxel;
    Object InnerVoxel;
    // triangle model
    std::vector<Object> Surface;
    Object InnerSurface;
    // iso-surface
    Object IsoSurface;
    Object Tex;
    Object Lattice;
    Object Plane;

    float near_plane = 0.01f, far_plane = 10000000.0f;
};

extern draw_model_cls drawModel;

extern bool startSOM;
extern void createLatThread();
extern void createTexThread();

#endif
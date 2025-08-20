#include "gui.h"

// int start = 0;
// int mode = -1;

// distance value
// int showDist = 0;

bool *previousFlags = nullptr; // store the precious state of Checkbox
bool *checkFlags = nullptr; // store the state of Checkbox
int lastIndex = 0;          // use to sure the num of Checkbox

int first = 0; // if the first time click the checkbox

// carving
bool emboss = false;
bool engrave = false;

int maxIndex = 0; // record the most outter surface layer
int minIndex = 0;// record the most inner surface layer

// // texture
// // wrap type
// int old_wrap_type = 0;
// // range
// float old_left_up[2] = {0.0f, 1.0f};
// float left_up[2] = {0.0f, 1.0f};
// float old_right_down[2] = {0.0f, 1.0f};
// float right_down[2] = {0.0f, 1.0f};
// glm::fvec2 old_res_w = {0.0f, 1.0f},  old_res_h = {0.0f, 1.0f};
// // translate
// float old_translate[3] = {0.0f, 0.0f, 0.0f};
// float translate[3] = {0.0f, 0.0f, 0.0f};

// // scale
// float old_scale[2] = {1.0f, 1.0f};
// float scale[2] = {1.0f, 1.0f};

// // angle
// int old_tex_angle[3] = {0, 0, 0};
// int tex_angle[3] = {0, 0, 0};

// time
double current_time = 0.0, mapping_time = 0.0;

void imgui_func();

void imgui_init(GLFWwindow *window)
{
    // ImGui setting
    const char *glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();
    // Setup Platform/ Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void imgui_create(GLFWwindow* window)
{
    // imGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    // train position(generate model surface)
    ImGui::Begin("SOM for lattice");
    ImGui::Text("lattice");
    static int lat_elem = PLANE;
    const char *lat_elems_names[COUNT] = {"plane", "cylinder", "donut", "ball", "threed"};
    const char *lat_elem_name = (lat_elem >= 0 && lat_elem < COUNT) ? lat_elems_names[lat_elem] : "Unknown";
    ImGui::SliderInt("lattice", &lat_elem, 0, COUNT - 1, lat_elem_name);
    if (ImGui::Button("load"))
    {
        int idx = rawModel.centerIdx;
        SOM.create_SOM(rawModel.layerSurfaces[idx].surfaceVoxels, rawModel.layerSurfaces[idx].surfaceVoxels.size(), glm::ivec3(rawModel.infData.resolution[1], rawModel.infData.resolution[2], rawModel.infData.resolution[0]), glm::ivec3(0, 0, 0), lat_elem);
        drawModel.Lattice_renew();
    }
    ImGui::Text("iter: %d", SOM.latticeData.iter);
    ImGui::Text("radius: %f", SOM.latticeData.radius_now);
    ImGui::Text("learning_rate: %f", SOM.latticeData.learning_rate_now);
    if (ImGui::Button("Start"))
    {
        SOM.latticeData.go = true;
        createLatThread();
    }
    if (ImGui::Button("Stop"))
    {
        if (SOM.latticeData.go == true)
            SOM.latticeData.go = false;
        else
            SOM.latticeData.go = true;
    }
    ImGui::End();
    // train texture coordinate(uv-parameterization)
    ImGui::Begin("SOM for texture");
    ImGui::Text("texture");
    ImGui::Text("iter: %d", SOM.t_latticeData.iter);
    ImGui::Text("radius: %f", SOM.t_latticeData.radius_now);
    ImGui::Text("learning_rate: %f", SOM.t_latticeData.learning_rate_now);
    if (ImGui::Button("Start"))
    {
        SOM.t_latticeData.go = true;
        createTexThread();
    }
    if (ImGui::Button("Stop"))
    {
        if (SOM.t_latticeData.go == true)
            SOM.t_latticeData.go = false;
        else
            SOM.t_latticeData.go = true;
    }
    // lattice to texture filter(usually use linear)
    static int lt_filnum = 0;
    const char *lt_fil_types[2] = {"linear", "nearset"};
    const char *lt_fil_name = (lt_filnum >= 0 && lt_filnum < 2) ? lt_fil_types[lt_filnum] : "Unknown";
    ImGui::SliderInt("filter (texture to lattice)", &lt_filnum, 0, 1, lt_fil_name);
    if(lt_filnum == LINEAR)
        mapping.latticeTextureFilter = LINEAR;
    else
        mapping.latticeTextureFilter = NEAREST;

    // texture wrapping
    ImGui::Text("texture wrap");
    ImGui::RadioButton("repeat", &tex.texMatrix2D.wrapType, 0);
    ImGui::SameLine();
    ImGui::RadioButton("border", &tex.texMatrix2D.wrapType, 1);

    // lattice to voxel filter(usually use linear)
    static int lv_filnum = 0;
    const char *lv_fil_types[2] = {"linear", "nearset"};
    const char *lv_fil_name = (lv_filnum >= 0 && lv_filnum < 2) ? lv_fil_types[lv_filnum] : "Unknown";
    ImGui::SliderInt("filter (voxel to lattice)", &lv_filnum, 0, 1, lv_fil_name);
    if(lv_filnum == LINEAR)
        mapping.latticeVoxelFilter = LINEAR;
    else
        mapping.latticeVoxelFilter = NEAREST;

    // after SOM training
    if (ImGui::Button("texture Mapping"))
    {
        carving.tm = true;
        current_time = glfwGetTime();
        mapping.textureMapping(checkFlags);
        mapping_time = glfwGetTime() - current_time;
        cout << "texture mapping time: " << mapping_time << " sec" << endl;
        // clear surface voxel before
        for(int l = 0; l < rawModel.choiceLayerNum; l++)
        {
            background.layerSurfaceBuffers[l].clear();
        }
        background.layerSurfaceBuffers.clear();
        drawModel.Layer_Voxel_renew();
        drawModel.IsoSurface_renew();
    }
    if(ImGui::Button("embossing"))
    {
        carving.emboss = true;
        for(int l = 0; l < rawModel.choiceLayerNum; l++)
        {
            background.layerSurfaceBuffers[l].clear();
        }
        background.layerSurfaceBuffers.clear();
        drawModel.Layer_Voxel_renew();
        drawModel.IsoSurface_renew();
    }
    ImGui::SameLine();
    if(ImGui::Button("engraving"))
    {
        carving.engrave = true;
        for(int l = 0; l < rawModel.choiceLayerNum; l++)
        {
            background.layerSurfaceBuffers[l].clear();
        }
        background.layerSurfaceBuffers.clear();
        drawModel.Layer_Voxel_renew();
        drawModel.IsoSurface_renew();
    }
    // show or not show surface voxel model
    std::string label = "surface voxel";
    if(ImGui::Checkbox(label.c_str(), &drawModel.showVoxel[0]))
    {
        if (drawModel.showVoxel[0] && !drawModel.preciousShowVoxel[0])
        {
            drawModel.showVoxel[0] = true;
        }
        else if (!drawModel.showVoxel[0] && drawModel.preciousShowVoxel[0])
        {
            drawModel.showVoxel[0] = false;
        }
        // update previousFlags
        memcpy(drawModel.preciousShowVoxel, drawModel.showVoxel, 7 * sizeof(bool));
    }
    ImGui::SameLine();
    // show or not show inner voxel model
    label = "inner voxel";
    if(ImGui::Checkbox(label.c_str(), &drawModel.showVoxel[1]))
    {
        if (drawModel.showVoxel[1] && !drawModel.preciousShowVoxel[1])
        {
            drawModel.showVoxel[1] = true;
        }
        else if (!drawModel.showVoxel[1] && drawModel.preciousShowVoxel[1])
        {
            drawModel.showVoxel[1] = false;
            background.innerVoxel.clear();
        }
        drawModel.Inner_Voxel_renew();
        // update previousFlags
        memcpy(drawModel.preciousShowVoxel, drawModel.showVoxel, 7 * sizeof(bool));
    }
    // show or not show traingle model
    label = "surface";
    if(ImGui::Checkbox(label.c_str(), &drawModel.showVoxel[2]))
    {
        if (drawModel.showVoxel[2] && !drawModel.preciousShowVoxel[2])
        {
            drawModel.showVoxel[2] = true;
        }
        else if (!drawModel.showVoxel[2] && drawModel.preciousShowVoxel[2])
        {
            drawModel.showVoxel[2] = false;
        }
        drawModel.Surface_renew();
        // update previousFlags
        memcpy(drawModel.preciousShowVoxel, drawModel.showVoxel, 7 * sizeof(bool));
    }
    ImGui::SameLine();
    // show or not show inner traingle model
    label = "inner surface";
    if(ImGui::Checkbox(label.c_str(), &drawModel.showVoxel[5]))
    {
        if (drawModel.showVoxel[5] && !drawModel.preciousShowVoxel[5])
        {
            drawModel.showVoxel[5] = true;
        }
        else if (!drawModel.showVoxel[5] && drawModel.preciousShowVoxel[5])
        {
            drawModel.showVoxel[5] = false;
        }
        drawModel.InnerSurface_renew();
        // update previousFlags
        memcpy(drawModel.preciousShowVoxel, drawModel.showVoxel, 7 * sizeof(bool));
    }
    // show or not show SOM lattice(Lines)
    label = "lattice";
    if(ImGui::Checkbox(label.c_str(), &drawModel.showVoxel[3]))
    {
        if (drawModel.showVoxel[3] && !drawModel.preciousShowVoxel[3])
        {
            drawModel.showVoxel[3] = true;
        }
        else if (!drawModel.showVoxel[3] && drawModel.preciousShowVoxel[3])
        {
            drawModel.showVoxel[3] = false;
        }
        // update previousFlags
        memcpy(drawModel.preciousShowVoxel, drawModel.showVoxel, 7 * sizeof(bool));
    }
    ImGui::SameLine();
    // show or not show SOM lattice(triangles)
    label = "plane";
    if(ImGui::Checkbox(label.c_str(), &drawModel.showVoxel[4]))
    {
        if (drawModel.showVoxel[4] && !drawModel.preciousShowVoxel[4])
        {
            drawModel.showVoxel[4] = true;
        }
        else if (!drawModel.showVoxel[4] && drawModel.preciousShowVoxel[4])
        {
            drawModel.showVoxel[4] = false;
        }
        // update previousFlags
        memcpy(drawModel.preciousShowVoxel, drawModel.showVoxel, 7 * sizeof(bool));
    }
    // show or not show iso-surface
    label = "iso-surface";
    if(ImGui::Checkbox(label.c_str(), &drawModel.showVoxel[6]))
    {
        if (drawModel.showVoxel[6] && !drawModel.preciousShowVoxel[6])
        {
            drawModel.showVoxel[6] = true;
        }
        else if (!drawModel.showVoxel[6] && drawModel.preciousShowVoxel[6])
        {
            drawModel.showVoxel[6] = false;
        }
        drawModel.IsoSurface_renew();
        // update previousFlags
        memcpy(drawModel.preciousShowVoxel, drawModel.showVoxel, 7 * sizeof(bool));
    }
    ImGui::End();
    imgui_func();
}

void imgui_func()
{
    // draw surface model
    // drawModel.Center_Voxel_renew();
}
void imgui_end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
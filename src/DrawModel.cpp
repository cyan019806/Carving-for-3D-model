#include "DrawModel.h"

draw_model_cls drawModel;

Shader shader;
Shader rayShader;
Shader depthShader;

glm::ivec2 left_up_point = {0, 0};
int texture_width = 200;
int texture_height = 200;

thread t1, t2;
double current_time_pos = 0.0, current_time_tex = 0.0, som_training_pos_time = 0.0, som_training_tex_time = 0.0;

float rotateAngle = 250.0f;

draw_model_cls::draw_model_cls()
{   // set light pos
    // glm::vec3 lightPos = glm::vec3(80, 50, 80); // vase, bunny
    glm::vec3 lightPos = glm::vec3(30, 100, -30); // teapot
    // set camera pos
    Camera camera(glm::vec3(rawModel.infData.resolution[1] / 2, rawModel.infData.resolution[2] / 2, rawModel.infData.resolution[0] / 2), glm::vec3(0.0f, 1.0f, 0.0f));
}

draw_model_cls::~draw_model_cls()
{
    destroyBackground();
    Boundingbox.Object_del();
    Floor.Object_del();
    Lightcube.Object_del();
    Voxel.Object_del();
    InnerVoxel.Object_del();
    for(int l = 0; l < rawModel.choiceLayerNum; l++)
    {
        Surface[l].Object_del();
    }
    InnerSurface.Object_del();
    Lattice.Object_del();
    Plane.Object_del();
}

void draw_model_cls::Shader_Create()
{
	// read raw model
	// rawModel.LoadFile("raw/input/vase_dist.inf", "raw/input/vase_df.raw", "raw/output/vase.inf"); // 花瓶
	// rawModel.LoadFile("raw/input/bunny_dist.inf", "raw/input/bunny_df.raw", "raw/output/bunny.inf"); // 兔子
	rawModel.LoadFile("raw/input/teapot_dist.inf", "raw/input/teapot_df.raw", "raw/output/teapot.inf"); // 茶壺
    // create background(bounding box, floor)
	createBackground();
    // create voxel model
    renew_center_voxel(rawModel.layerSurfaces, rawModel.layerSurfaces.size(), background.voxel, background.innerVoxel);
    // create SOM lattice (to filter surface)
    int idx = rawModel.centerIdx;
    SOM.create_SOM(rawModel.layerSurfaces[idx].surfaceVoxels, rawModel.layerSurfaces[idx].surfaceVoxels.size(), glm::ivec3(rawModel.infData.resolution[1], rawModel.infData.resolution[2], rawModel.infData.resolution[0]), glm::ivec3(0, 0, 0), PLANE);
    create_lattice();
    // endLayer: the inner layer
    int endLayer = rawModel.initLayer + rawModel.choiceLayerNum;
    background.layerSurfaceBuffers.resize(rawModel.choiceLayerNum);
    // create triangle model
    isoSurface.CreateTriangleModel(1, carving.emboss, carving.engrave, background.layerSurfaceBuffers[0]);
    // create iso-surface
    isoSurface.CreateIsoSurface3(idx, endLayer);
    drawModel.Surface_renew();
    // create bounding box
    // create_bounding_box();
    // Boundingbox.renewVBO(background.boundingbox);
    // shader
    rayShader = Shader("shader/rayShader.vs", "shader/rayShader.fs");
    shader = Shader("shader/shader.vs", "shader/shader.fs");
    depthShader = Shader("shader/depthShader.vs", "shader/depthShader.fs", "shader/depthShader.gs");
    // modify the position of camera
    Modify_position(rawModel.infData.resolution[0], rawModel.infData.resolution[1], rawModel.infData.resolution[2]);
    std::cout <<  camera.Position.x << ", " <<  camera.Position.y << ", " <<  camera.Position.z << std::endl;
    // create Object
    Boundingbox = Object(background.boundingbox);
    Floor = Object((background.floor));
    Lightcube = Object(background.boundingbox);
    Voxel = Object(background.voxel);
    InnerVoxel = Object(background.innerVoxel);
    InnerSurface = Object(background.innerSurface);
    IsoSurface = Object(background.isoSurface);
    Surface.resize(rawModel.choiceLayerNum);
    for (int l = 0; l < rawModel.choiceLayerNum; l++)
    {
        Surface[l] = Object(background.layerSurfaceBuffers[l]);
    }
    P_axis = Object(background.p_axis);
    Lattice = Object(background.lattice_line);
    Plane = Object(background.lattice_plane);
    // shader initialize
	Shader_init(0, true);
	Shader_init(1, true);
}
// move the camera and light pos
void draw_model_cls::Modify_position(int x, int y, int z){
    // modify camera
    // rate = std::max(std::max(x, y), z) / 2.0;
    rate = 5.0;
    camera.modifyPositionAndmoveSpeed(glm::vec3(x * 0.6, y / 2.0, z * 0.8), rate);
    // modify light
    // lightPos = glm::vec3(x, y, z); // vase, bunny
    lightPos = glm::vec3(0.5*x, y, 2.0*z); // teapot
}

void draw_model_cls::Shader_init(int n, bool settex) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if(n == 0){
        rayShader.use();
        if(settex){
            rayShader.setInt("shadowMap", 3);
            rayShader.setInt("intensityMap", 4);
            rayShader.setInt("colorMap", 5);
            rayShader.setFloat("bias", 1.0);
        }
    }else if(n == 1){
        shader.use();
        if(settex){
            shader.setInt("texturemap0", 0);
            shader.setInt("texturemap1", 1);
            shader.setInt("texturemap2", 2);
            shader.setInt("texturemap3D", 6);
        }
    }else if(n == 2){
        depthShader.use();
        if(settex){
            for (unsigned int i = 0; i < 6; ++i)
                depthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
            depthShader.setFloat("far_plane", far_plane);
            depthShader.setVec3("lightPos", lightPos);
        }
    }
}

void runthreadSomIter()
{
    current_time_pos = glfwGetTime();
    SOM.latticeData.iter = 0;
	while (SOM.latticeData.iter < SOM.latticeData.max_iter && SOM.latticeData.go == true)
	{
		SOM.SOM_lattice();
	}
    som_training_pos_time = glfwGetTime() - current_time_pos;
    cout << "pos training time: " << som_training_pos_time <<  " sec" << endl;
}

void createLatThread()
{
	if (t1.joinable())
	{
		t1.join();
	}
	t1 = thread(runthreadSomIter);
}

void runthreadSomTex()
{
    current_time_tex = glfwGetTime();
	while (SOM.t_latticeData.iter < SOM.t_latticeData.max_iter && SOM.t_latticeData.go == true)
	{
		SOM.SOM_texture();
	}
    som_training_tex_time = glfwGetTime() - current_time_tex;
    cout << "tex training time: " << som_training_tex_time <<  " sec" << endl;
}

void createTexThread()
{
	if (t2.joinable())
	{
		t2.join();
	}
	t2 = thread(runthreadSomTex);
}

void draw_model_cls::Lattice_renew()
{
    create_lattice();
    Lattice.renewVBO(background.lattice_line);
    Plane.renewVBO(background.lattice_plane);
}

void draw_model_cls::Center_Voxel_renew()
{
    renew_center_voxel(rawModel.layerSurfaces, rawModel.layerSurfaces.size(), background.voxel, background.innerVoxel);
    Voxel.renewVBO(background.voxel);
}

void draw_model_cls::Layer_Voxel_renew()
{
    renew_layer_voxel(rawModel.layerSurfaces, rawModel.layerSurfaces.size());
    Voxel.renewVBO(background.voxel);
}

void draw_model_cls::Inner_Voxel_renew()
{
    renew_inner_voxel(rawModel.layerSurfaces, rawModel.layerSurfaces.size());
    InnerVoxel.renewVBO(background.innerVoxel);
}

void draw_model_cls::Surface_renew()
{
    Surface.resize(rawModel.choiceLayerNum);
    for(int l = 0; l < rawModel.choiceLayerNum; l++)
    {
        if(background.layerSurfaceBuffers[l].size())
            Surface[l].renewVBO(background.layerSurfaceBuffers[l]);
    }
}

void draw_model_cls::InnerSurface_renew()
{
    InnerSurface.renewVBO(background.innerSurface);
}

void draw_model_cls::IsoSurface_renew()
{
    IsoSurface.renewVBO(background.surface2);
}

void draw_model_cls::Shader_Use(GLFWwindow *window)
{
    if(SOM.latticeData.iter < SOM.latticeData.max_iter && SOM.latticeData.go)
    {
        SOM.SOM_lattice();
        create_lattice();
        Lattice.renewVBO(background.lattice_line);
        Plane.renewVBO(background.lattice_plane);
    }
    if(SOM.t_latticeData.iter < SOM.t_latticeData.max_iter && SOM.t_latticeData.go)
    {
        SOM.SOM_texture();
        create_lattice();
        Lattice.renewVBO(background.lattice_line);
        Plane.renewVBO(background.lattice_plane);
    }
    //depth shader
    ViewProjection_Create(2);
    Shader_init(2, true);
    depthShader_model(window);
    //our shader
    Shader_init(0, false);
    ViewProjection_Create(0);
    rayShader_model(window);
    //light shader
    Shader_init(1, false);
    ViewProjection_Create(1);
    shader_model();
}

void draw_model_cls::ViewProjection_Create(int n) {
    if(n == 0){
        view.Save(camera.GetViewMatrix());
        // projection.Save(glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, near_plane, far_plane));
        projection.Save(glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, near_plane, far_plane));
        rayShader.setMat4("view", view.Top());
        rayShader.setMat4("projection", projection.Top());
        rayShader.setVec3("viewPos", camera.Position);
        rayShader.setVec3("lightPos", lightPos);

        rayShader.setFloat("far_plane", far_plane);
    }else if(n == 1){
        shader.setMat4("view", view.Top());
        shader.setMat4("projection", projection.Top());
    }else if(n == 2){
        MatrixStack shadowProj;
        shadowProj.Save(glm::perspective(glm::radians(90.0f), (float)tex.shadowTex.width / (float)tex.shadowTex.height, near_plane, far_plane));

        shadowTransforms.clear();
        shadowTransforms.push_back(shadowProj.Top() * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj.Top() * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj.Top() * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
        shadowTransforms.push_back(shadowProj.Top() * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj.Top() * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj.Top() * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    }
}

void draw_model_cls::rayShader_model(GLFWwindow *window){
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    tex.bindTexture(3); //depth texture
    tex.bindTexture(4);
    tex.bindTexture(5);
    if(showVoxel[0]) model_voxel_create(rayShader);
    if(showVoxel[1]) model_innervoxel_create(rayShader);
    if(showVoxel[2]) model_surface_create(rayShader);
    if(showVoxel[5]) model_innerSurface_create(rayShader);
    if(showVoxel[6]) model_IsoSurface_create(rayShader);
    // rotateAngle += 0.2f;
    model_floor_create(rayShader);
}

void draw_model_cls::shader_model(){
    // Model_axis_create(shader);
    model_lattice_create(shader);
    model_lightCube_create(shader);
    model_bound_create(shader);
}
void draw_model_cls::depthShader_model(GLFWwindow *window){
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, tex.shadowTex.width, tex.shadowTex.height);
    glBindFramebuffer(GL_FRAMEBUFFER, tex.shadowTex.depthFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
        if(showVoxel[0])
            model_voxel_create(depthShader);
        if(showVoxel[1])
            model_innervoxel_create(depthShader);
        if(showVoxel[2])
            model_surface_create(depthShader);
        if(showVoxel[5])
            model_innerSurface_create(depthShader);
        if(showVoxel[6])
            model_IsoSurface_create(depthShader);
    // rotateAngle += 2.0f;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void draw_model_cls::model_axis_create(Shader shader){
    shader.setBool("tex", false);
    // p_axis
    model.Push();
    model.Save(glm::scale(model.Top(),modelsize));
    shader.setMat4("model", model.Top());
    glBindVertexArray(P_axis.VAO);
    glDrawArrays(GL_LINES, 0, background.p_axis.size());
    model.Pop();
}

void draw_model_cls::model_lightCube_create(Shader shader){

    model.Push();
    model.Save(glm::translate(model.Top(),lightPos));
    model.Save(glm::scale(model.Top(), glm::vec3( 0.2f, 0.2f, 0.2f)));
    shader.setMat4("model", model.Top());
    shader.setBool("tex", false);
    glBindVertexArray(Lightcube.VAO);
    glDrawArrays(GL_TRIANGLES, 0, background.lightcube.size());
    model.Pop();

}

glm::fvec3 draw_model_cls::vector_matrix(glm::mat4 matrix, glm::fvec4 vec){
    glm::fvec4 tmp = {0.0, 0.0, 0.0, 0.0};
    glm::fvec3 ans;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            tmp[i] += matrix[j][i]*vec[j];
        }
    }
    for(int i = 0; i < 3; i++){
        ans[i] = tmp[i] / tmp[3];
    }
    return ans;
}

void draw_model_cls::model_surface_create(Shader shader)
{
    model.Push();
    // model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 0.0f, 0.0f)));
    // model.Save(glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f))); // bunny
    model.Save(glm::rotate(rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f))); // teapot
    model.Save(glm::scale(model.Top(), modelsize));
    shader.setVec3("modelSize", modelsize);
    shader.setBool("ray", false);
    shader.setMat4("model", model.Top());
    for(int l = 0; l < rawModel.choiceLayerNum; l++)
    {
        glBindVertexArray(Surface[l].VAO);
        glDrawArrays(GL_TRIANGLES, 0, background.layerSurfaceBuffers[l].size());
    }
    model.Pop();
}

void draw_model_cls::model_innerSurface_create(Shader shader)
{
    model.Push();
    // model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 0.0f, 0.0f)));
    // model.Save(glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f))); // bunny
    model.Save(glm::rotate(rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f))); // teapot
    model.Save(glm::scale(model.Top(), modelsize));
    shader.setVec3("modelSize", modelsize);
    shader.setBool("ray", false);
    shader.setMat4("model", model.Top());
    glBindVertexArray(InnerSurface.VAO);
    glDrawArrays(GL_TRIANGLES, 0, background.innerSurface.size());
    model.Pop();
}

void draw_model_cls::model_IsoSurface_create(Shader shader)
{
    model.Push();
    // model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 0.0f, 0.0f)));
    // model.Save(glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f))); // bunny
    model.Save(glm::rotate(rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f))); // teapot
    model.Save(glm::scale(model.Top(), modelsize));
    shader.setVec3("modelSize", modelsize);
    shader.setBool("ray", false);
    shader.setMat4("model", model.Top());
    glBindVertexArray(IsoSurface.VAO);
    glDrawArrays(GL_TRIANGLES, 0, background.surface2.size());
    model.Pop();
}

void draw_model_cls::model_voxel_create(Shader shader)
{
    model.Push();
    // model.Save(glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f))); // bunny
    model.Save(glm::rotate(rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f))); // teapot
    model.Save(glm::scale(model.Top(), modelsize));
    // model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 5.0f, 0.0f)));
    shader.setVec3("modelSize", modelsize);
    shader.setBool("ray", false);
    shader.setMat4("model", model.Top());
    glBindVertexArray(Voxel.VAO);
    glDrawArrays(GL_TRIANGLES, 0, background.voxel.size());
    model.Pop();
}

void draw_model_cls::model_innervoxel_create(Shader shader)
{
    model.Push();
    // model.Save(glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f))); // bunny
    model.Save(glm::rotate(rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f))); // teapot
    model.Save(glm::scale(model.Top(), modelsize));
    // model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 5.0f, 0.0f)));
    shader.setVec3("modelSize", modelsize);
    shader.setBool("ray", false);
    shader.setMat4("model", model.Top());
    glBindVertexArray(InnerVoxel.VAO);
    glDrawArrays(GL_TRIANGLES, 0, background.innerVoxel.size());
    model.Pop();
}

void draw_model_cls::model_floor_create(Shader shader)
{
    model.Push();
    model.Save(glm::scale(model.Top(), glm::vec3(10000.0f, 1.0f, 10000.0f)));
    model.Save(glm::translate(model.Top(), glm::vec3(-0.5f, 0.0f, -0.5f)));
    shader.setMat4("model", model.Top());
    shader.setBool("ray", false);
    glBindVertexArray(Floor.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model.Pop();
}

void draw_model_cls::model_lattice_create(Shader shader)
{
    model.Push();
    // glm::fvec3 max = {rawModel.infData.resolution[1], rawModel.infData.resolution[2], rawModel.infData.resolution[0]};
    // model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 3.0f, 0.0f))); // vase
    // model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 5.0f, 0.0f))); // bunny
    // model.Save(glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f))); // bunny
    model.Save(glm::rotate(rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f))); // teapot
    model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 0.3f, 0.0f))); // teapot
    model.Save(glm::scale(model.Top(), modelsize));
    shader.setMat4("model", model.Top());
    if(showVoxel[3] || showVoxel[4]) shader.setBool("tex", true);
    else shader.setBool("tex", false);
    // int texType = rawModel.layerSurfaces[0].surfaceVoxels[0].textype;
    int texType = 0;
    shader.setInt("texType", texType);
    tex.bindTexture(texType); //texture
    if(showVoxel[3]) // lattice
    {
        glBindVertexArray(Lattice.VAO);
        glDrawArrays(GL_LINES, 0, background.lattice_line.size());
    }
    if(showVoxel[4]) // plane
    {
        glBindVertexArray(Plane.VAO);
        glDrawArrays(GL_TRIANGLES, 0, background.lattice_plane.size());
    }
    model.Pop();
}

void  draw_model_cls::model_bound_create(Shader shader){
    model.Push();
    // model.Save(glm::translate(model.Top(), glm::vec3(0.0f, 3.0f, 0.0f)));
    // model.Save(glm::rotate(rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f)));
    model.Save(glm::scale(model.Top(), modelsize));
    shader.setVec3("modelSize", modelsize);
    shader.setBool("ray", true);
    if(showVoxel[0]){
        shader.setMat4("model", model.Top());
        glBindVertexArray(Boundingbox.VAO);
        glDrawArrays(GL_LINES, 0, background.boundingbox.size());
    }
    model.Pop();
}
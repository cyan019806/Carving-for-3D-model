#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "gui.h"
#include "Shader.h"
#include "background.h"
#include "Object.h"
#include "Camera.h"
#include "model.h"
#include "ReadVoxelModel.h"
#include "DrawModel.h"
#include "MarchingCubeTables.h"
#include "SOM.h"
#include "texture.h"
#include "twoStepMapping.h"
#include "time.h"
#include <thread>
#include <iostream>
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void createTexThread();
void createLinkedlistThread();

// time
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentTime = 0.0f;

// light
bool blinn = false;

// thread (speed up)
std::thread t4;

glm::fvec3 *color = isoSurface.ColorMap;

// rotate matrix
float angle = 0.05f;
float angle_to_radian = PI / 180.0;
float rotate_matrix_py[3][3] = {{cos(angle * angle_to_radian), 0.0, sin(angle * angle_to_radian)},
							 {0.0f, 1.0, 0.0},
							 {-sin(angle * angle_to_radian), 0.0, cos(angle * angle_to_radian)}};

float rotate_matrix_ny[3][3] = {{cos(-angle * angle_to_radian), 0.0, sin(-angle * angle_to_radian)},
							 {0.0f, 1.0, 0.0},
							 {-sin(-angle * angle_to_radian), 0.0, cos(-angle * angle_to_radian)}};

int main()
{
	// glfw initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfw window creation
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Distance_Fields", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glfwSetKeyCallback(window, keyPressFunc);
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// initialize imgui
	imgui_init(window);

	// create texture
	tex.create_texture();

	// create shader
	drawModel.Shader_Create();

	// create linked list
	// voxelmodel.CreateLinkedList();

	// set color map
	// isoSurface.create_color_map();

	glEnable(GL_DEPTH_TEST);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// move(keyboard & mouse)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		imgui_create(window);

		drawModel.Shader_Use(window);

		imgui_end();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// voxelmodel.FreeNode(voxelmodel.DoneHead);
	destroyBackground();
	SOM.destory_SOM();
	isoSurface.free_layer_surface_voxel();
	glfwTerminate();
	return 0;
}

// keyboard function
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(UP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(DOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(PITCHUP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(PITCHDOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(YAWUP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		drawModel.camera.ProcessKeyboard(YAWDOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		if (drawModel.showVoxel[3] == true)
			drawModel.showVoxel[3] = false;
		else
			drawModel.showVoxel[3] = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (drawModel.showVoxel[4] == true)
			drawModel.showVoxel[4] = false;
		else
			drawModel.showVoxel[4] = true;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		if (drawModel.showVoxel[0] == true)
			drawModel.showVoxel[0] = false;
		else
			drawModel.showVoxel[0] = true;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		if (drawModel.showVoxel[1] == true)
			drawModel.showVoxel[1] = false;
		else
			drawModel.showVoxel[1] = true;
	}
	
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		if (drawModel.showVoxel[2] == true)
			drawModel.showVoxel[2] = false;
		else
			drawModel.showVoxel[2] = true;
	}

	// light
	float angle = 0.0;
	float temp_pos[3] = {0.0f, 0.0f, 0.0f};
	// move speed
	float v = drawModel.rate * deltaTime * 10;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		drawModel.lightPos = drawModel.lightPos + glm::vec3(0.0, v, 0.0);
		// std::cout << "v: " << v << std::endl;
		// cout << "light pos: " << drawModel.lightPos.x << ", " << drawModel.lightPos.y << ", " << drawModel.lightPos.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		drawModel.lightPos = drawModel.lightPos + glm::vec3(0.0, -1 * v, 0.0);
		// std::cout << "v: " << v << std::endl;
		// cout << "light pos: " << drawModel.lightPos.x << ", " << drawModel.lightPos.y << ", " << drawModel.lightPos.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		// drawModel.lightPos = drawModel.lightPos + glm::vec3(-1 * v, 0.0, 0.0);
		for(int r = 0; r < 3; r++)
		{
			for(int c = 0; c < 3; c++)
			{
				temp_pos[r] += (rotate_matrix_ny[r][c] * drawModel.lightPos[c]);
			}
		}
		drawModel.lightPos[0] = temp_pos[0];
		drawModel.lightPos[1] = temp_pos[1];
		drawModel.lightPos[2] = temp_pos[2];
		drawModel.lightPos = {temp_pos[0], temp_pos[1], temp_pos[2]};
		// cout << "light pos: " << drawModel.lightPos.x << ", " << drawModel.lightPos.y << ", " << drawModel.lightPos.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		for(int r = 0; r < 3; r++)
		{
			for(int c = 0; c < 3; c++)
			{
				// temp_pos[r] += (rotate_matrix_py[r][c] * light_position[c]);
				temp_pos[r] += (rotate_matrix_py[r][c] * drawModel.lightPos[c]);
			}
		}
		drawModel.lightPos[0] = temp_pos[0];
		drawModel.lightPos[1] = temp_pos[1];
		drawModel.lightPos[2] = temp_pos[2];
		drawModel.lightPos = {temp_pos[0], temp_pos[1], temp_pos[2]};
		// cout << "light pos: " << drawModel.lightPos.x << ", " << drawModel.lightPos.y << ", " << drawModel.lightPos.z << endl;
	}
}

void runthreadLinkedlist()
{
	voxelmodel.CreateLinkedList();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void createLinkedlistThread()
{
	if (t4.joinable())
	{
		t4.join();
	}
	t4 = std::thread(runthreadLinkedlist);
}
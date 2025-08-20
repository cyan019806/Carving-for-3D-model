#ifndef GUI_H
#define GUI_H
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ReadVoxelModel.h"
#include "DrawModel.h"
#include "model.h"
#include "twoStepMapping.h"
#include "SOM.h"
#include "texture.h"
#include "carving.h"

#include <iostream>

using namespace std;

void imgui_init(GLFWwindow *window);
void imgui_create(GLFWwindow* window);
void imgui_end();
// void checkbox_init();

#endif
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Vertex.h"
#include "model.h"
#include "DrawModel.h"
#include "Object.h"
#include "ReadVoxelModel.h"
#include "MarchingCubeTables.h"
#include "SOM.h"
#include "carving.h"

#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
struct Background {
    // object
    std::vector<Vertex> voxel;
    std::vector<Vertex> innerVoxel;
    std::vector<Vertex> boundaryVoxel;
    std::vector<Vertex> engraveVoxel;
    std::vector<Vertex> embossVoxel;
    // std::vector<Vertex> surface;
    std::vector<Vertex> surface2;
    std::vector<Vertex> innerSurface;
    std::vector<Vertex> isoSurface;
    std::vector<Vertex> floor;
    std::vector<Vertex> boundingbox;
    std::vector<std::vector<Vertex>> layerSurfaceBuffers;
    // axis
    std::vector<Vertex> axis;
    std::vector<Vertex> p_axis;
    // som
    std::vector<Vertex> lattice_line;
    std::vector<Vertex> lattice_plane;
    // std::vector<Vertex> cube_lattice_square_four_edges;
    // texture
    std::vector<Vertex> tex;
    // light
    std::vector<Vertex> lightcube;

    bool ***isCarvingArea;
};

struct myColor{
    float r, g, b;
};

void createBackground();
void destroyBackground();
// void PointsRenewBackground();
// void XLinesRenewBackground(int xmin, int xmax);
// void ZLinesRenewBackground(int zmin, int zmax);
// void CubesRenewBackground(int ymin, int ymax, int zmin, int zmax, int xmin, int xmax); // 3D

void create_lattice();
void renew_center_voxel(std::vector<LayerSurfaceData> layerVoxels, int layerNum, std::vector<Vertex> &Object1, std::vector<Vertex> &Object2);
void renew_center_inner_voxel(std::vector<LayerSurfaceData> layerVoxels, int layerNum);
void renew_voxel(std::vector<surfaceVoxel> surfaceVoxels, int layern);
void renew_inner_voxel(std::vector<LayerSurfaceData> layerVoxels, int layerNum);
void renew_layer_voxel(std::vector<LayerSurfaceData>& layerVoxels, int layerNum);
void create_bounding_box();
void draw_voxel(surfaceVoxel& SurfaceVoxel, glm::fvec3 voxelColor, std::vector<Vertex> &Object);
bool is_in_resolution(int x, int y, int z);
bool has_draw_neighbor(int x, int y, int z, int c);
bool has_not_draw_neighbor(int x, int y, int z, int c);
void closing(std::vector<LayerSurfaceData>& layerSurfaceVoxels);
void opening(std::vector<LayerSurfaceData>& layerSurfaceVoxels);
void dilation(std::vector<LayerSurfaceData>& layerSurfaceVoxels, int c);
void erosion(std::vector<LayerSurfaceData>& layerSurfaceVoxels, int c);

extern struct Background background;

#endif
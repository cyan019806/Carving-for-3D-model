#include "background.h"

Background background = {};

double current_time_carving = 0.0, carving_time = 0.0, current_time_s = 0.0, smoothing_time = 0.0, current_time_o = 0.0, opening_time = 0.0, current_time_c = 0.0, closing_time = 0.0, current_time_voxeling = 0.0, voxel_time = 0.0, current_time_surfacing = 0.0, surface_time = 0.0;

void createBackground()
{
    background.boundingbox = {
        //          position             normal             texture             color
        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},

        Vertex{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},

        Vertex{{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},

        Vertex{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},

        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},

        Vertex{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0, 1.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0, 0.0, 0.0}, {1.0f, 1.0f, 1.0f}},
    };

    background.axis = background.p_axis = {
        // x
        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        Vertex{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        // y
        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        Vertex{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        // z
        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    };

    background.floor = {
        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},

        Vertex{{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
        Vertex{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
    };
}

void create_lattice()
{
    background.lattice_line.clear();
    background.lattice_plane.clear();

    for (int d = 0; d < SOM.latticeData.typeNum[SOM.latticeData.type]; d++)
    {
        for (int h = 0; h < SOM.latticeData.height; h++)
        {
            for (int w = 0; w < SOM.latticeData.width; w++)
            {
                glm::fvec3 pos, pos_right, pos_up, pos_up_right;
                glm::fvec3 pos_tex, pos_right_tex, pos_up_tex, pos_up_right_tex;

                pos = SOM.latticeData.lattice[d][h][w];
                pos_tex = SOM.t_latticeData.lattice[0][h][w]; //{w * sx, h * sy, 0.5};
                pos_tex = tex.lattice_to_texture(glm::fvec4(pos_tex.s, pos_tex.t, 0.5, 1.0), TWODTEX, SOM.latticeData.width);

                if (w != SOM.latticeData.width - 1)
                {
                    pos_right = SOM.latticeData.lattice[d][h][w + 1];
                    pos_right_tex = SOM.t_latticeData.lattice[0][h][w + 1]; //{(w + 1) * sx, h * sy, 0.5};
                    pos_right_tex = tex.lattice_to_texture(glm::fvec4(pos_right_tex.s, pos_right_tex.t, 0.5, 1.0), TWODTEX, SOM.latticeData.width);
                }

                if (h != SOM.latticeData.height - 1)
                {
                    pos_up = SOM.latticeData.lattice[d][h + 1][w];
                    pos_up_tex = SOM.t_latticeData.lattice[0][h + 1][w]; //{w * sx, (h + 1) * sy, 0.5};
                    pos_up_tex = tex.lattice_to_texture(glm::fvec4(pos_up_tex.s, pos_up_tex.t, 0.5, 1.0), TWODTEX, SOM.latticeData.width);
                }


                if (w != SOM.latticeData.width - 1 && h != SOM.latticeData.height - 1)
                {
                    pos_up_right = SOM.latticeData.lattice[d][h + 1][w + 1];
                    pos_up_right_tex = SOM.t_latticeData.lattice[0][h + 1][w + 1]; // {(w + 1) * sx, (h + 1) * sy, 0.5};
                    pos_up_right_tex = tex.lattice_to_texture(glm::fvec4(pos_up_right_tex.s, pos_up_right_tex.t, 0.5, 1.0), TWODTEX, SOM.latticeData.width);
                }

                glm::fvec3 n;
                if (d == 0)
                    n = glm::fvec3{0.0f, 1.0f, 0.0f};
                else if (d == 1)
                    n = glm::fvec3{0.0f, 0.0f, -1.0f};
                else if (d == 2)
                    n = glm::fvec3{-1.0f, 0.0f, 0.0f};
                else if (d == 3)
                    n = glm::fvec3{0.0f, 0.0f, 1.0f};
                else if (d == 4)
                    n = glm::fvec3{1.0f, 0.0f, 0.0f};
                else if (d == 5)
                    n = glm::fvec3{0.0f, -1.0f, 0.0f};

                if (d == 2 || d == 4)
                {
                    // draw lines
                    // --- (x, y) -> (x + 1, y)
                    if (w != SOM.latticeData.width - 1)
                    {
                        background.lattice_line.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.y, pos_tex.x, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_line.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.y, pos_right_tex.x, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                    // | (x, y) -> (x, y + 1)
                    if (h != SOM.latticeData.height - 1)
                    {
                        background.lattice_line.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.y, pos_tex.x, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_line.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.y, pos_up_tex.x, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                    // draw planes
                    // --- (x, y) -> (x + 1, y) and | (x, y) -> (x, y + 1)
                    if (w != SOM.latticeData.width - 1 && h != SOM.latticeData.height - 1)
                    {
                        background.lattice_plane.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.y, pos_up_tex.x, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.y, pos_tex.x, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.y, pos_right_tex.x, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});

                        background.lattice_plane.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.y, pos_right_tex.x, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_up_right.x, pos_up_right.y, pos_up_right.z}, {n.x, n.y, n.z}, {pos_up_right_tex.y, pos_up_right_tex.x, pos_up_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.y, pos_up_tex.x, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                }
                else // left, right
                {
                    // draw lines
                    // --- (x, y) -> (x + 1, y)
                    if (w != SOM.latticeData.width - 1)
                    {
                        background.lattice_line.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.x, pos_tex.y, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_line.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.x, pos_right_tex.y, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                    // | (x, y) -> (x, y + 1)
                    if (h != SOM.latticeData.height - 1)
                    {
                        background.lattice_line.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.x, pos_tex.y, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_line.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.x, pos_up_tex.y, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                    // draw planes
                    // --- (x, y) -> (x + 1, y) and | (x, y) -> (x, y + 1)
                    if (w != SOM.latticeData.width - 1 && h != SOM.latticeData.height - 1)
                    {
                        background.lattice_plane.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.x, pos_up_tex.y, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.x, pos_tex.y, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.x, pos_right_tex.y, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});

                        background.lattice_plane.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.x, pos_right_tex.y, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_up_right.x, pos_up_right.y, pos_up_right.z}, {n.x, n.y, n.z}, {pos_up_right_tex.x, pos_up_right_tex.y, pos_up_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.x, pos_up_tex.y, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                }
            }
        }
    }
}

void renew_center_voxel(std::vector<LayerSurfaceData> layerVoxels, int layerNum, std::vector<Vertex> &Object1, std::vector<Vertex> &Object2)
{
    Object1.clear();
    Object2.clear();
    int centeridx = rawModel.centerIdx;
    int surfaceNum = layerVoxels[centeridx].surfaceVoxels.size();
    int innerNum = layerVoxels[centeridx].innerVoxels.size();
    std::vector<surfaceVoxel> surfaceVoxels = layerVoxels[centeridx].surfaceVoxels;
    std::cout << "layer " << centeridx << std::endl;
    std::cout << "surfaceNum: " << layerVoxels[centeridx].surfaceVoxels.size() << std::endl;
    std::cout << "innerNum: " << layerVoxels[centeridx].innerVoxels.size() << std::endl;
    for(int l = 0; l < layerNum; l++)
    {
        if(layerVoxels[l].oneSurfaceVoxels.size())
            std::cout << "layer " << l << " surfaceNum: " << layerVoxels[l].oneSurfaceVoxels.size() << std::endl;
    }
    // std::cout << "boundaryNum: " << layerVoxels[centeridx].boundaryVoxels.size() << std::endl;
    std::vector<surfaceVoxel> innerVoxels = layerVoxels[centeridx].innerVoxels;
    rawModel.maxLayer = centeridx;
    rawModel.minLayer = centeridx;
    for (int v = 0; v < surfaceNum; v++)
    {
        int x = surfaceVoxels[v].locate.x;
        int y = surfaceVoxels[v].locate.y;
        int z = surfaceVoxels[v].locate.z;

        glm::fvec3 voxelColor = surfaceVoxels[v].color;
        // cout << "voxelColor: " << voxelColor.r << ", " << voxelColor.g << ", " << voxelColor.b << endl;
        draw_voxel(surfaceVoxels[v], voxelColor, Object1);
    }
    for (int i = 0; i < innerNum; i++)
    {
        int x = innerVoxels[i].locate.x;
        int y = innerVoxels[i].locate.y;
        int z = innerVoxels[i].locate.z;

        glm::fvec3 voxelColor = innerVoxels[i].color;
        // cout << "voxelColor: " << voxelColor.r << ", " << voxelColor.g << ", " << voxelColor.b << endl;
        draw_voxel(innerVoxels[i], voxelColor, Object2);
    }
}

void renew_center_inner_voxel(std::vector<LayerSurfaceData> layerVoxels, int layerNum)
{
    background.innerVoxel.clear();
    // the layer close to inner voxel
    for (int l = 0; l < layerNum; l++)
    {
        int innerNum = layerVoxels[l].innerVoxels.size();
        std::vector<surfaceVoxel> innerVoxels = layerVoxels[l].innerVoxels;
        for (int v = 0; v < innerNum; v++)
        {
            int x = innerVoxels[v].locate.x;
            int y = innerVoxels[v].locate.y;
            int z = innerVoxels[v].locate.z;

            glm::fvec3 voxelColor = innerVoxels[v].color;
            // cout << "voxelColor: " << voxelColor.r << ", " << voxelColor.g << ", " << voxelColor.b << endl;
            draw_voxel(innerVoxels[v], voxelColor, background.innerVoxel);
        }
        std::cout << "layer " << l << std::endl;
        std::cout << "innerNum: " << rawModel.layerSurfaces[l].innerVoxels.size() << std::endl;
    }
}

void draw_voxel(surfaceVoxel& Voxel, glm::fvec3 voxelColor, std::vector<Vertex> &Object)
{
    int x = Voxel.locate.x;
    int y = Voxel.locate.y;
    int z = Voxel.locate.z;

    // glm::fvec3 voxelColor = SurfaceVoxel.color;
    if (Voxel.faceAir[0]) // back
    {
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 0.0f + z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 0.0f + y, 0.0f + z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});

        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 0.0f + z}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
    }
    if (Voxel.faceAir[1]) // front
    {
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 1.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 0.0f + y, 1.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});

        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 1.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
    }
    if (Voxel.faceAir[2]) // left
    {
        Object.push_back(Vertex{{0.0f + x, 1.0f + y, 1.0f + z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 1.0f + y, 0.0f + z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 0.0f + z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});

        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 0.0f + z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 1.0f + z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 1.0f + y, 1.0f + z}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
    }
    if (Voxel.faceAir[3]) // right
    {
        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 1.0f + z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 0.0f + z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 0.0f + y, 0.0f + z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});

        Object.push_back(Vertex{{1.0f + x, 0.0f + y, 0.0f + z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 0.0f + y, 1.0f + z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 1.0f + z}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
    }
    if (Voxel.faceAir[4]) // bottom
    {
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 0.0f + z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 0.0f + y, 0.0f + z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 0.0f + y, 1.0f + z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});

        Object.push_back(Vertex{{1.0f + x, 0.0f + y, 1.0f + z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 1.0f + z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 0.0f + y, 0.0f + z}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
    }
    if (Voxel.faceAir[5]) // up
    {
        Object.push_back(Vertex{{0.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});

        Object.push_back(Vertex{{1.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 1.0f + y, 1.0f + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
        Object.push_back(Vertex{{0.0f + x, 1.0f + y, 0.0f + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {voxelColor.r, voxelColor.g, voxelColor.b}});
    }
}

bool is_surface_voxel(int x, int y, int z)
{
    if(rawModel.voxelData[y][x][z] == 0)
        return true;
    return false;
}

bool is_surface_voxel1(int x, int y, int z)
{
    if(rawModel.voxelData[y][x][z] == 1)
        return true;
    return false;
}

// triangle model smoothing
void laplacianSmoothing(int times)
{
    for (int y = 0; y < rawModel.infData.resolution[2] - 1; y++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1] - 1; x++)
        {
            for (int z = 0; z < rawModel.infData.resolution[0] - 1; z++)
            {
                double sum = 0.0;
                int count = 0, NeighborZeroSum = 0;

                // 取 6 鄰居 (6-connected neighborhood)
                int dx[] = {1, -1,  0,  0,  0,  0};
                int dy[] = {0,  0,  1, -1,  0,  0};
                int dz[] = {0,  0,  0,  0,  1, -1};
                for (int i = 0; i < 6; i++)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    int nz = z + dz[i];

                    if(nx < 0 || nx > rawModel.infData.resolution[1] - 1)
                        continue;
                    if(ny < 0 || ny > rawModel.infData.resolution[2] - 1)
                        continue;
                    if(nz < 0 || nz > rawModel.infData.resolution[0] - 1)
                        continue;

                    sum += rawModel.voxelData[ny][nx][nz];
                    count++;
                }
                    rawModel.smoothData[y][x][z] = sum / count;
            }
        }
    }
    // 將平滑結果覆蓋回原始距離場
    for (int y = 0; y < rawModel.infData.resolution[2] - 1; y++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1] - 1; x++)
        {
            for (int z = 0; z < rawModel.infData.resolution[0] - 1; z++)
            {
                rawModel.voxelData[y][x][z] = rawModel.smoothData[y][x][z];
            }
        }
    }
    // 反方向再做一次
    for (int y = rawModel.infData.resolution[2] - 2; y >= 0; y--)
    {
        for (int x = rawModel.infData.resolution[1] - 2; x >= 0; x--)
        {
            for (int z = rawModel.infData.resolution[0] - 2; z >= 0; z--)
            {
                double sum = 0.0;
                int count = 0;

                // 取 6 近鄰 (6-connected neighborhood)
                int dx[] = {1, -1,  0,  0,  0,  0};
                int dy[] = {0,  0,  1, -1,  0,  0};
                int dz[] = {0,  0,  0,  0,  1, -1};

                for (int i = 0; i < 6; i++)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    int nz = z + dz[i];

                    if(nx < 0 || nx > rawModel.infData.resolution[1] - 1)
                        continue;
                    if(ny < 0 || ny > rawModel.infData.resolution[2] - 1)
                        continue;
                    if(nz < 0 || nz > rawModel.infData.resolution[0] - 1)
                        continue;

                    sum += rawModel.voxelData[ny][nx][nz];
                    count++;
                }
                    rawModel.smoothData[y][x][z] = sum / count;
            }
        }
    }
    // 將平滑結果覆蓋回原始距離場
    for (int y = rawModel.infData.resolution[2] - 2; y >= 0; y--)
    {
        for (int x = rawModel.infData.resolution[1] - 2; x >= 0; x--)
        {
            for (int z = rawModel.infData.resolution[0] - 2; z >= 0; z--)
            {
                rawModel.voxelData[y][x][z] = rawModel.smoothData[y][x][z];
            }
        }
    }
}
// iso-surface smoothing
void laplacianSmoothing1(int times)
{
    for (int y = 0; y < rawModel.infData.resolution[2] - 1; y++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1] - 1; x++)
        {
            for (int z = 0; z < rawModel.infData.resolution[0] - 1; z++)
            {
                double sum = 0.0;
                int count = 0;

                // 取 6 鄰居 (6-connected neighborhood)
                int dx[] = {1, -1,  0,  0,  0,  0};
                int dy[] = {0,  0,  1, -1,  0,  0};
                int dz[] = {0,  0,  0,  0,  1, -1};
                for (int i = 0; i < 6; i++)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    int nz = z + dz[i];

                    if(nx < 0 || nx > rawModel.infData.resolution[1] - 1)
                        continue;
                    if(ny < 0 || ny > rawModel.infData.resolution[2] - 1)
                        continue;
                    if(nz < 0 || nz > rawModel.infData.resolution[0] - 1)
                        continue;

                    sum += rawModel.voxelData3[ny][nx][nz];
                    count++;
                }
                    rawModel.smoothData[y][x][z] = sum / count;
            }
        }
    }
    // 將平滑結果覆蓋回原始距離場
    for (int y = 0; y < rawModel.infData.resolution[2] - 1; y++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1] - 1; x++)
        {
            for (int z = 0; z < rawModel.infData.resolution[0] - 1; z++)
            {
                rawModel.voxelData3[y][x][z] = rawModel.smoothData[y][x][z];
            }
        }
    }
    // 反方向再做一次
    for (int y = rawModel.infData.resolution[2] - 2; y >= 0; y--)
    {
        for (int x = rawModel.infData.resolution[1] - 2; x >= 0; x--)
        {
            for (int z = rawModel.infData.resolution[0] - 2; z >= 0; z--)
            {
                double sum = 0.0;
                int count = 0;

                // 取 6 近鄰 (6-connected neighborhood)
                int dx[] = {1, -1,  0,  0,  0,  0};
                int dy[] = {0,  0,  1, -1,  0,  0};
                int dz[] = {0,  0,  0,  0,  1, -1};

                for (int i = 0; i < 6; i++)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    int nz = z + dz[i];

                    if(nx < 0 || nx > rawModel.infData.resolution[1] - 1)
                        continue;
                    if(ny < 0 || ny > rawModel.infData.resolution[2] - 1)
                        continue;
                    if(nz < 0 || nz > rawModel.infData.resolution[0] - 1)
                        continue;

                    sum += rawModel.voxelData3[ny][nx][nz];
                    count++;
                }
                rawModel.smoothData[y][x][z] = sum / count;
            }
        }
    }
    // 將平滑結果覆蓋回原始距離場
    for (int y = rawModel.infData.resolution[2] - 2; y >= 0; y--)
    {
        for (int x = rawModel.infData.resolution[1] - 2; x >= 0; x--)
        {
            for (int z = rawModel.infData.resolution[0] - 2; z >= 0; z--)
            {
                rawModel.voxelData3[y][x][z] = rawModel.smoothData[y][x][z];
            }
        }
    }
}

bool has_draw_neighbor(int x, int y, int z, int c)
{
    int count = 0;
    if(rawModel.rawData[y][x][z].draw) return true; // emboss時，原本有要畫的，就不要動
    for(int dx = -1; dx <= 1; dx++)
    {
        for(int dy = -1; dy <= 1; dy++)
        {
            for(int dz = -1; dz <= 1; dz++)
            {
                if(abs(dx) + abs(dy) + abs(dz) == 1) // 6 neighbor
                {
                    int nx = x + dx;
                    int ny = y + dy;
                    int nz = z + dz;
                    if(!is_in_resolution(nx, ny, nz)) continue;
                    if(rawModel.rawData[ny][nx][nz].draw)
                    {
                        count++;
                        // return true;
                    }
                }
            }
        }
    }
    if(count >= c)
        return true;
    else
        return false;
    // return false;
}

bool has_not_draw_neighbor(int x, int y, int z, int c)
{
    int count = 0;
    if(rawModel.rawData[y][x][z].draw) return true; // emboss時，原本有要畫的，就不要動
    for(int dx = -1; dx <= 1; dx++)
    {
        for(int dy = -1; dy <= 1; dy++)
        {
            for(int dz = -1; dz <= 1; dz++)
            {
                if(abs(dx) + abs(dy) + abs(dz) == 1) // 6 neighbor
                {
                    int nx = x + dx;
                    int ny = y + dy;
                    int nz = z + dz;
                    if(!is_in_resolution(nx, ny, nz)) continue;
                    if(!rawModel.rawData[ny][nx][nz].draw)
                    {
                        count++;
                        // return false;
                    }
                }
            }
        }
    }
    if(count >= c)
        return false;
    else
        return true;
    // return true;
}

bool is_in_resolution(int x, int y, int z)
{
    if(x >= 0 && x <= rawModel.infData.resolution[1] - 1 && y >= 0 && y <= rawModel.infData.resolution[2] - 1 && z >= 0 && z <= rawModel.infData.resolution[0] - 1)
        return true;
    return false;
}

void dilation(std::vector<LayerSurfaceData>& layerSurfaceVoxels, int c)
{
    // renew voxel color
    int layerNum = layerSurfaceVoxels.size();
    for(int l = 0; l < layerNum; l++)
    {
        std::vector<surfaceVoxel> &oneLayerVoxel = layerSurfaceVoxels[l].oneSurfaceVoxels;
        std::vector<bool> newDraw(oneLayerVoxel.size(), false);
        for (int i = 0; i < oneLayerVoxel.size(); i++)
        {
            int v_x = oneLayerVoxel[i].locate.x;
            int v_y = oneLayerVoxel[i].locate.y;
            int v_z = oneLayerVoxel[i].locate.z;
            if((v_y > mapping.y_max || v_y < mapping.y_min) || (v_x > mapping.x_max || v_x < mapping.x_min) || (v_z > mapping.z_max || v_z < mapping.z_min))
            {
                newDraw[i] = true;
                continue;
            }
            // if (!oneLayerVoxel[i].draw && has_draw_neighbor(v_x, v_y, v_z)) {
                newDraw[i] = has_draw_neighbor(v_x, v_y, v_z, c);
            // }
        }
        // std::cout << "renew " << count << " voxel after diiation." << std::endl;
        for (int i = 0; i < oneLayerVoxel.size(); ++i) {
            // if (newDraw[i]) {
                oneLayerVoxel[i].draw = newDraw[i];
                glm::ivec3 p = oneLayerVoxel[i].locate;
                rawModel.rawData[p.y][p.x][p.z].draw = oneLayerVoxel[i].draw;
            // }
        }
    }
}

void erosion(std::vector<LayerSurfaceData>& layerSurfaceVoxels, int c)
{
    // renew voxel color
    int layerNum = layerSurfaceVoxels.size();
    for(int l = 0; l < layerNum; l++)
    {
        std::vector<surfaceVoxel> &oneLayerVoxel = layerSurfaceVoxels[l].oneSurfaceVoxels;
        std::vector<bool> newDraw(oneLayerVoxel.size(), false);
        for (int i = 0; i < oneLayerVoxel.size(); ++i) {
            int v_x = oneLayerVoxel[i].locate.x;
            int v_y = oneLayerVoxel[i].locate.y;
            int v_z = oneLayerVoxel[i].locate.z;
            if((v_y > mapping.y_max || v_y < mapping.y_min) || (v_x > mapping.x_max || v_x < mapping.x_min) || (v_z > mapping.z_max || v_z < mapping.z_min))
            {
                newDraw[i] = true;
                continue;
            }
            newDraw[i] = has_not_draw_neighbor(v_x, v_y, v_z, c);
        }
        // std::cout << "renew " << count << " voxel  after erosion." << std::endl;
        for (int i = 0; i < oneLayerVoxel.size(); ++i)
        {
            oneLayerVoxel[i].draw = newDraw[i];
            glm::ivec3 p = oneLayerVoxel[i].locate;
            rawModel.rawData[p.y][p.x][p.z].draw = oneLayerVoxel[i].draw;
        }
    }
}

int count_draw_voxel() {
    int count = 0;
    for (int y = 0; y < rawModel.infData.resolution[2]; y++)
        for (int x = 0; x < rawModel.infData.resolution[1]; x++)
            for (int z = 0; z < rawModel.infData.resolution[0]; z++)
                if (rawModel.rawData[y][x][z].draw)
                    count++;
    return count;
}

void closing(std::vector<LayerSurfaceData>& layerSurfaceVoxels)
{
    std::cout << "Before closeing white count: " << count_draw_voxel() << std::endl;
    dilation(layerSurfaceVoxels, 2);
    std::cout << "After dilation white count: " << count_draw_voxel() << std::endl;
    erosion(layerSurfaceVoxels, 2);
    std::cout << "After erosion white count: " << count_draw_voxel() << std::endl;
}

void opening(std::vector<LayerSurfaceData>& layerSurfaceVoxels)
{
    std::cout << "Before opening white count: " << count_draw_voxel() << std::endl;
    erosion(layerSurfaceVoxels, 2);
    std::cout << "After erosion white count: " << count_draw_voxel() << std::endl;
    dilation(layerSurfaceVoxels, 2);
    std::cout << "After dilation white count: " << count_draw_voxel() << std::endl;
}

// bunny teapot
bool write_file(double*** data)
{
    // teapot: x = 217, y = 195, z = 398
    // bunny: x = 157, y = 184, z = 192
    int depth  = rawModel.infData.resolution[2];
    int height = rawModel.infData.resolution[1];
    int width  = rawModel.infData.resolution[0];

    // std::vector<float> volume(width * height * depth);

    FILE *file = fopen("C:\\2023 code\\Distance_Fields_3D\\teapot_water_emboss.txt", "w");
    for (int y = 0; y < depth; y++)
    {
        for (int z = 0; z < width; z++)
        {
            for (int x = 0; x < height; x++)
            {
                // if(data[y][x][z] != 0)
                    // volume[idx] = static_cast<float>(data[y][x][z]);
                // else
                //     volume[idx] = static_cast<float>(data[y][x][z]);

                // fprintf(file, "%c ", static_cast<unsigned char>(data[y][x][z] * 255.0f));
                if(data[y][x][z] == 0)
                    fprintf(file, "%d ", static_cast<int>(data[y][x][z]));
                else if(data[y][x][z] >= 1 && data[y][x][z] <= 5)
                    fprintf(file, "%d ", static_cast<int>(data[y][x][z] * 40.0f));
                else if(data[y][x][z] > 5)
                    fprintf(file, "%d ", static_cast<int>(240.0f));
            }
        }
    }
    fclose(file);
    // std::ofstream out("C:\\2023 code\\Distance_Fields_3D\\vase_engrave.raw", std::ios::binary);
    // if (!out)
    // {
    //     std::cerr << "Failed to open file for writing.\n";
    //     return false;
    // }

    // out.write(reinterpret_cast<char*>(volume.data()), volume.size() * sizeof(float));
    // out.close();
    return true;
    // // teapot: x = 217, y = 195, z = 398
    // // bunny: x = 157, y = 184, z = 192
    // int depth  = rawModel.infData.resolution[2];
    // int height = rawModel.infData.resolution[1];
    // int width  = rawModel.infData.resolution[0];

    // // std::vector<float> volume(width * height * depth);

    // FILE *file = fopen("C:\\2023 code\\Distance_Fields_3D\\vase_engrave.txt", "w");
    // for (int y = 0; y < depth; y++)
    // {
    //     for (int x = 0; x < height; x++)
    //     {
    //         for (int z = 0; z < width; z++)
    //         {
    //             // if(data[y][x][z] != 0)
    //                 // volume[idx] = static_cast<float>(data[y][x][z]);
    //             // else
    //             //     volume[idx] = static_cast<float>(data[y][x][z]);

    //             // fprintf(file, "%c ", static_cast<unsigned char>(data[y][x][z] * 255.0f));
    //             fprintf(file, "%d ", static_cast<int>(data[y][x][z] * 255.0f));
    //         }
    //     }
    // }
    // fclose(file);
    // // std::ofstream out("C:\\2023 code\\Distance_Fields_3D\\vase_engrave.raw", std::ios::binary);
    // // if (!out)
    // // {
    // //     std::cerr << "Failed to open file for writing.\n";
    // //     return false;
    // // }

    // out.write(reinterpret_cast<char*>(volume.data()), volume.size() * sizeof(float));
    // out.close();
    return true;
}

// // vase
// bool write_file(double*** data)
// {
//     // vase x = 205, y = 390, z = 205
//     int width  = rawModel.infData.resolution[1];  // x
//     int height = rawModel.infData.resolution[0];  // y
//     int depth  = rawModel.infData.resolution[2];  // z

//     std::vector<float> volume(width * height * depth);

//     for (int z = 0; z < depth; z++)      // output z → input y
//     {
//         for (int y = 0; y < height; y++) // output y → input z
//         {
//             for (int x = 0; x < width; x++) // output x → input x
//             {
//                 int idx = x + y * width + z * width * height;

//                 // output (x, y, z) → data[input_y][input_x][input_z]
//                 volume[idx] = static_cast<float>(data[z][x][y] * 17);
//             }
//         }
//     }

//     std::ofstream out("C:\\2023 code\\Distance_Fields_3D\\vase_17_to_85.raw", std::ios::binary);
//     if (!out) {
//         std::cerr << "Failed to open file for writing.\n";
//         return false;
//     }

//     out.write(reinterpret_cast<char*>(volume.data()), volume.size() * sizeof(float));
//     out.close();
//     return true;
// }

bool read_raw_file()
{
    FILE* file_p = fopen("C:\\2023 code\\Distance_Fields_3D\\vase_zero_to_sixty.raw", "r");
    int size = rawModel.infData.resolution[0] * rawModel.infData.resolution[1] * rawModel.infData.resolution[2];
    unsigned char* uc_voxelData;
    float* f_voxelData;
    double* d_voxelData;
    // if (rawModel.infData.type == 0)
    // {
        // uc_voxelData = (unsigned char *)malloc(sizeof(unsigned char) * size);
    // }
    // else if (rawModel.infData.type == 1)
    // {
        f_voxelData = (float *)malloc(sizeof(float) * size);
    // }
    // else if (rawModel.infData.type == 2)
    // {
    //     d_voxelData = (double *)malloc(sizeof(double) * size);
    // }
    float ***data = (float ***)malloc(sizeof(float **) * rawModel.infData.resolution[2]);
    for(int i = 0; i < rawModel.infData.resolution[2]; i++)
    {
        data[i] = (float **)malloc(sizeof(float *) * rawModel.infData.resolution[1]);
        for(int j = 0; j < rawModel.infData.resolution[1]; j++)
        {
            data[i][j] = (float *)malloc(sizeof(float) * rawModel.infData.resolution[0]);
            for(int k = 0; k < rawModel.infData.resolution[0]; k++)
            {
                data[i][j][k] = 0;
            }
        }
    }
    // if (rawModel.infData.type == 0)
    // {
        // fread(uc_voxelData, sizeof(unsigned char), size, file_p);
        fread(f_voxelData, sizeof(unsigned char), size, file_p);
        fclose(file_p);
        for (int i = 0; i < rawModel.infData.resolution[2]; i++)
        {
            for (int j = 0; j < rawModel.infData.resolution[1]; j++)
            {
                for (int k = 0; k < rawModel.infData.resolution[0]; k++)
                {
                    int num = k + j * rawModel.infData.resolution[0] + i * rawModel.infData.resolution[0] * rawModel.infData.resolution[1];
                    // data[i][j][k] = uc_voxelData[num];
                    data[i][j][k] = f_voxelData[num];
                    if(k == 85)
                        cout << data[i][j][k] << " ";
                }
                // std::cout << std::endl;
            }
            // std::cout << std::endl;
        }
        return true;
    // }
    // else if (rawModel.infData.type == 1)
    // {
    //     fread(f_voxelData, sizeof(float), size, file_p);
    //     fclose(file_p);
    //     for (int i = 1; i < rawModel.infData.resolution[2] - 1; i++)
    //     {
    //         for (int j = 1; j < rawModel.infData.resolution[1] - 1; j++)
    //         {
    //             for (int k = 1; k < rawModel.infData.resolution[0] - 1; k++)
    //             {
    //                 int num = k + j * rawModel.infData.resolution[0] + i * rawModel.infData.resolution[0] * rawModel.infData.resolution[1];
    //                 data[i][j][k] = f_voxelData[num];
    //                 if(k == 85)
    //                     cout << data[i][j][k] << " ";
    //             }
    //             // std::cout << std::endl;
    //         }
    //         // std::cout << std::endl;
    //     }
    //     return true;
    // }
    // else if (rawModel.infData.type == 2)
    // {
    //     fread(d_voxelData, sizeof(double), size, file_p);
    //     for (int i = 0; i < rawModel.infData.resolution[2]; i++)
    //     {
    //         for (int j = 0; j < rawModel.infData.resolution[1]; j++)
    //         {
    //             for (int k = 0; k < rawModel.infData.resolution[0]; k++)
    //             {
    //                 int num = k + j * rawModel.infData.resolution[0] + i * rawModel.infData.resolution[0] * rawModel.infData.resolution[1];
    //                 data[i][j][k] = d_voxelData[num];
    //                 if(k == 85)
    //                     cout << data[i][j][k] << " ";
    //             }
    //             // std::cout << std::endl;
    //         }
    //         // std::cout << std::endl;
    //     }
    //     return true;
    // }
    // return false;
}

void renew_layer_voxel(std::vector<LayerSurfaceData>& layerSurfaceVoxels, int layerNum)
{
    background.voxel.clear();
    // all distance of voxel reset to 0
    int n = 0;
    for(int y = 0; y < rawModel.infData.resolution[2]; y++)
    {
        for(int x = 0; x < rawModel.infData.resolution[1]; x++)
        {
            for(int z = 0; z < rawModel.infData.resolution[0]; z++)
            {
                rawModel.voxelData[y][x][z] = 0;
                // 表面以外的距離值皆設為 0
                // if(!(rawModel.voxelData3[y][x][z] >= 1 && rawModel.voxelData3[y][x][z] <= 5))
                if(rawModel.voxelData3[y][x][z] <= 0)
                    rawModel.voxelData3[y][x][z] = 0;
            }
        }
    }
    for(int l = 0; l < layerNum; l++)
    {
        std::vector<surfaceVoxel> &oneLayerVoxel = layerSurfaceVoxels[l].oneSurfaceVoxels;
        std::vector<surfaceVoxel> &innerVoxel = layerSurfaceVoxels[l].innerVoxels;
        if (oneLayerVoxel.empty()) continue;
        // Step 1: 將 surface voxel 對應的距離設為 1, 要畫的部分標示出來
        for (auto &v : oneLayerVoxel)
        {
            glm::ivec3 p = v.locate;
            // rawModel.voxelData3[p.y][p.x][p.z] = 1;
        }
    }
    if(carving.engrave || carving.emboss)
        current_time_carving = glfwGetTime();

    for(int l = 0; l < layerNum; l++)
    {
        std::vector<surfaceVoxel> &oneLayerVoxel = layerSurfaceVoxels[l].oneSurfaceVoxels;
        if (oneLayerVoxel.empty()) continue;
        rawModel.layerSurfaces[l].boundaryVoxels.clear();
        for (auto &v : oneLayerVoxel)
        {
            glm::ivec3 p = v.locate;
            glm::fvec3 voxelColor = v.color;
            v.draw = false;
            rawModel.rawData[p.y][p.x][p.z].draw = false;
            // if emboss, white voxel remain
            if (carving.emboss && voxelColor.r < 0.7f)
            {
                v.draw = true;
                rawModel.rawData[p.y][p.x][p.z].draw = true;
                voxelColor = {0.6235f, 0.2078f, 1.0f};
                rawModel.rawData[p.y][p.x][p.z].color = {0.0f, 0.0f, 0.0f};
            }
            // if engrave, black voxel remain
            else if (carving.engrave && voxelColor.r > 0.7f)
            {
                v.draw = true;
                rawModel.rawData[p.y][p.x][p.z].draw = true;
                voxelColor = {0.6235f, 0.2078f, 1.0f};
                rawModel.rawData[p.y][p.x][p.z].color = {1.0f, 1.0f, 1.0f};
            }
            // before carving, all voxel remain
            else if (!carving.emboss && !carving.engrave)
            {
                v.draw = true;
                rawModel.rawData[p.y][p.x][p.z].draw = true;
                voxelColor = {0.6235f, 0.2078f, 1.0f};
                rawModel.rawData[p.y][p.x][p.z].color = {1.0f, 1.0f, 1.0f};
            }
            // if carve the outter layer, and not in work space, then voxel should remain
            if(l == rawModel.maxLayer)
            {
                if(p.y > mapping.y_max || p.y < mapping.y_min)
                {
                    if(rawModel.voxelData3[p.y][p.x][p.z] >= 1 && rawModel.voxelData3[p.y][p.x][p.z] <= 5) // surface voxel
                    {
                        v.draw = true;
                        rawModel.rawData[p.y][p.x][p.z].draw = true;
                    }
                    rawModel.voxelData[p.y][p.x][p.z] = 0;
                    voxelColor = {0.6235f, 0.2078f, 1.0f};
                    if(carving.emboss)
                        rawModel.rawData[p.y][p.x][p.z].color = {0.0f, 0.0f, 0.0f};
                    else
                        rawModel.rawData[p.y][p.x][p.z].color = {1.0f, 1.0f, 1.0f};
                }
                if(p.x > mapping.x_max || p.x < mapping.x_min)
                {
                    if(rawModel.voxelData3[p.y][p.x][p.z] >= 1 && rawModel.voxelData3[p.y][p.x][p.z] <= 5) // surface voxel
                    {
                        v.draw = true;
                        rawModel.rawData[p.y][p.x][p.z].draw = true;
                    }
                    rawModel.voxelData[p.y][p.x][p.z] = 0;
                    voxelColor = {0.6235f, 0.2078f, 1.0f};
                    if(carving.emboss)
                        rawModel.rawData[p.y][p.x][p.z].color = {0.0f, 0.0f, 0.0f};
                    else
                        rawModel.rawData[p.y][p.x][p.z].color = {1.0f, 1.0f, 1.0f};
                }
                if(p.z > mapping.z_max || p.z < mapping.z_min)
                {
                    if(rawModel.voxelData3[p.y][p.x][p.z] >= 1 && rawModel.voxelData3[p.y][p.x][p.z] <= 5) // surface voxel
                    {
                        v.draw = true;
                        rawModel.rawData[p.y][p.x][p.z].draw = true;
                    }
                    rawModel.voxelData[p.y][p.x][p.z] = 0;
                    voxelColor = {0.6235f, 0.2078f, 1.0f};
                    if(carving.emboss)
                        rawModel.rawData[p.y][p.x][p.z].color = {0.0f, 0.0f, 0.0f};
                    else
                        rawModel.rawData[p.y][p.x][p.z].color = {1.0f, 1.0f, 1.0f};
                }
            }
            else
            {
                if(p.y > mapping.y_max || p.y < mapping.y_min)
                {
                    rawModel.voxelData[p.y][p.x][p.z] = 1;
                }
                if(p.x > mapping.x_max || p.x < mapping.x_min)
                {
                    rawModel.voxelData[p.y][p.x][p.z] = 1;
                }
                if(p.z > mapping.z_max || p.z < mapping.z_min)
                {
                    rawModel.voxelData[p.y][p.x][p.z] = 1;
                }
            }
        }
    }
    if(carving.engrave || carving.emboss)
    {
        voxel_time = glfwGetTime() - current_time_carving;
        cout << "voxeling time: " << voxel_time << " sec" << endl;
    }
    if(carving.engrave || carving.emboss)
    {
        carving_time = glfwGetTime() - current_time_carving;
        cout << "carving time: " << carving_time << " sec" << endl;
    }
    // Step 2: opening and closing 操作 (engrave -> opening, emboss -> closing)
    // current_time_o = glfwGetTime();
    if(carving.engrave)
    {
        opening(layerSurfaceVoxels);
        // opening_time = glfwGetTime() - current_time_o;
        // cout << "opening time: " << opening_time << " sec" << endl;
    }
    // current_time_c = glfwGetTime();
    if(carving.emboss)
    {
        closing(layerSurfaceVoxels);
        // closing_time = glfwGetTime() - current_time_c;
        // cout << "closing time: " << closing_time << " sec" << endl;
    }
    // renew surfcae voxel
    background.voxel.clear();
    for(int l = 0; l < layerNum; l++)
    {
        std::vector<surfaceVoxel> &oneLayerVoxel = layerSurfaceVoxels[l].oneSurfaceVoxels;
        if (oneLayerVoxel.empty()) continue;

        rawModel.layerSurfaces[l].boundaryVoxels.clear();
        for (auto &v : oneLayerVoxel)
        {
            // glm::fvec3 voxelColor = {0.6235f, 0.2078f, 1.0f}; // 紫色
            glm::fvec3 voxelColor = {1.0f, 0.8627f, 0.2078f}; // 黃色
            if(v.draw == true)
            {
                draw_voxel(v, voxelColor, background.voxel);
            }
        }
    }
    surface_time = glfwGetTime();
    if(carving.engrave || carving.emboss)
    {
        std::vector<std::vector<surfaceVoxel>> boundaryVoxel;
        boundaryVoxel.resize(rawModel.choiceLayerNum);
        background.layerSurfaceBuffers.resize(rawModel.choiceLayerNum);
        int layer = 0;

        for(int l = 0; l < layerNum; l++)
        {
            std::vector<surfaceVoxel> &oneLayerVoxel = layerSurfaceVoxels[l].oneSurfaceVoxels;
            int maxLayer = rawModel.maxLayer;
            std::vector<surfaceVoxel> &innerVoxel = layerSurfaceVoxels[maxLayer].innerVoxels;
            // layerSurfaceVoxels[maxLayer].boundaryVoxels.clear();
            if (oneLayerVoxel.empty()) continue;
            // Step 3: 備份 voxelData 到 voxelData2
            for(int y = 0; y < rawModel.infData.resolution[2]; y++)
            {
                for(int x = 0; x < rawModel.infData.resolution[1]; x++)
                {
                    for(int z = 0; z < rawModel.infData.resolution[0]; z++)
                    {
                        rawModel.voxelData2[y][x][z] = rawModel.voxelData[y][x][z];
                    }
                }
            }
            // Step 4: 創建 boundary voxels
            for (auto &v : oneLayerVoxel)
            {
                int x = v.locate.x;
                int y = v.locate.y;
                int z = v.locate.z;
                if (v.draw)
                {
                    // draw_voxel(v, v.color, background.voxel);
                    isoSurface.create_boundary_voxel(l, x, y, z, boundaryVoxel[layer]);
                }
                else // if not draw, then set distance 0
                {
                    // if((carving.emboss && (layer >= rawModel.choiceLayerNum - 3)) || (carving.engrave && (layer >= rawModel.choiceLayerNum - 3))) // 刻外面
                    if((carving.emboss && (layer <= rawModel.choiceLayerNum - 2)) || (carving.engrave && (layer <= rawModel.choiceLayerNum - 2))) // 刻裡面
                        rawModel.voxelData3[y][x][z] = 0;
                }
            }
            std::cout << "layer " << l << " surfaceNum: " << oneLayerVoxel.size() << std::endl;
            // std::cout << "layer " << l << " boundaryNum: " << layerSurfaceVoxels[l].boundaryVoxels.size() << std::endl;
            std::cout << "layer " << l << " boundaryNum: " << boundaryVoxel[layer].size() << std::endl;
            // Step 5: 對 voxelData 做 smoothing
            // laplacianSmoothing(3);
            // 根據 boundary voxels 畫出 Iso-Surface (橘色)
            // isoSurface.CreateIsoSurface2(l, 0.6f, glm::fvec3(1.0f, 0.6274f, 0.2588f), boundaryVoxel[layer], background.layerSurfaceBuffers[layer]);
            std::cout << "layer " << l << " triangleNum: " << background.layerSurfaceBuffers[layer].size() << std::endl;
            // layerSurfaceVoxels[l].boundaryVoxels.clear();
            // 還原 voxelData
            for(int y = 0; y < rawModel.infData.resolution[2]; y++)
                for(int x = 0; x < rawModel.infData.resolution[1]; x++)
                    for(int z = 0; z < rawModel.infData.resolution[0]; z++)
                        rawModel.voxelData[y][x][z] = rawModel.voxelData2[y][x][z];
            layer++;
        }
        // surface_time = glfwGetTime() - current_time_carving;
        // cout << "surface time: " << surface_time << " sec" << endl;
        current_time_s = glfwGetTime();
        // laplacianSmoothing1(3);
        smoothing_time = glfwGetTime() - current_time_s;
        cout << "smoothing time: " << smoothing_time << " sec" << endl;
        // laplacianSmoothing(3);
        write_file(rawModel.voxelData3);
        isoSurface.CreateIsoSurface4(0.3, carving.emboss, carving.engrave, background.surface2);
        drawModel.Surface_renew();
        for(int l = 0; l < rawModel.choiceLayerNum; l++)
        {
            boundaryVoxel[l].clear();
            // background.layerSurfaceBuffers[l].clear();
        }
        boundaryVoxel.clear();
        // read_raw_file();
    }
}
void renew_inner_voxel(std::vector<LayerSurfaceData> layerVoxels, int layerNum)
{
    background.innerVoxel.clear();
    // the layer close to inner voxel
    for (int l = 0; l < layerNum; l++)
    {
        // only draw inner voxels of the close layer
        if (l == rawModel.maxLayer)
        {
            int innerNum = layerVoxels[l].innerVoxels.size();
            std::vector<surfaceVoxel> innerVoxels = layerVoxels[l].innerVoxels;
            for (int v = 0; v < innerNum; v++)
            {
                int x = innerVoxels[v].locate.x;
                int y = innerVoxels[v].locate.y;
                int z = innerVoxels[v].locate.z;

                glm::fvec3 voxelColor = innerVoxels[v].color;
                // cout << "voxelColor: " << voxelColor.r << ", " << voxelColor.g << ", " << voxelColor.b << endl;
                draw_voxel(innerVoxels[v], voxelColor, background.innerVoxel);
            }
            std::cout << "layer " << l << std::endl;
            std::cout << "innerNum: " << rawModel.layerSurfaces[l].innerVoxels.size() << std::endl;
        }
    }
}

void create_bounding_box()
{
    background.boundingbox.clear();

    // glm::fvec3 max = {rawModel.infData.resolution[1], rawModel.infData.resolution[2], rawModel.infData.resolution[0]};
    glm::fvec3 max = {mapping.x_max, mapping.y_max, mapping.z_max};
    // glm::fvec3 min = {0.0f, 0.0f, 0.0f};
    glm::fvec3 min = {mapping.x_min, mapping.y_min, mapping.z_min};
    glm::fvec3 color = {0.0f, 0.0f, 1.0f};

    // bottom
    background.boundingbox.push_back(Vertex{{min.x, min.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{min.x, min.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{min.x, min.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{max.x, min.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{max.x, min.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{max.x, min.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{max.x, min.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{min.x, min.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    // top
    background.boundingbox.push_back(Vertex{{min.x, max.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{min.x, max.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{min.x, max.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{max.x, max.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{max.x, max.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{max.x, max.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{max.x, max.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{min.x, max.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    // surrounding
    background.boundingbox.push_back(Vertex{{min.x, min.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{min.x, max.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{min.x, min.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{min.x, max.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{max.x, min.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{max.x, max.y, max.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});

    background.boundingbox.push_back(Vertex{{max.x, min.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
    background.boundingbox.push_back(Vertex{{max.x, max.y, min.z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {color.r, color.g, color.b}});
}

void destroyBackground()
{
    background.boundingbox.clear();
    background.axis.clear();
    background.floor.clear();
    background.voxel.clear();
    background.innerVoxel.clear();
    background.engraveVoxel.clear();
    background.embossVoxel.clear();
    // background.surface.clear();
    for(int l = 0; l < rawModel.choiceLayerNum; l++)
    {
        background.layerSurfaceBuffers[l].clear();
    }
    background.layerSurfaceBuffers.clear();
    background.lattice_line.clear();
    background.lattice_plane.clear();
}

/*
    for (int d = 0; d < SOM.latticeData.typeNum[SOM.latticeData.type]; d++)
    {
        for (int h = 0; h < SOM.latticeData.height; h++)
        {
            for (int w = 0; w < SOM.latticeData.width; w++)
            {
                glm::fvec3 pos, pos_right, pos_up, pos_up_right;
                glm::fvec3 pos_tex, pos_right_tex, pos_up_tex, pos_up_right_tex;

                pos = SOM.latticeData.lattice[d][h][w];
                pos_tex = SOM.t_latticeData.lattice[0][h][w]; //{w * sx, h * sy, 0.5};

                if (w != SOM.latticeData.width - 1)
                {
                    pos_right = SOM.latticeData.lattice[d][h][w + 1];
                    pos_right_tex = SOM.t_latticeData.lattice[0][h][w + 1]; //{(w + 1) * sx, h * sy, 0.5};
                }

                if (h != SOM.latticeData.height - 1)
                {
                    pos_up = SOM.latticeData.lattice[d][h + 1][w];
                    pos_up_tex = SOM.t_latticeData.lattice[0][h + 1][w]; //{w * sx, (h + 1) * sy, 0.5};
                }

                if (w != SOM.latticeData.width - 1 && h != SOM.latticeData.height - 1)
                {
                    pos_up_right = SOM.latticeData.lattice[d][h + 1][w + 1];
                    pos_up_right_tex = SOM.t_latticeData.lattice[0][h + 1][w + 1]; // {(w + 1) * sx, (h + 1) * sy, 0.5};
                }

                glm::fvec3 n;
                if (d == 0)
                    n = glm::fvec3{0.0f, 1.0f, 0.0f};
                else if (d == 1)
                    n = glm::fvec3{0.0f, 0.0f, -1.0f};
                else if (d == 2)
                    n = glm::fvec3{-1.0f, 0.0f, 0.0f};
                else if (d == 3)
                    n = glm::fvec3{0.0f, 0.0f, 1.0f};
                else if (d == 4)
                    n = glm::fvec3{1.0f, 0.0f, 0.0f};
                else if (d == 5)
                    n = glm::fvec3{0.0f, -1.0f, 0.0f};

                if (d == 2 || d == 4)
                {
                    // draw lines
                    // --- (x, y) -> (x + 1, y)
                    if (w != SOM.latticeData.width - 1)
                    {
                        background.lattice_line.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.y, pos_tex.x, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_line.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.y, pos_right_tex.x, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                    // | (x, y) -> (x, y + 1)
                    if (h != SOM.latticeData.height - 1)
                    {
                        background.lattice_line.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.y, pos_tex.x, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_line.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.y, pos_up_tex.x, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                    // draw planes
                    // --- (x, y) -> (x + 1, y) and | (x, y) -> (x, y + 1)
                    if (w != SOM.latticeData.width - 1 && h != SOM.latticeData.height - 1)
                    {
                        background.lattice_plane.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.y, pos_up_tex.x, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.y, pos_tex.x, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.y, pos_right_tex.x, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});

                        background.lattice_plane.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.y, pos_right_tex.x, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_up_right.x, pos_up_right.y, pos_up_right.z}, {n.x, n.y, n.z}, {pos_up_right_tex.y, pos_up_right_tex.x, pos_up_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.y, pos_up_tex.x, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                }
                else // left, right
                {
                    // draw lines
                    // --- (x, y) -> (x + 1, y)
                    if (w != SOM.latticeData.width - 1)
                    {
                        background.lattice_line.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.x, pos_tex.y, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_line.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.x, pos_right_tex.y, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                    // | (x, y) -> (x, y + 1)
                    if (h != SOM.latticeData.height - 1)
                    {
                        background.lattice_line.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.x, pos_tex.y, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_line.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.x, pos_up_tex.y, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                    // draw planes
                    // --- (x, y) -> (x + 1, y) and | (x, y) -> (x, y + 1)
                    if (w != SOM.latticeData.width - 1 && h != SOM.latticeData.height - 1)
                    {
                        background.lattice_plane.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.x, pos_up_tex.y, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos.x, pos.y, pos.z}, {n.x, n.y, n.z}, {pos_tex.x, pos_tex.y, pos_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.x, pos_right_tex.y, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});

                        background.lattice_plane.push_back(Vertex{{pos_right.x, pos_right.y, pos_right.z}, {n.x, n.y, n.z}, {pos_right_tex.x, pos_right_tex.y, pos_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_up_right.x, pos_up_right.y, pos_up_right.z}, {n.x, n.y, n.z}, {pos_up_right_tex.x, pos_up_right_tex.y, pos_up_right_tex.z}, {1.0f, 1.0f, 1.0f}});
                        background.lattice_plane.push_back(Vertex{{pos_up.x, pos_up.y, pos_up.z}, {n.x, n.y, n.z}, {pos_up_tex.x, pos_up_tex.y, pos_up_tex.z}, {1.0f, 1.0f, 1.0f}});
                    }
                }
            }
        }
    }
// 20250424
void dilation(std::vector<LayerSurfaceData>& layerSurfaceVoxels)
{
    for(int y = 0; y < rawModel.infData.resolution[2]; y++)
    {
        for(int x = 0; x < rawModel.infData.resolution[1]; x++)
        {
            for(int z = 0; z < rawModel.infData.resolution[0]; z++)
            {
                int count = 0; // 6 neighbors
                // 3x3 mask (如果 mask 中 voxel color 有一個為白色，則全部變成白色)
                for(int dx = -1; dx <= 1; dx++)
                {
                    for(int dy = -1; dy <= 1; dy++)
                    {
                        for(int dz = -1; dz <= 1; dz++)
                        {
                            int nx = x + dx;
                            int ny = y + dy;
                            int nz = z + dz;
                            if(!is_in_resolution(nx, ny, nz)) continue;
                            if(abs(dx) + abs(dy) + abs(dz) <= 1) // 6 face
                            {
                                if(rawModel.rawData[ny][nx][nz].color.r - 1.0f < 0.0001 && rawModel.rawData[ny][nx][nz].color.g - 1.0f < 0.0001 && rawModel.rawData[ny][nx][nz].color.b - 1.0f < 0.0001)
                                    count++;
                            }
                        }
                    }
                }
                if(count != 0)
                {
                    for(int dx = -1; dx <= 1; dx++)
                    {
                        for(int dy = -1; dy <= 1; dy++)
                        {
                            for(int dz = -1; dz <= 1; dz++)
                            {
                                int nx = x + dx;
                                int ny = y + dy;
                                int nz = z + dz;
                                if(!is_in_resolution(nx, ny, nz)) continue;
                                if(abs(dx) + abs(dy) + abs(dz) <= 1) // 6 face
                                {
                                    rawModel.rawData[ny][nx][nz].color = glm::fvec3(1.0f, 1.0f, 1.0f);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // renew voxel color
    int layerNum = layerSurfaceVoxels.size();
    for(int l = 0; l < layerNum; l++)
    {
        std::vector<surfaceVoxel> &oneLayerVoxel = layerSurfaceVoxels[l].oneSurfaceVoxels;
        if (oneLayerVoxel.empty()) continue;
        for(auto &v: oneLayerVoxel)
        {
            int x = v.locate.x;
            int y = v.locate.y;
            int z = v.locate.z;
            if(rawModel.rawData[y][x][z].color.r - 1.0f < 0.0001 && rawModel.rawData[y][x][z].color.g - 1.0f < 0.0001 && rawModel.rawData[y][x][z].color.b - 1.0f < 0.0001)
                v.draw = true;
            else
                v.draw = false;
        }
    }
}

void erosion(std::vector<LayerSurfaceData>& layerSurfaceVoxels)
{
    for(int y = 0; y < rawModel.infData.resolution[2]; y++)
    {
        for(int x = 0; x < rawModel.infData.resolution[1]; x++)
        {
            for(int z = 0; z < rawModel.infData.resolution[0]; z++)
            {
                int count = 0; // 6 neighbors
                // 3x3 mask (如果 mask 中 voxel color 有一個為黑色，則全部變成黑色)
                for(int dx = -1; dx <= 1; dx++)
                {
                    for(int dy = -1; dy <= 1; dy++)
                    {
                        for(int dz = -1; dz <= 1; dz++)
                        {
                            int nx = x + dx;
                            int ny = y + dy;
                            int nz = z + dz;
                            if(!is_in_resolution(nx, ny, nz)) continue;
                            if(abs(dx) + abs(dy) + abs(dz) <= 1) // 6 face
                            {
                                if(!(rawModel.rawData[ny][nx][nz].color.r - 1.0f < 0.0001 && rawModel.rawData[ny][nx][nz].color.g - 1.0f < 0.0001 && rawModel.rawData[ny][nx][nz].color.b - 1.0f < 0.0001))
                                    count++;
                            }
                        }
                    }
                }
                if(count != 0)
                {
                    for(int dx = -1; dx <= 1; dx++)
                    {
                        for(int dy = -1; dy <= 1; dy++)
                        {
                            for(int dz = -1; dz <= 1; dz++)
                            {
                                int nx = x + dx;
                                int ny = y + dy;
                                int nz = z + dz;
                                if(!is_in_resolution(nx, ny, nz)) continue;
                                if(abs(dx) + abs(dy) + abs(dz) <= 1) // 6 face
                                {
                                    rawModel.rawData[ny][nx][nz].color = glm::fvec3(0.0f, 0.0f, 0.0f);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // renew voxel color
    int layerNum = layerSurfaceVoxels.size();
    for(int l = 0; l < layerNum; l++)
    {
        std::vector<surfaceVoxel> &oneLayerVoxel = layerSurfaceVoxels[l].oneSurfaceVoxels;
        if (oneLayerVoxel.empty()) continue;
        for(auto &v: oneLayerVoxel)
        {
            int x = v.locate.x;
            int y = v.locate.y;
            int z = v.locate.z;
            if(!(rawModel.rawData[y][x][z].color.r - 1.0f < 0.0001 && rawModel.rawData[y][x][z].color.g - 1.0f < 0.0001 && rawModel.rawData[y][x][z].color.b - 1.0f < 0.0001))
                v.draw = true;
            else
                v.draw = false;
        }
    }
}
*/
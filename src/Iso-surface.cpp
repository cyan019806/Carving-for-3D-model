#include "MarchingCubeTables.h"

iso_cls isoSurface;

iso_cls::iso_cls()
{
}
iso_cls::~iso_cls()
{
}

void iso_cls::create_color_map()
{
    // int colorSize = 64;
    // ColorMap = (glm::fvec3*)malloc(sizeof(glm::fvec3) * colorSize);
    for (int i = 0; i < 10; i++)
    {
        ColorMap[i].r = 0.1f * (i % 10);
        ColorMap[i].g = 0.0f;
        ColorMap[i].b = 0.1f * (i % 10);
    }
    ColorMap[10].r = 1.0f, ColorMap[10].g = 0.0f, ColorMap[10].b = 1.0f;
    for (int i = 11; i < 20; i++)
    {
        ColorMap[i].r = 1.0f - 0.1 * (i % 10);
        ColorMap[i].g = 0.0f;
        ColorMap[i].b = 1.0f;
    }
    ColorMap[20].r = 0.0f, ColorMap[20].g = 0.0f, ColorMap[20].b = 1.0f;
    for (int i = 21; i < 30; i++)
    {
        ColorMap[i].r = 0.0f;
        ColorMap[i].g = 0.1 * (i % 10);
        ColorMap[i].b = 1.0f;
    }
    ColorMap[30].r = 0.0f, ColorMap[30].g = 1.0f, ColorMap[30].b = 1.0f;
    for (int i = 31; i < 40; i++)
    {
        ColorMap[i].r = 0.0f;
        ColorMap[i].g = 1.0f;
        ColorMap[i].b = 1.0f - 0.1f * (i % 10);
    }
    ColorMap[40].r = 0.0f, ColorMap[40].g = 1.0f, ColorMap[40].b = 0.0f;
    for (int i = 41; i < 50; i++)
    {
        ColorMap[i].r = 0.1f * (i % 10);
        ColorMap[i].g = 1.0f;
        ColorMap[i].b = 0.0f;
    }
    ColorMap[50].r = 1.0f, ColorMap[50].g = 1.0f, ColorMap[50].b = 0.0f;
    for (int i = 51; i < 60; i++)
    {
        ColorMap[i].r = 1.0f;
        ColorMap[i].g = 1.0f - 0.1 * (i % 10);
        ColorMap[i].b = 0.0f;
    }
    ColorMap[60].r = 1.0f, ColorMap[60].g = 0.0f, ColorMap[60].b = 0.0f;
    for (int i = 61; i < 65; i++)
    {
        ColorMap[i].r = 1.0f;
        ColorMap[i].g = 0.1 * (i % 4);
        ColorMap[i].b = 0.1 * (i % 4);
    }
}

void iso_cls::delete_color_map()
{
    free(ColorMap);
}

glm::fvec3 iso_cls::crossProduct(glm::fvec3 v1, glm::fvec3 v2)
{
    glm::fvec3 v3;
    v3.x = v1.y * v2.z - v1.z * v2.y;
    v3.y = v1.z * v2.x - v1.x * v2.z;
    v3.z = v1.x * v2.y - v1.y * v2.x;
    return v3;
}

glm::fvec3 iso_cls::myNormalize(glm::fvec3 v)
{
    glm::fvec3 v1;
    double rate = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v1.x = v.x / rate;
    v1.y = v.x / rate;
    v1.z = v.x / rate;

    return v1;
}

void iso_cls::create_surface_voxel(int showDist)
{
    // clear
    // int layerNum = rawModel.layerSurfaces.size();
    // for(int l = 0; l < layerNum; l++)
    // {
    //     int surfaceNum = rawModel.layerSurfaces[l].surfaceVoxels.size();
    //     for(int v = 0; v < surfaceNum; v++)
    //     {
    //         rawModel.layerSurfaces[l].surfaceVoxels.clear();
    //     }
    // }
    // // find the choose layer voxel
    // layerNum = rawModel.choiceLayerNum;
    // rawModel.layerSurfaces.resize(layerNum);
    // for (int y = 0; y < rawModel.infData.resolution[2]; y++)
    // {
    //     for (int x = 0; x < rawModel.infData.resolution[1]; x++)
    //     {
    //         for (int z = 0; z < rawModel.infData.resolution[0]; z++)
    //         {
    //             // if ((rawModel.voxelData[y][x][z] <= showDist) && (rawModel.voxelData[y][x][z] > showDist - 1))
    //                 // num++;
    //             if(rawModel.rawData[y][x][z].layer == showDist)
    //             {
    //                 rawModel.layerSurfaces[num].surfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.0f, 0.8f}, {}});
    //                 rawModel.findSurfaceVoxel(y, x, z, rawModel.layerSurfaces[num].surfaceVoxels.size(), 0, SURFACE);
    //             }
    //             if(rawModel.rawData[y][x][z].layer > 0 && rawModel.rawData[y][x][z].layer < rawModel.initLayer + layerNum)
    //             {
    //                 rawModel.layerSurfaces[num].innerVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.2f, 0.7f}, {}});
    //                 rawModel.findSurfaceVoxel(y, x, z, rawModel.layerSurfaces[num].innerVoxels.size(), 0, INNER);
    //             }
    //             else
    //             {
    //                 rawModel.layerSurfaces[num].outterVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.2f, 0.7f}, {}});
    //                 rawModel.findSurfaceVoxel(y, x, z, rawModel.layerSurfaces[num].outterVoxels.size(), 0, OUTTER);
    //             }
    //         }
    //     }
    // }
    // // rawModel.surfaceNum = num;
    // // rawModel.surfaceVoxelData = (surfaceVoxel *)calloc(rawModel.surfaceNum, sizeof(surfaceVoxel));
    // num = 0;
    // for (int y = 0; y < rawModel.infData.resolution[2]; y++)
    // {
    //     for (int x = 0; x < rawModel.infData.resolution[1]; x++)
    //     {
    //         for (int z = 0; z < rawModel.infData.resolution[0]; z++)
    //         {
    //             if ((rawModel.voxelData[y][x][z] <= showDist) && (rawModel.voxelData[y][x][z] > showDist - 1))
    //             {
    //                 rawModel.surfaceVoxelData[num].locate = {(float)x, (float)y, (float)z};
    //                 if (SOM.t_latticeData.go == false)
    //                     rawModel.surfaceVoxelData[num].color = {1.0f, 1.0f, 1.0f};
    //                 set_surface_max_bounder(x, y, z, rawModel.surfaceMaxLocate);
    //                 set_surface_voxel_face_air(x, y, z, rawModel.surfaceVoxelData[num], showDist);
    //                 num++;
    //             }
    //         }
    //     }
    // }
}

// void iso_cls::update_k_layers_surface_voxel(int minLayer, int maxLayer)
void iso_cls::update_k_layers_surface_voxel(int layern, int showDist)
{
    // Clear only the specific layer being updated
    auto &currentLayer = rawModel.layerSurfaces[layern];
    currentLayer.surfaceVoxels.clear();
    currentLayer.innerVoxels.clear();

    int startLayer = rawModel.initLayer - 1;
    int endLayer = rawModel.initLayer + rawModel.choiceLayerNum;

    for (int y = 0; y < rawModel.infData.resolution[2]; y++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1]; x++)
        {
            for (int z = 0; z < rawModel.infData.resolution[0]; z++)
            {
                int voxelLayer = rawModel.rawData[y][x][z].layer;
                if (voxelLayer > startLayer && voxelLayer < endLayer) // surface
                {
                    // glm::fvec3 normal = rawModel.findNormal(x, y, z);
                    // currentLayer.surfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0, 0.0, 0.8}, {normal.x, normal.y, normal.z}});
                    currentLayer.surfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0, 0.0, 0.8}, {}});
                    rawModel.findSurfaceVoxel(y, x, z, currentLayer.surfaceVoxels.size() - 1, layern, SURFACE);
                    // isoSurface.findAllNormal(currentLayer.surfaceVoxels, showDist);

                }
                else if (voxelLayer == endLayer) // inner
                {
                    currentLayer.innerVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0, 0.2, 0.7}, {}});
                    rawModel.findSurfaceVoxel(y, x, z, currentLayer.innerVoxels.size() - 1, layern, INNER);
                }
            }
        }
    }
}

void iso_cls::create_inner_voxel()
{
    // int len = minIndex;
    // // clear voxel data
    // for (auto& layer : rawModel.innerVoxels)
    // {
    //     free(layer.data);
    //     layer.data = nullptr;
    //     layer.num = 0;
    // }
    // rawModel.innerVoxels.clear();
    // rawModel.innerVoxels.resize(len);
    // rawModel.totalInnerNum = 0;

    // for (int i = 0; i < rawModel.innerVoxels.size(); i++)
    // {
    //     int show_dist = (int)rawModel.maxDist - i; // inner -> outter
    //     int num = 0;
    //     for (int y = 0; y < rawModel.infData.resolution[2]; y++)
    //     {
    //         for (int x = 0; x < rawModel.infData.resolution[1]; x++)
    //         {
    //             for (int z = 0; z < rawModel.infData.resolution[0]; z++)
    //             {
    //                 if ((rawModel.voxelData[y][x][z] <= show_dist) && (rawModel.voxelData[y][x][z] > show_dist - 1))
    //                     num++;
    //             }
    //         }
    //     }

    //     rawModel.innerVoxels[i].data = (surfaceVoxel*)calloc(num, sizeof(surfaceVoxel));
    //     rawModel.innerVoxels[i].num = num;
    //     rawModel.totalInnerNum += num;
        
    //     int index = 0;
    //     for (int y = 0; y < rawModel.infData.resolution[2]; y++)
    //     {
    //         for (int x = 0; x < rawModel.infData.resolution[1]; x++)
    //         {
    //             for (int z = 0; z < rawModel.infData.resolution[0]; z++)
    //             {
    //                 if ((rawModel.voxelData[y][x][z] <= show_dist) && (rawModel.voxelData[y][x][z] > show_dist - 1))
    //                 {
    //                     rawModel.innerVoxels[i].data[index].locate = {(float)x, (float)y, (float)z};
    //                     if (!SOM.t_latticeData.go)
    //                         rawModel.innerVoxels[i].data[index].color = {0.41f, 0.41f, 0.41f}; // grey
    //                     set_surface_max_bounder(x, y, z, rawModel.innerVoxelMaxLocate);
    //                     set_surface_voxel_face_air(x, y, z, rawModel.innerVoxels[i].data[index], show_dist);
    //                     index++;
    //                 }
    //             }
    //         }
    //     }
    // }
}

void iso_cls::set_surface_max_bounder(int i, int j, int k, int *maxLocate)
{
    if(maxLocate[0] > i)
        maxLocate[0] = i;
    if(maxLocate[1] < j)
        maxLocate[1] = j;
    if(maxLocate[2] > k)
        maxLocate[2] = k;

    // if (rawModel.surfaceMaxLocate[0] > i)
    //     rawModel.surfaceMaxLocate[0] = i;
    // if (rawModel.surfaceMaxLocate[1] < j)
    //     rawModel.surfaceMaxLocate[1] = j;
    // if (rawModel.surfaceMaxLocate[2] < k)
    //     rawModel.surfaceMaxLocate[2] = k;
}

bool iso_cls::isFaceAir(int x, int y, int z, int showDist)
{
    if (x < 0 || y < 0 || z < 0 || x >= rawModel.infData.resolution[1] || y >= rawModel.infData.resolution[2] || z >= rawModel.infData.resolution[0] || (rawModel.voxelData[y][x][z] <= showDist && rawModel.voxelData[y][x][z] > showDist - 1))
        return false;
    else
        return true;
}

void iso_cls::set_surface_voxel_face_air(int x, int y, int z, surfaceVoxel &surfaceVoxel, int showDist)
{
    surfaceVoxel.faceAir[0] = isFaceAir(x, y, z - 1, showDist); // -z
    surfaceVoxel.faceAir[1] = isFaceAir(x, y, z + 1, showDist); // +z
    surfaceVoxel.faceAir[2] = isFaceAir(x - 1, y, z, showDist); // -x
    surfaceVoxel.faceAir[3] = isFaceAir(x + 1, y, z, showDist); // +x
    surfaceVoxel.faceAir[4] = isFaceAir(x, y - 1, z, showDist); // -y
    surfaceVoxel.faceAir[5] = isFaceAir(x, y + 1, z, showDist); // +y
}

void iso_cls::free_surface_voxel()
{
    // int idx = rawModel.centerIdx;
    // rawModel.layerSurfaces[idx].surfaceVoxels.clear();
    // free(rawModel.surfaceVoxelData);
    // rawModel.surfaceMaxLocate[0] = 0;
    // rawModel.surfaceMaxLocate[1] = 0;
    // rawModel.surfaceMaxLocate[2] = 0;
    // rawModel.surfaceNum = 0;
}

void iso_cls::free_layer_surface_voxel()
{
    // clear voxel data
    int layerNum = rawModel.layerSurfaces.size();
    for (int l = 0; l < layerNum; l++)
    {
        if(l == rawModel.maxLayer)
        {
            rawModel.layerSurfaces[l].surfaceVoxels.clear();
            rawModel.layerSurfaces[l].innerVoxels.clear();
        }
        rawModel.layerSurfaces[l].oneSurfaceVoxels.clear();
        rawModel.layerSurfaces[l].boundaryVoxels.clear();
    }
    rawModel.layerSurfaces.clear();
    // for (auto& layer : rawModel.layerSurfaces)
    // {
    //     free(layer.data);
    //     layer.data = nullptr;
    //     layer.num = 0;
    // }
    // rawModel.layerSurfaces.clear();

    // for(int i = 0; i < rawModel.layerSurfaces.size(); i++)
    // {
    //     free(rawModel.layerSurfaces[i].data);
    //     rawModel.layerSurfaces[i].num = 0;
    // }
    // rawModel.layerSurfaceMaxLocate[0] = 0;
    // rawModel.layerSurfaceMaxLocate[1] = 0;
    // rawModel.layerSurfaceMaxLocate[2] = 0;
    // rawModel.totalSurfaceNum = 0;
}

// glm::fvec3 iso_cls::findColor(int layern, int x, int y, int z, int n, int count, glm::fvec3 color)
// {
    // glm::ivec3 key(x, y, z);
    // auto it = surfaceVoxelMap.find(key);
    // if (it != surfaceVoxelMap.end()) {
    //     return it->second; // 找到該體素，回傳顏色
    // }
    // // 該體素不在表面體素中，如果其中一個鄰居有在其中就給它其顏色值
    // // int count = 0;
    // // glm::fvec3 color = glm::fvec3(0.0f, 0.0f, 0.0f);
    // if(x - n >= 0)
    // {
    //     glm::ivec3 key_1(x - n, y, z);
    //     auto left = surfaceVoxelMap.find(key_1);
    //     if (left != surfaceVoxelMap.end()) {
    //         count++;
    //         color += left->second;
    //     }
    // }
    // if(x + n <= rawModel.infData.resolution[1] - 1)
    // {
    //     glm::ivec3 key_2(x + n, y, z);
    //     auto right = surfaceVoxelMap.find(key_2);
    //     if (right != surfaceVoxelMap.end()) {
    //         count++;
    //         color += right->second;
    //     }
    // }
    // if(y - n >= 0)
    // {
    //     glm::ivec3 key_3(x, y - n, z);
    //     auto down = surfaceVoxelMap.find(key_3);
    //     if (down != surfaceVoxelMap.end()) {
    //         count++;
    //         color += down->second;
    //     }
    // }
    // if(y + n <= rawModel.infData.resolution[2] - 1)
    // {
    //     glm::ivec3 key_4(x, y + n, z);
    //     auto up = surfaceVoxelMap.find(key_4);
    //     if (up != surfaceVoxelMap.end()) {
    //         count++;
    //         color += up->second;
    //     }
    // }
    // if(z - n >= 0)
    // {
    //     glm::ivec3 key_5(x, y, z - n);
    //     auto front = surfaceVoxelMap.find(key_5);
    //     if (front != surfaceVoxelMap.end()) {
    //         count++;
    //         color += front->second;
    //     }
    // }
    // if(z + n <= rawModel.infData.resolution[0] - 1)
    // {
    //     glm::ivec3 key_6(x, y, z + n);
    //     auto back = surfaceVoxelMap.find(key_6);
    //     if (back != surfaceVoxelMap.end()) {
    //         count++;
    //         color += back->second;
    //     }
    // }
    // if(count == 0)
    //     color = glm::fvec3(1.0f, 1.0f, 1.0f); // 預設為白色
    // else
    //     color = glm::fvec3(color.x / count, color.y / count, color.z / count); 
    
    // return color;

    // int surfaceNum = rawModel.layerSurfaces[layern].surfaceVoxels.size();
    // for(int v = 0; v < surfaceNum; v++)
    // {
    //     int tx = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
    //     int ty = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
    //     int tz = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;

    //     if(x == tx && y == ty && z == tz)
    //         return rawModel.layerSurfaces[layern].surfaceVoxels[v].color;
    // }
    // return glm::fvec3(0.0f, 0.0f, 0.0f);
// }

void iso_cls::CreateTriangleModel(float isolevel, bool emboss, bool engrave, std::vector<Vertex>& Object)
{
    int cubeindex = 0;
    for (int i = 0; i < rawModel.infData.resolution[2] - 1; i++)
    {
        for (int j = 0; j < rawModel.infData.resolution[1] - 1; j++)
        {
            for (int k = 0; k < rawModel.infData.resolution[0] - 1; k++)
            {
                cubeindex = 0;
                // Determine the index into the edge table which tells us which vertices are inside of the surface
                if (rawModel.voxelData[i][j][k] >= isolevel)
                    cubeindex |= 1;
                if (rawModel.voxelData[i][j][k + 1] >= isolevel)
                    cubeindex |= 2;
                if (rawModel.voxelData[i + 1][j][k + 1] >= isolevel)
                    cubeindex |= 4;
                if (rawModel.voxelData[i + 1][j][k] >= isolevel)
                    cubeindex |= 8;
                if (rawModel.voxelData[i][j + 1][k] >= isolevel)
                    cubeindex |= 16;
                if (rawModel.voxelData[i][j + 1][k + 1] >= isolevel)
                    cubeindex |= 32;
                if (rawModel.voxelData[i + 1][j + 1][k + 1] >= isolevel)
                    cubeindex |= 64;
                if (rawModel.voxelData[i + 1][j + 1][k] >= isolevel)
                    cubeindex |= 128;

                // Cube is entirely in/out of the surface
                if (edgeTable[cubeindex] == 0)
                    continue;

                glm::fvec3 vertlist[12];
                glm::fvec3 p0 = {(float)j, (float)i, (float)k};
                glm::fvec3 p1 = {(float)j, (float)i, (float)k + 1};
                glm::fvec3 p2 = {(float)j, (float)i + 1, (float)k + 1};
                glm::fvec3 p3 = {(float)j, (float)i + 1, (float)k};
                glm::fvec3 p4 = {(float)j + 1, (float)i, (float)k};
                glm::fvec3 p5 = {(float)j + 1, (float)i, (float)k + 1};
                glm::fvec3 p6 = {(float)j + 1, (float)i + 1, (float)k + 1};
                glm::fvec3 p7 = {(float)j + 1, (float)i + 1, (float)k};

                if (edgeTable[cubeindex] & 1)
                    vertlist[0] = VertexInterp(isolevel, p0, p1, rawModel.voxelData[i][j][k], rawModel.voxelData[i][j][k + 1]); // 0 --- 1
                if (edgeTable[cubeindex] & 2)
                    vertlist[1] = VertexInterp(isolevel, p1, p2, rawModel.voxelData[i][j][k + 1], rawModel.voxelData[i + 1][j][k + 1]); // 1 --- 2
                if (edgeTable[cubeindex] & 4)
                    vertlist[2] = VertexInterp(isolevel, p2, p3, rawModel.voxelData[i + 1][j][k + 1], rawModel.voxelData[i + 1][j][k]); // 2 --- 3
                if (edgeTable[cubeindex] & 8)
                    vertlist[3] = VertexInterp(isolevel, p3, p0, rawModel.voxelData[i + 1][j][k], rawModel.voxelData[i][j][k]); // 3 --- 0
                if (edgeTable[cubeindex] & 16)
                    vertlist[4] = VertexInterp(isolevel, p4, p5, rawModel.voxelData[i][j + 1][k], rawModel.voxelData[i][j + 1][k + 1]); // 4 --- 5
                if (edgeTable[cubeindex] & 32)
                    vertlist[5] = VertexInterp(isolevel, p5, p6, rawModel.voxelData[i][j + 1][k + 1], rawModel.voxelData[i + 1][j + 1][k + 1]); // 5 --- 6
                if (edgeTable[cubeindex] & 64)
                    vertlist[6] = VertexInterp(isolevel, p6, p7, rawModel.voxelData[i + 1][j + 1][k + 1], rawModel.voxelData[i + 1][j + 1][k]); // 6 --- 7
                if (edgeTable[cubeindex] & 128)
                    vertlist[7] = VertexInterp(isolevel, p7, p4, rawModel.voxelData[i + 1][j + 1][k], rawModel.voxelData[i][j + 1][k]); // 7 --- 4
                if (edgeTable[cubeindex] & 256)
                    vertlist[8] = VertexInterp(isolevel, p0, p4, rawModel.voxelData[i][j][k], rawModel.voxelData[i][j + 1][k]); // 0 --- 4
                if (edgeTable[cubeindex] & 512)
                    vertlist[9] = VertexInterp(isolevel, p1, p5, rawModel.voxelData[i][j][k + 1], rawModel.voxelData[i][j + 1][k + 1]); // 1 --- 5
                if (edgeTable[cubeindex] & 1024)
                    vertlist[10] = VertexInterp(isolevel, p2, p6, rawModel.voxelData[i + 1][j][k + 1], rawModel.voxelData[i + 1][j + 1][k + 1]); // 2 --- 6
                if (edgeTable[cubeindex] & 2048)
                    vertlist[11] = VertexInterp(isolevel, p3, p7, rawModel.voxelData[i + 1][j][k], rawModel.voxelData[i + 1][j + 1][k]); // 3 --- 7

                // Create the triangle
                ntriang = 0;
                for (int l = 0; triTable[cubeindex][l] != -1; l += 3)
                {
                    glm::fvec3 v1, v2, normal;
                    v1.x = vertlist[triTable[cubeindex][l + 1]].x - vertlist[triTable[cubeindex][l]].x;
                    v1.y = vertlist[triTable[cubeindex][l + 1]].y - vertlist[triTable[cubeindex][l]].y;
                    v1.z = vertlist[triTable[cubeindex][l + 1]].z - vertlist[triTable[cubeindex][l]].z;

                    v2.x = vertlist[triTable[cubeindex][l + 2]].x - vertlist[triTable[cubeindex][l]].x;
                    v2.y = vertlist[triTable[cubeindex][l + 2]].y - vertlist[triTable[cubeindex][l]].y;
                    v2.z = vertlist[triTable[cubeindex][l + 2]].z - vertlist[triTable[cubeindex][l]].z;

                    normal = glm::normalize(glm::cross(v1, v2));

                    glm::fvec3 color = {1.0f, 1.0f, 1.0f};

                    Object.push_back(Vertex{{vertlist[triTable[cubeindex][l]].x, vertlist[triTable[cubeindex][l]].y, vertlist[triTable[cubeindex][l]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                    Object.push_back(Vertex{{vertlist[triTable[cubeindex][l + 1]].x, vertlist[triTable[cubeindex][l + 1]].y, vertlist[triTable[cubeindex][l + 1]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                    Object.push_back(Vertex{{vertlist[triTable[cubeindex][l + 2]].x, vertlist[triTable[cubeindex][l + 2]].y, vertlist[triTable[cubeindex][l + 2]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                    ntriang++;
                }
                // std::cout << background.curve.size() << "\n";
            }
        }
    }
// }
// rawModel.surfaceNum = num;
// std::cout << "Iso-surface End!" << std::endl;
}

bool iso_cls::in_boundary_voxel(int layern, int x, int y, int z)
{
    glm::ivec3 key(x, y, z);
    auto it = boundaryVoxelMap.find(key);
    if (it != boundaryVoxelMap.end()) {
        return it->second; // 如果體素在 boundary voxel 中回傳 true
    }
    return false;
    // int boundaryNum = rawModel.layerSurfaces[layern].boundaryVoxels.size();
    // for(int b = 0; b < boundaryNum; b++)
    // {
    //     surfaceVoxel boundaryVoxel = rawModel.layerSurfaces[layern].boundaryVoxels[b];
    //     int tx = boundaryVoxel.locate.x;
    //     int ty = boundaryVoxel.locate.y;
    //     int tz = boundaryVoxel.locate.z;
    //     if(tx == x && ty == y && tz == z)
    //         return true;
    // }
    // return false;
}

void iso_cls::CreateIsoSurface2(int layern, float isolevel, glm::fvec3 color, std::vector<surfaceVoxel>& voxels, std::vector<Vertex>& Object)
{
    int cubeindex = 0;
    // int boundaryNum = rawModel.layerSurfaces[layern].boundaryVoxels.size();
    int boundaryNum = voxels.size();
    int skipp = 0, totalCubes = 0;
    int ntriang = 0;
    for (auto& v : voxels)
    {
        int x = v.locate.x;
        int y = v.locate.y;
        int z = v.locate.z;
        // 跑 2x2x2 cube（因為 MC 是用 cube 的 8 個點）
        for (int dx = -1; dx <= 0; dx++)
        {
            for (int dy = -1; dy <= 0; dy++)
            {
                for (int dz = -1; dz <= 0; dz++)
                {
                    int i = y + dy;
                    int j = x + dx;
                    int k = z + dz;
                    if (!isoSurface.is_inside(j, i, k)) continue;
                    cubeindex = 0;

                    // Determine the index into the edge table which tells us which vertices are inside of the surface
                    double val[8] = {
                        rawModel.voxelData[i][j][k],
                        rawModel.voxelData[i][j][k + 1],
                        rawModel.voxelData[i + 1][j][k + 1],
                        rawModel.voxelData[i + 1][j][k],
                        rawModel.voxelData[i][j + 1][k],
                        rawModel.voxelData[i][j + 1][k + 1],
                        rawModel.voxelData[i + 1][j + 1][k + 1],
                        rawModel.voxelData[i + 1][j + 1][k]
                    };
                    // 確保至少一個 corner 的 voxelData 大於等於 isolevel，另一個小於
                    int above = 0, below = 0;
                    for (int c = 0; c < 8; c++) {
                        if (val[c] >= isolevel) above++;
                        else below++;
                    }
                    if (above == 0 || below == 0)
                        continue;  // 所有點都在同側，無需內插

                    // edge table index
                    if (val[0] >= isolevel) cubeindex |= 1;
                    if (val[1] >= isolevel) cubeindex |= 2;
                    if (val[2] >= isolevel) cubeindex |= 4;
                    if (val[3] >= isolevel) cubeindex |= 8;
                    if (val[4] >= isolevel) cubeindex |= 16;
                    if (val[5] >= isolevel) cubeindex |= 32;
                    if (val[6] >= isolevel) cubeindex |= 64;
                    if (val[7] >= isolevel) cubeindex |= 128;

                    // Cube is entirely in/out of the surface
                    if (edgeTable[cubeindex] == 0)
                        continue;

                    totalCubes++;

                    glm::fvec3 vertlist[12];
                    glm::fvec3 p0 = {(float)j, (float)i, (float)k};
                    glm::fvec3 p1 = {(float)j, (float)i, (float)k + 1};
                    glm::fvec3 p2 = {(float)j, (float)i + 1, (float)k + 1};
                    glm::fvec3 p3 = {(float)j, (float)i + 1, (float)k};
                    glm::fvec3 p4 = {(float)j + 1, (float)i, (float)k};
                    glm::fvec3 p5 = {(float)j + 1, (float)i, (float)k + 1};
                    glm::fvec3 p6 = {(float)j + 1, (float)i + 1, (float)k + 1};
                    glm::fvec3 p7 = {(float)j + 1, (float)i + 1, (float)k};

                    if (edgeTable[cubeindex] & 1)
                        vertlist[0] = VertexInterp(isolevel, p0, p1, rawModel.voxelData[i][j][k], rawModel.voxelData[i][j][k + 1]); // 0 --- 1
                    if (edgeTable[cubeindex] & 2)
                        vertlist[1] = VertexInterp(isolevel, p1, p2, rawModel.voxelData[i][j][k + 1], rawModel.voxelData[i + 1][j][k + 1]); // 1 --- 2
                    if (edgeTable[cubeindex] & 4)
                        vertlist[2] = VertexInterp(isolevel, p2, p3, rawModel.voxelData[i + 1][j][k + 1], rawModel.voxelData[i + 1][j][k]); // 2 --- 3
                    if (edgeTable[cubeindex] & 8)
                        vertlist[3] = VertexInterp(isolevel, p3, p0, rawModel.voxelData[i + 1][j][k], rawModel.voxelData[i][j][k]); // 3 --- 0
                    if (edgeTable[cubeindex] & 16)
                        vertlist[4] = VertexInterp(isolevel, p4, p5, rawModel.voxelData[i][j + 1][k], rawModel.voxelData[i][j + 1][k + 1]); // 4 --- 5
                    if (edgeTable[cubeindex] & 32)
                        vertlist[5] = VertexInterp(isolevel, p5, p6, rawModel.voxelData[i][j + 1][k + 1], rawModel.voxelData[i + 1][j + 1][k + 1]); // 5 --- 6
                    if (edgeTable[cubeindex] & 64)
                        vertlist[6] = VertexInterp(isolevel, p6, p7, rawModel.voxelData[i + 1][j + 1][k + 1], rawModel.voxelData[i + 1][j + 1][k]); // 6 --- 7
                    if (edgeTable[cubeindex] & 128)
                        vertlist[7] = VertexInterp(isolevel, p7, p4, rawModel.voxelData[i + 1][j + 1][k], rawModel.voxelData[i][j + 1][k]); // 7 --- 4
                    if (edgeTable[cubeindex] & 256)
                        vertlist[8] = VertexInterp(isolevel, p0, p4, rawModel.voxelData[i][j][k], rawModel.voxelData[i][j + 1][k]); // 0 --- 4
                    if (edgeTable[cubeindex] & 512)
                        vertlist[9] = VertexInterp(isolevel, p1, p5, rawModel.voxelData[i][j][k + 1], rawModel.voxelData[i][j + 1][k + 1]); // 1 --- 5
                    if (edgeTable[cubeindex] & 1024)
                        vertlist[10] = VertexInterp(isolevel, p2, p6, rawModel.voxelData[i + 1][j][k + 1], rawModel.voxelData[i + 1][j + 1][k + 1]); // 2 --- 6
                    if (edgeTable[cubeindex] & 2048)
                        vertlist[11] = VertexInterp(isolevel, p3, p7, rawModel.voxelData[i + 1][j][k], rawModel.voxelData[i + 1][j + 1][k]); // 3 --- 7

                    // Create the triangle
                    for (int l = 0; triTable[cubeindex][l] != -1; l += 3)
                    {
                        // myVector3D v1, v2, normal;
                        glm::fvec3 v1, v2, normal;
                        v1.x = vertlist[triTable[cubeindex][l + 1]].x - vertlist[triTable[cubeindex][l]].x;
                        v1.y = vertlist[triTable[cubeindex][l + 1]].y - vertlist[triTable[cubeindex][l]].y;
                        v1.z = vertlist[triTable[cubeindex][l + 1]].z - vertlist[triTable[cubeindex][l]].z;

                        v2.x = vertlist[triTable[cubeindex][l + 2]].x - vertlist[triTable[cubeindex][l]].x;
                        v2.y = vertlist[triTable[cubeindex][l + 2]].y - vertlist[triTable[cubeindex][l]].y;
                        v2.z = vertlist[triTable[cubeindex][l + 2]].z - vertlist[triTable[cubeindex][l]].z;

                        normal = glm::normalize(glm::cross(v1, v2));

                        Object.push_back(Vertex{{vertlist[triTable[cubeindex][l]].x, vertlist[triTable[cubeindex][l]].y, vertlist[triTable[cubeindex][l]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                        Object.push_back(Vertex{{vertlist[triTable[cubeindex][l + 1]].x, vertlist[triTable[cubeindex][l + 1]].y, vertlist[triTable[cubeindex][l + 1]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                        Object.push_back(Vertex{{vertlist[triTable[cubeindex][l + 2]].x, vertlist[triTable[cubeindex][l + 2]].y, vertlist[triTable[cubeindex][l + 2]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                        ntriang+=3;
                    }
                }
            }
        }
    }
    // std::cout << "totalCubes: " << totalCubes << ", skipped: " << skipp << std::endl;
}

void iso_cls::CreateIsoSurface3(int layern, float isolevel)
{
    int cubeindex = 0;
    int innerNum = rawModel.layerSurfaces[layern].innerVoxels.size();
    for (auto& v : rawModel.layerSurfaces[layern].innerVoxels)
    {
        int x = v.locate.x;
        int y = v.locate.y;
        int z = v.locate.z;
        // 跑 2x2x2 cube（因為 MC 是用 cube 的 8 個點）
        for (int dx = -1; dx <= 0; dx++)
        {
            for (int dy = -1; dy <= 0; dy++)
            {
                for (int dz = -1; dz <= 0; dz++)
                {
                    int i = y + dy;
                    int j = x + dx;
                    int k = z + dz;
                    if (!isoSurface.is_inside(j, i, k)) continue;
                    cubeindex = 0;

                    // Determine the index into the edge table which tells us which vertices are inside of the surface
                    double val[8] = {
                        rawModel.voxelData[i][j][k],
                        rawModel.voxelData[i][j][k + 1],
                        rawModel.voxelData[i + 1][j][k + 1],
                        rawModel.voxelData[i + 1][j][k],
                        rawModel.voxelData[i][j + 1][k],
                        rawModel.voxelData[i][j + 1][k + 1],
                        rawModel.voxelData[i + 1][j + 1][k + 1],
                        rawModel.voxelData[i + 1][j + 1][k]
                    };
                    // 確保至少一個 corner 的 voxelData 大於等於 isolevel，另一個小於
                    int above = 0, below = 0;
                    for (int c = 0; c < 8; c++) {
                        if (val[c] >= isolevel) above++;
                        else below++;
                    }
                    
                    if (above == 0 || below == 0)
                        continue;  // 所有點都在同側，無需內插

                    // edge table index
                    if (val[0] >= isolevel) cubeindex |= 1;
                    if (val[1] >= isolevel) cubeindex |= 2;
                    if (val[2] >= isolevel) cubeindex |= 4;
                    if (val[3] >= isolevel) cubeindex |= 8;
                    if (val[4] >= isolevel) cubeindex |= 16;
                    if (val[5] >= isolevel) cubeindex |= 32;
                    if (val[6] >= isolevel) cubeindex |= 64;
                    if (val[7] >= isolevel) cubeindex |= 128;

                    // Cube is entirely in/out of the surface
                    if (edgeTable[cubeindex] == 0)
                        continue;

                    glm::fvec3 vertlist[12];
                    glm::fvec3 p0 = {(float)j, (float)i, (float)k};
                    glm::fvec3 p1 = {(float)j, (float)i, (float)k + 1};
                    glm::fvec3 p2 = {(float)j, (float)i + 1, (float)k + 1};
                    glm::fvec3 p3 = {(float)j, (float)i + 1, (float)k};
                    glm::fvec3 p4 = {(float)j + 1, (float)i, (float)k};
                    glm::fvec3 p5 = {(float)j + 1, (float)i, (float)k + 1};
                    glm::fvec3 p6 = {(float)j + 1, (float)i + 1, (float)k + 1};
                    glm::fvec3 p7 = {(float)j + 1, (float)i + 1, (float)k};

                    if (edgeTable[cubeindex] & 1)
                        vertlist[0] = VertexInterp(isolevel, p0, p1, rawModel.voxelData[i][j][k], rawModel.voxelData[i][j][k + 1]); // 0 --- 1
                    if (edgeTable[cubeindex] & 2)
                        vertlist[1] = VertexInterp(isolevel, p1, p2, rawModel.voxelData[i][j][k + 1], rawModel.voxelData[i + 1][j][k + 1]); // 1 --- 2
                    if (edgeTable[cubeindex] & 4)
                        vertlist[2] = VertexInterp(isolevel, p2, p3, rawModel.voxelData[i + 1][j][k + 1], rawModel.voxelData[i + 1][j][k]); // 2 --- 3
                    if (edgeTable[cubeindex] & 8)
                        vertlist[3] = VertexInterp(isolevel, p3, p0, rawModel.voxelData[i + 1][j][k], rawModel.voxelData[i][j][k]); // 3 --- 0
                    if (edgeTable[cubeindex] & 16)
                        vertlist[4] = VertexInterp(isolevel, p4, p5, rawModel.voxelData[i][j + 1][k], rawModel.voxelData[i][j + 1][k + 1]); // 4 --- 5
                    if (edgeTable[cubeindex] & 32)
                        vertlist[5] = VertexInterp(isolevel, p5, p6, rawModel.voxelData[i][j + 1][k + 1], rawModel.voxelData[i + 1][j + 1][k + 1]); // 5 --- 6
                    if (edgeTable[cubeindex] & 64)
                        vertlist[6] = VertexInterp(isolevel, p6, p7, rawModel.voxelData[i + 1][j + 1][k + 1], rawModel.voxelData[i + 1][j + 1][k]); // 6 --- 7
                    if (edgeTable[cubeindex] & 128)
                        vertlist[7] = VertexInterp(isolevel, p7, p4, rawModel.voxelData[i + 1][j + 1][k], rawModel.voxelData[i][j + 1][k]); // 7 --- 4
                    if (edgeTable[cubeindex] & 256)
                        vertlist[8] = VertexInterp(isolevel, p0, p4, rawModel.voxelData[i][j][k], rawModel.voxelData[i][j + 1][k]); // 0 --- 4
                    if (edgeTable[cubeindex] & 512)
                        vertlist[9] = VertexInterp(isolevel, p1, p5, rawModel.voxelData[i][j][k + 1], rawModel.voxelData[i][j + 1][k + 1]); // 1 --- 5
                    if (edgeTable[cubeindex] & 1024)
                        vertlist[10] = VertexInterp(isolevel, p2, p6, rawModel.voxelData[i + 1][j][k + 1], rawModel.voxelData[i + 1][j + 1][k + 1]); // 2 --- 6
                    if (edgeTable[cubeindex] & 2048)
                        vertlist[11] = VertexInterp(isolevel, p3, p7, rawModel.voxelData[i + 1][j][k], rawModel.voxelData[i + 1][j + 1][k]); // 3 --- 7

                    // Create the triangle
                    ntriang = 0;
                    for (int l = 0; triTable[cubeindex][l] != -1; l += 3)
                    {
                        // myVector3D v1, v2, normal;
                        glm::fvec3 v1, v2, normal;
                        v1.x = vertlist[triTable[cubeindex][l + 1]].x - vertlist[triTable[cubeindex][l]].x;
                        v1.y = vertlist[triTable[cubeindex][l + 1]].y - vertlist[triTable[cubeindex][l]].y;
                        v1.z = vertlist[triTable[cubeindex][l + 1]].z - vertlist[triTable[cubeindex][l]].z;

                        v2.x = vertlist[triTable[cubeindex][l + 2]].x - vertlist[triTable[cubeindex][l]].x;
                        v2.y = vertlist[triTable[cubeindex][l + 2]].y - vertlist[triTable[cubeindex][l]].y;
                        v2.z = vertlist[triTable[cubeindex][l + 2]].z - vertlist[triTable[cubeindex][l]].z;

                        normal = glm::normalize(glm::cross(v1, v2));

                        // glm::fvec3 color = {1.0f, 0.6470f, 0.7450f}; // 淡橘色
                        glm::fvec3 color = {0.0f, 0.4470f, 0.8901f}; // 深藍色

                        background.innerSurface.push_back(Vertex{{vertlist[triTable[cubeindex][l]].x, vertlist[triTable[cubeindex][l]].y, vertlist[triTable[cubeindex][l]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                        background.innerSurface.push_back(Vertex{{vertlist[triTable[cubeindex][l + 1]].x, vertlist[triTable[cubeindex][l + 1]].y, vertlist[triTable[cubeindex][l + 1]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                        background.innerSurface.push_back(Vertex{{vertlist[triTable[cubeindex][l + 2]].x, vertlist[triTable[cubeindex][l + 2]].y, vertlist[triTable[cubeindex][l + 2]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                        ntriang++;
                    }           
                }
            }
        }
    }
    // std::cout << "totalCubes: " << totalCubes << ", skipped: " << skipp << std::endl;
}

void iso_cls::CreateIsoSurface4(float isolevel, bool emboss, bool engrave, std::vector<Vertex>& Object)
{
    int cubeindex = 0;
    for (int i = 0; i < (rawModel.infData.resolution[2] - 1); i++)
    {
        for (int j = 0; j < rawModel.infData.resolution[1] - 1; j++)
        {
            for (int k = 0; k < rawModel.infData.resolution[0] - 1; k++)
            {
                cubeindex = 0;
                // Determine the index into the edge table which tells us which vertices are inside of the surface
                if (rawModel.voxelData3[i][j][k] >= isolevel)
                    cubeindex |= 1;
                if (rawModel.voxelData3[i][j][k + 1] >= isolevel)
                    cubeindex |= 2;
                if (rawModel.voxelData3[i + 1][j][k + 1] >= isolevel)
                    cubeindex |= 4;
                if (rawModel.voxelData3[i + 1][j][k] >= isolevel)
                    cubeindex |= 8;
                if (rawModel.voxelData3[i][j + 1][k] >= isolevel)
                    cubeindex |= 16;
                if (rawModel.voxelData3[i][j + 1][k + 1] >= isolevel)
                    cubeindex |= 32;
                if (rawModel.voxelData3[i + 1][j + 1][k + 1] >= isolevel)
                    cubeindex |= 64;
                if (rawModel.voxelData3[i + 1][j + 1][k] >= isolevel)
                    cubeindex |= 128;

                // Cube is entirely in/out of the surface
                if (edgeTable[cubeindex] == 0)
                    continue;

                glm::fvec3 vertlist[12];
                glm::fvec3 p0 = {(float)j, (float)i, (float)k};
                glm::fvec3 p1 = {(float)j, (float)i, (float)k + 1};
                glm::fvec3 p2 = {(float)j, (float)i + 1, (float)k + 1};
                glm::fvec3 p3 = {(float)j, (float)i + 1, (float)k};
                glm::fvec3 p4 = {(float)j + 1, (float)i, (float)k};
                glm::fvec3 p5 = {(float)j + 1, (float)i, (float)k + 1};
                glm::fvec3 p6 = {(float)j + 1, (float)i + 1, (float)k + 1};
                glm::fvec3 p7 = {(float)j + 1, (float)i + 1, (float)k};

                if (edgeTable[cubeindex] & 1)
                    vertlist[0] = VertexInterp(isolevel, p0, p1, rawModel.voxelData3[i][j][k], rawModel.voxelData3[i][j][k + 1]); // 0 --- 1
                if (edgeTable[cubeindex] & 2)
                    vertlist[1] = VertexInterp(isolevel, p1, p2, rawModel.voxelData3[i][j][k + 1], rawModel.voxelData3[i + 1][j][k + 1]); // 1 --- 2
                if (edgeTable[cubeindex] & 4)
                    vertlist[2] = VertexInterp(isolevel, p2, p3, rawModel.voxelData3[i + 1][j][k + 1], rawModel.voxelData3[i + 1][j][k]); // 2 --- 3
                if (edgeTable[cubeindex] & 8)
                    vertlist[3] = VertexInterp(isolevel, p3, p0, rawModel.voxelData3[i + 1][j][k], rawModel.voxelData3[i][j][k]); // 3 --- 0
                if (edgeTable[cubeindex] & 16)
                    vertlist[4] = VertexInterp(isolevel, p4, p5, rawModel.voxelData3[i][j + 1][k], rawModel.voxelData3[i][j + 1][k + 1]); // 4 --- 5
                if (edgeTable[cubeindex] & 32)
                    vertlist[5] = VertexInterp(isolevel, p5, p6, rawModel.voxelData3[i][j + 1][k + 1], rawModel.voxelData3[i + 1][j + 1][k + 1]); // 5 --- 6
                if (edgeTable[cubeindex] & 64)
                    vertlist[6] = VertexInterp(isolevel, p6, p7, rawModel.voxelData3[i + 1][j + 1][k + 1], rawModel.voxelData3[i + 1][j + 1][k]); // 6 --- 7
                if (edgeTable[cubeindex] & 128)
                    vertlist[7] = VertexInterp(isolevel, p7, p4, rawModel.voxelData3[i + 1][j + 1][k], rawModel.voxelData3[i][j + 1][k]); // 7 --- 4
                if (edgeTable[cubeindex] & 256)
                    vertlist[8] = VertexInterp(isolevel, p0, p4, rawModel.voxelData3[i][j][k], rawModel.voxelData3[i][j + 1][k]); // 0 --- 4
                if (edgeTable[cubeindex] & 512)
                    vertlist[9] = VertexInterp(isolevel, p1, p5, rawModel.voxelData3[i][j][k + 1], rawModel.voxelData3[i][j + 1][k + 1]); // 1 --- 5
                if (edgeTable[cubeindex] & 1024)
                    vertlist[10] = VertexInterp(isolevel, p2, p6, rawModel.voxelData3[i + 1][j][k + 1], rawModel.voxelData3[i + 1][j + 1][k + 1]); // 2 --- 6
                if (edgeTable[cubeindex] & 2048)
                    vertlist[11] = VertexInterp(isolevel, p3, p7, rawModel.voxelData3[i + 1][j][k], rawModel.voxelData3[i + 1][j + 1][k]); // 3 --- 7

                // Create the triangle
                ntriang = 0;
                for (int l = 0; triTable[cubeindex][l] != -1; l += 3)
                {
                    // myVector3D v1, v2, normal;
                    glm::fvec3 v1, v2, normal;
                    v1.x = vertlist[triTable[cubeindex][l + 1]].x - vertlist[triTable[cubeindex][l]].x;
                    v1.y = vertlist[triTable[cubeindex][l + 1]].y - vertlist[triTable[cubeindex][l]].y;
                    v1.z = vertlist[triTable[cubeindex][l + 1]].z - vertlist[triTable[cubeindex][l]].z;

                    v2.x = vertlist[triTable[cubeindex][l + 2]].x - vertlist[triTable[cubeindex][l]].x;
                    v2.y = vertlist[triTable[cubeindex][l + 2]].y - vertlist[triTable[cubeindex][l]].y;
                    v2.z = vertlist[triTable[cubeindex][l + 2]].z - vertlist[triTable[cubeindex][l]].z;

                    normal = glm::normalize(glm::cross(v1, v2));

                    glm::fvec3 color = {1.0f, 1.0f, 1.0f};
                    
                    Object.push_back(Vertex{{vertlist[triTable[cubeindex][l]].x, vertlist[triTable[cubeindex][l]].y, vertlist[triTable[cubeindex][l]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                    Object.push_back(Vertex{{vertlist[triTable[cubeindex][l + 1]].x, vertlist[triTable[cubeindex][l + 1]].y, vertlist[triTable[cubeindex][l + 1]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                    Object.push_back(Vertex{{vertlist[triTable[cubeindex][l + 2]].x, vertlist[triTable[cubeindex][l + 2]].y, vertlist[triTable[cubeindex][l + 2]].z}, {normal.x, normal.y, normal.z}, {0.0, 0.0, 0.0}, {color.r, color.g, color.b}});
                    ntriang++;
                }
                // std::cout << background.curve.size() << "\n";
            }
        }
    }
// }
// rawModel.surfaceNum = num;
// std::cout << "Iso-surface End!" << std::endl;
}

glm::fvec3 iso_cls::VertexInterp(float isolevel, glm::fvec3 p1, glm::fvec3 p2, double valp1, double valp2)
{
    double mu;
    glm::fvec3 p;

    if (abs(isolevel - valp1) < 0.00001)
        return (p1);
    if (abs(isolevel - valp2) < 0.00001)
        return (p2);
    if (abs(valp1 - valp2) < 0.00001)
        return (p1);
    mu = (isolevel - valp1) / (valp2 - valp1);

    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return p;
}

bool iso_cls::is_inside(int x, int y, int z)
{
    if(x >= 0 && x <= rawModel.infData.resolution[1] - 1 && y >= 0 && y <= rawModel.infData.resolution[2] - 1 && z >= 0 && z <= rawModel.infData.resolution[0] - 1)
        return true;
    return false;
}

void iso_cls::create_boundary_voxel(int layern, int x, int y, int z, std::vector<surfaceVoxel>&voxel)
{
    std::unordered_set<glm::ivec3, ivec3_hash> visited;
    visited.clear(); // reset each voxel
    voxel.push_back(surfaceVoxel{{x, y, z}, {1.0f, 1.0f, 0.8f}, {}});
    for(int dx = -1; dx <= 1; dx++)
    {
        for(int dy = -1; dy <= 1; dy++)
        {
            for(int dz = -1; dz <= 1; dz++)
            {
                if(!is_inside(x + dx, y + dy, z + dz)) continue; // not in boundary range, jump
                if(dx == 0 && dy == 0 && dz == 0) continue; // surface voxel alreadey push, jump

                glm::ivec3 neighbor(x + dx, y + dy, z + dz);
                if (visited.count(neighbor)) continue;
                    visited.insert(neighbor);

                // if not surface Voxel, change distance value
                if(abs(dx) + abs(dy) + abs(dz) == 1) // face adjacent
                    rawModel.voxelData[y + dy][x + dx][z + dz] = 1;
                else if(abs(dx) + abs(dy) + abs(dz) == 2) // edge adjacent
                    rawModel.voxelData[y + dy][x + dx][z + dz] = sqrt(2);
                else if(abs(dx) + abs(dy) + abs(dz) == 3) // point adjacent
                    rawModel.voxelData[y + dy][x + dx][z + dz] = sqrt(3);
                voxel.push_back(surfaceVoxel{{x + dx, y + dy, z + dz}, {1.0f, 0.0f, 0.8f}, {}});
            }
        }
    }
    // std::cout << "boundaryNum: " << rawModel.layerSurfaces[layern].boundaryVoxels.size() << std::endl;
}
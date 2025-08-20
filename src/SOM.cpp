#include "SOM.h"
#include <time.h>

#include <glm/glm.hpp>
#include <iostream>

som_cls SOM;

glm::fvec3 **lattice;
glm::fvec3 *dataset;

int datasetNum = 0;

som_cls::som_cls()
{
}

som_cls::~som_cls()
{
}

LatData_t som_cls::init_SOM(LatData_t data)
{
    data.width = 16;
    data.height = 16;
    data.depth = 6;

    data.iter = 0;
    data.max_iter = 120000;

    data.learning_rate = 0.05;
    data.learning_rate_now = 0.05;

    data.radius = data.width / 2.0;
    data.radius_now = data.width / 2.0;

    data.type = 1; // 0: plane, 1: cylinder, 2: donut, 3: ball
    data.typeNum[0] = 1;
    data.typeNum[1] = 1;
    data.typeNum[2] = 1;
    data.typeNum[3] = 6;
    data.typeNum[4] = data.depth;


    latticeAnchorPoints[0] = {0, 0, 0};
    latticeAnchorPoints[1] = {0, 0, data.depth - 1};
    latticeAnchorPoints[2] = {data.width - 1, 0, data.depth - 1};
    latticeAnchorPoints[3] = {data.width - 1, 0, 0};
    latticeAnchorPoints[4] = {0, data.height - 1, 0};
    latticeAnchorPoints[5] = {0, data.height - 1, data.depth - 1};
    latticeAnchorPoints[6] = {data.width - 1, data.height - 1, data.depth - 1};
    latticeAnchorPoints[7] = {data.width - 1, data.height - 1, 0};
    latticeAnchorPoints[8] = {(data.width - 1) / 2, (data.height - 1) / 2, 0};
    latticeAnchorPoints[9] = {(data.width - 1) / 2, (data.height - 1) / 2, data.depth - 1};
    latticeAnchorPoints[10] = {0, (data.height - 1) / 2, (data.depth - 1) / 2};
    latticeAnchorPoints[11] = {data.width - 1, (data.height - 1) / 2, (data.depth - 1) / 2};
    latticeAnchorPoints[12] = {(data.width - 1) / 2, 0, (data.depth - 1) / 2};
    latticeAnchorPoints[13] = {(data.width - 1) / 2, data.height - 1, (data.depth - 1) / 2};

    textureAnchorPoints[0] = {0, 0};
    textureAnchorPoints[1] = {data.width - 1, 0};
    textureAnchorPoints[2] = {0, data.height - 1};
    textureAnchorPoints[3] = {data.width - 1, data.height - 1};
    textureAnchorPoints[4] = {(data.width - 1) / 2, (data.height - 1) / 2};

    return data;
}

LatData_t som_cls::init_texLat(LatData_t data)
{
    data.width = 16;
    data.height = 16;
    data.depth = 1; // plane

    data.iter = 0;
    data.max_iter = 120000;

    data.learning_rate = 0.005;
    data.learning_rate_now = 0.005;

    data.radius = data.width / 2.0;
    data.radius_now = data.width / 2.0;

    return data;
}

void som_cls::create_SOM(std::vector<surfaceVoxel> voxelData, int voxelNum, glm::ivec3 max, glm::ivec3 min, int type)
{
    srand(time(0));
    // initialize
    // lattie to voxel
    latticeData = init_SOM(latticeData);
    
    latticeData.type = type;

    latticeData.lattice = createLattice(latticeData, voxelData, voxelNum, max, min);
    inputData.input = createDataset(inputData, voxelData, voxelNum);
    inputData.num = voxelNum;
    // lattice to texture
    t_latticeData = init_texLat(t_latticeData);

    t_latticeData.lattice = createTexLat();
    t_inputData.input = createTexture();

    datasetNum = voxelNum;
}

glm::fvec3 som_cls::findNearestVoxel(glm::fvec3 init_anchor, std::vector<surfaceVoxel> voxelData, int voxelNum)
{
    double minDist = DBL_MAX;
    glm::fvec3 nearest = init_anchor;

    for (int i = 0; i < voxelNum; i++)
    {
        glm::fvec3 voxelPos = voxelData[i].locate;
        double dist = glm::distance(init_anchor, voxelPos);
        if (dist < minDist)
        {
            minDist = dist;
            nearest = voxelPos;
        }
    }
    return nearest;
}

glm::fvec3 ***som_cls::createLattice(LatData_t data, std::vector<surfaceVoxel> voxelData, int voxelNum, glm::ivec3 max, glm::ivec3 min)
{
    // set lattice size
    data.lattice = (glm::fvec3 ***)malloc(sizeof(glm::fvec3 **) * data.typeNum[data.type]);
    for (int d = 0; d < data.typeNum[data.type]; d++)
    {
        data.lattice[d] = (glm::fvec3 **)malloc(sizeof(glm::fvec3 *) * data.height);
        for (int h = 0; h < data.height; h++)
        {
            data.lattice[d][h] = (glm::fvec3 *)malloc(sizeof(glm::fvec3) * data.width);
        }
    }
    glm::ivec3 size = max - min;
    if (data.type == PLANE)
    {
        for (int h = 0; h < data.height; h++)
        {
            for (int w = 0; w < data.width; w++)
            {
                double i0, j0, k0;
                double ratio_w = (double)w / (double)(data.width - 1);
                double ratio_h = (double)h / (double)(data.height - 1);

                i0 = (double)size[0] * ratio_w + min[0];
                j0 = size[1] + min[1];
                k0 = (double)size[2] * ratio_h + min[2];

                data.lattice[0][h][w] = {i0, j0, k0};
            }
        }
    }
    else if (data.type == CYLINDER) // cylinder
    {
        for (int h = 0; h < data.height; h++)
        {
            for (int w = 0; w < data.width; w++)
            {
                double i0, j0, k0;

                double ratio_w = (double)w / (double)(data.width - 1);
                double ratio_h = (double)h / (double)(data.height - 1);

                i0 = ((double)size[0] / 2.0) * cos(2 * PI * ratio_w) + (size[0] / 2) + min[0];
                j0 = ratio_h * (double)size[1] + min[1];
                k0 = ((double)size[2] / 2.0) * sin(2 * PI * ratio_w) + (size[2] / 2) + min[2];
                // std::cout << i0 << ", " << size[0] << std::endl;
                data.lattice[0][h][w] = {i0, j0, k0};
            }
        }
    }
    else if (data.type == DONUT) // donut
    {
        float R = (size[2] + size[1]) / 4;
        float r = R / 5.0;
        for (int h = 0; h < data.height; h++)
        {
            for (int w = 0; w < data.width; w++)
            {
                double fi = 2 * PI * (double)w / (double)(data.width - 1);
                double theta = 2 * PI * (double)h / (double)(data.height - 1);

                double i0 = r * cos(theta) + ((double)size[0] / 2.0) + min[0];
                double j0 = cos(fi) * (R + r * sin(theta)) + ((double)size[1] / 2.0) + min[1];
                double k0 = sin(fi) * (R + r * sin(theta)) + ((double)size[2] / 2.0) + min[2];
                data.lattice[0][h][w] = {i0, j0, k0};
            }
        }
    }
    else if (latticeData.type == BALL)
    {
        double i0, j0, k0;
        for (int h = 0; h < data.height; h++)
        {
            for (int w = 0; w < data.width; w++)
            {
                double ratio_w = (double)w / (double)(data.width - 1);
                double ratio_h = (double)h / (double)(data.height - 1);
                // 0 x-z plane up
                i0 = size[0] * ratio_w + min[0];
                j0 = size[1] + min[1];
                k0 = size[2] * ratio_h + min[2];
                data.lattice[0][h][w] = {i0, j0, k0};
    
                // 5 x-z plane down
                i0 = size[0] * ratio_w + min[0];
                j0 = 0 + min[1];
                k0 = size[2] * ratio_h + min[2];
                data.lattice[5][h][w] = {i0, j0, k0};
    
                // 1 x-y plane back
                i0 = size[0] * ratio_w + min[0];
                j0 = size[1] * ratio_h + min[1];
                k0 = 0 + min[2];
                data.lattice[1][h][w] = {i0, j0, k0};
    
                // 3 x-y plane front
                i0 = size[0] * ratio_w + min[0];
                j0 = size[1] * ratio_h + min[1];
                k0 = size[2] + min[2];
                data.lattice[3][h][w] = {i0, j0, k0};
    
                // 2 y-z plane left
                i0 = 0 + min[0];
                j0 = size[1] * ratio_w + min[1];
                k0 = size[2] * ratio_h + min[2];
                data.lattice[2][h][w] = {i0, j0, k0};
    
                // 4 y-z plane right
                i0 = size[0] + min[0];
                j0 = size[1] * ratio_w + min[1];
                k0 = size[2] * ratio_h + min[2];
                data.lattice[4][h][w] = {i0, j0, k0};
            }
        }
        // step 2. 再單獨設定 anchor points (一次性完成，不要包在其他迴圈中!)
        for(auto anchor: latticeAnchorPoints)
        {
            glm::fvec3 init_anchor_pos = glm::fvec3(
                min.x + (anchor.x / float(data.width - 1)) * size.x,
                min.y + (anchor.y / float(data.height - 1)) * size.y,
                min.z + (anchor.z / float(data.depth - 1)) * size.z
            );

            glm::fvec3 corrected_anchor = findNearestVoxel(init_anchor_pos, voxelData, voxelNum);

            // 確保 anchor 在範圍內
            if (anchor.z < data.typeNum[data.type] && anchor.y < data.height && anchor.x < data.width)
            {
                data.lattice[anchor.z][anchor.y][anchor.x] = corrected_anchor;
            }
        }
        // if (isLatticeAnchor(j, i, k, latticeAnchorPoints)) continue;
    }
    else if (data.type == THREED)
    {
        double i0, j0, k0;
        for (int d = 0; d < data.typeNum[data.type]; d++)
        {
            for (int h = 0; h < data.height; h++)
            {
                for (int w = 0; w < data.width; w++)
                {
                    double ratio_w = (double)w / (double)(data.width - 1);
                    double ratio_h = (double)h / (double)(data.height - 1);
                    double ratio_d = (double)d / (double)(data.typeNum[data.type] - 1);

                    i0 = (double)size[0] * ratio_w + min[0];
                    j0 = (double)size[1] * ratio_h + min[1];
                    k0 = (double)size[2] * ratio_d + min[2];

                    data.lattice[d][h][w] = {i0, j0, k0};
                }
            }
        }
    }
    return data.lattice;
}

glm::fvec3 *som_cls::createDataset(InputData_t data, std::vector<surfaceVoxel> voxelData, int voxelNum)
{
    data.input = (glm::fvec3 *)malloc(sizeof(glm::fvec3) * voxelNum);
    for (int i = 0; i < voxelNum; i++)
    {
        data.input[i].x = voxelData[i].locate.x;
        data.input[i].y = voxelData[i].locate.y;
        data.input[i].z = voxelData[i].locate.z;
    }

    return data.input;
}

// glm::fvec3 ***som_cls::createTexLat()
// {
//     // set lattice size (depth = 1)
//     glm::fvec3 ***data = (glm::fvec3 ***)malloc(sizeof(glm::fvec3 **) * t_latticeData.depth);
//     for (int d = 0; d < t_latticeData.depth; d++)
//     {
//         data[d] = (glm::fvec3 **)malloc(sizeof(glm::fvec3 *) * t_latticeData.height);
//         for (int h = 0; h < t_latticeData.height; h++)
//         {
//             data[d][h] = (glm::fvec3 *)malloc(sizeof(glm::fvec3) * t_latticeData.width);
//             for(int w = 0; w < t_latticeData.width; w++)
//             {
//                 double i0 = (double)rand() / (double)(RAND_MAX + 1.0); // float between [0, 1)
//                 double j0 = (double)rand() / (double)(RAND_MAX + 1.0);
//                 double k0 = (double)rand() / (double)(RAND_MAX + 1.0);

//                 data[d][h][w] = {i0, j0, k0};
//             }
//         }
//     }
//     return data;
// }

bool som_cls::isTextureAnchor(int w, int h, glm::ivec2 archorPoint[5]) {
    for(int i = 0; i < 5; i++)
        if (w == archorPoint[i].x && h == archorPoint[i].y)
            return true;
    return false;
}

bool som_cls::isLatticeAnchor(int w, int h, int d, glm::ivec3 archorPoint[14]) {
    for(int i = 0; i < 14; i++)
        if (w == archorPoint[i].x && h == archorPoint[i].y && d == archorPoint[i].z)
            return true;
    return false;
}

glm::fvec3 ***som_cls::createTexLat()
{
    glm::fvec3 ***data = (glm::fvec3 ***)malloc(sizeof(glm::fvec3 **) * t_latticeData.depth);
    for (int d = 0; d < t_latticeData.depth; d++)
    {
        data[d] = (glm::fvec3 **)malloc(sizeof(glm::fvec3 *) * t_latticeData.height);
        for (int h = 0; h < t_latticeData.height; h++)
        {
            data[d][h] = (glm::fvec3 *)malloc(sizeof(glm::fvec3) * t_latticeData.width);
            for(int w = 0; w < t_latticeData.width; w++)
            {
                // if (isAnchor(w, h, textureAnchorPoint)) {
                    // if (w == textureAnchorPoint[0].x && h == textureAnchorPoint[0].y)
                    //     data[d][h][w] = glm::fvec3(0.0f, 0.0f, 0.0f);
                    // else if (w == textureAnchorPoint[1].x && h == textureAnchorPoint[1].y)
                    //     data[d][h][w] = glm::fvec3(2.0f, 0.0f, 0.0f);
                    // else if (w == textureAnchorPoint[2].x && h == textureAnchorPoint[2].y)
                    //     data[d][h][w] = glm::fvec3(0.0f, 2.0f, 0.0f);
                    // else if (w == textureAnchorPoint[3].x && h == textureAnchorPoint[3].y)
                    //     data[d][h][w] = glm::fvec3(2.0f, 2.0f, 0.0f);
                    // else if (w == textureAnchorPoint[4].x && h == textureAnchorPoint[4].y)
                    //     data[d][h][w] = glm::fvec3(1.0f, 1.0f, 0.0f);
                // }
                // else {
                    double i0 = (double)rand() / (double)(RAND_MAX + 1.0); // float between [0, 2)
                    double j0 = (double)rand() / (double)(RAND_MAX + 1.0);
                    double k0 = (double)rand() / (double)(RAND_MAX + 1.0);
                    data[d][h][w] = glm::fvec3(i0, j0, k0);
                    // float tex_u = ((float)w / (float)(t_latticeData.width - 1)) * 1.0f;
                    // float tex_v = ((float)h / (float)(t_latticeData.height - 1)) * 1.0f;
                    // data[d][h][w] = glm::fvec3(tex_u, tex_v, 0.0f);
                // }
                // if (isAnchor(w, h, archorPoint)) {
                //     if (w == archorPoint[0].x && h == archorPoint[0].y)
                //         data[d][h][w] = glm::fvec3(0.0f, 0.0f, 0.0f);
                //     else if (w == archorPoint[1].x && h == archorPoint[1].y)
                //         data[d][h][w] = glm::fvec3(2.0f, 0.0f, 0.0f);
                //     else if (w == archorPoint[2].x && h == archorPoint[2].y)
                //         data[d][h][w] = glm::fvec3(0.0f, 2.0f, 0.0f);
                //     else if (w == archorPoint[3].x && h == archorPoint[3].y)
                //         data[d][h][w] = glm::fvec3(2.0f, 2.0f, 0.0f);
                //     else if (w == archorPoint[4].x && h == archorPoint[4].y)
                //         data[d][h][w] = glm::fvec3(1.0f, 1.0f, 0.0f);
                // }
                // else {
                //     float tex_u = ((float)w / (float)(t_latticeData.width - 1)) * 2.0f;
                //     float tex_v = ((float)h / (float)(t_latticeData.height - 1)) * 2.0f;
                //     data[d][h][w] = glm::fvec3(tex_u, tex_v, 0.0f);
                // }
            }
        }
    }
    return data;
}

void som_cls::FreeTexLat()
{
    // 清除舊的 lattice
    for (int d = 0; d < t_latticeData.depth; d++)
    {
        for (int h = 0; h < t_latticeData.height; h++)
        {
            free(t_latticeData.lattice[d][h]);
        }
        free(t_latticeData.lattice[d]);
    }
    free(t_latticeData.lattice);
}

glm::ivec3 som_cls::computeNeiborhood(glm::ivec3 node, glm::ivec3 bmu)
{
    glm::ivec3 diff = {0, 0, 0};
    if (latticeData.type == PLANE)
    { // plane
        diff = node - bmu;
    }
    else if (latticeData.type == CYLINDER)
    { // cylinder
        int half = (latticeData.width - 1) / 2;
        if (bmu.y <= half)
        {
            if (node.y >= half + bmu.y)
                diff.y = bmu.y + ((latticeData.width - 1) - node.y);
            else
                diff.y = node.y - bmu.y;
        }
        else if (bmu.y > half)
        {
            if (node.y >= bmu.y - half)
                diff.y = bmu.y - node.y;
            else
                diff.y = node.y + ((latticeData.width - 1) - bmu.y);
        }
        diff.x = node.x - bmu.x;
    }
    else if (latticeData.type == DONUT)
    {
        int half = (latticeData.width - 1) / 2;
        if (bmu.x <= half)
        {
            if (node.x >= half + bmu.x)
            {
                diff.x = bmu.x + ((latticeData.width - 1) - node.x);
            }
            else
            {
                diff.x = node.x - bmu.x;
            }
        }
        else if (bmu.x > half)
        {
            if (node.x >= bmu.x - half)
            {
                diff.x = bmu.x - node.x;
            }
            else
            {
                diff.x = node.x + ((latticeData.width - 1) - bmu.x);
            }
        }

        half = (latticeData.height - 1) / 2;
        if (bmu.y <= half)
        {
            if (node.y >= half + bmu.y)
            {
                diff.y = bmu.y + ((latticeData.height - 1) - node.y);
            }
            else
            {
                diff.y = node.y - bmu.y;
            }
        }
        else if (bmu.y > half)
        {
            if (node.y >= bmu.y - half)
            {
                diff.y = bmu.y - node.y;
            }
            else
            {
                diff.y = node.y + ((latticeData.height - 1) - bmu.y);
            }
        }
    }
    else if (latticeData.type == BALL)
    { // half ball
        //   1          back
        // 2 0 4   left  top  right
        //   3          front
        diff = computeHalfballDist(node) - computeHalfballDist(bmu);
    }
    else if (latticeData.type == THREED)
    {
        diff = node - bmu;
    }
    return diff;
}

glm::ivec3 som_cls::computeHalfballDist(glm::ivec3 p0)
{
    glm::ivec3 a = {0, 0, 0};
    int w = latticeData.width - 1;
    if (p0.z == 0)
        a = {p0.x, w, p0.y};
    else if (p0.z == 1)
        a = {p0.x, p0.y, 0};
    else if (p0.z == 2)
        a = {0, p0.x, p0.y};
    else if (p0.z == 3)
        a = {p0.x, p0.y, w};
    else if (p0.z == 4)
        a = {w, p0.x, p0.y};
    else if (p0.z == 5)
        a = {p0.x, 0, p0.y};

    return a;
}

void som_cls::SOM_lattice()
{
    latticeData.learning_rate_now = compute_rate(latticeData.iter, latticeData.max_iter, latticeData.learning_rate);
    latticeData.radius_now = compute_radius(latticeData.iter, latticeData.max_iter, latticeData.radius);
    // get one input from the dataset
    const glm::fvec3 nowInput = getInput(inputData.input, inputData.num);
    double minDist = -1.0;
    glm::ivec3 bmu;
    // find BMU
    for (int k = 0; k < latticeData.typeNum[latticeData.type]; k++)
    {
        for (int i = 0; i < latticeData.height; i++)
        {
            for (int j = 0; j < latticeData.width; j++)
            {
                double temp = 0.0;
                float dx = (latticeData.lattice[k][i][j].x - nowInput.x);
                float dy = (latticeData.lattice[k][i][j].y - nowInput.y);
                float dz = (latticeData.lattice[k][i][j].z - nowInput.z);
                temp = dx * dx + dy * dy + dz * dz;
                if (minDist < 0.0)
                { // first time
                    minDist = temp;
                    bmu = {0, 0, 0};
                }
                if (minDist > temp)
                {
                    minDist = temp;
                    bmu = {j, i, k};
                }
            }
        }
    }

    // update BMU and the neighbors
    for (int k = 0; k < latticeData.typeNum[latticeData.type]; k++)
    {
        for (int i = 0; i < latticeData.height; i++)
        {
            for (int j = 0; j < latticeData.width; j++)
            {
                // if (isLatticeAnchor(j, i, k, latticeAnchorPoints)) continue;
                // std::cout << j << ", " << i << ", " << k << std::endl;
                glm::ivec3 node = glm::ivec3(j, i, k);
                glm::ivec3 diff = computeNeiborhood(node, bmu);
                double squareDist = static_cast<double>((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));
                if (is_in_neighborhood(squareDist, latticeData.radius_now))
                {
                    double radius_new = compute_influence(latticeData.radius_now, squareDist);
                    update_node(nowInput, node, radius_new, latticeData.learning_rate_now);
                }
            }
        }
    }
    latticeData.iter++;
    // std::cout <<nowInput.x<< ", " << nowInput.y << ", " << nowInput.z << ", bmu : " <<bmu.x<< ", " << bmu.y << ", " << bmu.z << ", iter" <<latticeData.iter <<std::endl;
    // is_som_finished = (iter > max_iter);
}

const glm::fvec3 som_cls::getInput(glm::fvec3 *dataset, int datasetNum)
{
    int r = ((double)rand() / (RAND_MAX + 1.0)) * datasetNum; // rand(): return int (0 ~ RAND_MAX)
    return dataset[r];
}

double som_cls::compute_rate(int iter, int max_iter, double learning_rate)
{
    // double learning_rate_now = latticeData.learning_rate * exp(-1 * (double)latticeData.iter / (double)latticeData.max_iter);
    double learning_rate_now = learning_rate * exp(-1 * ((double)iter) / ((double)(max_iter)));
    return learning_rate_now;
}

double som_cls::compute_radius(int iter, int max_iter, double radius)
{
    double lamda = ((double)max_iter) / log(radius);
    double radius_now = radius * exp(-1 * ((double)iter) / lamda);
    return radius_now;
}

double som_cls::compute_influence(double radius, double squareDist)
{
    double influence = exp((-1 * squareDist) / (2 * pow(radius, 2)));
    return influence;
}

bool som_cls::is_in_neighborhood(double squareDist, double radius)
{
    if (squareDist <= (radius * radius))
        return true;
    else
        return false;
}

void som_cls::update_node(glm::fvec3 nowInput, glm::ivec3 nodeId, double radius, double learning_rate)
{
    latticeData.lattice[nodeId.z][nodeId.y][nodeId.x].x += radius * learning_rate * (nowInput.x - latticeData.lattice[nodeId.z][nodeId.y][nodeId.x].x);
    latticeData.lattice[nodeId.z][nodeId.y][nodeId.x].y += radius * learning_rate * (nowInput.y - latticeData.lattice[nodeId.z][nodeId.y][nodeId.x].y);
    latticeData.lattice[nodeId.z][nodeId.y][nodeId.x].z += radius * learning_rate * (nowInput.z - latticeData.lattice[nodeId.z][nodeId.y][nodeId.x].z);
}

void som_cls::destory_SOM()
{
    // destory lattice, dataset
    destory_lattice();
    destory_dataset();

    // destory_lattice(v_latticeData);
    // destory_dataset(v_inputData);

    // destory_lattice(t_latticeData);
    // destory_texture(t_inputData);
}

void som_cls::destory_lattice()
{
    for (int d = 0; d < latticeData.typeNum[SOM.latticeData.type]; d++)
    {
        for (int h = 0; h < latticeData.height; h++)
        {
            free(latticeData.lattice[d][h]);
        }
        free(latticeData.lattice[d]);
    }
    free(latticeData.lattice);

    for (int d = 0; d < t_latticeData.typeNum[SOM.t_latticeData.type]; d++)
    {
        for (int h = 0; h < t_latticeData.height; h++)
        {
            free(t_latticeData.lattice[d][h]);
        }
        free(t_latticeData.lattice[d]);
    }
    free(t_latticeData.lattice);
}

void som_cls::destory_dataset()
{
    free(inputData.input);
    free(t_inputData.input);
}

glm::fvec3 *som_cls::createTexture()
{
    glm::fvec3 *dataset = (glm::fvec3 *)malloc(sizeof(glm::fvec3) * tex.imageTex[0].height * tex.imageTex[0].width * 1);
    t_inputData.num = 0;
    for (int h = 0; h < tex.imageTex[0].height; h++)
    {
        for (int w = 0; w < tex.imageTex[0].width; w++)
        {
            int n = w + h * tex.imageTex[0].width;
            dataset[n] = {1.0f * (double)w / (double)(tex.imageTex[0].width - 1), 1.0f * (double)h / (double)(tex.imageTex[0].height - 1), 0.0};
            t_inputData.num++;
        }
    }
    return dataset;
}
const glm::fvec3 som_cls::getTexInput()
{
    double rand_i = (double)rand() / (double)RAND_MAX;
    // int i = (rand_i * t_inputData.num); // rand(): return int (0 ~ width)
    int i = std::min(static_cast<int>(rand_i * t_inputData.num), t_inputData.num - 1);

    return t_inputData.input[i];
}

void som_cls::SOM_texture()
{
    t_latticeData.learning_rate_now = compute_rate(t_latticeData.iter, t_latticeData.max_iter, t_latticeData.learning_rate);
    t_latticeData.radius_now = compute_radius(t_latticeData.iter, t_latticeData.max_iter, t_latticeData.radius);
    // get one input from the dataset
    glm::fvec3 nowInput = getTexInput();
    // cout << "nowInput : " << nowInput.r << ", " << nowInput.g << ", " << nowInput.b << endl;
    double minDist = -1.0;
    glm::ivec2 bmu;
    // find BMU
    for (int h = 0; h < t_latticeData.height; h++)
    {
        for (int w = 0; w < t_latticeData.width; w++)
        {
            double temp = 0.0f;
            float dx = (t_latticeData.lattice[0][h][w].x - nowInput.x);
            float dy = (t_latticeData.lattice[0][h][w].y - nowInput.y);
            float dz = (t_latticeData.lattice[0][h][w].z - nowInput.z);
            temp = dx * dx + dy * dy + dz * dz;
            if (minDist < 0.0f)
            { // first time
                minDist = temp;
                bmu = {0, 0};
            }
            else
            {
                if (minDist > temp)
                {
                    minDist = temp;
                    bmu = {w, h};
                }
            }
        }
    }
    // update BMU and the neighbors
    for (int h = 0; h < t_latticeData.height; h++)
    {
        for (int w = 0; w < t_latticeData.width; w++)
        {
            glm::ivec2 node = glm::ivec2{w, h};
            glm::ivec2 diff = node - bmu; // plane
            // 判斷是否為 anchor 點，是的話跳過 update
            // if(isTextureAnchor(w, h, textureAnchorPoints)) continue;
            double squareDist = (diff.x * diff.x) + (diff.y * diff.y);
            if (is_in_neighborhood(squareDist, t_latticeData.radius_now))
            {
                double radius_new = compute_influence(t_latticeData.radius_now, squareDist);
                update_texture(nowInput, node, radius_new, t_latticeData.learning_rate_now);
            }
        }
    }
    t_latticeData.iter++;
}

void som_cls::update_texture(glm::fvec3 nowInput, glm::ivec2 nodeId, double radius, double learning_rate)
{
    t_latticeData.lattice[0][nodeId.y][nodeId.x].x += radius * learning_rate * (nowInput.x - t_latticeData.lattice[0][nodeId.y][nodeId.x].x);
    t_latticeData.lattice[0][nodeId.y][nodeId.x].y += radius * learning_rate * (nowInput.y - t_latticeData.lattice[0][nodeId.y][nodeId.x].y);
    t_latticeData.lattice[0][nodeId.y][nodeId.x].z += radius * learning_rate * (nowInput.z - t_latticeData.lattice[0][nodeId.y][nodeId.x].z);
}

/* 20250319
    else if (latticeData.type == BALL)
    {
        //   1          back
        // 2 0 4   left  top  right
        //   3          front
        // We need curly brackets because there are variable declarations
        double i0, j0, k0;
        for (int h = 0; h < data.height; h++)
        {
            for (int w = 0; w < data.width; w++)
            {
                double ratio_w = (double)w / (double)(data.width - 1);
                double ratio_h = (double)h / (double)(data.height - 1);
                // 0 x-z plane up
                i0 = size[0] * ratio_w + min[0];
                j0 = size[1] + min[1];
                k0 = size[2] * ratio_h + min[2];
                data.lattice[0][h][w] = {i0, j0, k0};

                // 5 x-z plane down
                i0 = size[0] * ratio_w + min[0];
                j0 = 0 + min[1];
                k0 = size[2] * ratio_h + min[2];
                data.lattice[5][h][w] = {i0, j0, k0};

                // 1 x-y plane back
                i0 = size[0] * ratio_w + min[0];
                j0 = size[1] * ratio_h + min[1];
                k0 = 0 + min[2];
                data.lattice[1][h][w] = {i0, j0, k0};

                // 3 x-y plane front
                i0 = size[0] * ratio_w + min[0];
                j0 = size[1] * ratio_h + min[1];
                k0 = size[2] + min[2];
                data.lattice[3][h][w] = {i0, j0, k0};

                // 2 y-z plane left
                i0 = 0 + min[0];
                j0 = size[1] * ratio_w + min[1];
                k0 = size[2] * ratio_h + min[2];
                data.lattice[2][h][w] = {i0, j0, k0};

                // 4 y-z plane right
                i0 = size[0] + min[0];
                j0 = size[1] * ratio_w + min[1];
                k0 = size[2] * ratio_h + min[2];
                data.lattice[4][h][w] = {i0, j0, k0};
            }
        }
    } 
*/
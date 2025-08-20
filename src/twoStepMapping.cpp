#include "twoStepMapping.h"
Mapping mapping;

#define Magnification 0
#define Minification 1

Mapping::Mapping()
{
}
Mapping::~Mapping()
{
}
glm::fvec3 Mapping::magFilter(glm::fvec2 imageRate1, int texType)
{
    // Magnificaton: tex < voxel
    float delta_s = imageRate1.s - (int)imageRate1.s;
    float delta_t = imageRate1.t - (int)imageRate1.t;
    int imageRate_ns = (imageRate1.s + 1 >= tex.imageTex[texType].width) ? (int)imageRate1.s : imageRate1.s + 1;
    int imageRate_nt = (imageRate1.t + 1 >= tex.imageTex[texType].height) ? (int)imageRate1.y : imageRate1.y + 1;

    glm::fvec3 c00, c01, c10, c11;
    c00 = tex.imageTex[texType].image[(int)delta_s][(int)delta_t];
    c01 = tex.imageTex[texType].image[(int)delta_s][(int)delta_t + 1];
    c10 = tex.imageTex[texType].image[(int)delta_s + 1][(int)delta_t];
    c11 = tex.imageTex[texType].image[(int)delta_s + 1][(int)delta_t + 1];
    
    // std::cout << "c00 : " << c00.x << ", " << c00.y << ", " << c00.z << std::endl;
    // std::cout << "c01 : " << c01.x << ", " << c01.y << ", " << c01.z << std::endl;
    // std::cout << "c10 : " << c10.x << ", " << c10.y << ", " << c10.z << std::endl;
    // std::cout << "c11 : " << c11.x << ", " << c11.y << ", " << c11.z << std::endl;

    // float h = minPindex.s - (float)s, k = minPindex.t - (float)t;
    // cout << "h : " << h << "k : " << k << endl;
    glm::fvec3 c = ((1 - delta_s) * (1 - delta_t) * c00 + delta_s * (1 - delta_t) * c01 + (1 - delta_s) * delta_t * c10 + delta_s * delta_t * c11) / 256.0f;

    return c;

    // int s = minPindex.s, t = minPindex.t;
    // int new_s = 0, new_t = 0;
    // // std::cout << SOM.t_latticeData.lattice[0][t][s].r << ", " << SOM.t_latticeData.lattice[0][t][s].g << ", " << SOM.t_latticeData.lattice[0][t][s].b << std::endl;
    // if (s == minPindex.s && t == minPindex.t)
    // {
    //     new_t = SOM.t_latticeData.lattice[0][t][s].y * (tex.imageTex->height - 1);
    //     new_s = SOM.t_latticeData.lattice[0][t][s].x * (tex.imageTex->width - 1);
    //     return tex.imageTex[0].image[new_t][new_s];
    // }

    // int delta_t = 1, delta_s = 1;
    // if (s == SOM.t_latticeData.width - 1)
    //     delta_s = -1;
    // if (t == SOM.t_latticeData.height - 1)
    //     delta_t = -1;

    // glm::fvec3 c00, c01, c10, c11;
    
    // c00 = tex.imageTex[0].image[t][s];
    // c01 = tex.imageTex[0].image[t][s + delta_s];
    // c10 = tex.imageTex[0].image[t + delta_t][s];
    // c11 = tex.imageTex[0].image[t + delta_t][s + delta_s];
    
    // std::cout << "c00 : " << c00.x << ", " << c00.y << ", " << c00.z << std::endl;
    // std::cout << "c01 : " << c01.x << ", " << c01.y << ", " << c01.z << std::endl;
    // std::cout << "c10 : " << c10.x << ", " << c10.y << ", " << c10.z << std::endl;
    // std::cout << "c11 : " << c11.x << ", " << c11.y << ", " << c11.z << std::endl;

    // float h = minPindex.s - (float)s, k = minPindex.t - (float)t;
    // // cout << "h : " << h << "k : " << k << endl;
    // glm::fvec3 c = (1 - h) * (1 - k) * c00 + h * (1 - k) * c01 + (1 - h) * k * c10 + h * k * c11;

    // return c;
}

glm::fvec3 Mapping::minFilter(glm::fvec2 imageRate0, glm::fvec2 imageRate2, int texType)
{
    // tex > voxel
    glm::fvec3 color = {0,0,0};
    int l, r, d, t;
    if(imageRate0.s < imageRate2.s)
    {
        l = imageRate0.s;
        r = imageRate2.s;
    }
    else
    {
        r = imageRate0.s;
        l = imageRate2.s;
    }
    if(imageRate0.t < imageRate2.t)
    {
        d = imageRate0.t;
        t = imageRate2.t;
    }
    else
    {
        t = imageRate0.t;
        d = imageRate2.t;
    }
    int num = 0;
    // glm::fvec3 color0 = {0.1, 0.21, 0.56}; // b
    // glm::fvec3 color1 = {1.0, 0.8941, 0.8824}; // y

    for(int w = l; w <= r; w++)
    {
        for(int h = d; h <= t; h++)
        {
            for(int c = 0; c < 3; c++)
            {
                // if(tex.imageTex[0].image[h][w][c] / 256.0 > 0.8)
                // {
                //     color[c] += color1[c];
                // }
                // else
                // {
                //     color[c] += color0[c];
                // }
                color[c] += tex.imageTex[texType].image[h][w][c] / 256.0;
            }
            num++;
        }
    }
    for(int c = 0; c < 3; c++){
        color[c] = color[c] / (num);
    }
    return color;
}

glm::fvec3 Mapping::computeTexel(glm::vec2 minPindex)
{
    int s = minPindex.s, t = minPindex.t;
    int new_s = 0, new_t = 0;
    // std::cout << SOM.t_latticeData.lattice[0][t][s].r << ", " << SOM.t_latticeData.lattice[0][t][s].g << ", " << SOM.t_latticeData.lattice[0][t][s].b << std::endl;
    if (s == minPindex.s && t == minPindex.t)
    {
        new_t = SOM.t_latticeData.lattice[0][t][s].y * (tex.imageTex->height - 1);
        new_s = SOM.t_latticeData.lattice[0][t][s].x * (tex.imageTex->width - 1);
        return tex.imageTex[0].image[new_t][new_s];
    }

    int delta_t = 1, delta_s = 1;
    if (s == SOM.t_latticeData.width - 1)
        delta_s = -1;
    if (t == SOM.t_latticeData.height - 1)
        delta_t = -1;

    glm::fvec3 c00, c01, c10, c11;
    
    c00 = tex.imageTex[0].image[t][s];
    c01 = tex.imageTex[0].image[t][s + delta_s];
    c10 = tex.imageTex[0].image[t + delta_t][s];
    c11 = tex.imageTex[0].image[t + delta_t][s + delta_s];
    
    std::cout << "c00 : " << c00.x << ", " << c00.y << ", " << c00.z << std::endl;
    std::cout << "c01 : " << c01.x << ", " << c01.y << ", " << c01.z << std::endl;
    std::cout << "c10 : " << c10.x << ", " << c10.y << ", " << c10.z << std::endl;
    std::cout << "c11 : " << c11.x << ", " << c11.y << ", " << c11.z << std::endl;

    float h = minPindex.s - (float)s, k = minPindex.t - (float)t;
    // cout << "h : " << h << "k : " << k << endl;
    glm::fvec3 c = (1 - h) * (1 - k) * c00 + h * (1 - k) * c01 + (1 - h) * k * c10 + h * k * c11;

    return c;
}

glm::fvec3 Mapping::barycentric_coord(glm::fvec3 o, glm::fvec3 a, glm::fvec3 b, glm::fvec3 projp, glm::fvec3 n)
{
    // compute center of gravity
    glm::fvec3 ratio;
    double area_oab = innerProduct(glm::cross(a - o, b - o), n);

    ratio[0] = innerProduct(glm::cross(a - projp, b - projp), n) / area_oab;
    ratio[1] = innerProduct(glm::cross(b - projp, o - projp), n) / area_oab;
    ratio[2] = innerProduct(glm::cross(o - projp, a - projp), n) / area_oab;

    return ratio;

    // double area_oab = innerProduct(glm::cross(b - o, a - o), n);

    // double ra = innerProduct(glm::cross(o - projp, b - projp), n) / area_oab;
    // double rb = innerProduct(glm::cross(a - projp, o - projp), n) / area_oab;
    // double ro = innerProduct(glm::cross(b - projp, a - projp), n) / area_oab;

    // return glm::fvec3{ra, rb, ro};
}
glm::fvec3 Mapping::point_to_triangle(glm::fvec3 o, glm::fvec3 a, glm::fvec3 b, glm::fvec3 p, double* minDist)
{
    // compute normal vector
    glm::fvec3 n = glm::normalize(glm::cross(a - o, b - o));
    glm::fvec3 np = p - o;

    // find projection point
    glm::fvec3 d;
    d = (-1 * innerProduct(np, n)) * n;
    glm::fvec3 projp = p + d;
    
    // if projp in triangle or not
    if (inTriangle(o, a, b, projp, n)) // inside
    {
        p = projp;
    }
    else // outside
    {
        p = outerProjp(o, a, b, projp, p);
        d = p - projp;
    }
    double tmpDist = d.x * d.x + d.y * d.y + d.z * d.z;
    if (tmpDist >= *minDist)
    {
        return glm::fvec3{-1, -1, -1};
    }
    *minDist = tmpDist;
    // compute dist of point to triangle
    glm::fvec3 ratio = barycentric_coord(o, a, b, projp, n);
    return ratio;
}

glm::fvec2 Mapping::surroundTriangles(double min, glm::fvec3 voxelPos, glm::ivec3 minLatticeCoord)
{
    glm::fvec2 twoDcoord = {minLatticeCoord.x, minLatticeCoord.y};

    double minDist = min;
    for(int i = 0; i < 4; i++)
    {
        glm::ivec2 A, B, P; // index value
        A.x = minLatticeCoord.x + dir[i][0];
        A.y = minLatticeCoord.y + dir[i][1];

        B.x = minLatticeCoord.x + dir[i + 1][0];
        B.y = minLatticeCoord.y + dir[i + 1][1];

        if(A.x < 0 || A.x > (SOM.latticeData.width - 1)) continue;
        if(A.y < 0 || A.y > (SOM.latticeData.height - 1)) continue;
        if(B.x < 0 || B.x > (SOM.latticeData.width - 1)) continue;
        if(B.y < 0 || B.y > (SOM.latticeData.height - 1)) continue;

        glm::fvec3 a = SOM.latticeData.lattice[minLatticeCoord.z][A.y][A.x];
        glm::fvec3 b = SOM.latticeData.lattice[minLatticeCoord.z][B.y][B.x];
        glm::fvec3 o = SOM.latticeData.lattice[minLatticeCoord.z][minLatticeCoord.y][minLatticeCoord.x];
        glm::fvec3 p = voxelPos;

        glm::fvec3 ratio = point_to_triangle(o, a, b, p, &minDist);

        if(ratio.x + ratio.y + ratio.z == -3){
            continue;
        }
        twoDcoord.x = ratio[0] * minLatticeCoord.x + ratio[1] * (A.x) + ratio[2] * (B.x);
        twoDcoord.y = ratio[0] * minLatticeCoord.y + ratio[1] * (A.y) + ratio[2] * (B.y);

        if(twoDcoord.x < 0.0) twoDcoord.x = 0.0;
        if(twoDcoord.x > (SOM.latticeData.width - 1)) twoDcoord.x = SOM.latticeData.width - 1;
        if(twoDcoord.y < 0.0) twoDcoord.y = 0.0;
        if(twoDcoord.y > (SOM.latticeData.height - 1)) twoDcoord.y = SOM.latticeData.height - 1;

    }
    return twoDcoord;
}

glm::fvec3 Mapping::latticeCoord_to_textureCoord_2D(glm::fvec2 latticeCoord)
{
    int s = latticeCoord.s, t = latticeCoord.t, r = 0;

    if (latticeCoord.s == s && latticeCoord.t == t)
    
        return SOM.t_latticeData.lattice[r][t][s];

    int delta_t = 1, delta_s = 1;
    if (s == SOM.t_latticeData.width - 1)
        delta_s = -1;
    if (t == SOM.t_latticeData.height - 1)
        delta_t = -1;

    glm::fvec3 c00 = SOM.t_latticeData.lattice[r][t][s];
    glm::fvec3 c01 = SOM.t_latticeData.lattice[r][t][s + delta_s];
    glm::fvec3 c10 = SOM.t_latticeData.lattice[r][t + delta_t][s];
    glm::fvec3 c11 = SOM.t_latticeData.lattice[r][t + delta_t][s + delta_s];

    float h = latticeCoord.s - (float)s, k = latticeCoord.t - (float)t;
    glm::fvec3 c = (1 - h) * (1 - k) * c00 + h * (1 - k) * c01 + (1 - h) * k * c10 + h * k * c11;

    return c;
}

void Mapping::computeTexel(int maxIdx, int centerIdx)
{
    int step = abs(maxIdx - centerIdx);
    int surfaceNum = rawModel.layerSurfaces[maxIdx].surfaceVoxels.size();
    for (int v = 0; v < surfaceNum; v++)
    {
        glm::fvec3 normal = rawModel.layerSurfaces[maxIdx].surfaceVoxels[v].normal;
        glm::fvec3 color = rawModel.layerSurfaces[maxIdx].surfaceVoxels[v].color;

        if(glm::length(normal) > 0.0f)
        {
            double v_x = rawModel.layerSurfaces[maxIdx].surfaceVoxels[v].locate.x + 0.5;
            double v_y = rawModel.layerSurfaces[maxIdx].surfaceVoxels[v].locate.y + 0.5;
            double v_z = rawModel.layerSurfaces[maxIdx].surfaceVoxels[v].locate.z + 0.5;
        
            double t_x = v_x + glm::round(normal.x * static_cast<float>(step)) + 0.5;
            double t_y = v_y + glm::round(normal.y * static_cast<float>(step)) + 0.5;
            double t_z = v_z + glm::round(normal.z * static_cast<float>(step)) + 0.5;

            // Find the corresponding voxel in the surface list
            for(int i = 0; i < surfaceNum; i++)
            {
                surfaceVoxel targetVoxels = rawModel.layerSurfaces[maxIdx].surfaceVoxels[i];
                if(targetVoxels.locate.x == t_x && targetVoxels.locate.y == t_y && targetVoxels.locate.z == t_z)
                {
                    targetVoxels.color = color;
                }
            }
        }
    }
}

void Mapping::textureMapping(bool* checkFlags)
{
    // Step 1. do texture mapping with surface layer
    // Step 2. according gradient(normal) to find texel of other layer
    for(int l = 0; l < rawModel.layerSurfaces.size(); l++)
    {
        if(rawModel.layerSurfaces[l].oneSurfaceVoxels.size())
            unitVoxelTextureMapping(rawModel.layerSurfaces[l].oneSurfaceVoxels);
        // if(rawModel.layerSurfaces[l].surfaceVoxels.size())
        //     unitVoxelTextureMapping(rawModel.layerSurfaces[l].surfaceVoxels);
    }
}

// filterType:  0: nearest , 1: linear
void Mapping::unitVoxelTextureMapping(std::vector<surfaceVoxel>& voxel)
{
    // int surfaceNum = rawModel.layerSurfaces[layern].surfaceVoxels.size();
    int surfaceNum = voxel.size();
    for (int v = 0; v < surfaceNum; v++)
    {
        // voxel -> som
        double v_x = voxel[v].locate.x + 0.5;
        double v_y = voxel[v].locate.y + 0.5;
        double v_z = voxel[v].locate.z + 0.5;

        int texType = voxel[v].textype;

        // texture Mapping in set range
        if(v_y > y_max || v_y < y_min)
        {
            voxel[v].color = {1.0f, 1.0f, 1.0f};
            int x = voxel[v].locate.x;
            int y = voxel[v].locate.y;
            int z = voxel[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
            continue;
        }
        if(v_z > z_max || v_z < z_min)
        {
            voxel[v].color = {1.0f, 1.0f, 1.0f};
            int x = voxel[v].locate.x;
            int y = voxel[v].locate.y;
            int z = voxel[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
            continue;
        }
        if(v_x > x_max || v_x < x_min)
        {
            voxel[v].color = {1.0f, 1.0f, 1.0f};
            int x = voxel[v].locate.x;
            int y = voxel[v].locate.y;
            int z = voxel[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
            continue;
        }
        double minDist = 100000;
        glm::fvec3 latticeTexCoord0, latticeTexCoord1, latticeTexCoord2, tmpCoord;
        glm::ivec3 N = findBMU(v_x, v_y, v_z, &minDist); // index value
        // nearest filter: projp = bmu
        glm::vec2 minPindex = {(float)N.x, (float) N.y};
        if (latticeVoxelFilter == LINEAR) // linear filter
        {
            // v_x, v_y, v_z
            glm::fvec2 voxelToLatticeCoord;
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x, v_y, v_z}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 ) // SOM.cpp 491(out of texture bounding box)
            {
                voxel[v].color = {1.0f, 1.0f, 1.0f};
                int x = voxel[v].locate.x;
                int y = voxel[v].locate.y;
                int z = voxel[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord1 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.latticeData.width);
            // 2 x 2
            // latticeTexCoord1 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
            // v_x - 0.5, v_y - 0.5, v_z - 0.5
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x - 0.5, v_y - 0.5, v_z - 0.5}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 ) // SOM.cpp 491 (out of texture bounding box)
            {
                voxel[v].color = {1.0f, 1.0f, 1.0f};
                int x = voxel[v].locate.x;
                int y = voxel[v].locate.y;
                int z = voxel[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord0 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.latticeData.width);
            // 2 x 2
            // latticeTexCoord0 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
            // v_x + 0.5, v_y + 0.5, v_z + 0.5
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x + 0.5, v_y + 0.5, v_z + 0.5}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 )
            {
                voxel[v].color = {1.0f, 1.0f, 1.0f};
                int x = voxel[v].locate.x;
                int y = voxel[v].locate.y;
                int z = voxel[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord2 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.latticeData.width);
            // 2 x 2
            // latticeTexCoord2 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
        }
        if (latticeVoxelFilter == NEAREST) // nearest filter
        {
            tmpCoord = latticeCoord_to_textureCoord_2D({N.x, N.y});
            latticeTexCoord0 = latticeTexCoord1 = latticeTexCoord2 = voxel[v].texcoord = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.t_latticeData.width);
        }
        if (latticeTexCoord0.x < 0.0 || latticeTexCoord0.x > 1.0 || latticeTexCoord0.y < 0.0 || latticeTexCoord0.y > 1.0)
        {
            voxel[v].color = {1.0f, 1.0f, 1.0f};
            int x = voxel[v].locate.x;
            int y = voxel[v].locate.y;
            int z = voxel[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else if (latticeTexCoord1.x < 0.0 || latticeTexCoord1.x > 1.0 || latticeTexCoord1.y < 0.0 || latticeTexCoord1.y > 1.0)
        {
            voxel[v].color = {1.0f, 1.0f, 1.0f};
            int x = voxel[v].locate.x;
            int y = voxel[v].locate.y;
            int z = voxel[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else if (latticeTexCoord2.x < 0.0 || latticeTexCoord2.x > 1.0 || latticeTexCoord2.y < 0.0 || latticeTexCoord2.y > 1.0)
        {
            voxel[v].color = {1.0f, 1.0f, 1.0f};
            int x = voxel[v].locate.x;
            int y = voxel[v].locate.y;
            int z = voxel[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else
        {
            if (latticeTextureFilter == NEAREST)
            {
                glm::fvec3 color0 = {0.1, 0.21, 0.56}; // b
                glm::fvec3 color1 = {1.0, 0.8941, 0.8824}; // pink
                // 1 x 1
                glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1), latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                // 2 x 2
                // glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1) * 0.5, latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};

                int x = voxel[v].locate.x;
                int y = voxel[v].locate.y;
                int z = voxel[v].locate.z;
                for (int c = 0; c < 3; c++)
                {
                    if(tex.imageTex[texType].image[(int)imageRate1.y][(int)imageRate1.x][c] / 256.0 > 0.8)
                    {
                        voxel[v].color[c] = 1.0f; // color1[c];
                        // put the color information into rawData for draw iso-surface
                        rawModel.rawData[y][x][z].color[c] = 1.0f;
                    }
                    else
                    {
                        voxel[v].color[c] = 0.0f; // color1[c];
                        // put the color information into rawData for draw iso-surface
                        rawModel.rawData[y][x][z].color[c] = 0.0f;
                    }
                    // rawmodel.voxelModel.somVoxel[layer][block][n].color[c] = tex.imageTex[texType].image[(int)imageRate1.y][(int)imageRate1.x][c] / 256.0;
                }
            }
            if (latticeTextureFilter == LINEAR)
            {
                // check texSize voxelSize
                int texSize = tex.imageTex[texType].height * tex.imageTex[texType].width;
                if(surfaceNum > texSize)
                    texfilter = MAGNIFICATON;

                if (texfilter == MAGNIFICATON)
                {
                    // tex < voxel
                    // 1 x 1
                    glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1), latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                    // 2 x 2
                    // glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1) * 0.5, latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                    voxel[v].color = magFilter(imageRate1, texType);
                    // put the color information into rawData for draw iso-surface
                    int x = voxel[v].locate.x;
                    int y = voxel[v].locate.y;
                    int z = voxel[v].locate.z;
                    rawModel.rawData[y][x][z].color = voxel[v].color;
                }
                else
                {
                    // 1 x 1
                    glm::ivec2 imageRate0 = {(latticeTexCoord0.x * (double)(tex.imageTex[texType].width - 1)), (latticeTexCoord0.y * (double)(tex.imageTex[texType].height - 1))};
                    glm::ivec2 imageRate2 = {(latticeTexCoord2.x * (double)(tex.imageTex[texType].width - 1)), (latticeTexCoord2.y * (double)(tex.imageTex[texType].height - 1))};
                    // 2 x 2
                    // glm::ivec2 imageRate0 = {(latticeTexCoord0.x * (double)(tex.imageTex[texType].width - 1) * 0.5), (latticeTexCoord0.y * (double)(tex.imageTex[texType].height - 1))};
                    // glm::ivec2 imageRate2 = {(latticeTexCoord2.x * (double)(tex.imageTex[texType].width - 1) * 0.5), (latticeTexCoord2.y * (double)(tex.imageTex[texType].height - 1))};
                    voxel[v].color = minFilter(imageRate0, imageRate2, texType);
                    // put the color information into rawData for draw iso-surface
                    int x = voxel[v].locate.x;
                    int y = voxel[v].locate.y;
                    int z = voxel[v].locate.z;
                    rawModel.rawData[y][x][z].color = voxel[v].color;
                }
            }
        }
        // int x = voxel[v].locate.x;
        // int y = voxel[v].locate.y;
        // int z = voxel[v].locate.z;
    }
    std::cout << "mapping end~" << std::endl;
}
glm::vec3 Mapping::findBMU(double v_x, double v_y, double v_z, double *minDist)
{
    glm::vec3 bmu = {0, 0, 0};
    // cout << "minDist: " << minDist << endl;
    // cout << "w: " << SOM.latticeData.width << ", h: " << SOM.latticeData.height << endl;
    for (int d = 0; d < SOM.latticeData.typeNum[SOM.latticeData.type]; d++) // if lattice is Ball has 6 faces
    {
        for (int h = 0; h < SOM.latticeData.height; h++)
        {
            for (int w = 0; w < SOM.latticeData.width; w++)
            {
                double l_x = SOM.latticeData.lattice[d][h][w].x;
                double l_y = SOM.latticeData.lattice[d][h][w].y;
                double l_z = SOM.latticeData.lattice[d][h][w].z;

                double dist = (l_x - v_x) * (l_x - v_x) + (l_y - v_y) * (l_y - v_y) + (l_z - v_z) * (l_z - v_z);
                // cout << "dist: " << dist << endl;
                if (dist < *minDist)
                {
                    *minDist = dist;
                    bmu = {w, h, d};
                }
            }
        }
    }
    return bmu;
}
float Mapping::innerProduct(glm::vec3 a, glm::vec3 b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

glm::fvec3 Mapping::crossProduct(glm::vec3 a, glm::vec3 b)
{
    glm::fvec3 c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = -1 * (a.x * b.z - a.z * b.x);
    c.z = a.x * b.y - a.y * b.x;

    return c;
}

bool Mapping::inTriangle(glm::fvec3 projp, glm::fvec3 bmu, glm::fvec3 a, glm::fvec3 b, glm::fvec3 n)
{
    glm::fvec3 projpb, projpa, projpBMU;
    projpb = b - projp;
    projpa = a - projp;
    projpBMU = bmu - projp;

    glm::fvec3 Cba = glm::normalize(crossProduct(bmu - projp, a - projp));
    glm::fvec3 CaBMU = glm::normalize(crossProduct(a - projp, b - projp));
    glm::fvec3 CBMUb = glm::normalize(crossProduct(b - projp, bmu - projp));

    if ((Cba.x * CaBMU.x < 0.0) || (Cba.x * CBMUb.x  < 0.0) || (CaBMU.x * CBMUb.x < 0.0)) // cross product in the same side -> inside triangle
    {
        return true;
    }
    else
    {
        return false;
    }
}

glm::vec3 Mapping::outerProjp(glm::vec3 bmu, glm::vec3 a, glm::vec3 b, glm::vec3 projp, glm::vec3 p)
{
    glm::fvec3 newp = bmu;
    // if close to BMU, newp = BMU
    // o-a
    glm::fvec3 bmua = a - bmu;
    float bmuaDist = bmua.x * bmua.x + bmua.y * bmua.y + bmua.z * bmua.z;
    // do projection with projpa to bmua  
    float r = innerProduct(a - projp, bmua) / sqrt(bmuaDist);

    glm::fvec3 tmpa;
    if (r > 1.0)
    {
        tmpa = a;
    }
    else if (r < 0.0)
    {
        tmpa = bmu;
    }
    else
    {
        tmpa = (bmu + r * bmua);
    }
    double tmpaDist = (p.x - tmpa.x) * (p.x - tmpa.x) + (p.y - tmpa.y) * (p.y - tmpa.y) + (p.z - tmpa.z) * (p.z - tmpa.z);
    // o-b
    glm::fvec3 bmub = b - bmu;
    float bmubDist = bmub.x * bmub.x + bmub.y * bmub.y + bmub.z * bmub.z;
    r = innerProduct(b - projp, bmub) / sqrt(bmubDist);

    glm::fvec3 tmpb;
    if (r > 1.0)
    {
        tmpb = b;
    }
    else if (r < 0.0)
    {
        tmpb = bmu;
    }
    else
    {
        tmpb = (bmu + r * bmub);
    }
    double tmpbDist = (p.x - tmpb.x) * (p.x - tmpb.x) + (p.y - tmpb.y) * (p.y - tmpb.y) + (p.z - tmpb.z) * (p.z - tmpb.z);

    if (tmpaDist < tmpbDist)
        return tmpa;
    else
        return tmpb;
}

/*
void Mapping::textureMapping()
{
    for (int v = 0; v < rawModel.surfaceNum; v++)
    {
        // voxel -> som
        double v_x = rawModel.surfaceVoxelData[v].locate.x;
        double v_y = rawModel.surfaceVoxelData[v].locate.y;
        double v_z = rawModel.surfaceVoxelData[v].locate.z;
        double minDist = 100000;
        glm::ivec3 N = findBMU(v_x, v_y, v_z, &minDist); // index value
        // nearest filter: projp = bmu
        glm::vec2 minPindex = {(float)N.x, (float) N.y};
        if (filterType == 1) // linear filter
        {
            glm::ivec2 A, B, P; // index value
            glm::vec2 minPindex = {(float)N.x, (float)N.y};

            double minDist = 10000;
            for (int i = 0; i < 4; i++)
            {
                A.x = N.x + dir[i][0];
                A.y = N.y + dir[i][1];

                B.x = N.x + dir[i + 1][0];
                B.y = N.y + dir[i + 1][1];

                if (A.x < 0 || A.x > (SOM.latticeData.width - 1) || A.y < 0 || A.y > (SOM.latticeData.height - 1) || B.x < 0 || B.x > (SOM.latticeData.width - 1) || B.y < 0 || B.y > (SOM.latticeData.height - 1)) // boundary
                    continue;
                // model space coordinate
                // compute projectional point
                glm::vec3 a = SOM.latticeData.lattice[N.z][A.y][A.x];
                glm::vec3 b = SOM.latticeData.lattice[N.z][B.y][B.x];
                glm::vec3 o = SOM.latticeData.lattice[N.z][N.y][N.x];
                glm::vec3 p = {rawModel.surfaceVoxelData[v].locate.x, rawModel.surfaceVoxelData[v].locate.y, rawModel.surfaceVoxelData[v].locate.z};

                glm::vec3 ratio = point_to_triangle(a, b, o, p, &minDist);

                if (ratio.x + ratio.y + ratio.z == -3)
                {
                    continue;
                }
                minPindex = {ratio.x * A.x + ratio.y * B.x + ratio.z * N.x, ratio.x * A.y + ratio.y * B.y + ratio.z * N.y};
            }
        }
        glm::fvec3 color = computeTexture(minPindex);

        // std::cout << "v: " << v << ", N: " << N.x << ", " << N.y << ": " << color.r << ", " << color.g << ", " << color.b << std::endl;
        // set voxel data
        rawModel.surfaceVoxelData[v].color = {color.r, color.g, color.b};
    }
}
glm::vec3 Mapping::outerProjp(glm::vec3 bmu, glm::vec3 a, glm::vec3 b, glm::vec3 projp, glm::vec3 p)
{
    // outer point close to triangle point
    // bmu
    glm::fvec3 newp = bmu;
    glm::fvec3 tmp = p - bmu;
    float tmpDist = (tmp.x * tmp.x) + (tmp.y * tmp.y) + (tmp.z * tmp.z);
    float dist = tmpDist;
    // outer point close to triangle point
    // bmu a
    tmp = a - bmu;
    float r = innerProduct(a - bmu, projp - bmu) / (tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
    tmp = p - ((a - bmu) * r + bmu);
    tmpDist = tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
    if (dist > tmpDist && r >= 0.0 && r <= 1.0)
    {
        dist = tmpDist;
        newp = (a - bmu) * r + bmu;
    }
    // bmu b
    tmp = b - bmu;
    r = innerProduct(b - bmu, projp - bmu) / (tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
    tmp = p - ((b - bmu) * r + bmu);
    tmpDist = tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
    if (dist > tmpDist && r >= 0.0 && r <= 1.0)
    {
        dist = tmpDist;
        newp = (b - bmu) * r + bmu;
    }
    // a
    tmp = p - a;
    tmpDist = tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
    if (dist > tmpDist)
    {
        dist = tmpDist;
        newp = a;
    }
    // b
    tmp = p - b;
    tmpDist = tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
    if (dist > tmpDist)
    {
        dist = tmpDist;
        newp = b;
    }
    // a b
    tmp = b - a;
    r = innerProduct(b - a, projp - a) / (tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
    tmp = p - ((b - a) * r + a);
    tmpDist = tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
    if (dist > tmpDist && r >= 0.0 && r <= 1.0)
    {
        dist = tmpDist;
        newp = (b - a) * r + a;
    }
    return newp;
    // glm::vec3 projBMU, BMUa;
    // projBMU.x = projp.x - bmu.x;
    // projBMU.y = projp.y - bmu.y;
    // projBMU.z = projp.z - bmu.z;
    // BMUa.x = a.x - bmu.x;
    // BMUa.y = a.y - bmu.y;
    // BMUa.z = a.z - bmu.z;

    // float na = (bmu.x - a.x) * (bmu.x - a.x) + (bmu.y - a.y) * (bmu.y - a.y) + (bmu.z - a.z) * (bmu.z - a.z);

    // float r = innerProduct(projBMU, BMUa) / na;
    // if (r > 1.0)
    // {
    //     return a;
    // }
    // else if (r < 0.0)
    // {
    //     return bmu;
    // }
    // else
    // {
    //     return (bmu + r * BMUa);
    // }
}
// filterType:  0: nearest , 1: linear
void Mapping::unitVoxelTextureMapping(int layern)
{
    int surfaceNum = rawModel.layerSurfaces[layern].surfaceVoxels.size();
    for (int v = 0; v < surfaceNum; v++)
    {
        // voxel -> som
        double v_x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x + 0.5;
        double v_y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y + 0.5;
        double v_z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z + 0.5;

        double minDist = 100000;
        glm::fvec3 latticeTexCoord0, latticeTexCoord1, latticeTexCoord2, tmpCoord;
        glm::ivec3 N = findBMU(v_x, v_y, v_z, &minDist); // index value
        // nearest filter: projp = bmu
        glm::vec2 minPindex = {(float)N.x, (float) N.y};
        if (latticeVoxelFilter == LINEAR) // linear filter
        {
            // v_x, v_y, v_z
            glm::fvec2 trueMinLatticeCoord = surroundTriangles(minDist, {v_x, v_y, v_z}, N);
            tmpCoord = {trueMinLatticeCoord.x / (SOM.latticeData.width - 1), trueMinLatticeCoord.y / (SOM.latticeData.height - 1), 0.5};
            latticeTexCoord1 = rawModel.layerSurfaces[layern].surfaceVoxels[v].texcoord = tex.compute_voxel_texture(glm::fvec4(tmpCoord, 1.0));
            // v_x - 0.5, v_y - 0.5, v_z - 0.5
            trueMinLatticeCoord = surroundTriangles(minDist, {v_x - 0.5, v_y - 0.5, v_z - 0.5}, N);
            tmpCoord = {trueMinLatticeCoord.x / (SOM.latticeData.width - 1), trueMinLatticeCoord.y / (SOM.latticeData.height - 1), 0.5};
            glm::fvec3 latticeTexCoord0 = rawModel.layerSurfaces[layern].surfaceVoxels[v].texcoord = tex.compute_voxel_texture(glm::fvec4(tmpCoord, 1.0));
            // v_x, v_y, v_z
            trueMinLatticeCoord = surroundTriangles(minDist, {v_x + 0.5, v_y + 0.5, v_z + 0.5}, N);
            tmpCoord = {trueMinLatticeCoord.x / (SOM.latticeData.width - 1), trueMinLatticeCoord.y / (SOM.latticeData.height - 1), 0.5};
            glm::fvec3 latticeTexCoord2 = rawModel.layerSurfaces[layern].surfaceVoxels[v].texcoord = tex.compute_voxel_texture(glm::fvec4(tmpCoord, 1.0));

            if(latticeTexCoord1.x < 0.0 || latticeTexCoord1.x > 1.0 || latticeTexCoord1.y < 0.0 || latticeTexCoord1.y > 1.0)
            {
                rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0, 1.0, 1.0};
            }
            else
            {
                glm::ivec2 imageRate0 = {(latticeTexCoord0.x * (double)(tex.imageTex[0].width - 1)) + 0.5, (latticeTexCoord0.y * (double)(tex.imageTex[0].height - 1)) + 0.5};
                glm::ivec2 imageRate2 = {(latticeTexCoord2.x * (double)(tex.imageTex[0].width - 1)) + 0.5, (latticeTexCoord2.y * (double)(tex.imageTex[0].height - 1)) + 0.5};
                glm::ivec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[0].width - 1), latticeTexCoord1.y * (double)(tex.imageTex[0].height - 1)};
                glm::fvec3 color;
                // if(filterType == 0)
                // {
                    glm::fvec3 color0 = {0.1, 0.21, 0.56}; // b
                    glm::fvec3 color1 = {1.0, 0.8941, 0.8824}; // y
                    for(int c = 0; c < 3; c++)
                    {
                        if(tex.imageTex[0].image[(int)imageRate1.y][(int)imageRate1.x][c] / 256.0 > 0.8)
                        {
                            color[c] = color1[c];
                        }
                        else
                        {
                            color[c] = color0[c];
                        }
                        // color[c] = tex.imageTex[0].image[(int)imageRate1.y][(int)imageRate1.x][c] / 256.0;
                    }
                // }
                // texture resolution < voxel resolution -> Magnification
                if((tex.imageTex->width * tex.imageTex->height) < surfaceNum)
                {
                    color = magFilter(imageRate1);
                }
                else
                {
                    color = minFilter(imageRate0, imageRate2);
                }
                // set voxel data
                rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {color.r, color.g, color.b};
            }
                    // for (int i = 0; i < 4; i++)
                    // {
                    //     A.x = N.x + dir[i][0];
                    //     A.y = N.y + dir[i][1];

                    //     B.x = N.x + dir[i + 1][0];
                    //     B.y = N.y + dir[i + 1][1];

                    //     if(A.x < 0 || A.x > (SOM.latticeData.width - 1)) continue;
                    //     if(A.y < 0 || A.y > (SOM.latticeData.height - 1)) continue;
                    //     if(B.x < 0 || B.x > (SOM.latticeData.width - 1)) continue;
                    //     if(B.y < 0 || B.y > (SOM.latticeData.height - 1)) continue;

                    //     // model space coordinate
                    //     // compute projectional point
                    //     glm::vec3 a = SOM.latticeData.lattice[N.z][A.y][A.x];
                    //     glm::vec3 b = SOM.latticeData.lattice[N.z][B.y][B.x];
                    //     glm::vec3 o = SOM.latticeData.lattice[N.z][N.y][N.x];
                    //     glm::vec3 p = {v_x, v_y, v_z};

                    //     glm::vec3 ratio = point_to_triangle(o, a, b, p, &minDist);

                    //     if (ratio.x + ratio.y + ratio.z == -3)
                    //     {
                    //         continue;
                    //     }
                    //     // (pi, pj) = ro * (Ni, Nj) + ra * (ai, aj) + rb * (bi, bj)
                    //     // minPindex = {ratio.x * A.x + ratio.y * B.x + ratio.z * N.x, ratio.x * A.y + ratio.y * B.y + ratio.z * N.y};
                    //     minPindex = {ratio[1] * A.x + ratio[2] * B.x + ratio[0] * N.x, ratio[1] * A.y + ratio[2] * B.y + ratio[0] * N.y};
                    // }
        }
        else
        {
            glm::fvec3 color = {1.0f, 1.0f, 1.0f};
            color = computeTexel(minPindex);
            // std::cout << "v: " << v << ", N: " << N.x << ", " << N.y << ": " << color.r << ", " << color.g << ", " << color.b << std::endl;
            // set voxel data
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {color.r, color.g, color.b};
        }
    }
    // if(minPindex.x < 0.0) minPindex.x = 0.0;
    // if(minPindex.x > SOM.latticeData.width - 1) minPindex.x = SOM.latticeData.width - 1;
    // if(minPindex.y < 0.0) minPindex.y = 0.0;
    // if(minPindex.y > SOM.latticeData.height - 1) minPindex.y = SOM.latticeData.height - 1;
    std::cout << "mapping end~" << std::endl;
}
*/

/*20250319
void Mapping::unitVoxelTextureMapping(int layern)
{
    int surfaceNum = rawModel.layerSurfaces[layern].surfaceVoxels.size();
    for (int v = 0; v < surfaceNum; v++)
    {
        // voxel -> som
        double v_x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x + 0.5;
        double v_y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y + 0.5;
        double v_z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z + 0.5;

        int texType = rawModel.layerSurfaces[layern].surfaceVoxels[v].textype;

        // 花瓶瓶口不要做 texture Mapping
        if(v_y > 320.0f)
        {
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
            int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
            int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
            int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
            continue;
        }

        double minDist = 100000;
        glm::fvec3 latticeTexCoord0, latticeTexCoord1, latticeTexCoord2, tmpCoord;
        glm::ivec3 N = findBMU(v_x, v_y, v_z, &minDist); // index value
        // nearest filter: projp = bmu
        glm::vec2 minPindex = {(float)N.x, (float) N.y};
        if (latticeVoxelFilter == LINEAR) // linear filter
        {
            // v_x, v_y, v_z
            glm::fvec2 voxelToLatticeCoord;
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x, v_y, v_z}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 ) // SOM.cpp 491(out of texture bounding box)
            {
                rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
                int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord1 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.t_latticeData.width);
            // 2 x 2
            // latticeTexCoord1 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
            // v_x - 0.5, v_y - 0.5, v_z - 0.5
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x - 0.5, v_y - 0.5, v_z - 0.5}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 ) // SOM.cpp 491 (out of texture bounding box)
            {
                rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
                int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord0 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.t_latticeData.width);
            // 2 x 2
            // latticeTexCoord0 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
            // v_x + 0.5, v_y + 0.5, v_z + 0.5
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x + 0.5, v_y + 0.5, v_z + 0.5}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 ) // SOM.cpp 491(out of texture bounding box)
            {
                rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
                int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord2 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.t_latticeData.width);
            // 2 x 2
            // latticeTexCoord2 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
        }
        if (latticeVoxelFilter == NEAREST) // nearest filter
        {
            tmpCoord = latticeCoord_to_textureCoord_2D({N.x, N.y});
            latticeTexCoord0 = latticeTexCoord1 = latticeTexCoord2 = rawModel.layerSurfaces[layern].surfaceVoxels[v].texcoord = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.t_latticeData.width);
        }
        if (latticeTexCoord0.x < 0.0 || latticeTexCoord0.x > 1.0 || latticeTexCoord0.y < 0.0 || latticeTexCoord0.y > 1.0)
        {
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
            int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
            int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
            int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else if (latticeTexCoord1.x < 0.0 || latticeTexCoord1.x > 1.0 || latticeTexCoord1.y < 0.0 || latticeTexCoord1.y > 1.0)
        {
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
            int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
            int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
            int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else if (latticeTexCoord2.x < 0.0 || latticeTexCoord2.x > 1.0 || latticeTexCoord2.y < 0.0 || latticeTexCoord2.y > 1.0)
        {
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
            int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
            int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
            int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else
        {
            if (latticeTextureFilter == NEAREST)
            {
                glm::fvec3 color0 = {0.1, 0.21, 0.56}; // b
                glm::fvec3 color1 = {1.0, 0.8941, 0.8824}; // pink
                // 1 x 1
                glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1), latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                // 2 x 2
                // glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1) * 0.5, latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};

                int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                for (int c = 0; c < 3; c++)
                {
                    if(tex.imageTex[texType].image[(int)imageRate1.y][(int)imageRate1.x][c] / 256.0 > 0.8)
                    {
                        rawModel.layerSurfaces[layern].surfaceVoxels[v].color[c] = 1.0f; // color1[c];
                        // put the color information into rawData for draw iso-surface
                        rawModel.rawData[y][x][z].color[c] = 1.0f;
                    }
                    else
                    {
                        rawModel.layerSurfaces[layern].surfaceVoxels[v].color[c] = 0.0f; // color1[c];
                        // put the color information into rawData for draw iso-surface
                        rawModel.rawData[y][x][z].color[c] = 0.0f;
                    }
                    // rawmodel.voxelModel.somVoxel[layer][block][n].color[c] = tex.imageTex[texType].image[(int)imageRate1.y][(int)imageRate1.x][c] / 256.0;
                }
            }
            if (latticeTextureFilter == LINEAR)
            {
                // check texSize voxelSize
                int texSize = tex.imageTex[texType].height * tex.imageTex[texType].width;
                if(surfaceNum > texSize)
                    texfilter = MAGNIFICATON;

                if (texfilter == MAGNIFICATON)
                {
                    // tex < voxel
                    // 1 x 1
                    glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1), latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                    // 2 x 2
                    // glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1) * 0.5, latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                    rawModel.layerSurfaces[layern].surfaceVoxels[v].color = magFilter(imageRate1, texType);
                    // put the color information into rawData for draw iso-surface
                    int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                    int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                    int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                    rawModel.rawData[y][x][z].color = rawModel.layerSurfaces[layern].surfaceVoxels[v].color;
                }
                else
                {
                    // 1 x 1
                    glm::ivec2 imageRate0 = {(latticeTexCoord0.x * (double)(tex.imageTex[texType].width - 1)), (latticeTexCoord0.y * (double)(tex.imageTex[texType].height - 1))};
                    glm::ivec2 imageRate2 = {(latticeTexCoord2.x * (double)(tex.imageTex[texType].width - 1)), (latticeTexCoord2.y * (double)(tex.imageTex[texType].height - 1))};
                    // 2 x 2
                    // glm::ivec2 imageRate0 = {(latticeTexCoord0.x * (double)(tex.imageTex[texType].width - 1) * 0.5), (latticeTexCoord0.y * (double)(tex.imageTex[texType].height - 1))};
                    // glm::ivec2 imageRate2 = {(latticeTexCoord2.x * (double)(tex.imageTex[texType].width - 1) * 0.5), (latticeTexCoord2.y * (double)(tex.imageTex[texType].height - 1))};
                    rawModel.layerSurfaces[layern].surfaceVoxels[v].color = minFilter(imageRate0, imageRate2, texType);
                    // put the color information into rawData for draw iso-surface
                    int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                    int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                    int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                    rawModel.rawData[y][x][z].color = rawModel.layerSurfaces[layern].surfaceVoxels[v].color;                    
                }
            }
        }
        int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
        int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
        int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
    }
    std::cout << "mapping end~" << std::endl;
}
glm::fvec3 Mapping::latticeCoord_to_textureCoord_2D(glm::fvec2 latticeCoord)
{
    int s = latticeCoord.s, t = latticeCoord.t, r = 0;

    if (latticeCoord.s == s && latticeCoord.t == t)
    
        return SOM.t_latticeData.lattice[r][t][s];

    int delta_t = 1, delta_s = 1;
    if (s == SOM.t_latticeData.width - 1)
        delta_s = -1;
    if (t == SOM.t_latticeData.height - 1)
        delta_t = -1;

    glm::fvec3 c00 = SOM.t_latticeData.lattice[r][t][s];
    glm::fvec3 c01 = SOM.t_latticeData.lattice[r][t][s + delta_s];
    glm::fvec3 c10 = SOM.t_latticeData.lattice[r][t + delta_t][s];
    glm::fvec3 c11 = SOM.t_latticeData.lattice[r][t + delta_t][s + delta_s];

    float h = latticeCoord.s - (float)s, k = latticeCoord.t - (float)t;
    glm::fvec3 c = (1 - h) * (1 - k) * c00 + h * (1 - k) * c01 + (1 - h) * k * c10 + h * k * c11;

    return c;
}
//20250407
void Mapping::unitVoxelTextureMapping(int layern)
{
    int surfaceNum = rawModel.layerSurfaces[layern].surfaceVoxels.size();
    for (int v = 0; v < surfaceNum; v++)
    {
        // voxel -> som
        double v_x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x + 0.5;
        double v_y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y + 0.5;
        double v_z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z + 0.5;

        int texType = rawModel.layerSurfaces[layern].surfaceVoxels[v].textype;

        // 花瓶瓶口不要做 texture Mapping
        if(v_y > 320.0f)
        {
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
            int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
            int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
            int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
            continue;
        }

        double minDist = 100000;
        glm::fvec3 latticeTexCoord0, latticeTexCoord1, latticeTexCoord2, tmpCoord;
        glm::ivec3 N = findBMU(v_x, v_y, v_z, &minDist); // index value
        // nearest filter: projp = bmu
        glm::vec2 minPindex = {(float)N.x, (float) N.y};
        if (latticeVoxelFilter == LINEAR) // linear filter
        {
            // v_x, v_y, v_z
            glm::fvec2 voxelToLatticeCoord;
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x, v_y, v_z}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 ) // SOM.cpp 491(out of texture bounding box)
            {
                rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
                int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord1 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.latticeData.width);
            // 2 x 2
            // latticeTexCoord1 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
            // v_x - 0.5, v_y - 0.5, v_z - 0.5
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x - 0.5, v_y - 0.5, v_z - 0.5}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 ) // SOM.cpp 491 (out of texture bounding box)
            {
                rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
                int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord0 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.latticeData.width);
            // 2 x 2
            // latticeTexCoord0 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
            // v_x + 0.5, v_y + 0.5, v_z + 0.5
            voxelToLatticeCoord = surroundTriangles(minDist, {v_x + 0.5, v_y + 0.5, v_z + 0.5}, N);
            tmpCoord = latticeCoord_to_textureCoord_2D(voxelToLatticeCoord);
            if (tmpCoord.x == 10000.0 || tmpCoord.y == 10000.0 ) // SOM.cpp 491(out of texture bounding box)
            {
                rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
                int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
                return;
            }
            // 1 x 1
            latticeTexCoord2 = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.latticeData.width);
            // 2 x 2
            // latticeTexCoord2 = tex.lattice_to_texture(glm::fvec4(tmpCoord * 0.5f, 1.0), TWODTEX, SOM.t_latticeData.width);
        }
        if (latticeVoxelFilter == NEAREST) // nearest filter
        {
            tmpCoord = latticeCoord_to_textureCoord_2D({N.x, N.y});
            latticeTexCoord0 = latticeTexCoord1 = latticeTexCoord2 = rawModel.layerSurfaces[layern].surfaceVoxels[v].texcoord = tex.lattice_to_texture(glm::fvec4(tmpCoord, 1.0), TWODTEX, SOM.t_latticeData.width);
        }
        if (latticeTexCoord0.x < 0.0 || latticeTexCoord0.x > 1.0 || latticeTexCoord0.y < 0.0 || latticeTexCoord0.y > 1.0)
        {
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
            int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
            int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
            int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else if (latticeTexCoord1.x < 0.0 || latticeTexCoord1.x > 1.0 || latticeTexCoord1.y < 0.0 || latticeTexCoord1.y > 1.0)
        {
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
            int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
            int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
            int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else if (latticeTexCoord2.x < 0.0 || latticeTexCoord2.x > 1.0 || latticeTexCoord2.y < 0.0 || latticeTexCoord2.y > 1.0)
        {
            rawModel.layerSurfaces[layern].surfaceVoxels[v].color = {1.0f, 1.0f, 1.0f};
            int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
            int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
            int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
            rawModel.rawData[y][x][z].color = {1.0f, 1.0f, 1.0f};
        }
        else
        {
            if (latticeTextureFilter == NEAREST)
            {
                glm::fvec3 color0 = {0.1, 0.21, 0.56}; // b
                glm::fvec3 color1 = {1.0, 0.8941, 0.8824}; // pink
                // 1 x 1
                glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1), latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                // 2 x 2
                // glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1) * 0.5, latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};

                int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                for (int c = 0; c < 3; c++)
                {
                    if(tex.imageTex[texType].image[(int)imageRate1.y][(int)imageRate1.x][c] / 256.0 > 0.8)
                    {
                        rawModel.layerSurfaces[layern].surfaceVoxels[v].color[c] = 1.0f; // color1[c];
                        // put the color information into rawData for draw iso-surface
                        rawModel.rawData[y][x][z].color[c] = 1.0f;
                    }
                    else
                    {
                        rawModel.layerSurfaces[layern].surfaceVoxels[v].color[c] = 0.0f; // color1[c];
                        // put the color information into rawData for draw iso-surface
                        rawModel.rawData[y][x][z].color[c] = 0.0f;
                    }
                    // rawmodel.voxelModel.somVoxel[layer][block][n].color[c] = tex.imageTex[texType].image[(int)imageRate1.y][(int)imageRate1.x][c] / 256.0;
                }
            }
            if (latticeTextureFilter == LINEAR)
            {
                // check texSize voxelSize
                int texSize = tex.imageTex[texType].height * tex.imageTex[texType].width;
                if(surfaceNum > texSize)
                    texfilter = MAGNIFICATON;

                if (texfilter == MAGNIFICATON)
                {
                    // tex < voxel
                    // 1 x 1
                    glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1), latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                    // 2 x 2
                    // glm::fvec2 imageRate1 = {latticeTexCoord1.x * (double)(tex.imageTex[texType].width - 1) * 0.5, latticeTexCoord1.y * (double)(tex.imageTex[texType].height - 1)};
                    rawModel.layerSurfaces[layern].surfaceVoxels[v].color = magFilter(imageRate1, texType);
                    // put the color information into rawData for draw iso-surface
                    int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                    int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                    int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                    rawModel.rawData[y][x][z].color = rawModel.layerSurfaces[layern].surfaceVoxels[v].color;
                }
                else
                {
                    // 1 x 1
                    glm::ivec2 imageRate0 = {(latticeTexCoord0.x * (double)(tex.imageTex[texType].width - 1)), (latticeTexCoord0.y * (double)(tex.imageTex[texType].height - 1))};
                    glm::ivec2 imageRate2 = {(latticeTexCoord2.x * (double)(tex.imageTex[texType].width - 1)), (latticeTexCoord2.y * (double)(tex.imageTex[texType].height - 1))};
                    // 2 x 2
                    // glm::ivec2 imageRate0 = {(latticeTexCoord0.x * (double)(tex.imageTex[texType].width - 1) * 0.5), (latticeTexCoord0.y * (double)(tex.imageTex[texType].height - 1))};
                    // glm::ivec2 imageRate2 = {(latticeTexCoord2.x * (double)(tex.imageTex[texType].width - 1) * 0.5), (latticeTexCoord2.y * (double)(tex.imageTex[texType].height - 1))};
                    rawModel.layerSurfaces[layern].surfaceVoxels[v].color = minFilter(imageRate0, imageRate2, texType);
                    // put the color information into rawData for draw iso-surface
                    int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
                    int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
                    int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
                    rawModel.rawData[y][x][z].color = rawModel.layerSurfaces[layern].surfaceVoxels[v].color;               
                }
            }
        }
        int x = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.x;
        int y = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.y;
        int z = rawModel.layerSurfaces[layern].surfaceVoxels[v].locate.z;
    }
    std::cout << "mapping end~" << std::endl;
}
*/
#include "texture.h"
texture_cls tex;
texture_cls::texture_cls()
{
    texMatrix2D.texture_m.Save(glm::translate(texMatrix2D.texture_m.Top(), glm::vec3(0.5f, 0.5f, 0.0f)));
    texMatrix2D.texture_m.Push();
    texMatrix2D.texture_m.Push();
    texMatrix3D.texture_m.Save(glm::translate(texMatrix3D.texture_m.Top(), glm::vec3(0.5f, 0.5f, 0.5f)));
    texMatrix3D.texture_m.Push();
}

texture_cls::~texture_cls()
{
    free(intensityTex.data);
    free(colormapTex.data);
    free(threeDTex.data);
}

glm::fvec3 texture_cls::lattice_to_texture(glm::fvec4 texCoord, int degree, int resolution)
{
    textureMap_t matrix;
    if (degree == TWODTEX)
    {
        matrix = texMatrix2D;
        matrix.texture_m.Save(glm::translate(matrix.texture_m.Top(), glm::vec3(-0.5f, -0.5f, 0.0f)));
    }
    if (degree == THREEDTEX)
    {
        matrix = texMatrix3D;
        matrix.texture_m.Save(glm::translate(matrix.texture_m.Top(), glm::vec3(-0.5f, -0.5f, -0.5f)));
    }

    // compute new texture coord
    glm::fvec3 coord = {0.0, 0.0, 0.0};
    float down = 0.0;
    for (int t = 0; t < 4; t++)
    {
        down += matrix.texture_m.Top()[t][3] * texCoord[t];
    }
    for (int texNum = 0; texNum < 3; texNum++)
    {
        float tmp = 0.0, l_tmp = 0.0;
        for (int t = 0; t < 4; t++)
        {
            tmp += matrix.texture_m.Top()[t][texNum] * texCoord[t];
        }

        coord[texNum] = (down == 0) ? 0.0 : tmp / down;
    }
    // cout << "coord : " << coord.s << ", "<<coord.t<<endl;

    float wb, we, hb, he, db, de, gap_w, gap_h, gap_d;
    wb = matrix.resolution_w.x;
    we = matrix.resolution_w.y;
    hb = matrix.resolution_h.x;
    he = matrix.resolution_h.y;
    db = matrix.resolution_d.x;
    de = matrix.resolution_d.y;
    gap_w = we - wb;
    gap_h = he - hb;
    gap_d = de - db;
    // in the range
    if ((coord.s <= we && coord.s >= wb) && (coord.t <= he && coord.t >= hb) && (coord.p <= de && coord.p >= db))
    {
        // if(coord.s == we) coord.s -= 0.0001;
        // if(coord.t == he) coord.t -= 0.0001;
        // if(coord.p == de) coord.p -= 0.0001;
        return coord;
    }
    // out of range
    // resolution && repeat
    if (matrix.wrapType == REPEAT)
    {
        float tmpCoord;
        if (coord.s > we)
        {
            tmpCoord = coord.s;
            while (tmpCoord > we)
            {
                tmpCoord -= gap_w;
            }
            coord.s = tmpCoord;
        }
        if (coord.s < wb)
        {
            tmpCoord = coord.s;
            while (tmpCoord < wb)
            {
                tmpCoord += gap_w;
            }
            coord.s = tmpCoord;
        }
        if (coord.t > he)
        {
            tmpCoord = coord.t;
            while (tmpCoord > he)
            {
                tmpCoord -= gap_h;
            }
            coord.t = tmpCoord;
        }
        if (coord.t < hb)
        {
            tmpCoord = coord.t;
            while (tmpCoord < hb)
            {
                tmpCoord += gap_h;
            }
            coord.t = tmpCoord;
        }
        if (coord.p > de)
        {
            tmpCoord = coord.p;
            while (tmpCoord > de)
            {
                tmpCoord -= gap_d;
            }
            coord.p = tmpCoord;
        }
        if (coord.p < db)
        {
            tmpCoord = coord.p;
            while (tmpCoord < db)
            {
                tmpCoord += gap_d;
            }
            coord.p = tmpCoord;
        }
        glm::fvec2 l_coord = mapping.latticeCoord_to_textureCoord_2D(glm::fvec2(coord.s * (float)(resolution - 1), coord.t * (float)(resolution - 1)));
        coord.s = l_coord.s;
        coord.t = l_coord.t;
    }

    // resolution && border
    if (matrix.wrapType == BORDER)
    {
        if (coord.s > we)
            coord.s = 100.1;
        if (coord.s < wb)
            coord.s = -100.1;
        if (coord.t > he)
            coord.t = 100.1;
        if (coord.t < hb)
            coord.t = -100.1;
        if (coord.p > db)
            coord.p = 100.1;
        if (coord.p < db)
            coord.p = -100.1;
    }

    return coord;
}

void texture_cls::create_texture()
{
    create_img_tex();
    create_depth_tex();
    create_ray_tex();
    create_3D_tex();
}

void texture_cls::bindTexture(int bind)
{

    glActiveTexture(GL_TEXTURE0 + bind);

    if (bind == 0)
    {
        glBindTexture(GL_TEXTURE_3D, imageTex[0].texture);
    }
    else if (bind == 1)
    {
        glBindTexture(GL_TEXTURE_3D, imageTex[1].texture);
    }
    else if (bind == 2)
    {
        glBindTexture(GL_TEXTURE_3D, imageTex[2].texture);
    }
    else if (bind == 3)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex.depthCubemap);
    }
    else if (bind == 4)
    {
        glBindTexture(GL_TEXTURE_3D, intensityTex.texture3D);
    }
    else if (bind == 5)
    {
        glBindTexture(GL_TEXTURE_1D, colormapTex.texture1D);
    }
    else if (bind == 6)
    {
        glBindTexture(GL_TEXTURE_3D, threeDTex.texture);
    }
}

void texture_cls::updataColorMap(std::vector<float> newdata)
{
    for (int c = 0; c < colormapTex.color + 1; c++)
    {
        for (int j = 0; j < 4; j++)
        {
            colormapTex.data[c * 4 + j] = newdata[c * 4 + j];
        }
    }
    glBindTexture(GL_TEXTURE_1D, colormapTex.texture1D);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, colormapTex.color + 1, 0, GL_RGBA, GL_FLOAT, colormapTex.data);
}

void texture_cls::updateIntensityMap()
{
    for (int z = 0; z < intensityTex.z; z++)
    {
        for (int y = 0; y < intensityTex.y; y++)
        {
            for (int x = 0; x < intensityTex.x; x++)
            {
                int num = x * 4 + y * intensityTex.x * 4 + z * intensityTex.x * intensityTex.y * 4;
                intensityTex.data[num + 3] = rawModel.rawData[y][x][z].layer / 255.0;
            }
        }
    }
    glBindTexture(GL_TEXTURE_3D, intensityTex.texture3D);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, intensityTex.x, intensityTex.y, intensityTex.z, 0, GL_RGBA, GL_FLOAT, intensityTex.data);
}
void texture_cls::create_ray_tex()
{
    // init
    intensityTex.x = rawModel.infData.resolution[1];
    intensityTex.y = rawModel.infData.resolution[2];
    intensityTex.z = rawModel.infData.resolution[0];
    intensityTex.data = (float *)calloc(intensityTex.x * intensityTex.y * intensityTex.z * 4, sizeof(float));
    for (int z = 0; z < intensityTex.z; z++)
    {
        for (int y = 0; y < intensityTex.y; y++)
        {
            for (int x = 0; x < intensityTex.x; x++)
            {
                int num = x * 4 + y * intensityTex.x * 4 + z * intensityTex.x * intensityTex.y * 4;
                float gx, gy, gz;
                if (x == 0)
                    gx = rawModel.rawData[y][x + 1][z].layer - rawModel.rawData[y][x][z].layer;
                else if (x == intensityTex.x - 1)
                    gx = rawModel.rawData[y][x][z].layer - rawModel.rawData[y][x - 1][z].layer;
                else
                {
                    gx = (rawModel.rawData[y][x + 1][z].layer - rawModel.rawData[y][x - 1][z].layer) / 2.0;
                    if (rawModel.rawData[y][x + 1][z].layer == 0 || rawModel.rawData[y][x - 1][z].layer == 0)
                        gx *= -1;
                }
                if (y == 0)
                    gy = rawModel.rawData[y + 1][x][z].layer - rawModel.rawData[y][x][z].layer;
                else if (y == intensityTex.y - 1)
                    gy = rawModel.rawData[y][x][z].layer - rawModel.rawData[y - 1][x][z].layer;
                else
                {
                    gy = (rawModel.rawData[y + 1][x][z].layer - rawModel.rawData[y - 1][x][z].layer) / 2.0;
                    if (rawModel.rawData[y + 1][x][z].layer == 0 || rawModel.rawData[y - 1][x][z].layer == 0)
                        gy *= -1;
                }

                if (z == 0)
                    gz = rawModel.rawData[y][x][z + 1].layer - rawModel.rawData[y][x][z].layer;
                else if (z == intensityTex.z - 1)
                    gz = rawModel.rawData[y][x][z].layer - rawModel.rawData[y][x][z - 1].layer;
                else
                {
                    gz = (rawModel.rawData[y][x][z + 1].layer - rawModel.rawData[y][x][z - 1].layer) / 2.0;
                    if (rawModel.rawData[y][x][z + 1].layer == 0 || rawModel.rawData[y][x][z - 1].layer == 0)
                        gz *= -1;
                }

                float dist = sqrt(gx * gx + gy * gy + gz * gz);
                intensityTex.data[num + 0] = gx / dist;
                intensityTex.data[num + 1] = gy / dist;
                intensityTex.data[num + 2] = gz / dist;
                intensityTex.data[num + 3] = rawModel.rawData[y][x][z].layer / 255.0;
            }
        }
    }
    colormapTex.data = (float *)calloc((colormapTex.color + 1) * 4, sizeof(float));
    colormapTex.data[0] = -1;

    // set
    glGenTextures(1, &intensityTex.texture3D);
    glBindTexture(GL_TEXTURE_3D, intensityTex.texture3D);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, intensityTex.x, intensityTex.y, intensityTex.z, 0, GL_RGBA, GL_FLOAT, intensityTex.data);

    glGenTextures(1, &colormapTex.texture1D);
    glBindTexture(GL_TEXTURE_1D, colormapTex.texture1D);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, colormapTex.color + 1, 0, GL_RGBA, GL_FLOAT, colormapTex.data);
}
void texture_cls::create_3D_tex()
{
    int t = 1;
    threeDTex.width = 220;  // imageTex[t].width;
    threeDTex.height = 220; // imageTex[t].height;
    threeDTex.depth = 11;
    threeDTex.data = (float *)calloc(3 * threeDTex.width * threeDTex.height * threeDTex.depth, sizeof(float));
    for (int z = 0; z < threeDTex.depth; z++)
    {
        for (int y = 0; y < threeDTex.height; y++)
        {
            for (int x = 0; x < threeDTex.width; x++)
            {
                int num = x * 3 + y * threeDTex.width * 3 + z * threeDTex.width * threeDTex.height * 3;
                int num1 = x * 3 + y * threeDTex.width * 3 + (10 - z) * threeDTex.width * threeDTex.height * 3;
                int edge = threeDTex.width / threeDTex.depth;
                // 1.
                if (z <= 1 || z >= threeDTex.depth - 2)
                {
                    if ((x <= edge * 2 || x >= edge * (threeDTex.depth - 2)) || (y <= edge * 2 || y >= edge * (threeDTex.depth - 2)))
                    {
                        threeDTex.data[num] = 0.0;
                        threeDTex.data[num + 1] = 0.0;
                        threeDTex.data[num + 2] = 1.0;
                    }
                    else
                    {
                        threeDTex.data[num] = 1.0;
                        threeDTex.data[num + 1] = 0.0;
                        threeDTex.data[num + 2] = 0.0;
                    }
                }
                else
                {
                    if ((x <= edge * 2 || x >= edge * (threeDTex.depth - 2)) && (y <= edge * 2 || y >= edge * (threeDTex.depth - 2)))
                    {
                        threeDTex.data[num] = 0.0;
                        threeDTex.data[num + 1] = 0.0;
                        threeDTex.data[num + 2] = 1.0;
                    }
                    else
                    {
                        threeDTex.data[num] = 1.0;
                        threeDTex.data[num + 1] = 0.0;
                        threeDTex.data[num + 2] = 0.0;
                    }
                }
                // 2.
                // if((x%200 >= 0 && x%200 <= 99)||(y%200 >= 0 && y%200 <= 99)){
                // // if((x <= 100 || x >= 1000)||(y<= 100 || y >= 1000)){
                //     threeDTex.data[num] = 1.0;
                //     threeDTex.data[num+1] = 0.0;
                //     threeDTex.data[num+2] = 0.0;
                // }else{
                //     threeDTex.data[num] = 0.0;
                //     threeDTex.data[num+1] = 0.0;
                //     threeDTex.data[num+2] = 1.0;
                // }
                // 3.
                // int up = (6-z)*100+10;
                // int down = (5-z)*100-10;
                // if(((x <= up && x >= down)||(x >= 1100-up && x <=1100- down))
                // &&((y<= up && y >= down)||(y>= 1100-up && y <= 1100-down))){
                //     threeDTex.data[num] = 0.0;
                //     threeDTex.data[num+1] = 0.0;
                //     threeDTex.data[num+2] = 1.0;
                //     threeDTex.data[num1] = 0.0;
                //     threeDTex.data[num1+1] = 0.0;
                //     threeDTex.data[num1+2] = 1.0;
                // }else{
                //     threeDTex.data[num] = 1.0;
                //     threeDTex.data[num+1] = 0.0;
                //     threeDTex.data[num+2] = 0.0;
                //     threeDTex.data[num1] = 1.0;
                //     threeDTex.data[num1+1] = 0.0;
                //     threeDTex.data[num1+2] = 0.0;
                // }
                // if(z == threeDTex.depth/2){
                //     if(((x <= (6-z)*100 && x >= (5-z)*100)||(x >= 1100-((6-z)*100) && x <=1100- ((5-z)*100)))
                //     ||((y<= (6-z)*100 && y >= (5-z)*100)||(y>= 1100-((6-z)*100) && y <= 1100-((5-z)*100)))){
                //         threeDTex.data[num] = 0.0;
                //         threeDTex.data[num+1] = 0.0;
                //         threeDTex.data[num+2] = 1.0;
                //         threeDTex.data[num1] = 0.0;
                //         threeDTex.data[num1+1] = 0.0;
                //         threeDTex.data[num1+2] = 1.0;
                //     }else{
                //         threeDTex.data[num] = 1.0;
                //         threeDTex.data[num+1] = 0.0;
                //         threeDTex.data[num+2] = 0.0;
                //         threeDTex.data[num1] = 1.0;
                //         threeDTex.data[num1+1] = 0.0;
                //         threeDTex.data[num1+2] = 0.0;
                //     }
                // }
            }
        }
    }
    glGenTextures(1, &threeDTex.texture);
    glBindTexture(GL_TEXTURE_3D, threeDTex.texture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, threeDTex.width, threeDTex.height, threeDTex.depth, 0, GL_RGB, GL_FLOAT, threeDTex.data);
}

void texture_cls::create_depth_tex()
{
    glGenFramebuffers(1, &shadowTex.depthFBO);

    glGenTextures(1, &shadowTex.depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex.depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowTex.width, shadowTex.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowTex.depthFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex.depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// to read three texture in the same time(can choose different texture in UI)(我沒有做)
void texture_cls::create_img_tex()
{
    for (int t = 0; t < 3; t++)
    {
        glGenTextures(1, &imageTex[t].texture);
        glBindTexture(GL_TEXTURE_3D, imageTex[t].texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // load image, create texture and generate mipmaps
        int nrChannels;
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        imageTex[t].data = stbi_load(texName[t], &imageTex[t].width, &imageTex[t].height, &nrChannels, 0);

        if (imageTex[t].data)
        {
            if (nrChannels == 3)
            {
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, imageTex[t].width, imageTex[t].height, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, imageTex[t].data);
            }
            else if (nrChannels == 4)
            {
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, imageTex[t].width, imageTex[t].height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageTex[t].data);
            }
            else
            {
                cout << "ncrChannel error!" << endl;
            }
            // glGenerateMipmap(GL_TEXTURE_3D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        texture1Dto2D(nrChannels, t);

        stbi_image_free(imageTex[t].data);
    }
}

void texture_cls::texture1Dto2D(int nrChannels, int t)
{
    // create image array
    imageTex[t].image = (glm::fvec3 **)calloc(imageTex[t].height, sizeof(glm::fvec3 *));
    for (int h = 0; h < imageTex[t].height; h++)
    {
        imageTex[t].image[h] = (glm::fvec3 *)calloc(imageTex[t].width, sizeof(glm::fvec3));
        for (int w = 0; w < imageTex[t].width; w++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (nrChannels == 3)
                {
                    imageTex[t].image[h][w][k] = imageTex[t].data[h * imageTex[t].width * 3 + w * 3 + k];
                }
                if (nrChannels == 4)
                {
                    imageTex[t].image[h][w][k] = imageTex[t].data[h * imageTex[t].width * 4 + w * 4 + k];
                }
            }
        }
    }
    std::cout << "Image(w x h) " << t << " : " << imageTex[t].width << " x " << imageTex[t].height << std::endl;
}

void texture_cls::free_image_memory()
{
    //     free(img.data);

    //     glm::ivec2 boundary;
    //     boundary.x = left_up.x + width - 1;
    //     boundary.y = left_up.y + height - 1;

    //     if(boundary.x > tex.img.width)
    //         boundary.x = tex.img.width;

    //     if(boundary.y > tex.img.height)
    //         boundary.y = tex.img.height;

    //     for (int i = left_up.x; i < boundary.x; i++)
    //     {
    //         for (int j = left_up.y; j < boundary.y; j++)
    //         {
    //             free(img.image[i][j]); // release the channels of image
    //         }
    //         free(img.image[i]); // release the rows of image
    //     }
    //     free(img.image);  // release the entire image
}
#include "ReadVoxelModel.h"

rawModel_cls rawModel;

rawModel_cls::rawModel_cls()
{
}

rawModel_cls::~rawModel_cls()
{
    // free(surfaceVoxelData);

    for (int i = 0; i < infData.resolution[2]; i++)
    {
        for (int j = 0; j < infData.resolution[1]; j++)
        {
            free(voxelData[i][j]);
            free(voxelData2[i][j]);
            free(voxelData3[i][j]);
            free(weightData[i][j]);
        }
        free(voxelData[i]);
        free(voxelData2[i]);
        free(voxelData3[i]);
        free(weightData[i]);
    }
    free(voxelData);
    free(voxelData2);
    free(voxelData3);
    free(weightData);
}

void rawModel_cls::LoadFile(const char *infFileName, const char *rawFileName, const char *o_inf)
{
    LoadInfFile(infFileName, o_inf);
    CreateRawData();
    LoadRawFile(rawFileName);
}

bool rawModel_cls::LoadInfFile(const char* infFileName, const char* o_inf)
{
    FILE *file = NULL, *o_file = NULL;
    errno_t err;                    // record error
    file = fopen(infFileName, "r"); // err = 0 success, err != fail

    if (file == NULL)
    {
        std::cout << "Failed to open inffile" << std::endl;
        return false;
    }

    char buffer[133];
    int lineno = 0; // record line number // total 12 line
    char c[10];
    // start reading
    fgets(buffer, sizeof(buffer), file); // raw-file=XXX.raw
    lineno++;
    fgets(buffer, sizeof(buffer), file); // resolution=XX*XX*XX
    sscanf(buffer, "resolution=%dx%dx%d", &infData.resolution[0], &infData.resolution[1], &infData.resolution[2]);
    std::cout << "resolution : " << infData.resolution[0] << ", " << infData.resolution[1] << ", " << infData.resolution[2] << std::endl;

    fgets(buffer, sizeof(buffer), file); // sample-type=XXXXXX
    sscanf(buffer, "sample-type=%s", c);
    if (set_sample_type(c) == false)
    {
        std::cout << "Failed to set sample type" << std::endl;
        return false;
    }
    std::cout << infData.sample_type << std::endl;

    fgets(buffer, sizeof(buffer), file); // voxel-size=XXX:XXX:XXX
    sscanf(buffer, "voxel-size=%f:%f:%f", &infData.voxel_size[0], &infData.voxel_size[1], &infData.voxel_size[2]);
    // std::cout << "voxel-size : "<<infdata.voxelSize[0] << ", " << infdata.voxelSize[1] << ", " << infdata.voxelSize[2] << std::endl;

    fgets(buffer, sizeof(buffer), file); // endian=XXX
    sscanf(buffer, "endian=%s", infData.endian);
    // std::cout << infdata.endian << std::endl;

    if (feof(file))
    {
        std::cout << "End of file reached!" << std::endl;
    }
    else if (ferror(file))
    {
        std::cout << "Encountered an error while reading the file!" << std::endl;
    }

    fclose(file);
    file = fopen(infFileName, "r");
    o_file = fopen(o_inf, "w");
    char ch;
    while ((ch = getc(file)) != EOF)
    {
        putc(ch, o_file);
    }
    fclose(file);
    fclose(o_file);

    return true;
}

void rawModel_cls::CreateRawData()
{
    if (infData.type == 0)
    {
        uc_voxelData = (unsigned char *)malloc(sizeof(unsigned char) * infData.resolution[0] * infData.resolution[1] * infData.resolution[2]);
    }
    else if (infData.type == 1)
    {
        f_voxelData = (float *)malloc(sizeof(float) * infData.resolution[0] * infData.resolution[1] * infData.resolution[2]);
    }
    else if (infData.type == 2)
    {
        d_voxelData = (double *)malloc(sizeof(double) * infData.resolution[0] * infData.resolution[1] * infData.resolution[2]);
    }
    rawData = (RawData_t ***)malloc(sizeof(RawData_t **) * infData.resolution[2]);
    smoothData = (double ***)malloc(sizeof(double **) * infData.resolution[2]);
    voxelData = (double ***)malloc(sizeof(double **) * infData.resolution[2]);
    voxelData2 = (double ***)malloc(sizeof(double **) * infData.resolution[2]);
    voxelData3 = (double ***)malloc(sizeof(double **) * infData.resolution[2]);
    weightData = (double ***)malloc(sizeof(double **) * infData.resolution[2]);
    voteData = (int ***)malloc(sizeof(int **) * infData.resolution[2]);
    for (int i = 0; i < infData.resolution[2]; i++)
    {
        rawData[i] = (RawData_t **)malloc(sizeof(RawData_t *) * infData.resolution[1]);
        smoothData[i] = (double **)malloc(sizeof(double *) * infData.resolution[1]);
        voxelData[i] = (double **)malloc(sizeof(double *) * infData.resolution[1]); // distance
        voxelData2[i] = (double **)malloc(sizeof(double *) * infData.resolution[1]);
        voxelData3[i] = (double **)malloc(sizeof(double *) * infData.resolution[1]);
        weightData[i] = (double **)malloc(sizeof(double *) * infData.resolution[1]);
        voteData[i] = (int **)malloc(sizeof(int *) * infData.resolution[1]);
        for (int j = 0; j < infData.resolution[1]; j++)
        {
            rawData[i][j] = (RawData_t *)malloc(sizeof(RawData_t) * infData.resolution[0]);
            smoothData[i][j] = (double *)malloc(sizeof(double) * infData.resolution[0]);
            voxelData[i][j] = (double *)malloc(sizeof(double) * infData.resolution[0]);
            voxelData2[i][j] = (double *)malloc(sizeof(double) * infData.resolution[0]);
            voxelData3[i][j] = (double *)malloc(sizeof(double) * infData.resolution[0]);
            weightData[i][j] = (double *)malloc(sizeof(double) * infData.resolution[0]);
            voteData[i][j] = (int *)malloc(sizeof(int) * infData.resolution[0]);
            for (int k = 0; k < infData.resolution[0]; k++)
            {
                // distance
                rawData[i][j][k].layer = -1;
                rawData[i][j][k].air = false;
                rawData[i][j][k].draw = false;
                // iso distance
                smoothData[i][j][k] = -1;
                weightData[i][j][k] = 1.0;
            }
        }
    }
}

bool rawModel_cls::LoadRawFile(const char* rawFileName)
{
    FILE *file = NULL;
    errno_t err;                    // record error
    file = fopen(rawFileName, "r"); // 0: success, 1: fail
    if (file == NULL)
    {
        std::cout << "Failed to open raw file!" << std::endl;
        return false;
    }
    // read raw file to 3D array
    if (!ReadRawFile(file))
    {
        std::cout << "Failed to load raw file!" << std::endl;
    }
    // laplacian smoothing
    // laplacianSmoothing(3);
    // set voxelData(0: air, 1: bounder, 2: inside)
    set_raw_data();
    // error detect
    if (feof(file))
        std::cout << "End of file reached!" << std::endl;
    else if (ferror(file))
        std::cout << "Encountered an error while reading the file!" << std::endl;

    fclose(file);
    return true;
}

glm::fvec3 rawModel_cls::findGradient(int x, int y, int z)
{
    glm::fvec3 normal;
    if(x == 0 || x == infData.resolution[1] - 1)
        normal.x = 0;
    if(y == 0 || y == infData.resolution[2] - 1)
        normal.y = 0;
    if(z == 0 || z == infData.resolution[0] - 1)
        normal.z = 0;
    else
        normal = {(rawData[y][x + 1][z].layer - rawData[y][x - 1][z].layer) * 0.5f, (rawData[y + 1][x][z].layer - rawData[y - 1][x][z].layer) * 0.5f, (rawData[y][x][z + 1].layer - rawData[y][x][z - 1].layer) * 0.5f};
    normal = glm::normalize(normal); // Normalize the normal vector
    
    return normal;
}


void rawModel_cls::findNeighbor(int x, int y, int z)
{
    int count = 0;
    // 6-connected neighborhood
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
        count++;
    }
    weightData[y][x][z] = 1.0 / (double)(count + 1);
}

void rawModel_cls::set_raw_data()
{
    int layerNum = choiceLayerNum;
    int len = abs(maxDist) + abs(minDist) + 1;
    layerSurfaces.resize(len);
    centerIdx = (len + 1) / 2;
    int startLayer = rawModel.initLayer - 1;
    int endLayer = rawModel.initLayer + rawModel.choiceLayerNum;
    int layerIdx;
    for (int y = 0; y < infData.resolution[2]; y++)
    {
        for (int x = 0; x < infData.resolution[1]; x++)
        {
            for (int z = 0; z < infData.resolution[0]; z++)
            {
                // findNeighbor(x, y, z);
                int voxelLayer = rawData[y][x][z].layer;
                if(voxelLayer > startLayer && voxelLayer < endLayer) // surface model (SOM need to filter this layer)
                {
                    // surface voxels
                    glm::fvec3 normal = findGradient(x, y, z);
                    voxelData[y][x][z] = 0;
                    voxelData2[y][x][z] = 0;
                    layerSurfaces[centerIdx].surfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.2f, 0.7f}, {normal.x, normal.y, normal.z}});
                    findSurfaceVoxel(y, x, z, layerSurfaces[centerIdx].surfaceVoxels.size() - 1, centerIdx, SURFACE);
                    if(voxelLayer > startLayer && voxelLayer <= startLayer + 1) // dis 1
                    {
                        layerIdx = centerIdx;
                        layerSurfaces[layerIdx].oneSurfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.2f, 0.7f}, {normal.x, normal.y, normal.z}});
                    }
                    else if(voxelLayer > startLayer + 1 && voxelLayer <= startLayer + 2) // dis 2
                    {
                        layerIdx = centerIdx - 1;
                        layerSurfaces[layerIdx].oneSurfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.2f, 0.7f}, {normal.x, normal.y, normal.z}});
                    }
                    else if(voxelLayer > startLayer + 2 && voxelLayer <= startLayer + 3) // dis 3
                    {
                        layerIdx = centerIdx - 2;
                        layerSurfaces[layerIdx].oneSurfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.2f, 0.7f}, {normal.x, normal.y, normal.z}});
                    }
                    else if(voxelLayer > startLayer + 3 && voxelLayer <= startLayer + 4) // dis 4
                    {
                        layerIdx = centerIdx - 3;
                        layerSurfaces[layerIdx].oneSurfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.2f, 0.7f}, {normal.x, normal.y, normal.z}});
                    }
                    else if(voxelLayer > startLayer + 4 && voxelLayer <= startLayer + 5) // dis 5
                    {
                        layerIdx = centerIdx - 4;
                        layerSurfaces[layerIdx].oneSurfaceVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.2f, 0.7f}, {normal.x, normal.y, normal.z}});
                    }
                }
                else if(voxelLayer == endLayer) // inner voxel
                {
                    // layerSurfaces[centerIdx].innerVoxels.push_back(surfaceVoxel{{x, y, z}, {1.0f, 0.8627f, 0.2078f}, {}}); // 黃色
                    layerSurfaces[centerIdx].innerVoxels.push_back(surfaceVoxel{{x, y, z}, {0.6235f, 0.2078f, 1.0f}, {}}); // 紫色
                    findSurfaceVoxel(y, x, z, layerSurfaces[centerIdx].innerVoxels.size() - 1, centerIdx, INNER);
                }
            }
            // std::cout << std::endl;
        }
        // std::cout << std::endl;
    }
    // std::cout << std::endl;
}

bool rawModel_cls::read_raw_file(FILE *file)
{
    int size = infData.resolution[0] * infData.resolution[1] * infData.resolution[2];
    // surfaceNum = 0;
    if (infData.type == 0)
    {
        fread(uc_voxelData, sizeof(unsigned char), size, file);
        for (int i = 0; i < infData.resolution[2]; i++)
        {
            for (int j = 0; j < infData.resolution[1]; j++)
            {
                for (int k = 0; k < infData.resolution[0]; k++)
                {
                    int num = k + j * infData.resolution[0] + i * infData.resolution[0] * infData.resolution[1];
                    voxelData[i][j][k] = uc_voxelData[num];
                    voxelData2[i][j][k] = uc_voxelData[num];
                    voxelData3[i][j][k] = uc_voxelData[num];
                    // if (voxelData[i][j][k] == 255)
                        // surfaceNum++;
                }
            }
        }
        // std::cout << "surfaceNum: " << surfaceNum << std::endl;
        // surfaceVoxelData = (surfaceVoxel *)(calloc(surfaceNum, sizeof(surfaceVoxel)));
        return true;
    }
    else if (infData.type == 1)
    {
        fread(f_voxelData, sizeof(float), size, file);
        for (int i = 0; i < infData.resolution[2]; i++)
        {
            for (int j = 0; j < infData.resolution[1]; j++)
            {
                for (int k = 0; k < infData.resolution[0]; k++)
                {
                    int num = k + j * infData.resolution[0] + i * infData.resolution[0] * infData.resolution[1];
                    voxelData[i][j][k] = f_voxelData[num];
                    voxelData2[i][j][k] = f_voxelData[num];
                    voxelData3[i][j][k] = f_voxelData[num];
                    // if (voxelData[i][j][k] == 255)
                    //     surfaceNum++;
                }
            }
        }
        // std::cout << "surfaceNum: " << surfaceNum << std::endl;
        // surfaceVoxelData = (surfaceVoxel *)(calloc(surfaceNum, sizeof(surfaceVoxel)));
        return true;
    }
    else if (infData.type == 2)
    {
        fread(d_voxelData, sizeof(double), size, file);
        for (int i = 0; i < infData.resolution[2]; i++)
        {
            for (int j = 0; j < infData.resolution[1]; j++)
            {
                for (int k = 0; k < infData.resolution[0]; k++)
                {
                    int num = k + j * infData.resolution[0] + i * infData.resolution[0] * infData.resolution[1];
                    voxelData[i][j][k] = d_voxelData[num];
                    voxelData2[i][j][k] = d_voxelData[num];
                    voxelData3[i][j][k] = d_voxelData[num];
                    // if (voxelData[i][j][k] == 255)
                        // surfaceNum++;
                }
            }
        }
        // std::cout << "surfaceNum: " << surfaceNum << std::endl;
        // surfaceVoxelData = (surfaceVoxel *)(calloc(surfaceNum, sizeof(surfaceVoxel)));
        return true;
    }
    return false;
}

bool rawModel_cls::ReadRawFile(FILE *file)
{
    int layerNum = choiceLayerNum;
    layerVoxelNum.resize(layerNum);
    int size = infData.resolution[0] * infData.resolution[1] * infData.resolution[2];
    if (infData.type == 0)
    {
        fread(uc_voxelData, sizeof(unsigned char), size, file);
        fclose(file);
        for (int i = 1; i < infData.resolution[2] - 1; i++)
        {
            for (int j = 1; j < infData.resolution[1] - 1; j++)
            {
                for (int k = 1; k < infData.resolution[0] - 1; k++)
                {
                    int num = k + j * infData.resolution[0] + i * infData.resolution[0] * infData.resolution[1];
                    voxelData[i][j][k] = uc_voxelData[num];
                    voxelData2[i][j][k] = uc_voxelData[num];
                    voxelData3[i][j][k] = uc_voxelData[num];
                    rawData[i][j][k].layer = uc_voxelData[num];
                    if (maxDist < uc_voxelData[num])
                        maxDist = uc_voxelData[num];
                    if (minDist > uc_voxelData[num])
                        minDist = uc_voxelData[num];
                    for(int n = 0; n < layerNum; n++)
                    {
                        if(rawData[i][j][k].layer == initLayer + n)
                        {
                            layerVoxelNum[n]++;
                        }
                    }
                }
            }
        }
        std::cout << "maxDist: " << maxDist << " " << "minDist: " << minDist << std::endl;
        return true;
    }
    else if (infData.type == 1)
    {
        fread(f_voxelData, sizeof(float), size, file);
        for (int i = 0; i < infData.resolution[2]; i++)
        {
            for (int j = 0; j < infData.resolution[1]; j++)
            {
                for (int k = 0; k < infData.resolution[0]; k++)
                {
                    int num = k + j * infData.resolution[0] + i * infData.resolution[0] * infData.resolution[1];
                    voxelData[i][j][k] = f_voxelData[num];
                    voxelData2[i][j][k] = f_voxelData[num];
                    voxelData3[i][j][k] = f_voxelData[num];
                    rawData[i][j][k].layer = f_voxelData[num];
                    if (maxDist < f_voxelData[num])
                        maxDist = f_voxelData[num];
                    if (minDist > f_voxelData[num])
                        minDist = f_voxelData[num];
                    for(int n = 0; n < layerNum; n++)
                    {
                        if(rawData[i][j][k].layer == initLayer + n)
                        {
                            layerVoxelNum[n]++;
                        }
                    }
                }
            }
        }
        std::cout << "maxDist: " << maxDist << " " << "minDist: " << minDist << std::endl;
        return true;
    }
    else if (infData.type == 2)
    {
        fread(d_voxelData, sizeof(double), size, file);
        for (int i = 0; i < infData.resolution[2]; i++)
        {
            for (int j = 0; j < infData.resolution[1]; j++)
            {
                for (int k = 0; k < infData.resolution[0]; k++)
                {
                    int num = k + j * infData.resolution[0] + i * infData.resolution[0] * infData.resolution[1];
                    voxelData[i][j][k] = d_voxelData[num];
                    voxelData2[i][j][k] = d_voxelData[num];
                    voxelData3[i][j][k] = d_voxelData[num];
                    rawData[i][j][k].layer = d_voxelData[num];
                    if (maxDist < d_voxelData[num])
                        maxDist = d_voxelData[num];
                    if (minDist > d_voxelData[num])
                        minDist = d_voxelData[num];
                    for(int n = 0; n < layerNum; n++)
                    {
                        if(rawData[i][j][k].layer == initLayer + n)
                        {
                            layerVoxelNum[n]++;
                        }
                    }
                }
            }
        }
        std::cout << "maxDist: " << maxDist << " " << "minDist: " << minDist << std::endl;
        return true;
    }
    return false;
}

bool rawModel_cls::isFaceAir(int x, int y, int z)
{
    if(x < 0 || y < 0 || z < 0 || x >= rawModel.infData.resolution[1] || y >= rawModel.infData.resolution[2] || z >= rawModel.infData.resolution[0] || rawModel.voxelData[y][x][z] == 0)
        return false;
    else
        return true;
}

void rawModel_cls::set_surface_voxel_face_air(int x, int y, int z, surfaceVoxel &surfaceVoxel)
{
    surfaceVoxel.faceAir[0] = isFaceAir(x, y, z - 1); // -z
    surfaceVoxel.faceAir[1] = isFaceAir(x, y, z + 1); // +z
    surfaceVoxel.faceAir[2] = isFaceAir(x - 1, y, z); // -x
    surfaceVoxel.faceAir[3] = isFaceAir(x + 1, y, z); // +x
    surfaceVoxel.faceAir[4] = isFaceAir(x, y - 1, z); // -y
    surfaceVoxel.faceAir[5] = isFaceAir(x, y + 1, z); // +y
}

void rawModel_cls::findSurfaceVoxel(int y, int x, int z, int num, int layer, int voxelType)
{
    if(z + 1 < infData.resolution[0])
    {
        if(rawData[y][x][z + 1].layer != num)
        {
            if(voxelType == INNER)
                layerSurfaces[layer].innerVoxels[num].faceAir[0] = true;
            if(voxelType == SURFACE)
                layerSurfaces[layer].surfaceVoxels[num].faceAir[0] = true;
        }
    }
    if(z - 1 >= 0)
    {
        if(rawData[y][x][z - 1].layer != num)
        {
            if(voxelType == INNER)
                layerSurfaces[layer].innerVoxels[num].faceAir[1] = true;
            if(voxelType == SURFACE)
                layerSurfaces[layer].surfaceVoxels[num].faceAir[1] = true;
        }
    }
    if(x + 1 < infData.resolution[1])
    {
        if(rawData[y][x + 1][z].layer != num)
        {
            if(voxelType == INNER)
                layerSurfaces[layer].innerVoxels[num].faceAir[2] = true;
            if(voxelType == SURFACE)
                layerSurfaces[layer].surfaceVoxels[num].faceAir[2] = true;
        }
    }
    if(x - 1 >= 0)
    {
        if(rawData[y][x - 1][z].layer != num)
        {
            if(voxelType == INNER)
                layerSurfaces[layer].innerVoxels[num].faceAir[3] = true;
            if(voxelType == SURFACE)
                layerSurfaces[layer].surfaceVoxels[num].faceAir[3] = true;
        }
    }
    if(y + 1 < infData.resolution[2])
    {
        if(rawData[y + 1][x][z].layer != num)
        {
            if(voxelType == INNER)
                layerSurfaces[layer].innerVoxels[num].faceAir[4] = true;
            if(voxelType == SURFACE)
                layerSurfaces[layer].surfaceVoxels[num].faceAir[4] = true;
        }
    }
    if(y - 1 >= 0)
    {
        if(rawData[y - 1][x][z].layer != num)
        {
            if(voxelType == INNER)
                layerSurfaces[layer].innerVoxels[num].faceAir[5] = true;
            if(voxelType == SURFACE)
                layerSurfaces[layer].surfaceVoxels[num].faceAir[5] = true;
        }
    }
}

bool rawModel_cls::set_sample_type(const char *type)
{
    if (!strcmp(type, "unsigned"))
    {
        // memset(void* ptr, int value, size_t num)
        memset(infData.sample_type, '\0', sizeof(infData.sample_type)); // sample type of inf file set to '\0'
        strcat(infData.sample_type, "unsigned char");
        infData.type = 0;
        return true;
    }
    else if (!strcmp(type, "float"))
    {
        memset(infData.sample_type, '\0', sizeof(infData.sample_type));
        strcat(infData.sample_type, "float");
        infData.type = 1;
        return true;
    }
    else if (!strcmp(type, "double"))
    {
        memset(infData.sample_type, '\0', sizeof(infData.sample_type));
        strcat(infData.sample_type, "double");
        infData.type = 2;
        return true;
    }
    return false;
}

void rawModel_cls::check_computer_endian()
{
    short int a = 0x1234;
    char *p = (char *)&a;

    std::cout << "p=" << *p << std::endl;

    if (*p == 0x34)
        std::cout << "Little Endian \n"
                  << std::endl;
    else if (*p == 0x12)
        std::cout << "Big Endian \n"
                  << std::endl;
    else
        std::cout << "Unknow Endian \n"
                  << std::endl;
}

bool rawModel_cls::is_surface_voxel(int x, int y, int z)
{
    int startLayer = rawModel.initLayer - 1;
    if(rawModel.rawData[y][x][z].layer > startLayer && rawModel.rawData[y][x][z].layer <= startLayer + rawModel.choiceLayerNum)
        return true;
    return false;
}

void rawModel_cls::laplacianSmoothing(int times)
{
    for (int y = 0; y < rawModel.infData.resolution[2] - 1; y++)
    {
        for (int x = 0; x < rawModel.infData.resolution[1] - 1; x++)
        {
            for (int z = 0; z < rawModel.infData.resolution[0] - 1; z++)
            {
            //     double sum = 0.0, weightSum = 0.0;
            //     int count = 0;
                double neighborSum = 0.0, neighborWeight = 0.0;
                float selfWeight = rawModel.weightData[y][x][z]; // set to 1.0
                double selfVal = rawModel.rawData[y][x][z].layer;
                // 6-connected neighborhood
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

                    float weight = rawModel.weightData[ny][nx][nz];
                    // sum += rawModel.rawData[ny][nx][nz].layer * weight;
                    // weightSum += weight;
                    neighborSum += rawModel.rawData[ny][nx][nz].layer * weight;
                    neighborWeight += weight;
                }
                // double selfWeight = rawModel.weightData[y][x][z];
                // sum += rawModel.rawData[y][x][z].layer * selfWeight;
                // weightSum += selfWeight;
                // rawModel.smoothData[y][x][z] = is_surface_voxel(x, y, z) ? 0 : sum / weightSum;
                // α 可調整自身比重，避免過度平滑
                float alpha = 0.2f;
                rawModel.smoothData[y][x][z] = is_surface_voxel(x, y, z) ? 0 :
                (alpha * selfWeight * selfVal + (1 - alpha) * neighborSum) /
                (alpha * selfWeight + (1 - alpha) * neighborWeight);
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
                double sum = 0.0, weightSum = 0.0;
                int count = 0;
                // 6-connected neighborhood
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

                    float weight = rawModel.weightData[ny][nx][nz];
                    sum += rawModel.voxelData[ny][nx][nz] * weight;
                    weightSum += weight;
                }
                // double selfWeight = rawModel.weightData[y][x][z];
                // sum += rawModel.voxelData[y][x][z] * selfWeight;
                // weightSum += selfWeight;
                rawModel.smoothData[y][x][z] = is_surface_voxel(x, y, z) ? 0 : sum / weightSum;
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
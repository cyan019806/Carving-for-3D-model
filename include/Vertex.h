#ifndef VERTEX_H
#define VERTEX_H

struct Vertex{
    struct Position{
        float x, y, z;
    };

    struct Normal {
        float xn, yn, zn;
    };

    struct Texcoord {
        float s, t, r;
    };

    struct Color{
        float r, g, b;
    };

    Position position;
    Normal normal;
    Color color;
    Texcoord texcoord;
};

#endif
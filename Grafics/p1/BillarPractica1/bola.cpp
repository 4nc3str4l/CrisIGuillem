#include "bola.h"

vec3 v[4] = {
    {0.0, 0.0, 1.0},
    {0.0, 0.942809, -0.333333},
    {-0.816497, 0.471405, -0.333333},
    {0.816497, -0.471405, -0.333333}
};


int k = 0;
void Bola::triangle(vec3 a, vec3 b, vec3 c)
{
    points[k] = a; ++k;
    points[k] = b; ++k;
    points[k] = c; ++k;
}

void Bola::divide_triangle(vec3 a, vec3 b, vec3 c, int n)
{
    vec3 v1, v2, v3;
    int j;

    if(n>0)
    {
        v1 = normalize(a + b);
        v2 = normalize(a + c);
        v3 = normalize(b + c);

        divide_triangle(a, v2, v1, n-1);
        divide_triangle(c, v3, v2, n-1);
        divide_triangle(b, v1, v3, n-1);
        divide_triangle(v1, v2, v3, n-1);
    }
    else
        triangle(a, b, c);
}

void Bola::tetrahedron(int n)
{
    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n);
    divide_triangle(v[0], v[3], v[1], n);
    divide_triangle(v[0], v[2], v[3], n);
}

const int iter = 5;
const int len = 4 * (int)std::pow(4, iter) * 3;

Bola::Bola(vec3 color):
    Objecte(len * 2)
{
    tam = 1;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    tetrahedron(iter);

    for (int i = 0; i <  len * 2; ++i) {
        if (i < len) {
            points[len * 2 - i - 1] = vec4(-points[i].x, -points[i].y, -points[i].z, 1);
        }
        colors[i] = color;
    }

    calculCapsa3D();
}

Bola::~Bola()
{

}



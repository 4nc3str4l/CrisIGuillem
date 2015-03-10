#include "bola.h"

vec3 v[4] = {
    vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.942809, -0.333333),
    vec3(-0.816497, 0.471405, -0.333333),
    vec3(0.816497, -0.471405, -0.333333)
};


void Bola::triangle(vec3 a, vec3 b, vec3 c)
{
    points[k] = vec4(a);
    points[k+1] = vec4(b);
    points[k+2] = vec4(c);
    k += 3;
}

void Bola::divide_triangle(vec3 a, vec3 b, vec3 c, int n)
{
    vec3 v1, v2, v3;

    if(n > 0)
    {
        vec3 v1 = normalize(a + b);
        vec3 v2 = normalize(a + c);
        vec3 v3 = normalize(b + c);

        divide_triangle(a, v2, v1, n-1);
        divide_triangle(c, v3, v2, n-1);
        divide_triangle(b, v1, v3, n-1);
        divide_triangle(v1, v2, v3, n-1);
    }
    else
        triangle(a, b, c);
}

void Bola::generar(int n)
{
    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n);
    divide_triangle(v[0], v[3], v[1], n);
    divide_triangle(v[0], v[2], v[3], n);
}

const unsigned int iter = 5;
const unsigned int len = 4 * (int)std::pow(4, iter) * 3;

Bola::Bola(vec3 color):
    Objecte(len * 2),
    k(0)
{
    tam = 1;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    generar(iter);

    for (unsigned int i = 0; i <  len * 2; ++i) {
        if (i < len) {
            points[len * 2 - i - 1] = vec4(-points[i].x, -points[i].y, -points[i].z, 1);
        }
        colors[i] = color;
    }

    this->calculCapsa3D();

}

Bola::~Bola()
{

}



#ifndef _CARA_H_
#define _CARA_H_

#include <vector>
#include <Common.h>

using namespace std;

typedef vec4 Vertices;
// Face - representa una cara d'un objecte 3D
class Cara
{
 public:
    Cara();

	// constructor a partir de 3 o 4 indexs a vertex
    Cara(int i1, int i2, int i3, int i4=-1);

    // Color
    Cara setColor(int i1, int i2, int i3, int i4 = -1);
    Cara setColor(vec4 color);

	// atributs
    bool homogeneousColor;
    vec4 color;
    vector<int> idxColors;
    vector<int> idxVertices;  // vector amb els indexs dels vertexs de la cara

    vec3 normal;
    void calculaNormal(vector<Vertices> &);
};



#endif

#include "cara.h"
#include <cmath>

Cara::Cara() 
{
    normal.x=0.0;
    normal.y=0.0;
    normal.z=0.0;

    homogeneousColor = false;
    color = vec3(0.0, 0.0, 1.0);
}

Cara::Cara(int i1, int i2, int i3, int i4)
{
    Cara();

    idxVertices.push_back(i1);
    idxVertices.push_back(i2);
    idxVertices.push_back(i3);
    if (i4!=-1) idxVertices.push_back(i4);
}

Cara Cara::setColor(int i1, int i2, int i3, int i4) {
    idxColors.push_back(i1);
    idxColors.push_back(i2);
    idxColors.push_back(i3);
    if (i4!=-1) idxColors.push_back(i4);

    return *this;
}

Cara Cara::setColor(vec4 color) {
    this->color = color;
    this->homogeneousColor = true;
    return *this;
}

void Cara::calculaNormal(vector<Vertices> &v)
{
  int j;
  normal.x=0.0;
  normal.y=0.0;
  normal.z=0.0;
  int n = idxVertices.size();
  for (int i=0; i<n; i++)
  {
    j = (i+1)%n;

    normal.x += ((v[idxVertices[i]].z+v[idxVertices[j]].z)*
                 (v[idxVertices[i]].y-v[idxVertices[j]].y));
    normal.y += ((v[idxVertices[i]].x+v[idxVertices[j]].x)*
                 (v[idxVertices[i]].z-v[idxVertices[j]].z));
    normal.z += ((v[idxVertices[i]].y+v[idxVertices[j]].y)*
                 (v[idxVertices[i]].x-v[idxVertices[j]].x));
  }

  normal.x *= 0.5;
  normal.y *= 0.5;
  normal.z *= 0.5;
//  normal.normalize();
}


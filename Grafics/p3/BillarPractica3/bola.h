#ifndef BOLA_H
#define BOLA_H

#include <set>
#include <map>
#include <vector>

#include "objecte.h"
#include "cara.h"

struct cmpPoint {
    bool operator()(const point4& a, const point4& b) const {
        return a.x < b.x && a.z < b.z && a.z < b.z;
    }
};

struct CaraEx
{
    point4 a;
    point4 b;
    point4 c;
    int norm;

    CaraEx(point4 a_, point4 b_, point4 c_, int norm_)
    {
        a = a_;
        b = b_;
        c = c_;
        norm = norm_;
    }
};

class Bola : public Objecte
{
public:
    Bola(vec3 color);
    virtual ~Bola();

    void triangle(vec3 a, vec3 b, vec3 c);
    void divide_triangle(vec3 a, vec3 b, vec3 c, int n);
    void make();
    void setAngle(float angle);
    float getAngle();

private:
    unsigned int k;
    float angle;

    std::map< float, std::map<float, std::map<float, std::vector< std::pair<CaraEx, int> > > > > verts;
};

#endif // BOLA_H

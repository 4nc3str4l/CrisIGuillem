#include "circle.h"

Circle::Circle()
{

}

Circle::Circle(double r):
    radius(r)
{
}

Circle::~Circle()
{

}

double Circle::getPerimeter()
{
    return 2*M_PI*radius;
}

void Circle::setRadius(double r)
{
   this->radius = r;
}

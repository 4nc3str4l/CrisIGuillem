#ifndef CIRCLE_H
#define CIRCLE_H

# define M_PI  3.14159265358979323846  /* pi */
#include "figure.h"


class Circle : public Figure
{
public:
    Circle();
    Circle(double radius);
    virtual ~Circle();
    virtual double getPerimeter();
    void setRadius(double r);
private:
    double radius;
};

#endif // CIRCLE_H

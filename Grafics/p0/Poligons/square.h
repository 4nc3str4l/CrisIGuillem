#ifndef SQUARE_H
#define SQUARE_H

#include "figure.h"

class Square : public Figure
{
public:
    Square();
    Square(double x);
    virtual ~Square();
    virtual double getPerimeter();

private:
    double faceLen;
};
#endif // SQUARE_H

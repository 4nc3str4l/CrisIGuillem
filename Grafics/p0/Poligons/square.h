#ifndef SQUARE_H
#define SQUARE_H

#include "figure.h"

class Square : public Figure
{
public:
    Square();
    Square(double x);
    ~Square();
    virtual double getPerimeter();

private:
    double fanceLen;
};
#endif // SQUARE_H

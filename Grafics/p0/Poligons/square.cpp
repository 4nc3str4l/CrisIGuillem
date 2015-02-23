#include "square.h"

Square::Square()
{

}

Square::Square(double x):
    faceLen(x)
{
}

Square::~Square()
{

}

double Square::getPerimeter(){
    return 4 * this->faceLen;
}


#include "square.h"

Square::Square()
{

}

Square::Square(double x)
{
    this->fanceLen = x;
}

Square::~Square()
{

}

double Square::getPerimeter(){
    return 4 * this->fanceLen;
}


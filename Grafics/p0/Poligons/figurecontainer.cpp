#include "figurecontainer.h"
#include "circle.h"
#include "square.h"
#include "figure.h"

FigureContainer::FigureContainer()
{

}

FigureContainer::~FigureContainer()
{
    for(int i = 0;i < this->figures.size(); i++){
        if(dynamic_cast<Square*>(figures[i])) delete(Square *)(figures[i]);
        else delete(Circle *)(figures[i]);
    }
}

void FigureContainer::addFigure(Figure *figure)
{
    this->figures.push_back(figure);
}

double FigureContainer::getPerimeters()
{
    double sum = 0.0;
    for(int i = 0; i < this->figures.size();i++){
        sum += (figures[i])->getPerimeter();
    }
    return sum;
}

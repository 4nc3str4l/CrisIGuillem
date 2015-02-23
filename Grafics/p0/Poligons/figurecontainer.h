#ifndef FIGURECONTAINER_H
#define FIGURECONTAINER_H

#include "figure.h"
#include <iostream>
#include <vector>

using namespace std;

class FigureContainer
{
public:
    FigureContainer();
    virtual ~FigureContainer();
    void addFigure(Figure *figure);
    double getPerimeters();

private:
    vector<Figure*> figures;
};

#endif // FIGURECONTAINER_H

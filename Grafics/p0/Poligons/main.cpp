#include "square.h"
#include "figurecontainer.h"
#include "circle.h"

#include <iostream>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    //Exercici 1:
    cout << "Exercici 1" << endl;
    Figure* square = new Square(10);
    Figure* circle = new Circle(10.5);
    cout << square->getPerimeter() << endl;
    cout << circle->getPerimeter() << endl;

    //Exercici 2:
    cout << "Exercici 2" << endl;
    FigureContainer figures;
    figures.addFigure(square);
    figures.addFigure(circle);

    cout << figures.getPerimeters() << endl;

    //Exercici 3:
    cout << "Exercici 3" << endl;
    static_cast<Circle*>(circle)->setRadius(2.4);
    cout << figures.getPerimeters() << endl;

    return 0;
}

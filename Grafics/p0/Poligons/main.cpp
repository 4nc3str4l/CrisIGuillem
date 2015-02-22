#include <iostream>
#include "square.h"
#include "figurecontainer.h"
#include "circle.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    //Exercici 1:
    cout << "Exercici 1" << endl;
    Square square(10);
    Circle circle(10.5);
    cout << square.getPerimeter() << endl;
    cout << circle.getPerimeter() << endl;

    //Exercici 2:
    cout << "Exercici 2" << endl;
    FigureContainer figures;
    figures.addFigure(&square);
    figures.addFigure(&circle);

    cout << figures.getPerimeters() << endl;

    //Exercici 3:
    cout << "Exercici 3" << endl;
    circle.setRadius(2.4);
    cout << figures.getPerimeters() << endl;

    delete &figures;

    return 0;

}


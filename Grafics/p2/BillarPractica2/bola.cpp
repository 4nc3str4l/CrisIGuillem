#include "bola.h"

//coordenades inicials de la bola
vec3 v[4] = {
    vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.942809, -0.333333),
    vec3(-0.816497, -0.471405, -0.333333),
    vec3(0.816497, -0.471405, -0.333333)
};

/*Genera un triagle a partir de 3 vectors, en aquest moment aprofitant que iterem per cada un dels
* punt associem també les textures.
*/
void Bola::triangle(vec3 a, vec3 b, vec3 c)
{
    points[k] = vec4(a);
    points[k+1] = vec4(b);
    points[k+2] = vec4(c);
    textures[k] = vec2(0.5 + std::atan2(a.y,a.x) / (2 * M_PI), 0.5 - std::asin(a.z) / M_PI);
    textures[k+1] = vec2(0.5 + std::atan2(b.y, b.x) / (2 * M_PI), 0.5 - std::asin(b.z) / M_PI);
    textures[k+2] = vec2(0.5 + std::atan2(c.y, c.x) / (2 * M_PI), 0.5 - std::asin(c.z) / M_PI);
    k += 3;
}

/* Mètode recursiu, que divideix el triangle en mes triangles i va generant la bola */
void Bola::divide_triangle(vec3 a, vec3 b, vec3 c, int n)
{
    vec3 v1, v2, v3;

    if(n > 0)
    {
        vec3 v1 = normalize(a + b);
        vec3 v2 = normalize(a + c);
        vec3 v3 = normalize(b + c);

        divide_triangle(a, v1, v2, n-1);
        divide_triangle(c, v2, v3, n-1);
        divide_triangle(b, v3, v1, n-1);
        divide_triangle(v1, v3, v2, n-1);
    }
    else
        triangle(a, b, c);
}

/*
 * El mètode que crida al mètode recursiu de generar els subtriangles, aquí és on especifiquem el nombre de iteracions (n)
 * */
void Bola::generar(int n)
{
    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n);
    divide_triangle(v[0], v[3], v[1], n);
    divide_triangle(v[0], v[2], v[3], n);
}

const unsigned int iter = 5;
const unsigned int len = (int)std::pow(4, iter + 1) * 3;

/*
 * Hereda d'objecte, pots pasar-li un color per defecte, es genera amb els codis abans esmetats
 * i calcula la seva capça 3d.
 * */
Bola::Bola(vec3 color):
    Objecte(len),
    k(0)
{
    tam = 1;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;


    generar(iter);

    for (unsigned int i = 0; i <  len; ++i) {
        colors[i] = color;
    }

    this->calculCapsa3D();

    //Ens serveix per mes tard poder recuperar l'objecte.
    this->setTipus(BOLA);
}

Bola::~Bola()
{

}



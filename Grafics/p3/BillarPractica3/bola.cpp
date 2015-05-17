#include "bola.h"

const int num_iter = 5;

//coordenades inicials de la bola
vec3 v[4] = {
    vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.942809, -0.333333),
    vec3(-0.816497, -0.471405, -0.333333),
    vec3(0.816497, -0.471405, -0.333333)
};

inline vec2 calculaTexCoord(vec3 p)
{
    return vec2(0.5 + std::atan2(p.y, p.x) / (2 * M_PI), 0.5 - std::asin(p.z) / M_PI);
}


/*Genera un triagle a partir de 3 vectors, en aquest moment aprofitant que iterem per cada un dels
* punt associem també les textures.
*/
void Bola::triangle(vec3 a, vec3 b, vec3 c)
{
    points[k] = vec4(a);
    points[k+1] = vec4(b);
    points[k+2] = vec4(c);

    CaraEx cara(a, b, c, k);
    verts[a.x][a.y][a.z].push_back(std::make_pair(cara, k));
    verts[b.x][b.y][b.z].push_back(std::make_pair(cara, k + 1));
    verts[c.x][c.y][c.z].push_back(std::make_pair(cara, k + 2));

    normals[k] = normalAt(a, b, c);
    normals[k+1] = normals[k];
    normals[k+2] = normals[k];

    textures[k] = calculaTexCoord(a);
    textures[k+1] = calculaTexCoord(b);
    textures[k+2] = calculaTexCoord(c);

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
void Bola::make()
{
    divide_triangle(v[0], v[1], v[2], num_iter);
    divide_triangle(v[3], v[2], v[1], num_iter);
    divide_triangle(v[0], v[3], v[1], num_iter);
    divide_triangle(v[0], v[2], v[3], num_iter);

    std::map<float, std::map<float, std::map<float, std::vector< std::pair<CaraEx, int> > > > >::iterator it3;
    std::map<float, std::map<float, std::vector< std::pair<CaraEx, int> > > >::iterator it2;
    std::map<float, std::vector< std::pair<CaraEx, int> > >::iterator it1;
    std::vector< std::pair<CaraEx, int> >::iterator it0;

    for (it3 = verts.begin(); it3 != verts.end(); ++it3)
    {
        float x = it3->first;
        std::map<float, std::map<float, std::vector< std::pair<CaraEx, int> > > > map2 = it3->second;


        for (it2 = map2.begin(); it2 != map2.end(); ++it2)
        {
            float y = it2->first;
            std::map<float, std::vector< std::pair<CaraEx, int> > > map1 = it2->second;

            for (it1 = map1.begin(); it1 != map1.end(); ++it1)
            {
                float z = it1->first;
                std::vector< std::pair<CaraEx, int> > cares = it1->second;

                vec4 suma = vec4(0, 0, 0, 0);
                for (it0 = cares.begin(); it0 != cares.end(); ++it0)
                {
                    // TODO: Evitar sumar X vegades el punt central
                    std::pair<CaraEx, int> p = ( std::pair<CaraEx, int> )*it0;
                    suma += normals[p.first.norm];
                    suma += normals[p.first.norm + 1];
                    suma += normals[p.first.norm + 2];
                }
                //suma /= cares.size();

                for (it0 = cares.begin(); it0 != cares.end(); ++it0)
                {
                    std::pair<CaraEx, int> p = ( std::pair<CaraEx, int> )*it0;
                    gouraud[p.second] = normalize(vec3(suma.x, suma.y, suma.z));
                }
            }
        }
    }
}

const unsigned int len = (int)std::pow(4, num_iter + 1) * 3;

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

    make();

    this->calculCapsa3D();

    //Ens serveix per mes tard poder recuperar l'objecte.
    this->setTipus(BOLA);
}


void Bola::setAngle(float angle)
{
    this->angle = angle;
}

float Bola::getAngle()
{
    return this->angle;
}

Bola::~Bola()
{

}



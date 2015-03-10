#include "objecte.h"

Objecte::Objecte(int npoints, QObject *parent) : numPoints(npoints) ,
    QObject(parent)
{
    points = new point4[npoints];
    colors = new color4[npoints];
}

Objecte::Objecte(int npoints, QString n) : numPoints(npoints)
{
    points = new point4[npoints];
    colors = new color4[npoints];
    std::cout<<"Estic en el constructor parametritzat del objecte\n";

    xRot = 0;
    yRot = 0;
    zRot = 0;

    readObj(n);

    make();
}


Objecte::~Objecte()
{
    delete [] points;
    delete [] colors;
}


Capsa3D Objecte::calculCapsa3D()
{
    for (int i = 0; i < numPoints; ++i) {
        if (points[i].x < capsa.pmin.x || i == 0)
            capsa.pmin.x = points[i].x;

        if (points[i].y < capsa.pmin.y || i == 0)
            capsa.pmin.y = points[i].y;

        if (points[i].z < capsa.pmin.z || i == 0)
            capsa.pmin.z = points[i].z;

        if (points[i].x > capsa.pmax.x || i == 0)
            capsa.pmax.x = points[i].x;

        if (points[i].y > capsa.pmax.y || i == 0)
            capsa.pmax.y = points[i].y;

        if (points[i].z > capsa.pmax.z || i == 0)
            capsa.pmax.z = points[i].z;
    }

    capsa.center = (capsa.pmax + capsa.pmin) / 2;
    capsa.toCenter = Translate(-capsa.center);
    capsa.fromCenter = Translate(capsa.center);

//    std::cout << std::endl;
//    std::cout << capsa.pmin.x << " " << capsa.pmin.y << " " << capsa.pmin.z << std::endl;
//    std::cout << capsa.pmax.x << " " << capsa.pmax.y << " " << capsa.pmax.z << std::endl;
//    std::cout << capsa.center.x << " " << capsa.center.y << " " << capsa.center.z << std::endl;

    return capsa;
}

void Objecte::aplicaTG(mat4 m)
{
    aplicaTGPoints(m);

    // Actualitzacio del vertex array per a preparar per pintar
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * numPoints, &points[0] );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Objecte::aplicaTGPoints(mat4 m)
{
    for ( int i = 0; i < numPoints; ++i ) {
        points[i] = m * points[i];
    }
}

void Objecte::aplicaTGCentrat(mat4 m)
{
    aplicaTG(capsa.fromCenter * m * capsa.toCenter);
}

void Objecte::toGPU(QGLShaderProgram* program){

    this->program = program;

    std::cout<<"Passo les dades de l'objecte a la GPU\n";

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4) * numPoints + sizeof(color4) * numPoints, NULL, GL_STATIC_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * numPoints, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * numPoints, sizeof(color4) * numPoints, &colors[0] );

    // Per a conservar el buffer
    int vertexLocation = program->attributeLocation("vPosition");
    int colorLocation = program->attributeLocation("vColor");

    program->enableAttributeArray(vertexLocation);
    program->enableAttributeArray(colorLocation);

    program->link();
    program->bind();

    glEnable( GL_DEPTH_TEST );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Pintat en la GPU.
void Objecte::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);
    program->setAttributeBuffer("vColor", GL_FLOAT, sizeof(point4) * numPoints, 4);

    glEnableClientState(GL_VERTEX_ARRAY);

    glPolygonMode(GL_FRONT_AND_BACK, Common::getWireframeView());
    glDrawArrays( GL_TRIANGLES, 0, numPoints );

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Objecte::make()
{

    static vec3  base_colors[] = {
        vec3( 1.0, 0.0, 0.0 ),
        vec3( 0.0, 1.0, 0.0 ),
        vec3( 0.0, 0.0, 1.0 ),
        vec3( 1.0, 1.0, 0.0 )
    };
    // Recorregut de totes les cares per a posar-les en les estructures de la GPU
    // Cal recorrer l'estructura de l'objecte per a pintar les seves cares

    int index = 0;

    for(unsigned int i=0; i<cares.size(); i++)
    {
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++)
        {
            points[index] = vertexs[cares[i].idxVertices[j]];

            if (cares[i].idxColors.size() > j) {
                colors[index] = vertexColors[cares[i].idxColors[j]];
            }
            else if (cares[i].homogeneousColor) {
                colors[index] = cares[i].color;
            }
            else {
                colors[index] = vec4(base_colors[1], 1.0);
            }

            index++;
        }
    }

    // S'ha de dimensionar uniformement l'objecte a la capsa de l'escena i s'ha posicionar en el lloc corresponent
}


// Llegeix un fitxer .obj
//  Si el fitxer referencia fitxers de materials (.mtl), encara no es llegeixen
//  Tots els elements del fitxer es llegeixen com a un unic objecte.
//

void Objecte::readObj(QString filename)
{

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        while (true)
        {
            char *comment_ptr = ReadFile::fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */
                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = ReadFile::fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = ReadFile::words[0];

            if (!strcmp (first_word, "v"))
            {
                if (nwords < 4)
                {
                    fprintf (stderr, "Too few coordinates: '%s'", ReadFile::str_orig);
                    exit (-1);
                }
                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();

                if (nwords == 5)
                {
                    QString sw(ReadFile::words[4]);
                    double w = sw.toDouble();
                    x/=w;
                    y/=w;
                    z/=w;
                }
                // S'afegeix el vertex a l'objecte
                vertexs.push_back(point4(x, y, z, 1));

            }
            else if (!strcmp (first_word, "vn")) {
            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'objecte
                // A modificar si es vol carregar mes de un objecte
                construeix_cara (&ReadFile::words[1], nwords-1, this, 0);
            }
            // added
            else if (!strcmp (first_word, "mtllib")) {
                //read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                //int size = strlen(words[1])-1;
                //while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                //currentMaterial = matlib.index(words[1]);
            }
            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }
        }
    }

    capsa = calculCapsa3D();
}

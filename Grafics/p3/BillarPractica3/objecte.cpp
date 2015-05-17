#include "objecte.h"
#include "material.h"

/**
 * Creem les arrays per guardar punts, colors i textures
 * @brief Objecte::Objecte
 * @param npoints
 * @param parent
 */
Objecte::Objecte(int npoints, QObject *parent) : numPoints(npoints) ,
    QObject(parent)
{
    material = NULL;

    points = new point4[npoints];
    normals = new vec3[npoints];
    gouraud = new vec3[npoints];
    colors = new color4[npoints];
    textures = new vec2[npoints];
}

/**
 * Fa el mateix inicialitzant unes variables per defecte
 * @brief Objecte::Objecte
 * @param npoints
 * @param n
 */
Objecte::Objecte(int npoints, QString n) : numPoints(npoints)
{
    material = NULL;

    points = new point4[npoints];
    normals = new vec3[npoints];
    gouraud = new vec3[npoints];
    colors = new color4[npoints];
    textures = new vec2[npoints];

    xRot = 0;
    yRot = 0;
    zRot = 0;

    readObj(n);

    make();
}

/**
 * Constructor per defecte
 * @brief Objecte::Objecte
 * @param parent
 */
Objecte::Objecte(QObject *parent):
    numPoints(0)
{
    material = NULL;

    points = NULL;
    normals = NULL;
    gouraud = NULL;
    colors = NULL;
    textures = NULL;
}

/**
 * LLibera tota la memoria dels vectors que hem allotjat per formar l'objecte i les propietats
 * @brief Objecte::~Objecte
 */
Objecte::~Objecte()
{
    //Si tenim memoria allotjada la lliberem
    if(points){
        delete [] points;
        delete [] normals;
        delete [] gouraud;
        delete [] colors;
        delete [] textures;
    }

    if (material)
    {
        delete material;
    }

    //LLiberem la memoria de tots els fills del objecte
    for (std::vector<Objecte*>::iterator it = fills.begin(); it != fills.end(); ++it)
    {
        delete *it;
    }
}

void Objecte::setMaterial(QGLShaderProgram* program, float ka, float kd, float ks, float shinesess)
{
    material = new Material(program, ka, kd, ks, shinesess);
}

/**
 * Iterem tots els punts de cada una de les dimensions amb l'objectiu d'obtenir la seva capça minima contenidora
 * @brief Objecte::calculCapsa3D
 * @return
 */
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

    return capsa;
}

/**
 * Ens permet realitzar canvis en la posicio, tamany i/o rotacio del objecte segons les matrius que reb com entrada.
 * @brief Objecte::aplicaTG
 * @param m
 */
void Objecte::aplicaTG(mat4 m)
{
    aplicaTGPoints(m);

    // Actualitzacio del vertex array per a preparar per pintar
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * numPoints, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * numPoints + sizeof(color4) * numPoints + sizeof(vec2) * numPoints, sizeof(vec3) * numPoints, gouraud );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Objecte::aplicaTGPoints(mat4 m)
{
    for ( int i = 0; i < numPoints; ++i ) {
        points[i] = m * points[i];

        vec4 t = m * vec4(gouraud[i], 0);
        gouraud[i] = normalize(vec3(t.x, t.y, t.z));
    }
}

/**
 * Mou l'objecte al centre de l'escena aplica la matriu de transofrmacio i torna a la posicio del centre de l'objecte.
 * @brief Objecte::aplicaTGCentrat
 * @param m
 * @param capsa
 */
void Objecte::aplicaTGCentrat(mat4 m, Capsa3D* capsa)
{
    if (!capsa)
    {
        capsa = &this->capsa;
    }

    aplicaTG(capsa->fromCenter * m * capsa->toCenter);

    for (std::vector<Objecte*>::iterator it = fills.begin(); it != fills.end(); ++it)
    {
        (*it)->aplicaTGCentrat(m, capsa);
        (*it)->calculCapsa3D();
    }
}

/**
 * Li passem la textura com a parametre a part del shader.
 * @brief Objecte::toGPU
 * @param program
 * @param texture
 */
void Objecte::toGPU(QGLShaderProgram* program, QOpenGLTexture* texture){

    this->program = program;
    _texture = texture;
    program->bind();

    if (material)
    {
        material->toGPU(program);
    }

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    //reservem espai per els punts, colors i textures.
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4) * numPoints + sizeof(color4) * numPoints + sizeof(vec2) * numPoints + sizeof(vec3) * numPoints, NULL, GL_STATIC_DRAW );

    //Les enviem a la gpu.
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * numPoints, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * numPoints, sizeof(color4) * numPoints, &colors[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * numPoints + sizeof(color4) * numPoints, sizeof(vec2) * numPoints, textures );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * numPoints + sizeof(color4) * numPoints + sizeof(vec2) * numPoints, sizeof(vec3) * numPoints, normals );

    // Per a conservar el buffer
    int vertexLocation = program->attributeLocation("vPosition");
    int colorLocation = program->attributeLocation("vColor");
    int coordTextureLocation = program->attributeLocation("vCoordTexture");
    int normalsLocation = program->attributeLocation("vNormals");

    program->enableAttributeArray(vertexLocation);
    program->enableAttributeArray(colorLocation);
    //Informem al shader de que existeix una array que conte les coordenades de la textura
    program->enableAttributeArray(coordTextureLocation);
    program->enableAttributeArray(normalsLocation);

    glEnable( GL_DEPTH_TEST );
    glEnable(GL_TEXTURE_2D);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Pintat en la GPU.
void Objecte::draw()
{
    program->bind();

    //Indiquem la posicio del objecte a la GPU
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);
    program->setAttributeBuffer("vColor", GL_FLOAT, sizeof(point4) * numPoints, 4);
    program->setAttributeBuffer("vCoordTexture", GL_FLOAT, sizeof(vec4) * numPoints + sizeof(vec4) * numPoints, 2);
    program->setAttributeBuffer("vNormals", GL_FLOAT, sizeof(vec4) * numPoints + sizeof(vec4) * numPoints + sizeof(vec2) * numPoints, 3);

    //SI hi ha textura li indiquem que utilitzi la textura, sino la desactivem.
    if (_texture)
    {
        _texture->bind(0);
        program->setUniformValue("texMap", 0);
    }else{
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glPolygonMode(GL_FRONT_AND_BACK, Common::getWireframeView());
    glDrawArrays( GL_TRIANGLES, 0, numPoints );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Dibuxem els fills del objecte.
    for (std::vector<Objecte*>::iterator it = fills.begin(); it != fills.end(); ++it)
    {
        (*it)->draw();
    }
}

/**
 * Converteix cada vertex de cada cara en un punt de GPU.
 * @brief Objecte::make
 */
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

            if (index > 0 && (index + 1) % 3 == 0)
            {
                normals[index] = normalAt(points[index - 2], points[index - 1], points[index]);
                normals[index - 1] = normals[index];
                normals[index - 2] = normals[index];
            }

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

    // TODO: Fer servir calcul i no tal qual
    memcpy(gouraud, normals, index * sizeof(vec3));
}

/**
 * Retorna el fills del objecte.
 * @brief Objecte::getFill
 * @param tipus
 * @return
 */
Objecte* Objecte::getFill(TIPUS_OBJECTE tipus)
{
    for (std::vector<Objecte*>::iterator it = fills.begin(); it != fills.end(); ++it)
    {
        if ((*it)->getTipus() == tipus)
        {
            return *it;
        }
    }
}

bool Objecte::intersects(Objecte* obj, vec4 delta)
{
    return capsa.pmin.x + delta.x < obj->capsa.pmax.x &&
            capsa.pmax.x + delta.x > obj->capsa.pmin.x &&
            capsa.pmin.z + delta.z < obj->capsa.pmax.z &&
            capsa.pmax.z + delta.z > obj->capsa.pmin.z;
}

// Llegeix un fitxer .obj
//  Si el fitxer referencia fitxers de materials (.mtl), encara no es llegeixen
//  Tots els elements del fitxer es llegeixen com a un unic objecte.
//

void Objecte::readObj(QString filename)
{
    QFile fp(filename);
    fp.open(QIODevice::ReadOnly);

    if (!fp.isOpen())
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        while (true)
        {
            char *comment_ptr = ReadFile::fetch_line (&fp);

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

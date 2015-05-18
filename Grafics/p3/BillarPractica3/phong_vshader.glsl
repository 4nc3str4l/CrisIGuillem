#version 130

#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif


uniform mat4 model_view;
uniform mat4 projection;

IN vec4 vPosition;
IN vec3 vNormals;
IN vec2 vCoordTexture;
IN int idMaterial;

OUT vec4 position;
OUT vec3 normals;
OUT vec2 v_texcoord;
flat OUT int matID;

void main()
{
    gl_Position = projection * model_view * vPosition;
    //gl_Position /= gl_Position.w;
    //color = vColor * vec4(llumAmbient, 1);


    // Pas de les coordenades de textura al fragment shader
    // El valor del color i les coordenades de textura s'interpolaran automaticament
    // en els fragments interiors a les cares dels polígons
    position = vPosition;
    v_texcoord = vCoordTexture;
    matID = idMaterial;
    normals = vNormals;
}

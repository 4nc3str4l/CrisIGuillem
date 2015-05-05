#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

struct tipusLlum
{
    vec4 LightPosition;
    vec3 Ld;

    float coef_a;
    float coef_b;
    float coef_c;
};

uniform mat4 model_view;
uniform mat4 projection;

uniform vec3 llumAmbient;
uniform tipusLlum light;

IN vec4 vPosition;
IN vec4 vColor;
IN vec3 vNormals;
IN vec2 vCoordTexture;

OUT vec4 intensitat;
OUT vec2 v_texcoord;


void main()
{
    gl_Position = projection * model_view * vPosition;
    //color = vColor * vec4(llumAmbient, 1);
    intensitat = vec4(llumAmbient, 1);

    // Pas de les coordenades de textura al fragment shader
    // El valor del color i les coordenades de textura s'interpolaran automaticament
    // en els fragments interiors a les cares dels polígons
    v_texcoord = vCoordTexture;
}

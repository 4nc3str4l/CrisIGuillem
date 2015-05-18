#version 130

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
    vec4 LighDirection;
    float angle;

    vec3 Ld;
    vec3 Ls;
    vec3 La;

    float coef_a;
    float coef_b;
    float coef_c;
};

struct tipusMaterial
{
    vec3 ka;
    vec3 kd;
    vec3 ks;

    float shineness;

    vec4 color;
};

uniform mat4 model_view;
uniform mat4 projection;

uniform vec3 llumAmbient;

uniform tipusLlum puntual[2];
uniform tipusLlum direccional[2];
uniform tipusLlum spot[2];

uniform tipusMaterial material[19];

uniform vec4 camera;

IN vec4 vPosition;
IN vec4 vColor;
IN vec3 vNormals;
IN vec2 vCoordTexture;
IN int idMaterial;

OUT vec4 color;
OUT vec4 intensitat;
OUT vec2 v_texcoord;

void main()
{
    gl_Position = projection * model_view * vPosition;
    //gl_Position /= gl_Position.w;
    //color = vColor * vec4(llumAmbient, 1);

    vec3 sumatori = vec3(0, 0, 0);
    for (int i = 0; i < 1; ++i)
    {
        float d = distance(vPosition, puntual[i].LightPosition);
        float atenuacio = puntual[i].coef_a + puntual[i].coef_b * d + puntual[i].coef_c * pow(d,2);

        vec4 L = puntual[i].LightPosition - vPosition;
        vec4 V = camera - vPosition;
        vec4 H = normalize(normalize(L) + normalize(V));

        sumatori += (1.0 / atenuacio) * (
            material[idMaterial].kd * puntual[i].Ld * max(dot(vNormals, L.xyz), 0) + // Difusa
            material[idMaterial].ks * puntual[i].Ls * pow(max(dot(vNormals, H.xyz), 0), material[idMaterial].shineness * 128) + // Especular
            material[idMaterial].ka * puntual[i].La // Ambient
        );
    }

    intensitat = vec4(
        material[idMaterial].ka * llumAmbient +  // Ambient Global
        sumatori // Sumatori intensitats llums
    , 1.0);

    // Pas de les coordenades de textura al fragment shader
    // El valor del color i les coordenades de textura s'interpolaran automaticament
    // en els fragments interiors a les cares dels polígons
    v_texcoord = vCoordTexture;
    color = material[idMaterial].color;
}

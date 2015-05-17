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
    vec3 Ls;
    vec3 La;

    float coef_a;
    float coef_b;
    float coef_c;
};

struct tipusMaterial
{
    float ka;
    float kd;
    float ks;

    float shineness;
};


uniform mat4 model_view;
uniform mat4 projection;

uniform vec3 llumAmbient;
uniform tipusLlum light;

uniform tipusMaterial material;

uniform vec4 camera;

IN vec4 vPosition;
IN vec4 vColor;
IN vec3 vNormals;
IN vec2 vCoordTexture;

OUT vec4 intensitat;
OUT vec2 v_texcoord;

void main()
{
    gl_Position = projection * model_view * vPosition;
    //gl_Position /= gl_Position.w;
    //color = vColor * vec4(llumAmbient, 1);

    float d = distance(vPosition, light.LightPosition);
    float atenuacio = light.coef_a + light.coef_b * d + light.coef_c * pow(d,2);

    vec4 L = light.LightPosition - vPosition;
    vec4 V = camera - vPosition;
    vec4 H = normalize(L + V);

    intensitat = vec4(
        material.ka * llumAmbient +  // Ambient Global
        (1.0 / atenuacio) * (
            material.kd * light.Ld * max(dot(vNormals, L.xyz), 0) + // Difusa
            material.ks * light.Ls * max(pow(dot(vNormals, H), material.shineness), 0) + // Especular
            material.ka * light.La // Ambient
        )
    , 1.0);


    // Pas de les coordenades de textura al fragment shader
    // El valor del color i les coordenades de textura s'interpolaran automaticament
    // en els fragments interiors a les cares dels polígons
    v_texcoord = vCoordTexture;
}

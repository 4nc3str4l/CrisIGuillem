#version 130

#if __VERSION__<130
#define IN varying
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

    vec4 color;
};

IN vec4 position;
IN vec3 normals;
IN vec2 v_texcoord;
flat IN int matID;

uniform vec3 llumAmbient;
uniform tipusLlum light;

uniform tipusMaterial material[19];

uniform vec4 camera;

uniform sampler2D texMap;
uniform bool only_color;


void main()
{
    float d = distance(position, light.LightPosition);
    float atenuacio = light.coef_a + light.coef_b * d + light.coef_c * pow(d,2);

    vec4 L = light.LightPosition - position;
    vec4 V = camera - position;
    vec4 H = normalize(L + V);

    vec4 intensitat = vec4(
            material[matID].ka * llumAmbient +  // Ambient Global
            (1.0 / atenuacio) * (
                material[matID].kd * light.Ld * max(dot(normals, L.xyz), 0) + // Difusa
                material[matID].ks * light.Ls * max(pow(dot(normals, H.xyz), material[matID].shineness), 0) + // Especular
                material[matID].ka * light.La // Ambient
            )
        , 1.0);

    if (only_color)
        gl_FragColor = intensitat * material[matID].color;
    else
        gl_FragColor = intensitat * (0.25f * material[matID].color + 0.75f * texture2D(texMap, v_texcoord));
}

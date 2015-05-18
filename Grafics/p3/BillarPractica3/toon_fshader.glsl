#version 130

#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif


#define PUNTUAL     0
#define DIRECCIONAL 1
#define SPOTLIGH    2

struct tipusLlum
{
    int tipus;

    vec4 LightPosition;
    vec4 LightDirection;
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

IN vec4 position;
IN vec3 normals;
IN vec2 v_texcoord;
flat IN int matID;

uniform vec3 llumAmbient;

uniform tipusLlum light[2];

uniform tipusMaterial material[19];

uniform vec4 camera;

uniform sampler2D texMap;
uniform bool only_color;

void main()
{
    vec3 normal = normalize(normals);

    float intensity = 0;
    for (int i = 0; i < 2; ++i)
    {
        float d = distance(position, light[i].LightPosition);
        float atenuacio = light[i].coef_a + light[i].coef_b * d + light[i].coef_c * pow(d,2);

        vec4 L = light[i].LightPosition - position;

        bool calculaLlum = (light[i].tipus == PUNTUAL);

        if (light[i].tipus == DIRECCIONAL)
        {
            calculaLlum = dot((light[i].LightPosition - light[i].LightDirection).xyz, L.xyz) > 0;
        }

        if (calculaLlum)
        {
            intensity += (1.0 / pow(atenuacio, 2)) * dot(L.xyz, normal);
        }
    }

    vec4 intensitat;

    if (intensity > 0.95)
        intensitat = vec4(1.0,0.5,0.5,1.0);
    else if (intensity > 0.5)
        intensitat = vec4(0.6,0.3,0.3,1.0);
    else if (intensity > 0.25)
        intensitat = vec4(0.4,0.2,0.2,1.0);
    else
        intensitat = vec4(0.2,0.1,0.1,1.0);

    if (only_color)
        gl_FragColor = intensitat * material[matID].color;
    else
        gl_FragColor = intensitat * (0.25f * material[matID].color + 0.75f * texture2D(texMap, v_texcoord));
}

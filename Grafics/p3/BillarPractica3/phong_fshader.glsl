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

IN vec4 position;
IN vec3 normals;
IN vec2 v_texcoord;
flat IN int matID;

uniform vec3 llumAmbient;

uniform tipusLlum puntual[2];
uniform tipusLlum direccional[2];
uniform tipusLlum spot[2];

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
    vec4 H = normalize(normalize(L) + normalize(V));

    vec3 normal = normalize(normals);

    vec4 intensitat = vec4(
            material[matID].ka * llumAmbient +  // Ambient Global
            (1.0 / atenuacio) * (
                material[matID].kd * light.Ld * max(dot(normal, L.xyz), 0) + // Difusa
                material[matID].ks * light.Ls * pow(max(dot(normal, H.xyz), 0), material[matID].shineness * 128) + // Especular
                material[matID].ka * light.La // Ambient
            )
        , 1.0);

    if (only_color)
        gl_FragColor = intensitat * material[matID].color;
    else
        gl_FragColor = intensitat * (0.25f * material[matID].color + 0.75f * texture2D(texMap, v_texcoord));
}

#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 color;
IN vec4 intensitat;
IN vec2 v_texcoord;

uniform sampler2D texMap;
uniform bool only_color;

void main()
{
    if (only_color)
        gl_FragColor = intensitat * color;
    else
        gl_FragColor = intensitat * (0.25f * color + 0.75f * texture2D(texMap, v_texcoord));
}

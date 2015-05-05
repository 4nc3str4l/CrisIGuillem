#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 intensitat;
IN vec2 v_texcoord;

uniform sampler2D texMap;

void main()
{
    gl_FragColor = intensitat * texture2D(texMap, v_texcoord);
}

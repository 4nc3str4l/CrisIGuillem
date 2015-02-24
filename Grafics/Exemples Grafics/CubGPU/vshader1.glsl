#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif


IN vec4 vPosition;
IN vec4 vColor;

OUT vec4 color;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
  vec2 v = vec2(color.y,vPosition.x);
  gl_Position = vPosition;
  color = vColor+rand(v)*0.5;
}

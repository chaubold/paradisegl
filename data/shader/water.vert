/*
GLSL conversion of Michael Horsch water demo
http://www.bonzaisoftware.com/wfs.html

Converted by Mars_999
8/20/2005
*/
varying vec4 waterTex0;
varying vec4 waterTex1;
varying vec4 waterTex2;
varying vec4 waterTex3;
varying vec4 waterTex4;
uniform vec4 viewpos, lightpos;
uniform float time, time2;

uniform mat4 mat_projection;
uniform mat4 mat_modelview;


void main(void)
{
vec4 mpos, temp;
vec4 tangent = vec4(1.0, 0.0, 0.0, 0.0);
vec4 norm = vec4(0.0, 1.0, 0.0, 0.0);
vec4 binormal = vec4(0.0, 0.0, 1.0, 0.0);

mpos = mat_modelview * gl_Vertex;

temp = viewpos - mpos;
waterTex4.x = dot(temp, tangent);
waterTex4.y = dot(temp, binormal);
waterTex4.z = dot(temp, norm);
waterTex4.w = 0.0;

temp = lightpos - mpos;
waterTex0.x = dot(temp, tangent);
waterTex0.y = dot(temp, binormal);
waterTex0.z = dot(temp, norm);
waterTex0.w = 0.0;

mpos = mat_projection * mpos;

vec4 t1 = vec4(0.0, -time, 0.0,0.0);
vec4 t2 = vec4(0.0, -time2, 0.0,0.0);

waterTex1 = gl_MultiTexCoord0 + t1;
waterTex2 = gl_MultiTexCoord0 + t2;

waterTex3 = mpos;

gl_Position = mpos;
}

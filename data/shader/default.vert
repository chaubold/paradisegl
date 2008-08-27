uniform mat4 mat_projection;
uniform mat4 mat_modelview;

varying vec4 color;

void main()
{
	//mat_projection = gl_ProjectionMatrix;
	//mat_modelview = gl_ModelViewMatrix;

	vec4 pos = mat_modelview * gl_Vertex;
	pos = mat_projection * pos;
	gl_Position    = pos;
	color  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

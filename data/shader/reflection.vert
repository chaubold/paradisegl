uniform mat4 mat_projection;
uniform mat4 mat_modelview;

varying float height;

void main()
{
	//mat_projection = gl_ProjectionMatrix;
	//mat_modelview = gl_ModelViewMatrix;

	vec4 pos = mat_modelview * gl_Vertex;
	height = gl_Vertex.y;
	pos = mat_projection * pos;
	gl_Position    = pos;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

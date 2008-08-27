// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform float time;
uniform sampler2D tex0;

uniform float water_height;
varying float height;

void main()
{
	if(height < water_height)
		discard;
	gl_FragColor = texture2D(tex0, gl_TexCoord[0].xy);
}

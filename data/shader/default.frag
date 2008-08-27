// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform float time;
uniform sampler2D tex0;

varying vec4 color;

void main()
{
	gl_FragColor = texture2D(tex0, gl_TexCoord[0].xy);
}

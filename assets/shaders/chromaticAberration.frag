#version 130

uniform sampler2D tex;

void main()
{
	//load texel (texture pixel value)
	vec2 texel = 1.0 / textureSize(tex, 0) * 0.25;
	//extract coordinates from gl_TexCoord
	vec2 coord = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);
	vec4 pixel1 = texture(tex, coord + vec2(texel.x, 0.0)) * vec4(1.0, 0.25, 0.0, 1.0);
	vec4 pixel2 = texture(tex, coord - vec2(texel.x, 0.0)) * vec4(0.0, 0.75, 1.0, 1.0);


	// send out pixel color
	gl_FragColor = pixel1 + pixel2;
}
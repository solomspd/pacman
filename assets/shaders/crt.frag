#version 130

const float noiseStrength = 0.07;
const float bend = 5.0;

uniform sampler2D tex;
uniform sampler2D matteTex;
uniform sampler2D blurTex;

uniform float time;

float noise(float n)
{
    return fract(cos(n * 89.42) * 343.42);
}

vec3 alphaBlend(vec4 a, vec4 b)
{   
    return a.rgb * a.aaa + b.rgb * b.aaa * (vec3(1.0, 1.0, 1.0) - a.aaa);;
}

void main()
{
	//extract coordinates from gl_TexCoord
	vec2 coord = vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
	//convert to symmetric coordinates
	coord = (coord - 0.5) * 2.0;

	//deform coordinates (bend x and y by 5)
	coord.x *= 1.0 + pow((abs(coord.y) / bend), 2.0);
	coord.y *= 1.0 + pow((abs(coord.x) / bend), 2.0);

	//transform back to 0.0 - 1.0 coordinates
	coord  = (coord / 2.0) + 0.5;
	
	//load original pixel (invert y because sfml is stupid)
	vec3 pixel = texture(tex, coord).rgb;
	//load matte pixel
	vec3 matte = texture(matteTex, coord).rgb;
	//load blurred pixel
	vec3 blurred = texture(blurTex, coord).rgb;

	//apply matte to original pixel
	pixel *= vec3(1.0, 1.0, 1.0) - matte * 1.5;
	pixel *= vec3(0.8, 0.8, 0.8);

	//add blurred
	pixel += blurred * 0.8;

	//add blurred matted
	pixel += blurred * 3.0 * matte;

	//add noise
	float n = noise(gl_TexCoord[0].x * gl_TexCoord[0].y + time) * 0.7;
	n += pixel.r;
	pixel = alphaBlend(vec4(n, n, n, noiseStrength), vec4(pixel, 1.0));

	// send out pixel color
	gl_FragColor = vec4(pixel, 1.0);
}
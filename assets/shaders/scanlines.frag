#version 130

uniform sampler2D tex;
uniform float time;

float speed = 50.0;
int size = 2;
int separation = 8;

void main(){
	vec2 coord = vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
	vec2 texel = 1.0 / textureSize(tex, 0);

	// lookup the pixel in the texture
    vec3 pixel = texture(tex, coord).rgb;

	//scanline -> modulus of y coordinate by separation is < size
	if(mod(coord.y / texel.y - time * speed, separation) < size && pixel != vec3(0.0, 0.0, 0.0)){
		//screen blend mode
		pixel = (1.0 - (1.0 - pixel) * (1.0 - vec3(0.01, 0.03, 0.05)));
	}

    // send out pixel color
    gl_FragColor = vec4(pixel, 1.0);
}
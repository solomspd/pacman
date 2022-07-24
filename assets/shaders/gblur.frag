#version 130

uniform sampler2D tex;
uniform bool horizontal = true;
//gaussian blur kernel
uniform float kernel[6] = float[] (0.382925, 0.24173, 0.060598, 0.005977, 0.000229, 0.000003);

void main()
{
   vec2 coord = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);
   vec2 texel = 1.0 / textureSize(tex, 0);

	// lookup the pixel in the texture
    vec3 pixel = texture(tex, coord).rgb * kernel[0];

	if(horizontal){
		for(int i = 1; i < 6; i++){
			pixel += texture(tex, coord + vec2(i * texel.x, 0.0)).rgb * kernel[i];
			pixel += texture(tex, coord - vec2(i * texel.x, 0.0)).rgb * kernel[i];
		}
	} else {
		for(int i = 1; i < 6; i++){
			pixel += texture(tex, coord + vec2(0.0, i * texel.y)).rgb * kernel[i];
			pixel += texture(tex, coord - vec2(0.0, i * texel.y)).rgb * kernel[i];
		}
	}

    // send out pixel color
    gl_FragColor = vec4(pixel, 1.0);
}
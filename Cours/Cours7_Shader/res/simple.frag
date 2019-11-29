#version 120
uniform sampler2D texture;
uniform float time;
uniform float moving;

void main() {
    vec2 coord = gl_TexCoord[0].xy;
    vec4 pixel_color = texture2D(texture, coord);
	pixel_color.r = mod(time,1.0);
	pixel_color.g = mod(-time, 1.0);
	pixel_color.b = mod(time * 250, 1.0);
	gl_FragColor = pixel_color * gl_Color;
}
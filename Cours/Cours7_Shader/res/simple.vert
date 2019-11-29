#version 120

uniform float time;

void main()
{
	vec4 vert = gl_Vertex;

	//vert.x += time * 200.0;
	//vert.y += time * 100.0;
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * vert;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}
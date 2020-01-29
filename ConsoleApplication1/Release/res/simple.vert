#version 120

uniform float time;
uniform float positionx;
uniform float positiony;
vec4 position2;


void main()
{
	position2.x = positionx;
	position2.y = positiony;
	
	mat4 Model = mat4(1.0f);
	Model[0][3] = 0.1f;
	Model[1][3] = -0.2f;
	Model[2][3] = 0.5f;
	float szClamped = (((Model * gl_Vertex).x - positionx) / 80.0);
	float szc01 = ((szClamped+1)*0.5);
	float sz = szc01 * 0.25 + 0.75;

	// transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	/*if(gl_Vertex.x > 0.5) gl_Position.x -= 0.02*cos(time);
	if(gl_Vertex.x < 0.5) gl_Position.x += 0.02*cos(time);
	if(gl_Vertex.y > 0.5) gl_Position.y -= 0.02*cos(time+ 3.14);
	if(gl_Vertex.y < 0.5) gl_Position.y += 0.02*cos(time+ 3.14);*/
	//gl_Position.x += 0.1*cos(time);
	//gl_Position.y += 0.1*sin(time);
	

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0 ;

    // forward the vertex color

	//float scale = (cos(gl_TexCoord[0].x*3.14 - time) + 1)/2;
	position2 = gl_ModelViewProjectionMatrix * position2;
	float scale = (gl_Position.x - position2.x) * 10.0 + 0.5;
    gl_FrontColor = gl_Color * sz;
		
	
}
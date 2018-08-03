#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;

out vec3 exNormal;
out vec3 exSurface;

uniform bool useBasis;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform mat4 basisMat;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	
	texCoord = inTexCoord;

	// method used if there are non uniform scaling
	exNormal = mat3(transpose(inverse(mdlMatrix))) * inNormal;

    exSurface = vec3(mdlMatrix * vec4(inPosition,1.0));


	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
	
}

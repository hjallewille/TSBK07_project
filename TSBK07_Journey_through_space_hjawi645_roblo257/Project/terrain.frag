#version 150


in vec3 exNormal; 
in vec3 exSurface; 

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D texUnit;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform int isDirectional[4];

// where the camera is
uniform vec3 viewPos;

vec3 lightDirection;
vec3 normal;

vec3 specular;
vec3 diffuse;
vec3 result0;
vec3 result;

uniform bool lightOn;
uniform bool textureOn;


// CALCULATIONS ARE THE SAME FOR DIRECTIONAL AND NON-DIRECTIONAL LIGHT EXCEPT FOR lightDirection-VRIABLE 

void main(void) {


// set specular strength here instead of each time we use specular component
float specularStrength = 0.8f;

// set diffuse strength
float diffuseStrength = 0.7f;

normal = normalize(exNormal);


for (int i = 0; i < 4; i++) {

if (isDirectional[i] > 0) {


	// diffuse part of shading
	float diff = max(dot(normal, lightSourcesDirPosArr[i]), 0.0);
	diffuse = diff * lightSourcesColorArr[i];

	// specular part of shading, DON'T NEGATE LIGHTSOURCE DUE TO DIRECTION OF LIGHT
	vec3 viewDir = normalize(viewPos - exSurface);
	vec3 reflectDir = -normalize(reflect(lightSourcesDirPosArr[i], normal)); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent[i]);
	specular = spec * lightSourcesColorArr[i];

	// both diffuse and specular part added
	result0 = (diffuseStrength * diffuse + specularStrength * specular); 
	result = result0 + result;


} else {


	lightDirection = normalize(lightSourcesDirPosArr[i]-exSurface);

	// diffuse part of shading
	float diff = max(dot(normal, lightDirection), 0.0);
	diffuse = diff * lightSourcesColorArr[i];

	// specular part of shading, NEGATE LIGHTDIRECTION DUE TO DIRECTION
	vec3 viewDir = normalize(viewPos - exSurface);
	vec3 reflectDir = -normalize(reflect(-lightDirection, normal)); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent[i]);
	specular = spec * lightSourcesColorArr[i]; 

	// both diffuse and specular part added
	result0 = (diffuseStrength * diffuse + specularStrength * specular); 
	result = result0 + result;

	} // IF-STATEMENT ENDS HERE__________________


} // FOR-LOOP ENDS HERE__________________________
	
if (lightOn && textureOn){

	outColor = texture(texUnit, texCoord) * vec4(result, 1.0f); 

} else if (!lightOn && textureOn){

	outColor = texture(texUnit, texCoord); 

	} else if (lightOn && !textureOn){

	outColor = vec4(result, 1.0f); 

	}
}


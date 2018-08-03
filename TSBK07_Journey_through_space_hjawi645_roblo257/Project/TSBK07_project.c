#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "CallMeAL.h"
#include "CatmullRom_Splines/CatmullRom_Splines.h"
#include "Terrain_Gen/Generate_terrain.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
//______________________
#include <math.h>
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

mat4 projectionMatrix;
mat4 worldToViewMatrix;
vec3 camPos = {0.0f, 0.0f, -5.0f};
vec3 fwdVec = {0.0f, 0.0f, 1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};

vec3 lookPoint, catchDer, catchDer2, newCatchDer2, rightDirection, v0,v1,v2,v3, catch,catch2, catch_cam, catch_planet, newPosition, v_cam0, v_cam1, v_cam2, v_cam3, v_plan0, v_plan1, v_plan2, v_plan3;

mat4 newBasis;
// sun light
Point3D lightSourcesColorsArr[] = { {1.0f, 0.831f, 0.039f}, // Yellow light
                                 {1.0f, 1.0f, 1.0f}, // White light
                                 {1.0f, 0.831f, 0.039f}, // Yellow light
                                 {1.0f, 1.0f, 1.0f} }; // White light
GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};

GLint isDirectional[] = {0,0,1,1};

Point3D lightSourcesDirectionsPositions[] = { {0.0f, 40.0f, -20.0f}, 
                                       		  {0.0f, 40.0f, -20.0f}, 
                                      		  {0.0f, 1.0f, 0.5f}, 
                                       		  {0.0f, 1.0f, 0.5f} }; 


int i = 0, j =0, k = 0, counter = 0, int_1 = 0, int_2 = 0;
GLint lightOn, textureOn, useBasis; // We use these to pass boolean values to the fragment shader

bool first_run = true, draw_earth = true;
 
Model *m, *m2, *tm, *planet, *earth, *skyBox, *alien, *spaceship;
GLuint program, tex1, tex2, textures[5]; // texture array 0 or 1

TextureData ttex; // terrain


GLfloat curr_x, curr_y, curr_z, left_x,left_y, left_z, right_x, right_y, right_z, lower_x, lower_y, lower_z, upper_x, upper_y, upper_z, cosTheta, cosAngle, rot_around_y; 


GLfloat u,time_t2 ,seconds, thousands, u_used, u_curr_1, u_curr_2; var_to_counter_frames_1 = 0.0f, var_to_counter_frames_2 = 0.0f, sec2, yaw   = -180.0f, pitch =  0.0f, rotation_around_y=0.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right, so we initially rotate a bit to the left.

void do_movement();


vec3 onPlanetPoints[] = {		{295.0f,  12.0f, 109.0f}, 
                            	{ 305.0f,  12.9f, 107.0f},
                            	{ 305.0f, 10.8f, 120.3f},
                            	{ 295.0f,  10.7f, 110.4f},
                            	{295.0f,  12.0f, 109.0f}, 
                            	{ 305.0f,  12.9f, 107.0f},
                            	{ 305.0f, 10.8f, 120.3f},
                            	{ 295.0f,  10.7f, 110.4f}
                            	
};

vec3 cameraPathPoints[] = {		{-10.0f,0.0f,0.0f},
								{-30.0f,10.0f,10.0f},
								{-50.0f,10.0f,20.0f},	
								{-20.0f,10.0f,30.0f},
                				{0.0f,10.0f,40.0f},
								{15.0f,10.0f,50.0f},
								{25.0f,10.0f,60.0f},
								{15.0f,00.0f,60.0f},
								{0.0f,10.0f,40.0f},
								{15.0f,10.0f,30.0f},
								{25.0f,10.0f,30.0f},
								{10.0f,00.0f,30.0f}
                            	
};

vec3 shipPoints[] = {		
				{ 0.0f, 0.0f,-15.9f},
				{ 0.0f, 0.0f,-14.9f},
				{ 0.0f, 0.0f,-14.9f},
				{ 0.0f, 0.0f,-14.9f},
				{ -7.0f, 0.0f, 2.5f},
				{-20.0f, 1.5f,20.0f},	
				{-10.0f, 1.5f, 50.0f},	
				{ 0.0f, -1.5f, 60.0f},
				{ 10.0f, 0.0f, 50.0f},
				{ 10.0f, 0.0f, 20.0f},
				{ 25.0f, 3.0f, 5.0f},
				{ 25.0f ,-2.5f,-40.0f},
				{ -10.0f,0.0f,-50.0f},
				{ -15.0f ,-2.5f,-25.0f}
};

mat4 transformToNewBase(vec3 firstDer, vec3 secondDer) {

	vec3 t, n, up;
	mat4 newBasis;

	t = Normalize(firstDer);
	n = Normalize(secondDer);

	up = Normalize(CrossProduct(t,n));

	n = Normalize(CrossProduct(up,t));

	newBasis = SetMat4( n.x, up.x, t.x, 0.0f,
						n.y, up.y, t.y, 0.0f,
						n.z, up.z, t.z, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

return newBasis;

}

void init(void)
{
	// Load terrain data
	
	LoadTGATextureData("Textures/fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	// GL inits
	glClearColor(0.0,0.0,0.0,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0);
	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	//glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	//LoadTGATextureSimple("conc.tga", &tex1);
	planet = LoadModelPlus("Objects/sphereTest.obj");
	earth = LoadModelPlus("Objects/sphereTest.obj");
	skyBox = LoadModelPlus("Objects/spaceBoxxx.obj");
	alien = LoadModelPlus("Objects/alienWtex.obj");
	spaceship = LoadModelPlus("Objects/Spaceship.obj");	

	LoadTGATextureSimple("Textures/StarBox-DensityOne.tga", &textures[0]);
	LoadTGATextureSimple("Textures/GreenSmilev2.tga", &textures[1]);
	LoadTGATextureSimple("Textures/earth-texture.tga", &textures[2]);
	LoadTGATextureSimple("Textures/mars-texture.tga", &textures[3]);
	LoadTGATextureSimple("Textures/sand.tga", &textures[4]);
	glUniform1i(glGetUniformLocation(program, "texUnit" ), 0); // Texture unit, need only one when not multitexturing. Bind textures to this unit.

	

}
void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total,total1, trans, trans1,rot,rot1, rot2, scale, tempRot;
	
	printError("pre display");
	
	glUseProgram(program);
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	GLfloat seconds = t/1000;
	
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, 		&lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
	do_movement();

	if (pitch > 89.0f) {
    	pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	
	fwdVec.x = sin(yaw * M_PI/180)* cos(pitch * M_PI/180);
    fwdVec.y = sin(pitch * M_PI/180);
	fwdVec.z = -cos(yaw * M_PI/180)* cos(pitch * M_PI/180);
	fwdVec = Normalize(fwdVec);

// Add forward vector to camera position to get look point
	time_t2 = seconds;
if(time_t2 > 0 && time_t2 < 45.7) {

	camPos.x = catch_cam.x;
	camPos.y = catch_cam.y;
	camPos.z = catch_cam.z;
	worldToViewMatrix = lookAtv(camPos, catch, cameraUp);

	//lookPoint.x = 0.0f;
	//lookPoint.y = 0.0f;
	//lookPoint.z = 50.0f;

	GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z); 

		// Model matrix
	trans1 = T(camPos.x,camPos.y,camPos.z);
	rot1 = Ry(0);
	total1 = Mult(trans1, rot1);

} else {

	Point3D blue_light = {0.0f,0.0f,0.9f};
	Point3D brown_light = {0.6f,0.262f,0.0f};

	lightSourcesColorsArr[1] = blue_light;
	lightSourcesColorsArr[2] = brown_light;
	lightSourcesColorsArr[3] = blue_light;	

	lookPoint.x = 300.0f;
	lookPoint.y = 11.25f;
	lookPoint.z = 105.0f;

	camPos.x = catch_planet.x;
	camPos.y = catch_planet.y;
	camPos.z = catch_planet.z;
	
	worldToViewMatrix = lookAtv(camPos, lookPoint, cameraUp);

	GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z); 

	trans1 = T(camPos.x,camPos.y,camPos.z);
	rot1 = Ry(0);
	total1 = Mult(trans1, rot1);
	time_t2 = fmod(time_t2, 96);
}

	glDisable(GL_DEPTH_TEST); // Has to be done for skyBox


	glUniform1i(glGetUniformLocation(program, "textureOn"), 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(glGetUniformLocation(program, "lightOn"), 0);
	glUniform1i(glGetUniformLocation(program, "texUnit" ), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total1.m);
	DrawModel(skyBox,program,"inPosition", "inNormal", "inTexCoord");
	glEnable(GL_DEPTH_TEST); // Activate z-buffer again

	scale = S(5.0f,5.0f,5.0f);
	rot = Ry(-M_PI/2);
	rot2 = Rx(M_PI/6);
	trans = T(0.0f,0.0f,-30.0f);
	total = Mult(rot2, rot);
	total = Mult(trans, total);
	total = Mult(total,scale);	

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	if (draw_earth) {
	DrawModel(earth,program,"inPosition", "inNormal", "inTexCoord");
 	}

	seconds = 3*t/1000;
		
	thousands = seconds - floor(seconds);
	seconds = seconds - thousands;

	u_used = ((int) seconds) % 10;

	u_used = u_used/10 + thousands/10;



	u_curr_2 = u_used;

	if (first_run) 
	{
	v3 = shipPoints[4];
	v2 = shipPoints[3];
	v1 = shipPoints[2];
	v0 = shipPoints[1];

	v_cam3 = cameraPathPoints[3];
	v_cam2 = cameraPathPoints[2];
	v_cam1 = cameraPathPoints[1];
	v_cam0 = cameraPathPoints[0];

	v_plan3 = onPlanetPoints[3];
	v_plan2 = onPlanetPoints[2];
	v_plan1 = onPlanetPoints[1];
	v_plan0 = onPlanetPoints[0];

	first_run = false;
	u_curr_1 = u_curr_2;
	i = 4;
	j = 4;
	k = 4;
	}

	

	if (u_curr_1 > u_curr_2) {
	i = i + 1;
	i = i % 15;
	if (i == 0)
	{i=5;
	counter++;}
	j = j + 1;
	j = j % 8;
	if (j == 0)
	{j=5;}
	k = k + 1;
	k = k % 8;
	if (k == 0)
	{k=5;}
	v0 = v1;
	v1 = v2;
	v2 = v3;
	v3 = shipPoints[i];

	v_plan0 = v_plan1;
	v_plan1 = v_plan2;
	v_plan2 = v_plan3;
	v_plan3 = onPlanetPoints[j];

	v_cam0 = v_cam1;
	v_cam1 = v_cam2;
	v_cam2 = v_cam3;
	v_cam3 = cameraPathPoints[k];

	}

	u_curr_1 = u_used;
	

	catch_cam = CatmullRom(u_used,v_cam0,v_cam1,v_cam2,v_cam3);
	catch_planet = CatmullRom(u_used,v_plan0,v_plan1,v_plan2,v_plan3);

	catch = CatmullRom(u_used,v0,v1,v2,v3);

	catchDer = CatmullRomDerivative(u_used, v0, v1, v2, v3);
	catchDer2 = CatmullRomDerivative2(u_used, v0, v1, v2, v3);

if (i < 5 && counter == 0) 
		{
			catchDer = SetVector(0,0,1);
			catchDer2 = SetVector(1,0,0);
			catch = SetVector(0,0,-14.9);
		}

	catchDer = Normalize(catchDer);
	catchDer2 = Normalize(catchDer2);

	if (DotProduct(catchDer, catchDer2) == 1 || DotProduct(catchDer, catchDer2) == -1 || Norm(catchDer) == 0 || Norm(catchDer2) == 0) { // means they're parallell 

			printf("PARALLELL OR ZERO\n");


	}

	newBasis = transformToNewBase(catchDer, catchDer2);

	scale = S(0.3f,0.3f,0.3f);
	trans = T(catch.x,catch.y,catch.z);
	tempRot = Rx(-M_PI/2);
	total = Mult(trans, newBasis);
	total = Mult(total, tempRot);
	
	total = Mult(total,scale);

	glUniform1i(glGetUniformLocation(program, "textureOn"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glUniform1i(glGetUniformLocation(program, "lightOn"), 1);
	glUniform1i(glGetUniformLocation(program, "texUnit" ), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	DrawModel(spaceship,program,"inPosition", "inNormal", "inTexCoord"); 


	//The spaceship on the planet
	scale = S(0.3f,0.3f,0.3f);
	trans = T(303.0f,9.9f,100.0f);
	tempRot = Rx(M_PI);
	total = Mult(trans, tempRot);
	total = Mult(total,scale);


	glUniform1i(glGetUniformLocation(program, "textureOn"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glUniform1i(glGetUniformLocation(program, "lightOn"), 1);
	glUniform1i(glGetUniformLocation(program, "texUnit" ), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	DrawModel(spaceship,program,"inPosition", "inNormal", "inTexCoord"); 

	mat4 trans2, circle;
	trans2 = T(10.0f,10+4*sin(t/500),0.0f);
	rot2 = Ry(-t/1000);

	circle = Mult(rot2, trans2);
	scale = S(0.1f,0.1f,0.1f);
	rot = Ry(M_PI);
	trans = T(300.0f,11.25f,105.0f);
	total = Mult(trans, rot);
	total = Mult(total,scale);
	total = Mult(total, circle);
		glUniform1i(glGetUniformLocation(program, "textureOn"), 1);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	DrawModel(alien,program,"inPosition", "inNormal", "inTexCoord");
	
	scale = S(1.0f,1.0f,1.0f);
	trans = T(0.0f,0.0f,50.0f);
	total = Mult(trans,scale);
	
	glUniform1i(glGetUniformLocation(program, "lightOn"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(earth,program,"inPosition", "inNormal", "inTexCoord");

	glUniform1i(glGetUniformLocation(program, "lightOn"), 1);

	scale = S(1.0f,1.0f,1.0f);
	trans = T(200.0f,10.0f,15.0f);
	total = Mult(trans,scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, textures[4]);		
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	glUniform1i(glGetUniformLocation(program, "lightOn"), 0);

	printError("display 2");
	
	glutSwapBuffers();
}
void do_movement()
{
// How much to rotate/move
if (glutKeyIsDown('d')) { 
	yaw = yaw + 1.5f;
	}
if (glutKeyIsDown('a')) { 
	yaw = yaw - 1.5f;
    }
if (glutKeyIsDown('w')) { 
	
	pitch = pitch + 1.5f;
	}
if (glutKeyIsDown('s')) { 
	pitch = pitch - 1.5f;
	}
if (glutKeyIsDown('i')) { // move fwd
	camPos = VectorAdd(camPos,ScalarMult(fwdVec,0.5));
	}
if (glutKeyIsDown('k')) { 
	camPos = VectorSub(camPos,ScalarMult(fwdVec,0.5));
	}
	rightDirection = CrossProduct(fwdVec,cameraUp);
if (glutKeyIsDown('l')) { // move sideways
	camPos = VectorAdd(camPos,ScalarMult(rightDirection,0.3));
	}
if (glutKeyIsDown('j')) { 
	camPos = VectorSub(camPos,ScalarMult(rightDirection,0.3));
	}

if (glutKeyIsDown('q')) { 
	draw_earth = true;
	}

if (glutKeyIsDown('e')) {
	draw_earth = false;
	}
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}
void mouse(int x, int y)
{

}
int main(int argc, char **argv)
{


InitCallMeAL(1);

ALuint soundBuffer = LoadSound("sounds/Relocation.wav");

if (soundBuffer)
{
    PlaySoundInChannel(soundBuffer, 0);

   do
   {   
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("Journey through space");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
}
   while (ChannelIsPlaying(0));
    
    alDeleteBuffers(1,&soundBuffer);
    printf("ALL DONE!\n");
  }
  else
    printf("FAILED TO CREATE BUFFER!\n");
  
	HaltCallMeAL();

	exit(0);

}









#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include "VectorUtils3.h"

GLfloat CatmullRom1D(GLfloat u,GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3);
vec3 CatmullRom(GLfloat u, vec3 v0, vec3 v1, vec3 v2, vec3 v3);

GLfloat CatmullRom1DDerivative(GLfloat u,GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3);
vec3 CatmullRomDerivative(GLfloat u, vec3 v0, vec3 v1, vec3 v2, vec3 v3);
GLfloat CatmullRom1DDerivative2(GLfloat u,GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3);
vec3 CatmullRomDerivative2(GLfloat u, vec3 v0, vec3 v1, vec3 v2, vec3 v3);



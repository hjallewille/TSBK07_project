#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include "VectorUtils3.h"

GLfloat CatmullRom1D(GLfloat u,GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3)
{
        return
		(0.5 * ((2*p1) + (p2-p0)*u + 
		(2*p0 - 5*p1 + 4*p2 - p3)*u*u + 
		(3*p1-p0-3*p2+p3)*u*u*u));
}                

vec3 CatmullRom(GLfloat u, vec3 v0, vec3 v1, vec3 v2, vec3 v3)
{
        vec3 return_vec3;

        return_vec3.x = CatmullRom1D(u, v0.x, v1.x, v2.x, v3.x);
        return_vec3.y = CatmullRom1D(u, v0.y, v1.y, v2.y, v3.y);
        return_vec3.z = CatmullRom1D(u, v0.z, v1.z, v2.z, v3.z);

        return return_vec3;
}


GLfloat CatmullRom1DDerivative(GLfloat u,GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3) {

	return (3*u*u*(3*p1-p0-3*p2+p3) + 2*u*(2*p0 - 5*p1 + 4*p2 - p3) + (p2-p0)) * 0.5;
}

vec3 CatmullRomDerivative(GLfloat u, vec3 v0, vec3 v1, vec3 v2, vec3 v3)
{
        vec3 return_vec3;

        return_vec3.x = CatmullRom1DDerivative(u, v0.x, v1.x, v2.x, v3.x);
        return_vec3.y = CatmullRom1DDerivative(u, v0.y, v1.y, v2.y, v3.y);
        return_vec3.z = CatmullRom1DDerivative(u, v0.z, v1.z, v2.z, v3.z);

        return return_vec3;
}

GLfloat CatmullRom1DDerivative2(GLfloat u,GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3) {

        return (6*u*(3*p1-p0-3*p2+p3) + 2*(2*p0 - 5*p1 + 4*p2 - p3)) * 0.5;


}

vec3 CatmullRomDerivative2(GLfloat u, vec3 v0, vec3 v1, vec3 v2, vec3 v3)
{
        vec3 return_vec3;

        return_vec3.x = CatmullRom1DDerivative2(u, v0.x, v1.x, v2.x, v3.x);
        return_vec3.y = CatmullRom1DDerivative2(u, v0.y, v1.y, v2.y, v3.y);
        return_vec3.z = CatmullRom1DDerivative2(u, v0.z, v1.z, v2.z, v3.z);

        return return_vec3;
}

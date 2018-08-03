
#include GenerateTerrain.h

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	GLfloat scaleX = 1.0f;
	GLfloat scaleZ = 1.0f;
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / scaleX;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / scaleZ;

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x / scaleX; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z / scaleZ; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{

		if (x == 0 || z == 0) {
			
			// Set normalVector to (0,1,0)
			normalArray[(x + z * tex->width)*3 + 0] = 0;
			normalArray[(x + z * tex->width)*3 + 1] = 1;
			normalArray[(x + z * tex->width)*3 + 2] = 0;


		} else if (x == tex-> width || z == tex->height) {
			
			normalArray[(x + z * tex->width)*3 + 0] = 0;
			normalArray[(x + z * tex->width)*3 + 1] = 1;
			normalArray[(x + z * tex->width)*3 + 2] = 0;

		} else {
			//Left
				left_x = vertexArray[(x-1 + z * tex->width)*3 + 0];
				left_y = vertexArray[(x-1 + z * tex->width)*3 + 1];
				left_z = vertexArray[(x-1 + z * tex->width)*3 + 2];
			//Right
				right_x = vertexArray[(x+1 + z * tex->width)*3 + 0];
				right_y = vertexArray[(x+1 + z * tex->width)*3 + 1];
				right_z = vertexArray[(x+1 + z * tex->width)*3 + 2];

				//Upper
				upper_x = vertexArray[(x + (z+1) * tex->width)*3 + 0];
				upper_y = vertexArray[(x + (z+1) * tex->width)*3+ 1];
				upper_z = vertexArray[(x + (z+1) * tex->width)*3+ 2];

				//Lower
				lower_x = vertexArray[(x + (z-1) * tex->width)*3 + 0];
				lower_y = vertexArray[(x + (z-1) * tex->width)*3 + 1];
				lower_z = vertexArray[(x + (z-1) * tex->width)*3 + 2];

				vec3 lower = {lower_x, lower_y, lower_z};
				vec3 upper = {upper_x, upper_y, upper_z};
				vec3 left = {left_x, left_y, left_z};
				vec3 right = {right_x, right_y, right_z};

				vec3 right_left_plane = {right_x - left_x, right_y - left_y, right_z - left_z};
				vec3 upper_lower_plane = {upper_x - lower_x, upper_y - lower_y, upper_z - lower_z};

				vec3 normalVec = Normalize(CrossProduct(upper_lower_plane, right_left_plane));

				normalArray[(x + z * tex->width)*3 + 0] = normalVec.x;
				normalArray[(x + z * tex->width)*3 + 1] = normalVec.y;
				normalArray[(x + z * tex->width)*3 + 2] = normalVec.z;
				
		}



	}




	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}

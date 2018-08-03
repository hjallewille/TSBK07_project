# set this variable to the director in which you saved the common files
commondir = ../common/

all : TSBK07_project

TSBK07_project : TSBK07_project.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)CallMeAL.c Terrain_Gen/Generate_terrain.c CatmullRom_Splines/CatmullRom_Splines.c $(commondir)Mac/MicroGlut.m 
	gcc -Wall -o TSBK07_project -I$(commondir) -I../common/Mac -framework CoreFoundation -framework Carbon -framework Cocoa -framework OpenAL -framework OpenGL -DGL_GLEXT_PROTOTYPES TSBK07_project.c $(commondir)GL_utilities.c Terrain_Gen/Generate_terrain.c CatmullRom_Splines/CatmullRom_Splines.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)CallMeAL.c $(commondir)Mac/MicroGlut.m


clean :
	rm TSBK07_project

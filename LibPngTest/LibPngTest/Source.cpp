#ifndef TEST_FOR_LIBPNG
#define TEST_FOR_LIBPNG

#include <iostream>
#include <cstdlib>
#include "png.h"
#include "freeglut.h"

#define PNGSIGSIZE 8

using namespace std;

struct P
{
	int x, y;
	P() {x = y = 0;}
	P(unsigned int X, unsigned int Y) { x = X, y = Y; }
};

P imageSize;
bool hasAlpha;
int setHeight, setWidth;
GLuint textureData;

//next add the binary for the
//image data, as a byte array
png_byte * textureBinary;
GLuint texture;

bool validate(FILE *file)
{
	//Allocate a buffer of 8 bytes, where we can put the file signature.
	png_byte pngsig[PNGSIGSIZE];
	int is_png = 0;

	//Check if the read worked...
	if(file == NULL) return false;

	rewind(file);
	//Read the 8 bytes from the stream into the sig buffer.
	fread_s(pngsig, PNGSIGSIZE*sizeof(png_byte), 1, PNGSIGSIZE, file);

	//Let LibPNG check the sig. If this function returns 0, everything is OK.
	is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
	return (is_png == 0);
}

GLuint readData(FILE *file, int * imageWidth, int * imageHeight)
{
	//Check to see if the file contains the
	//correct png header
	if(!validate(file)) return false;
	
	//generate the png pointer which is the
	//structure that hold all of the data
	//that is contained in the png
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngPtr) return false;

	//create the info pointer, this is the
	//data contained in the info chunks of
	//the png file
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr) return false;

	//move the file pointer to the end of the
	//png signature
	png_set_sig_bytes(pngPtr, PNGSIGSIZE);

	//load the png data stream
	png_init_io(pngPtr, file);

	//Read the info chunk and store it
	//in the info pointer
	png_read_info(pngPtr, infoPtr);

	//Get the end of the png for clean up
	png_infop endPtr = png_create_info_struct(pngPtr);
	setjmp(png_jmpbuf(pngPtr));

	//Next we need the information from the
	//information pointer, 
	//this is where it is
	int bitDepth, colorType;
	png_uint_32 width, height;
	png_set_interlace_handling(pngPtr);
	//next get the info about info about
	//the png by checking the IHDR chunk
	png_get_IHDR(pngPtr, infoPtr, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL);

	//This is just for debugging to 
	//make sure the read is working
	cout << width << endl << height << endl << bitDepth << endl << colorType << endl;

	//Next read the info structure
	png_read_update_info(pngPtr, infoPtr);

	//now for the actual texturing information
	//we need to get the number of rowbytes
	int rowBytes = png_get_rowbytes(pngPtr, infoPtr);

	//openGL requires rows to be 4 byte aligned
	rowBytes += 3 - ((rowBytes - 1) % 4);

	//This is for debugging to
	//make sure that the system is working
	cout << rowBytes << endl;
	
	//allocate the correct number 
	//of bytes for the images
	textureBinary = (png_byte*)malloc(rowBytes * height * sizeof(png_byte) + 15);

	//Next we need to set the row
	//pointers so that we know where
	//the data is inside the block
	//of data
	png_bytep *rowPointers = (png_bytep*)malloc(height*sizeof(png_bytep));

	//next set the offsets 
	//to the row pointers
	for(int i = 0; i < height; i++)
	{
		rowPointers[height - 1 - i] = textureBinary + i * rowBytes;
	}
	//read the png into the imagedata through the rows
	png_read_image(pngPtr, rowPointers);
	
	png_destroy_read_struct(&pngPtr, &infoPtr, &endPtr);
	//free(imageData);
	//free(rowPointers);
	fclose(file);

	*imageHeight = height;
	*imageWidth = width;

	return texture;
}

void generateTexture(void)
{
	//Generate the texture for OGL
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, setWidth, setHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBinary);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void display(void)
{
	glutInitDisplayMode(GL_DEPTH | GL_DOUBLE | GL_RGBA);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	generateTexture();
	gluOrtho2D(0, 900, 0, 900);
	glViewport(0, 0, 900, 900);

	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0, 0);
		glVertex2f(0, 0);
		glTexCoord2f(0, 1);
		glVertex2f(0, 512);
		glTexCoord2f(1, 1);
		glVertex2f(512, 512);
		glTexCoord2f(1, 1);
		glVertex2f(512, 512);
		glTexCoord2f(1, 0);
		glVertex2f(512, 0);
		glTexCoord2f(0, 0);
		glVertex2f(0, 0);
	glEnd();
	glutSwapBuffers();
	glFlush();
}

void main(int argc, char *argv[])
{
	FILE *testImage;
	
	fopen_s(&testImage, "test.png", "rb+");
	textureData = readData(testImage, &setWidth, &setHeight);

	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutCreateWindow("testing");
	glutDisplayFunc(display);
	glutMainLoop();
}

#endif
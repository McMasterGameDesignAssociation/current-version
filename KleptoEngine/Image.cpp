/*
COPYRIGHT BENJAMIN ISHERWOOD 23/10/2014
THIS SOFTWARE IS INTENDED FOR OPEN SOURCE USE, REDISTRIBUTION
IS ENCOURAGED

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef IMAGE_M
#define IMAGE_M

#include "Image.h"

Image::Image(const char* startImage, Pos2D imageDivison) : 
BasicEntity(startImage, "Image"), hasAlpha(false), imageAvailable(false), 
textureBinary(NULL), divisionNumber(imageDivison), temporaryFile(NULL), type(TileMap)
{
	glGenTextures(1, &texture);
	readImage();
}

Image::Image(string startImage, string desc, Pos2D imageDivision) : 
BasicEntity(startImage, desc), hasAlpha(false), imageAvailable(false),
textureBinary(NULL), divisionNumber(imageDivision), temporaryFile(NULL), type(TileMap)
{
	glGenTextures(1, &texture);
	readImage();
}

Image::Image(string startImage, string desc, Pos2D imageDivision, ImageType setType) :
BasicEntity(startImage, desc), hasAlpha(false), imageAvailable(false),
textureBinary(NULL), divisionNumber(imageDivision), temporaryFile(NULL), type(setType)
{
	glGenTextures(1, &texture);
	readImage();
}

Image::~Image(void)
{
	glDeleteTextures(1, &texture);
	//delete[] textureBinary;
}

ImageType Image::getType(void) const { return type; }

Pos2D Image::getDivisionSize(void) const { return divisionNumber; }

bool Image::validatePNG(void)
{
	//Allocate a buffer of 8 bytes, where we can put the file signature.
	png_byte pngsig[PNGSIGSIZE];
	string name = getName();
	fopen_s(&temporaryFile, &name[0], "rb");
	//Check if the read worked...
	if(temporaryFile == nullptr)
	{
#ifdef _DEBUG
		cout << "That file does not exist" << endl;
#endif
		return false;
	}

	//Read the 8 bytes from the stream into the sig buffer.
	fread_s(pngsig, PNGSIGSIZE*sizeof(png_byte), 1, PNGSIGSIZE, temporaryFile);
	//Let LibPNG check the sig. If this function returns 0, everything is OK.
	return (png_sig_cmp(pngsig, 0, PNGSIGSIZE) == 0);
}

void Image::readImage(void)
{
#ifdef _DEBUG
	cout << "reading Image" << endl;
#endif
	//Check to see if the file contains the
	//correct png header
	if(!validatePNG()) return;
#ifdef _DEBUG
	cout << "PNG validated" << endl;
#endif
	//generate the png pointer which is the
	//structure that hold all of the data
	//that is contained in the png
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
#ifdef _DEBUG
	cout << "PNG read structure created" << endl;
#endif
	if(!pngPtr) return;

	//create the info pointer, this is the
	//data contained in the info chunks of
	//the png file
	png_infop infoPtr = png_create_info_struct(pngPtr);
#ifdef _DEBUG
	cout << "PNG info structure created" << endl;
#endif
	if(!infoPtr) return;

	//move the file pointer to the end of the
	//png signature
	png_set_sig_bytes(pngPtr, PNGSIGSIZE);
#ifdef _DEBUG
	cout << "PNG pointer moved to the end of the signature" << endl;
#endif
	//load the png data stream
	png_init_io(pngPtr, temporaryFile);
#ifdef _DEBUG
	cout << "PNG i/o initialized" << endl;
#endif
	//Read the info chunk and store it
	//in the info pointer
	png_read_info(pngPtr, infoPtr);
#ifdef _DEBUG
	cout << "PNG read info created" << endl;
#endif
	//Get the end of the png for clean up
	png_infop endPtr = png_create_info_struct(pngPtr);
	setjmp(png_jmpbuf(pngPtr));

	png_set_interlace_handling(pngPtr);

	//next get the info about info about
	//the png by checking the IHDR chunk
	Uint width, height;

	png_get_IHDR(pngPtr, infoPtr, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL);

	//Next read the info structure
	png_read_update_info(pngPtr, infoPtr);
	//png_get_IHDR(pngPtr, infoPtr, &width, &height, &bit_depth, &color_type,
	//	&interlace_type, NULL, NULL);
	Pos2D imageSize(width, height);
	changeObjectSize(imageSize);
	//(color_type == 6) ? hasAlpha = true : hasAlpha = false;
	int rowBytes = png_get_rowbytes(pngPtr, infoPtr);

	//openGL requires rows to be 4 byte aligned
	rowBytes += 3 - ((rowBytes - 1) % 4);

	textureBinary = (unsigned char*)malloc(rowBytes * size_t(imageSize.y) + 1);
	
	png_bytep *rowPointers = new png_bytep[height];

	//next set the offsets 
	//to the row pointers
	for(Uint i = 0; i < height; i++)
	{
		rowPointers[height - 1 - i] = textureBinary + i * rowBytes;
	}

	//read the png into the imagedata through the rows
	png_read_image(pngPtr, rowPointers);

	/* Clean up after the read,
	* and free any memory allocated */
	png_destroy_read_struct(&pngPtr, &infoPtr, &endPtr);
	delete[] rowPointers;
	/* Close the file */
	fclose(temporaryFile);

	/* That's it */
	imageAvailable = true;
}

void Image::enableSetUp(void)
{
	Pos2D imageSize = getSize();
	glEnable(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, GLsizei(imageSize.x),
				 GLsizei(imageSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE,
				 textureBinary);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

void Image::disableSetUp(void)
{
	glDisable(GL_BLEND | GL_SRC_ALPHA | GL_UNPACK_ALIGNMENT |
			  GL_ONE_MINUS_SRC_ALPHA | (hasAlpha ? GL_RGBA : GL_RGB) | 
			  GL_UNSIGNED_BYTE | GL_TEXTURE_WRAP_S | GL_CLAMP |
			  GL_TEXTURE_MAG_FILTER | GL_LINEAR | GL_TEXTURE_WRAP_T |
			  GL_TEXTURE_MIN_FILTER | GL_TEXTURE_2D);
}

GLuint Image::getTexture(void) const { return texture; }
GLubyte* Image::getTextureBinary(void) const { return textureBinary; }
bool Image::isAvailable(void) const { return imageAvailable; }

/*
	getRelativeSize is used so that users can get the
	relative size of the texture slider, i.e. how much
	percent each tile in the set takes up
*/
FV2 Image::getRelativeSize(void) const
{
	Pos2D imageSize = getSize();
	return FV2(double(divisionNumber.x) / imageSize.x, double(divisionNumber.y) / divisionNumber.y);
}

#endif
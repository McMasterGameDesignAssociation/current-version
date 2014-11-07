/*
COPYRIGHT BENJAMIN ISHERWOOD 23/10/2014
THIS SOFTWARE IS INTENDED FOR OPEN SOURCE USE, REDISTRIBUTION
IS ENCOURAGE

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARsE.
*/
#ifndef IMAGE_M
#define IMAGE_M

#include "Image.h"
Image::Image(const char* startImage, Pos2D chunkDim)
{
	changeName(startImage);
	chunkSize = chunkDim;
	glGenTextures(1, &texture);
	buildBlock();
	readImage();
}

Image::Image(string startImage, string desc, Pos2D chunkDim) : BasicEntity(startImage, desc)
{
	chunkSize = chunkDim;
	glGenTextures(1, &texture);
	buildBlock();
	readImage();
}

Image::~Image(void)
{
//	glDeleteTextures(1, &texture);
//	delete[] textureBinary;
}

/*
	Build block sets up the initial position of the
	texture slider in for the tiles that are used in
	the game
*/
void Image::buildBlock(void)
{
	ULong row = 0, column = 0;
	Pos2D imageSize = getSize();
	if(imageSize.x > 0 && chunkSize.x > 0 && chunkSize.y > 0)
	{
		//These are hard coded at the moment, but they will
		//be based on the image size when we are finished
		row = imageSize.x / chunkSize.x;
		column = imageSize.y / chunkSize.y;
	}
	else return;
}

void Image::readImage(void)
{
#ifdef _DEBUG
	cout << "Beginning read" << endl;
	cout << "Checking if the image " << getName() << " exists" << endl;
#endif
	string name = getName();
	const char *imageName = &name[0];
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
	FILE *fp;
	fopen_s(&fp, imageName, "rb");
	if(fp == NULL)
	{
#ifdef _DEBUG
		cout << "Image file "  << imageName << " is missing"  << endl;
#endif
		imageAvailable = false;
		return;
	}
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);
	if(png_ptr == NULL)
	{
		fclose(fp);
		imageAvailable = false;
		return;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		imageAvailable = false;
		return;
	}
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		/* Free all of the memory associated
		* with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		/* If we get here, we had a
		* problem reading the file */
		imageAvailable = false;
		return;
	}

	/* Set up the output control if
	* you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* If we have already
	* read some of the signature */
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
	png_uint_32 width, height;
	int bit_depth;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);
	Pos2D imageSize(width, height);
	changeObjectSize(imageSize);
	(color_type == 6) ? hasAlpha = true : hasAlpha = false;
	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	textureBinary = (unsigned char*)malloc(row_bytes * size_t(imageSize.y) + 1);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	for(unsigned int i = height; i--;)
	{
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(textureBinary + (row_bytes * (height - 1 - i)), row_pointers[i], row_bytes);
	}

	/* Clean up after the read,
	* and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	/* Close the file */
	fclose(fp);
	fp = NULL;
	delete fp;

	/* That's it */
	imageAvailable = true;
#ifdef _DEBUG
	cout << "image exists" << endl;
#endif
}

void Image::enableSetUp(void)
{
	Pos2D imageSize = getSize();
	glEnable(GL_TEXTURE_2D | GL_BLEND);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, GLsizei(imageSize.x),
		GLsizei(imageSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		textureBinary);
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
		GL_ONE_MINUS_SRC_ALPHA | (hasAlpha ? GL_RGBA : GL_RGB)
		| GL_UNSIGNED_BYTE | GL_TEXTURE_WRAP_S | GL_CLAMP |
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
	return FV2(double(chunkSize.x) / imageSize.x, double(chunkSize.y) / imageSize.y);
}

#endif
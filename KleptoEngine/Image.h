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
#ifndef IMAGE_H
#define IMAGE_H

#include "engineStdafx.h"
#include "BaseEntity.h"

class Image : public BasicEntity
{

	private:
		//Is the xy divisons of each image, 
		//i.e. a tilemap has 10 rows and 10 columns
		//or a sprite has 4 facing directions and 
		//15 frames for the animation
		Pos2D divisionNumber;

		//This is the image that we are using
		GLubyte *textureBinary;
		GLuint texture;

		//Use this file as the intermediate
		//step in the texturing process
		FILE * temporaryFile;
		
		int bitDepth, colorType;
		bool hasAlpha, imageAvailable;

		void readImage(void);
		bool validatePNG(void);

		//type is used to identify what kind of
		//image file this image is, if the image
		//can be overlapped then it will have
		//to be stored as a Sprite or Object map
		//While tiles don't overlap
		ImageType type;

	protected:
		friend Renderer;
		Image(const char* startImage, Pos2D chunkDim);
		Image(string startImage, string desc, Pos2D chunkDim);
		Image(string startImage, string desc, Pos2D chunkDim, ImageType setType);

		void enableSetUp(void);
		void disableSetUp(void);

		bool isAvailable (void) const;
		
		GLubyte* getTextureBinary(void) const;
		GLuint getTexture(void) const;
		FV2 getRelativeSize(void) const;

		Pos2D getDivisionSize(void) const;
		ImageType getType(void) const;

	public:
		~Image(void);
};

#endif
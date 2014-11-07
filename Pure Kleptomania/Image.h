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

#include "stdafx.h"
#include "BaseEntity.h"

class Image : public BasicEntity
{

	private:
		//Is the xy dimensions of the
		//each division of an image
		Pos2D chunkSize;
		//This is the image that we are using
		GLubyte *textureBinary;
		GLuint texture;
		
		bool hasAlpha;
		bool imageAvailable;
		void readImage(void);
		void buildBlock(void);

	protected:
		friend Renderer;
		Image(const char* startImage, Pos2D chunkDim);
		Image(string startImage, string desc, Pos2D chunkDim);

		void enableSetUp(void);
		void disableSetUp(void);

		bool isAvailable (void) const;
		
		GLubyte* getTextureBinary(void) const;
		GLuint getTexture(void) const;
		FV2 getRelativeSize(void) const;
	public:
		~Image(void);
};

#endif
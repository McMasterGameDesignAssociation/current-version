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
#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H

#include "stdafx.h"
#include "BaseEntity.h"
#include "Image.h"

/*
	quick aside for the texture data
	I need a way to move the data, and set
	the chunk size, so what will happen is
	in the populate arrays function we will set
	the chunk size and then move the coordinates
	to the correct place
*/

class BasicObject : public BasicEntity
{
	private:
		//bitmapName is used to make sure
		//the correct image is available
		string bitmapName;

		//Can animate is used to tell the
		//user whether or not the object
		//has an animation set up
		bool canAnimate;

		//Texture information

		//ChunkNumber is the number
		//of divisions in the image
		//used to texture the object
		Pos2D chunkNumber;

		/*
		imageDimensions, is the xy 
		pixel dimensions of the image
		that the 
		*/
		Pos2D imageDimensions;

		/*TextureId is used to 
		make the render process 
		much much faster, i.e. 
		not forcing a swap when 
		more objects are attached 
		to the same asset textureId 
		refers to the position 
		in the vector that an 
		image is stored*/
		ULong * textureId;

		//Texture coords are the 2D floating point
		//values of the bitmap coordinates, since
		//all bitmap sliders are marked as a percentage
		//of the image i.e. values between 0.0 - 1.0
		FV2 * textureCoords;

	protected:
		BasicObject(void);
		BasicObject(ULong id, string name, string desc);
		BasicObject(ULong id, string name, string desc, Pos2D size);
		BasicObject(ULong id, string name, string desc, string bitmap,
					Pos2D size, bool isAnimated);

		//changeBitmap changes the stored bitmap
		//name to the new one, when the bitmap is 
		//changed, if the bitmap does not match
		//the new bitmap will be loaded
		void changeBitmap(string bitmap);
		
		//Change animation sets the boolean value of
		//can animate, true means that the object
		//has an animation, and false means that
		//the object does not have an animation
		void changeAnimation(bool animate);

		//getBitmap returns the name of the current
		//bitmap that the object is using
		string getBitmap(void)		const;
		
		//isAnimated returns the 
		//canAnimate value
		bool isAnimated(void)		const;

		//Texture information

		//SetChunkNumber determines the number
		//of x and y divisions that are available
		//in the image
		void setChunkNumber(Pos2D xyNumber);

		/*
			setImageDimensions, lets the object
			know the size of the image for the
			texture, this is used for selecting
			the tiles
		*/
		void setImageDimensions(Pos2D textureSize);

		Pos2D getImageSize(void) const;
		Pos2D getImageDivisions(void) const;

		//linkTextureCoords links the objects
		//texture pointer to the render's draw
		//pointers
		void linkTextureCoords(FV2 *newCoords);

		//moveCoordTo moves the current location
		//of the texture slider to the 
		//xy positions by expanding the
		//2, 2D vectors into 6
		void moveCoordTo(FV2 * newCoords);

		ULong * getTextureId(void);
		ULong getTextureValue(void) const;
		FV2 * getTextureCoords(void);
};

#endif
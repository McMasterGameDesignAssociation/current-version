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

class BasicObject : public BasicEntity
{
	private:
		//bitmapName is used to make sure
		//the correct image is available
		string bitmapName;
		bool canAnimate;

		//Texture information
		Pos2D chunkNumber;

		//TextureId is used to make the render
		//process much much faster, i.e. not 
		//forcing a swap when more objects
		//are attached to the same asset
		//textureId refers to the position in
		//the vector that an image is stored
		ULong * textureId;
		FV2 * textureCoords;

	protected:
		BasicObject(void);
		BasicObject(ULong id, string name, string desc);
		BasicObject(ULong id, string name, string desc, string bitmap,
					Pos2D size, bool isAnimated);

		void changeBitmap(string bitmap);
		void changeAnimation(bool animate);

		string getBitmap(void)		const;
		bool isAnimated(void)		const;

		//Texture information
		void setChunkNumber(Pos2D xyNumber);
		void linkTextureCoords(FV2 *newCoords);
		void moveCoordTo(Pos2D xyNumber);
		ULong * getTextureId(void);
		ULong getTextureValue(void);
		FV2 * getTextureCoords(void);
};

#endif
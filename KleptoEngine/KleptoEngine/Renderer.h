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
#ifndef RENDERER_H
#define RENDERER_H

#include "stdafx.h"
#include "Image.h"
#include "World2D.h"

class Renderer
{
	private:
		//Dimensions are the size of the screen
		Pos2D dimensions;
		
		//View port position is the current location
		//That the viewport is located in the scene
		Pos2D viewPortPosition;
		
		//The view port size is the current area that the
		//player can be situated in i.e. is the view port
		//is 50% the size of the dimension than the player
		//can move the screen position by pushing against
		//the first or third quarter of the screen from 
		//the center
		Pos2D viewPortSize;

		/*
			basicBuffer and working buffers work as follows
			basicBuffer is the total size of the screen buffer
			this is what will be passed into the draw functions
			next the working buffer will be initiated to the
			basic buffer = color
			pixel buffer = shape

			each texture requires it own location, color, and texture
			coordinate buffer
		*/
		vector<FV3*> basicBuffer;
		vector<IV2*> pixels;
		
		//The maxTriangles gives the total number
		//of triangles in the render pipeline
		vector<ULong> maxNodes;
		
		vector<Image> imageFiles;
		
		/*
			The way that texture coords will works as follows:

			Each image index will match the textureCoords index
			in the populate sequence the renderer will subdivide
			all objects by into lists based on the textureIds

			each set of textureCoords will require a call to the
			render pipeline
		*/
		vector<FV2*> textureCoords;
		vector<ULong> currentPointer;
		//These texture Ids are used solely
		//for tracking the other objects relations
		//to images 
		vector<ULong *> textureIds;
		void addImage(Image image);
		static void display(void);
		static Image * currentImage;
		void linkTextureId(ULong *baseId);

		//Verify lists is used ot make sure that
		//all of the lists are the same length, i.e.
		//all of them have been processed
		bool verifyLists(void);

	protected:
		friend World2D;

		//Tiles don't move around in the world
		//Therefore they don't need to be linked
		//by address to the object that will be
		//moving around
		void addTileToDraw(Pos2D position, Pos2D size, bool canPass, ULong id);
		void setNewMax(ULong nodes, ULong id);

		//Actors and players move around in the
		//world and therefore they need to be
		//linked to the memory addresses of the
		//object so that a repopulate does
		//not need to occur in the draw cycles
		void linkSprite(Sprite * sprite);
		void linkTextureCoords(FV2 * linkage, ULong textureId, Uint tileNumber);
		bool loadImage(string imageName, string desc, Pos2D chunkSize, ULong * linkId, ImageType imageType);

		/*
		getImageDivisions, returns the image divisions at a specific
		position in the image set
		*/
		Pos2D getImageDivisions(Uint index) const;

		/*
		getImageSize, returns the size of a texture map at a specific
		place in the image set
		*/
		Pos2D getImageSize(Uint index) const;

	public:
		Renderer(void);
		void initWindow(int argc, char *argv[]);
		void render(void);
		void populateArrays(World2D * world);
};

#endif
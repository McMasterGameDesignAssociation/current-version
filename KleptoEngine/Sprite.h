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
#ifndef SPRITE_H
#define SPRITE_H

#include "engineStdafx.h"
#include "BasicObject.h"

class Sprite : public BasicObject
{
	private:
		FV2 position;
		Uint speed;
		Uint maxSpeed;
		double direction;
		IV2 *pixels;
		SpriteType type;
		void setUpSquare(void);
		
		//We need some animation components

		/*
			animation step lets the sprite
			know the intended column that
			the texture is coming from
		*/
		Uint animationStep = 0;

		/*
			currentTextureRow, is the row
			that the texture comes from
		*/
		Uint currentTextureRow = 0;

		/*
			This gives the largest value of
			the texturing index, so that the
			textures will wrap at 
			the specified location
		*/
		Uint maximumTextureIndex = 6;
		Uint minimumTextureIndex = 0;

		void updateFramePosition(void);

	protected:
		friend Renderer;
		Sprite(SpriteType chosenType);
		Sprite(FV2 initPos, Uint topSpeed, SpriteType chosenType);
		Sprite(ULong id, string name, string desc, FV2 initPos, Uint topSpeed, SpriteType chosenType);
		Sprite(ULong id, string name, string desc, Pos2D size, FV2 initPos, Uint topSpeed, SpriteType chosenType);
		Sprite(ULong id, string name, string desc, Pos2D size, FV2 initPos, Uint topSpeed, bool isAnimated, SpriteType chosenType);

		void changePosition(FV2 newPos);
		void changeSpeed(Uint newSpeed);
		void changeMaxSpeed(Uint topSpeed);
		void changeDirection(double newDirection);

		void setLocationPointer(IV2 *linkage);

		double getDirection(void)	const;
		Uint getSpeed(void)			const;
		Uint getMaxSpeed(void)		const;
		FV2 getPosition(void)		const;

		vector<Pos2D> rayTrace(World2D * world, Pos2D start, Pos2D end);

		void updatePosition(World2D * world);
		void resetAnimationStep(void);
		SpriteType getType(void) const;
};

#endif
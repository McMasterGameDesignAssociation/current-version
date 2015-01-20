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

#include "stdafx.h"
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
		
	protected:
		friend Renderer;
		Sprite(SpriteType chosenType);
		Sprite(FV2 initPos, Uint topSpeed, SpriteType chosenType);
		Sprite(ULong id, string name, string desc, FV2 initPos, Uint topSpeed, SpriteType chosenType);
		Sprite(ULong id, string name, string desc, Pos2D size, FV2 initPos, Uint topSpeed, SpriteType chosenType);

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
		SpriteType getType(void) const;
};

#endif
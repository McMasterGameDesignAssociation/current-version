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
#ifndef SPRITE_M
#define SPRITE_M

#include "Sprite.h"
#include "World2D.h"

Sprite::Sprite(SpriteType chosenType) :
BasicObject(), position(FV2()), pixels(new IV2[6]),
speed(0), maxSpeed(1), type(chosenType) {}

Sprite::Sprite(FV2 initPos, Uint topSpeed, SpriteType chosenType) :
BasicObject(), pixels(new IV2[6]), speed(0), maxSpeed(topSpeed), type(chosenType)
{
	if(initPos.x < 0 || initPos.y < 0) position = FV2();
	else position = initPos;
}

Sprite::Sprite(ULong id, string name, string desc, FV2 initPos, Uint topSpeed, SpriteType chosenType) : 
BasicObject(id, name, desc), pixels(new IV2[6]), speed(0), maxSpeed(topSpeed), type(chosenType)
{
	if (initPos.x < 0 || initPos.y < 0) position = FV2();
	else position = initPos;
}

Sprite::Sprite(ULong id, string name, string desc, Pos2D size, FV2 initPos, Uint topSpeed, SpriteType chosenType) : 
BasicObject(id, name, desc, size), pixels(new IV2[6]), speed(0), maxSpeed(topSpeed), type(chosenType)
{
	if(initPos.x < 0 || initPos.y < 0) position = FV2();
	else position = initPos;
}

Sprite::Sprite(ULong id, string name, string desc, Pos2D size, FV2 initPos, Uint topSpeed, bool isAnimated, SpriteType chosenType) :
Sprite(id, name, desc, size, initPos, topSpeed, chosenType) {changeAnimation(isAnimated);}


/*
    Set pointer location gives a linkage between the renderer
    and actors in the world, this will allow for the renderer
    to be updated directly from the objects, actors and the 
    player in the scene
*/
void Sprite::setLocationPointer(IV2 *linkage) 
{
	pixels = &linkage[0];
	setUpSquare();
}

void Sprite::changePosition(FV2 newPos) {position = newPos;}
void Sprite::changeSpeed(Uint newSpeed) {speed = newSpeed;}
void Sprite::changeMaxSpeed(Uint topSpeed) {maxSpeed = topSpeed;}

void Sprite::changeDirection(double newDirection) 
{
	if(newDirection < 90) currentTextureRow = 2;
	else if(newDirection < 180) currentTextureRow = 1;
	else if(newDirection < 270) currentTextureRow = 0;
	else if(newDirection < 360) currentTextureRow = 3;
	direction = newDirection;
}

double Sprite::getDirection(void) const {return direction;}
Uint Sprite::getSpeed(void) const {return speed;}
Uint Sprite::getMaxSpeed(void) const {return maxSpeed;}
FV2 Sprite::getPosition(void) const {return position;}

/*
    Setup square generates the two triangles that are 
    required for rendering the sprite in the world map

    NOTE: pixels is designed to be memory linked directly
          to the renderer's buffer
*/
void Sprite::setUpSquare(void)
{
	Pos2D size = getSize();
	size.x = size.x >> 1, size.y = size.y >> 1;
	/*
	[3] [2]
	[0] [1]

	[1] [2]
	[0]

		[5]
	[3] [4]

		[3]
	[5] [4]
	*/
	
	pixels[0].x = pixels[5].x = pixels[1].x = int(position.x - size.x);
	pixels[0].y = pixels[5].y = pixels[4].y = int(position.y - size.y);
	pixels[2].x = pixels[4].x = pixels[3].x = int(position.x + size.x);
	pixels[1].y = pixels[2].y = pixels[3].y = int(position.y + size.y);
}

/*
    rayTrace is designed send a single ray across a map and return the
    position of all of the spaces that the ray passes through, this will
    be useful later to for AI function 
*/
vector<Pos2D> Sprite::rayTrace(World2D* world, Pos2D start, Pos2D end)
{
	vector<Pos2D> intersection;
	ULong ** map = world -> getTileMap();
	ILong xDelta = ILong(end.x) - ILong(start.x), yDelta = ILong(end.y) - ILong(start.y);
	ILong slope = 2*yDelta - xDelta;
	int sign;
	yDelta > 0 ? sign = 1 : sign = -1;
	intersection.push_back(start);
	while(start != end)
	{
		start.x == end.x ? NULL : (start.x > end.x ? start.x-- : start.x++);
		if(slope > 0)
		{
			start.y == end.y ? NULL : (start.y > end.y ? start.y-- : start.y++);
			intersection.push_back(start);
			slope += sign*(2*yDelta - 2*xDelta);
		}
		else
		{
			intersection.push_back(start);
			slope += sign*(2*yDelta);
		}
#ifdef _VERBOSE
		cout << "Position found: " << start.x << " " << start.y << " world passable: " << world -> isPassable(start) << endl;
#endif
	}

	return intersection;
}

void Sprite::resetAnimationStep(void) 
{
	animationStep = 0;
	updateFramePosition();
}

void Sprite::updateFramePosition(void)
{
	//First make sure that the texture is in
	//a valid position, i.e. between the minimum
	//and maximum values for the slider
	if(animationStep == maximumTextureIndex && isAnimated() && isDirty()) 
		animationStep = minimumTextureIndex;
	else if(isAnimated() && isDirty()) animationStep++;

	//This part of the code moves the texture
	//slider to the correct position for the
	//sprite that is being rendered
	FV2 tileSize = getRelativeTileSize();
	FV2 *pointSet = new FV2[2];
	pointSet[0] = FV2((tileSize.x * (animationStep)), 1 - (tileSize.y * (currentTextureRow + 1)));
	pointSet[1] = FV2((tileSize.x * (animationStep + 1)), 1 - (tileSize.y * (currentTextureRow)));
	//cout << endl << pointSet[0].x << " " << pointSet[0].y << endl;
	//cout << pointSet[1].x << " " << pointSet[1].y << endl;
	moveCoordTo(pointSet);
	cleanObject();
	delete[] pointSet;
}

/*
    updatePosition checks for collision then moves the 
    position of the sprite.

    Collision detection is done by dividing the permiter
    of the sprite into nodal points. Each node represents
    the amount of space a single world tile takes up, i.e.
    sprite size/tile size = nodes per side, once the perimiter
    has been divided into nodes check each node, if they
    intersect with an occupied space.
*/
void Sprite::updatePosition(World2D * world)
{
	if(!isDirty() && animationStep != 0)
	{
		animationStep = 0;
		updateFramePosition();
		return;
	}
	else if(!isDirty()) return;
	//I am not sure what the increment is for
	//My best guess is that this is how big
	//each texture tile is
	FV2 increment = FV2(0, 0);

	//pos is used as part of the collision
	//detection, so that the sprite nows
	//how large of an area to check when it
	//is being moved around
	Pos2D pos = world->getDefaultTileSize();

	//The check is used to check 
	//whether or not the sprite 
	//has collided with an object
	//in the world space
	bool check = true;

	//
	Pos2D currentPoint, maximumPoint;

	//Size is used to fit the sprite in the current world
	//we perform a bitwise shift of 1 to the left to divide
	//by two effectively and effeciently i.e. x >> 1 == x*0.5
	IV2 size = IV2((int(getSize().x >> 1) - 1), int((getSize().y >> 1) - 1));

	//processAngle is used to find the x and y components of the
	//movement direction of the current sprite, this step converts
	//degress to radians to be processed by the math library
	double processAngle = TO_RAD(direction);
	
	//nextPos takes the angle of the direction and determines
	//the x and y components of the next position
	FV2 nextPos(sin(processAngle), cos(processAngle));

	//get new position by multiplying by the speed
	nextPos *= speed;

	//All the of next section of code handles how the 
	//sprite moves in the space, aswell as calculates
	//the collisions

	FV2 tempPosition = nextPos + position;
	double posInv[] = { 1 / double(pos.x), 1 / double(pos.y)};
	currentPoint.x = ULong((tempPosition.x - size.x)*posInv[0]);
	currentPoint.y = ULong((tempPosition.y - size.y)*posInv[1]);
	maximumPoint.x = ULong((tempPosition.x + size.x)*posInv[0]);
	maximumPoint.y = ULong((tempPosition.y + size.y)*posInv[1]);
	increment.x = (double(pos.x) / double(getSize().x))*(maximumPoint.x - currentPoint.x);
	increment.y = (double(pos.y) / double(getSize().y))*(maximumPoint.y - currentPoint.y);
	for(double i = double(currentPoint.x); i < double(maximumPoint.x); i += increment.x)
	{
		check &= world->isPassable(Pos2D(ULong(i), currentPoint.y));
		check &= world->isPassable(Pos2D(ULong(i), maximumPoint.y));
	}
	for(double i = double(currentPoint.y); i < double(maximumPoint.y); i += increment.y)
	{
		check &= world->isPassable(Pos2D(currentPoint.x, ULong(i)));
		check &= world->isPassable(Pos2D(maximumPoint.x, ULong(i)));
	}
	check &= world->isPassable(maximumPoint);
	if(check)
	{
		//Increment the animation frame
		if(frameLimter.updateTimer()) updateFramePosition();

		//if the sprite doesn't collide 
		//with anything then the new position 
		//will be the calculated position
		position = tempPosition;

		//Set up the draw data, and 
		//the texture data of the 
		//current sprite's data
		 setUpSquare();
	}
	speed = 0;
}

SpriteType Sprite::getType(void) const { return type; }
#endif
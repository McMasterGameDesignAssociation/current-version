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

Sprite::Sprite(SpriteType chosenType) : BasicObject()
{
	position = FV2();
	pixels = new IV2[6];
	speed = 0;
	maxSpeed = 1;
	type = chosenType;
}

Sprite::Sprite(FV2 initPos, Uint topSpeed, SpriteType chosenType) : BasicObject()
{
	pixels = new IV2[6];
	if(initPos.x < 0 || initPos.y < 0) position = FV2();
	else position = initPos;
	speed = 0;
	maxSpeed = topSpeed;
	type = chosenType;
}

Sprite::Sprite(ULong id, string name, string desc, FV2 initPos, Uint topSpeed, SpriteType chosenType) : BasicObject(id, name, desc)
{
	pixels = new IV2[6];
	if (initPos.x < 0 || initPos.y < 0) position = FV2();
	else position = initPos;
	speed = 0;
	maxSpeed = topSpeed;
	type = chosenType;
}

Sprite::Sprite(ULong id, string name, string desc, Pos2D size, FV2 initPos, Uint topSpeed, SpriteType chosenType) : BasicObject(id, name, desc)
{
	pixels = new IV2[6];
	changeObjectSize(size);
	if(initPos.x < 0 || initPos.y < 0) position = FV2();
	else position = initPos;
	speed = 0;
	maxSpeed = topSpeed;
	type = chosenType;
#ifdef _VERBOSE
	cout << endl;
	cout << "Sprite generated" << endl;
	cout << "id: " << getId() << endl;
	cout << "name: " << getName() << endl;
	cout << "description: " << getDesc() << endl;
	cout << "size: " << getSize().x << " " << getSize().y << endl;
	cout << "initial position: " << initPos.x << " " << initPos.y << endl;
	cout << "top speed: " << getMaxSpeed() << endl;
#endif
}

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
void Sprite::changeDirection(double newDirection) {direction = newDirection;}

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
	/*
	[3] [2]
	[0] [1]

	[1] [2]
	[0]

	[5]
	[3] [4]
	*/
	pixels[0].x = pixels[3].x = pixels[1].x = int(position.x - size.x*0.5);
	pixels[0].y = pixels[3].y = pixels[4].y = int(position.y - size.y*0.5);
	pixels[2].x = pixels[4].x = pixels[5].x = int(position.x + size.x*0.5);
	pixels[1].y = pixels[2].y = pixels[5].y = int(position.y + size.y*0.5);
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
#ifdef _VERBOSE
	cout << "Checking the placement of the sprite" << endl;
	cout << "Raw sprite position: " << position.x << " " << position.y << endl;
#endif
	FV2 increment = FV2(0, 0);
	Pos2D pos = world->getDefaultTileSize();
	bool check = true;

	Pos2D currentPoint, maximumPoint;
	currentPoint.x;

	IV2 size = IV2(int(getSize().x*0.5), int(getSize().y*0.5));
	double processAngle = TO_RAD(direction);
	FV2 nextPos(sin(processAngle), cos(processAngle));
	nextPos *= speed;
	FV2 tempPosition = nextPos + position;
	double posInv[] = { 1 / double(pos.x), 1 / double(pos.y)};
	currentPoint.x = ULong((tempPosition.x - size.x - 1)*posInv[0]);
	currentPoint.y = ULong((tempPosition.y - size.y - 1)*posInv[1]);
	maximumPoint.x = ULong((tempPosition.x + size.x - 1)*posInv[0]);
	maximumPoint.y = ULong((tempPosition.y + size.y - 1)*posInv[1]);
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
		position = tempPosition;
		setUpSquare();
	}
}

SpriteType Sprite::getType(void) const { return type; }
#endif
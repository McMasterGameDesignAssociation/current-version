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
#ifndef WORLD2D_M
#define WORLD2D_M

#include "World2D.h"
#include "Renderer.h"

Tile * World2D::currentTile;

World2D::World2D(void) : BasicEntity(0, "DEBUG WORLD", "THIS IS THE CURRENT VERSION OF THE WORLD BEING USED FOR DEBUG", Pos2D(1,1))
{
	textureId = new ULong();
	tileLocations = new ULong*[1];
	tileLocations[0] = new ULong[1];
}

World2D::World2D(Pos2D dimensions, Pos2D tileSize) : BasicEntity(0, "DEBUG WORLD", "THIS IS THE CURRENT VERSION OF THE WORLD BEING USED FOR DEBUG", dimensions)
{
	textureId = new ULong();
	defaultTileSize = tileSize;
}

void World2D::clearMaps(void)
{
	Pos2D mapDimensions = getSize();
	for (ULong i = mapDimensions.y - 1; i--;)
	{
		delete[] tileLocations[i];
		delete[] objectLocations[i];
	}
	delete[] tileLocations;
	delete[] objectLocations;
}

void World2D::setDimensions(Pos2D dimensions)
{
	/*
	first value is the y and the second is the x
	map[y][x]
	*/
	clearMaps();
	changeObjectSize(dimensions);
	tileLocations = new ULong*[Uint(dimensions.y)];
	objectLocations = new ULong*[Uint(dimensions.y)];
	for(int i = 0;  i < dimensions.y; i++)
	{
		tileLocations[i] = new ULong[Uint(dimensions.x)];
		objectLocations[i] = new ULong[Uint(dimensions.x)];
	}
	for(int i = 0; i < dimensions.y; i++)
	{ 
		for(int j = 0; j < dimensions.x; j++)
		{
			tileLocations[i][j] = 0;
			objectLocations[i][j] = 0;
		}
	}
}

void World2D::setTileSize(Pos2D dimensions) {defaultTileSize = dimensions;}

void World2D::addTile(string name, string desc, bool pass)
{
	ULong size = tileSet.size();
	currentTile = new Tile(size, name, desc, pass);
	tileSet.push_back(*currentTile);
}

void World2D::setTile(ULong setId, Pos2D location)
{
	Pos2D mapDimensions = getSize();
	if(mapDimensions.x > location.x || mapDimensions.y > location.y)
		tileLocations[location.y][location.x] = setId;
}

void World2D::changePlayerSpeed(Uint speed) {currentPlayer->changeSpeed(speed);}
void World2D::changePlayerDirection(double  direction) {currentPlayer->changeDirection(direction);}
void World2D::updateWorld(void)
{
	for(Uint i = 0; i < actorSet.size(); i++)
		actorSet.at(i) -> updatePosition(this);
	currentPlayer -> updatePosition(this);
}

void World2D::printToConsole(void)
{
	Pos2D mapDimensions = getSize();
	cout << endl;
	cout << "Map dimensions: " << mapDimensions.x << " " << mapDimensions.y << endl;
	for(int i = 0; i < mapDimensions.y; i++)
	{
		for(int j = 0; j < mapDimensions.x; j++)
			cout << tileLocations[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

World2D::~World2D(void) {}

ULong ** World2D::getTileMap(void) const {return tileLocations;}
Tile World2D::getTile(ULong id) const
{
	if(Uint(id) >= tileSet.size())
	{
#ifdef _DEBUG
		cout << "Tile id requested exceeds the size of the tile list" << endl;
#endif
#ifdef OS_WINDOWS
		SetLastError(0xAAAA0002);
#endif
		return Tile();
	}
	return tileSet.at(Uint(id));
}

Actor * World2D::getActor(ULong id) const
{
	if(id >= actorSet.size())
	{
#ifdef _DEBUG
		cout << "Tile id requested exceeds the size of the tile list" << endl;
#endif
#ifdef OS_WINDOWS
		SetLastError(0xAAAA0002);
#endif
		return new Actor();
	}
	return actorSet.at(Uint(id));
}

Player * World2D::getPlayer(void)		const {return currentPlayer;}
ULong World2D::getActorNumber(void)		const {return actorSet.size();}
ULong World2D::getTileNumber(void)		const 
{
	Pos2D mapDimensions = getSize();
	return mapDimensions.x * mapDimensions.y;
}
ULong World2D::getObjectNumber(void)	const {return 0;}
Pos2D World2D::getDimensions(void)		const {return getSize();}

bool World2D::isPassable(Pos2D tilePos)	const
{
	Pos2D mapDimensions = getSize();
	if(tilePos.x >= mapDimensions.x || tilePos.y >= mapDimensions.y) return false;
	else return tileSet.at(Uint(tileLocations[Uint(tilePos.y)][Uint(tilePos.x)])).getPass();
}

ULong World2D::getTileAt(Pos2D pos)		const
{
	Pos2D mapDimensions = getSize();
	if(pos.x > mapDimensions.x || pos.y > mapDimensions.y) return 0;
	else return tileLocations[pos.y][pos.x];
}

Pos2D World2D::getDefaultTileSize(void) const {return defaultTileSize;}

void World2D::addSprite(string name, string desc, Pos2D size, FV2 initPos,
	Uint topSpeed, string imageName, Renderer * render,
	Pos2D imageSeparations, SpriteType type)
{ 
	if(type == PlayerToken)
	{
		currentPlayer = new Player(0, name, desc, size, initPos, topSpeed);
		render->loadImage(imageName, desc + " image", imageSeparations, currentPlayer->getTextureId());
	}
	else if(type == ActorToken)
	{
		Actor * temp = new Actor(actorSet.size(), name, desc, initPos, size, topSpeed);
		render->loadImage(imageName, desc + " image", imageSeparations, currentPlayer->getTextureId());
		actorSet.push_back(temp);
	}
}

void World2D::setTileMap(string imageName, Pos2D divisionSize, Renderer * render)
{render->loadImage(imageName, "texture data for world: " + getName(), divisionSize, textureId);}

void World2D::testRayTrace(Pos2D start, Pos2D end)
{currentPlayer->rayTrace(this, start, end);}

ULong World2D::getTextureValue(void) const {return *textureId;}

#endif
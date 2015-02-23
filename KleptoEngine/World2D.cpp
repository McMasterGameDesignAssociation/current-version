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

bool World2D::verifyPlayer(void) {return (currentPlayer != NULL);}

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

void World2D::addTileToSet(string name, string desc, bool pass)
{
#ifdef _VERBOSE
	cout << "Currently adding tile" << tileSet.size() << endl;
#endif
	currentTile = new Tile(tileSet.size(), name, desc, pass);
	tileSet.push_back(*currentTile);
#ifdef _VERBOSE
	cout << "Tile added to tile set, new tile set size is: " << tileSet.size() << endl;
#endif
}

void World2D::addTileToSet(string name, string desc, Uint textureId, bool pass)
{
#ifdef _VERBOSE
	cout << "Currently adding tile, tileSet size is: " << tileSet.size() << endl;
#elif _DEBUG
	cout << "adding a tile" << endl;
#endif
	currentTile = new Tile(tileSet.size(), name, desc, textureId, pass);
	tileSet.push_back(*currentTile);
#ifdef _VERBOSE
	cout << "Tile added to tile set, new tile set size is: " << tileSet.size() << endl;
#endif
}

void World2D::setTile(ULong setId, Pos2D location)
{
	Pos2D mapDimensions = getSize();
	if(mapDimensions.x > location.x || mapDimensions.y > location.y)
		tileLocations[location.y][location.x] = setId;
}

void World2D::changePlayerSpeed(Uint speed) {currentPlayer->changeSpeed(speed);}

void World2D::changePlayerDirection(double  direction)  
{ 
	if(verifyPlayer())
	{
		currentPlayer->changeDirection(direction);
	}
}

void World2D::updateWorld(void)
{
	for(Uint i = 0; i < actorSet.size(); i++)
		actorSet.at(i) -> updatePosition(this);
	if(verifyPlayer()) currentPlayer->updatePosition(this);
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
Uint World2D::getTileTexture(Pos2D tilePos) const 
{ 
	ULong tileId = getTileAt(tilePos);
	if(tileId == ERROR_INTEGER) return Uint(tileId);
	return Uint(tileSet.at(Uint(tileId)).getImageIndex()); 
}

bool World2D::isPassable(Pos2D tilePos)	const
{
	ULong tileId = getTileAt(tilePos);
	if(tileId == ERROR_INTEGER) return false;
	else return tileSet.at(Uint(tileId)).getPass();
}

ULong World2D::getTileAt(Pos2D pos)		const
{
	Pos2D mapDimensions = getSize();
	if(pos.x > mapDimensions.x || pos.y > mapDimensions.y) return ERROR_INTEGER;
	else return tileLocations[pos.y][pos.x];
}

Pos2D World2D::getDefaultTileSize(void) const {return defaultTileSize;}

void World2D::addSprite(string name, string desc, Pos2D size, FV2 initPos,
	Uint topSpeed, string imageName, Renderer * render,
	Pos2D imageSeparations, bool isAnimated, SpriteType type)
{ 
	if(type == PlayerToken)
	{
		currentPlayer = new Player(0, name, desc, size, initPos, topSpeed, isAnimated);
		render->loadImage(imageName, desc + " image", imageSeparations, currentPlayer->getTextureId(),  SpriteMap);
		Pos2D imageSize = render->getImageSize(*currentPlayer->getTextureId());
		currentPlayer->setRelativeTileSize(imageSize, imageSeparations);
#ifdef _DEBUG
		cout << "Player image size is: " << imageSize.x << " " << imageSize.y << endl;
#endif
	}
	else if(type == ActorToken)
	{
		Actor * temp = new Actor(actorSet.size(), name, desc, initPos, size, topSpeed, isAnimated);
		render->loadImage(imageName, desc + " image", imageSeparations, temp->getTextureId(), SpriteMap);
		Pos2D imageSize = render->getImageSize(*temp->getTextureId());
		temp->setRelativeTileSize(imageSize, imageSeparations);
		actorSet.push_back(temp);
#ifdef _DEBUG
		cout << "Actor image size is: " << imageSize.x << " " << imageSize.y << endl;
#endif
	}
}

void World2D::updateTileTexture(Renderer * render)
{
	//First the engine needs to get the
	//divison number of the tile map
#ifdef _VERBOSE
	cout << endl << "Running the update texture routine for world: " << getName() << endl  << endl;
#endif
	Pos2D divisions = render -> getImageDivisions(Uint(*textureId));
	Pos2D imageSize = render -> getImageSize(Uint(*textureId));

	/* now that we have the division 
	size we need to loop through
	the tile list and update all
	the tile divisions*/
	for(Uint i = 0; i < tileSet.size(); i++)
	{
		tileSet.at(i).setChunkNumber(divisions);
		tileSet.at(i).setImageDimensions(imageSize);
		/*Once all of the tiles have
		the updated divison numbers
		move the tile texture coords
		to the requested positions*/
		tileSet.at(i).moveCoords();
	}

}

void World2D::resetPlayerAnimationCycle(void) { currentPlayer->resetAnimationStep(); }

FV2 * World2D::getTileTextureCoords(Pos2D location)
{ return tileSet.at(Uint(getTileAt(location))).getTextureCoords(); }

void World2D::setTileMap(string imageName, Pos2D divisionSize, Renderer * render)
{
#ifdef _DEBUG
	cout << "renderer location " << &render << "loading image" << endl;
#endif
	//Load the image into the system
	render->loadImage(imageName, "texture data for world: " + getName(), divisionSize, textureId, TileMap);
}

void World2D::testRayTrace(Pos2D start, Pos2D end)
{currentPlayer->rayTrace(this, start, end);}

ULong World2D::getTextureValue(void) const {return *textureId;}
ULong World2D::getPlayerTexture(void)	const {return currentPlayer->getTextureValue();}
ULong World2D::getActorTexture(ULong id) const 
{ 
	if(id > actorSet.size()) return 0xFFFFFFFF;
	return actorSet.at(Uint(id))->getTextureValue();
}

void World2D::makePlayerDirty(void) { currentPlayer->makeObjectDirty(); }
void World2D::makePlayerClean(void) { currentPlayer->makeObjectDirty(); }
bool World2D::isPlayerDirty(void) const { return currentPlayer->isDirty(); }

#endif
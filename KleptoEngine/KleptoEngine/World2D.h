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
#ifndef WORLD2D_H
#define WORLD2D_H

#include "stdafx.h"
#include "Tile.h"
#include "Player.h"
#include "Actor.h"
#include "Inventory.h"
#include "BaseEntity.h"

class World2D : public BasicEntity
{
	private:
		//This is the default size of
		//the world tiles in pixels, in
		//x and y dimensions respectively
		Pos2D defaultTileSize;
		
		//availableItems is the inventory
		//of the world, it should be used
		//to store all items that can be
		//placed in the current world
		Inventory availableItems;

		//tileSet is the list of available
		//tiles that can be used to make up
		//the current world
		vector<Tile> tileSet;

		//tileLocations is a set of id numbers
		//that link to the tileSet, tileLocations
		//is also set up as a 2D matrix and is linked
		//directly to the position of 
		//the intended tile location
		ULong ** tileLocations;

		//currentPlayer is a pointer to
		//the player that is currently
		//being used to play the game
		Player * currentPlayer;

		//actorSet is the set of all
		//available NPCs that exist in
		//the world
		vector<Actor*> actorSet;

		//objectLocations is a 2D matrix
		//of all available objects in the
		//world, each object has a unique
		//id number and cannot be duplicated
		ULong ** objectLocations;

		//clearMaps is a reset routine
		//used to clean up all of the 2D
		//location matrices
		void clearMaps(void);
		
		//currentTile is used to manage
		//the tile factory
		static Tile * currentTile;
		
		//textureId is the tileMap
		//image id number
		ULong * textureId;

		//verifyPlayer is used to verify
		//if the player has been initialized
		//if the player has not been initialized
		//this function returns false
		bool verifyPlayer(void);

		//getTileFromLocation is a shortcut function
		//that is used to get tile 
		Uint getTileFromLocation(Pos2D tilePos) const;

	protected:
		friend Renderer;
		friend Sprite;

		/*
		updateTileTexture, is a routine that sets all of
		the tiles to the correct coordinates, using the
		division size stored in the tilemap
		*/
		void updateTileTexture(Renderer * render);

		//getTileMap returns an instance
		//of the entire tileMap matrix
		ULong ** getTileMap(void)			const;

		//getTile returns the tile at
		//the specified id in the 
		//tileSet list
		Tile getTile(ULong id)				const;

		//getActor returns a pointer to
		//the actor stored in the actorSet
		//list stored at the specific id
		Actor * getActor(ULong id)			const;

		//getActorNumber returns the number
		//of actors currently stored in 
		//the current world
		ULong getActorNumber(void)			const;

		//getTileNumber returns the number
		//of tiles currently stored in 
		//the current world
		ULong getTileNumber(void)			const;

		//getObjctNumber returns the number
		//of objects currently stored in 
		//the current world
		ULong getObjectNumber(void)			const;

		//getDimensions returns the x and y dimensions
		//of the number of tiles that the world
		//contains
		Pos2D getDimensions(void)			const;

		//getDefaultTileSize returns the x and y
		//size of the defualt tile size
		Pos2D getDefaultTileSize(void)		const;

		//getPlayer returns a pointer to the
		//current player stored in the world
		Player * getPlayer(void)			const;

		/*isPassable checks the tile map at the 
		given position and returns boolean value
		of whether or not an entity can pass through
		that space*/
		bool isPassable(Pos2D tilePos)		const;

		/*getTileTexture returns the index value
		of the tile texture that is used to 
		represent the tile in the world*/
		Uint getTileTexture(Pos2D tilePos)	const;

		/*setTileChunkSize sets the 
		relaed image x and y 
		division sizes*/
		void setTileImageDimensions(Pos2D xyValue, Uint tileID);

		void setTileImagePos(Pos2D xyValue);
#ifndef _DEBUG
		//This returns the tileMap image id
		ULong getTextureValue(void)			const;
#endif

		ULong getPlayerTexture(void)		const;
		ULong getActorTexture(ULong id)		const;
		FV2 * getTileTextureCoords(Pos2D locations);

	public:
		World2D(void);
		World2D(Pos2D dimensions, Pos2D tileSize);
		void setDimensions(Pos2D dimensions);
		void setTileSize(Pos2D dimensions);
		void addTileToSet(string name, string desc, bool pass);
		void addTileToSet(string name, string desc, Uint textureId, bool pass);
		/*
			addSprite builds a sprite entity and adds it to the list of
			sprites that are currently available to the world, there
			are two types of sprites currently, being: players and actors.

			Actors are autonomous NPCs while players are playable only sprites
			currently only one player can be generated per world
		*/
		void addSprite(string name, string desc, Pos2D size, FV2 initPos, 
					   Uint topSpeed, string imageName, Renderer * render,
					   Pos2D imageSeparations, SpriteType type);
		
		void setTile(ULong setId, Pos2D location);
		void printToConsole(void);

		void changePlayerSpeed(Uint speed);
		void changePlayerDirection(double direction);
		void updateWorld(void);
		void setTileMap(string imageName, Pos2D divisionSize, Renderer * render);
		ULong getTileAt(Pos2D pos) const;

		~World2D(void);

		void testRayTrace(Pos2D start, Pos2D end);

#ifdef _DEBUG
		//This is the debug reference to the getTextureValue
		//This returns the tileMap image id
		ULong getTextureValue(void)		const;
#endif
		
};

#endif
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
		Pos2D defaultTileSize;
		Inventory availableItems;

		vector<Tile> tileSet;
		ULong ** tileLocations;
		Player * currentPlayer;
		vector<Actor*> actorSet;
		ULong ** objectLocations;
		void clearMaps(void);
		//currentTile is used to manage
		//the tile factory
		static Tile * currentTile;
		ULong * textureId;

	protected:
		friend Renderer;
		friend Sprite;
		ULong ** getTileMap(void)		const;
		Tile getTile(ULong id)			const;
		Actor * getActor(ULong id)		const;
		ULong getActorNumber(void)		const;
		ULong getTileNumber(void)		const;
		ULong getObjectNumber(void)		const;
		Pos2D getDimensions(void)		const;
		Pos2D getDefaultTileSize(void)	const;
		Player * getPlayer(void)		const;
		bool isPassable(Pos2D tilePos)	const;
#ifndef _DEBUG
		ULong getTextureValue(void)		const;
#endif
		
	public:
		World2D(void);
		World2D(Pos2D dimensions, Pos2D tileSize);
		void setDimensions(Pos2D dimensions);
		void setTileSize(Pos2D dimensions);
		void addTile(string name, string desc, bool pass);
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
		ULong getTextureValue(void)		const;
#endif
		
};

#endif
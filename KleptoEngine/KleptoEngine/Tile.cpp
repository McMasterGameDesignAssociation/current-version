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
#ifndef TILE_M
#define TILE_M

#include "Tile.h"

Tile::Tile(void) : WorldObject2D(), imageIndex(0) {}

Tile::Tile(ULong id, string name, string desc, Uint textureIndex, bool pass) : 
WorldObject2D(id, name, desc, pass), imageIndex(textureIndex)  {}

Tile::Tile(ULong id, string name, string desc, Uint textureIndex) : 
Tile(id, name, desc, textureIndex, false) {}

Uint Tile::getImageIndex(void) const { return imageIndex; }

void Tile::moveCoords(void)
{
#ifdef _VERBOSE
	cout << "currently updating tile coords of: " << getName() << endl;
#endif
	/*First we need to get 
	the x and y position
	of the tile location 
	in the image which are
	located at 
	x = imageIndex/division.x*tileSize, 
	and 
	y = imageInex%division.y*tileSize*/

	Pos2D imageSize = getImageSize(), divisions = getImageDivisions();
	Pos2D tileSize = getImageSize() / divisions;
	Pos2D xyPlacement = Pos2D((imageIndex % divisions.y)*tileSize.x, (imageIndex / divisions.x)*tileSize.y);

#ifdef _VERBOSE
	cout << "current tile size: " << tileSize.x << " " << tileSize.y << endl;
	cout << "position should be x: " << (imageIndex % divisions.y) << " and y: " << (imageIndex / divisions.x) << endl;
	cout << "at position x: " << xyPlacement.x << " y: " << xyPlacement.y << endl;
	cout << "relative position x: " << double(xyPlacement.x) / double(imageSize.x) << " y: " << double(xyPlacement.y) / double(imageSize.y) << endl;
#endif
	FV2 * tempValue = new FV2[2];
	tempValue[0] = FV2(double(xyPlacement.x) / double(imageSize.x), 1 - (double(xyPlacement.y) / double(imageSize.y)));
	tempValue[1] = FV2(double(xyPlacement.x + tileSize.x) / double(imageSize.x), 1 - (double(xyPlacement.y + tileSize.y) / double(imageSize.y)));
	moveCoordTo(tempValue);
	delete[] tempValue;
}

#endif
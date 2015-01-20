/*
COPYRIGHT BENJAMIN ISHERWOOD 23/10/2014
THIS SOFTWARE IS INTENDED FOR OPEN SOURCE USE, REDISTRIBUTION
IS ENCOURAGED

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef BASIC_OBJECT_M
#define BASIC_OBJECT_M

#include "BasicObject.h"

BasicObject::BasicObject(void) : BasicEntity(), textureCoords(new FV2[6]),
textureId(new ULong()), bitmapName("No Bitmap"), canAnimate(false) {}

BasicObject::BasicObject(ULong id, string name, string desc) :
BasicEntity(id, name, desc), textureCoords(new FV2[6]),
textureId(new ULong()), bitmapName("No Bitmap"), canAnimate(false) {}

BasicObject::BasicObject(ULong id, string name, string desc, string bitmap, Pos2D size, bool isAnimated) : 
BasicEntity(id, name, desc, size), textureId(new ULong()), 
textureCoords(new FV2[6]), bitmapName(bitmap), canAnimate(isAnimated) {}

BasicObject::BasicObject(ULong id, string name, string desc, Pos2D size) : 
BasicEntity(id, name, desc, size), textureCoords(new FV2[6]),
textureId(new ULong()), bitmapName("No Bitmap"), canAnimate(false) {}

string	BasicObject::getBitmap(void)	const {return bitmapName;}
bool	BasicObject::isAnimated(void)	const {return canAnimate;}
void BasicObject::changeBitmap(string bitmap) {bitmapName = bitmap;}
void BasicObject::changeAnimation(bool animate) {canAnimate = animate;}
void BasicObject::linkTextureCoords(FV2 * newCoords) {textureCoords = &newCoords[0];}
FV2 * BasicObject::getTextureCoords(void) {return &textureCoords[0];}
ULong * BasicObject::getTextureId(void) {return &textureId[0];}
void BasicObject::setChunkNumber(Pos2D xyNumber) {chunkNumber = xyNumber;}
ULong BasicObject::getTextureValue(void) const { return *textureId; }

void BasicObject::moveCoordTo(Pos2D xyNumber)
{
	if(xyNumber.x >= chunkNumber.x || xyNumber.y >= chunkNumber.y)
	{
#ifdef OS_WINDOWS
		SetLastError(0xAAAA0004);
#elif defined(OS_LINUX)
		perror(0xAAAA0004);
#endif
#ifdef _DEBUG
		cout << "the new texture coordinates exceed the size of the number of tiles in the image" << endl;
#endif
		return;
	}
	//x = imageSize
#ifdef _VERBOSE
	cout << "moving the texture coords to new position" << endl;
#endif
	FV2 start = FV2(0, 0);
	FV2 end = FV2(0.125, 0.125);
	//FV2 start = FV2(double(xyNumber.x) / chunkNumber.x, double(xyNumber.y) / chunkNumber.y);
	//FV2 end = FV2(double(xyNumber.x + 1) / chunkNumber.x, double(xyNumber.y + 1) / chunkNumber.y);
	/*
	[1][2]
	[0]
	   [3]
	[5][4]
	*/
	textureCoords[0] = textureCoords[5] = start;
	textureCoords[1] = FV2(start.x, end.y);
	textureCoords[2] = textureCoords[3] = end;
	textureCoords[4] = FV2(end.x, start.y);
}

void BasicObject::setImageTileSize(Pos2D setSize) {tileSize = setSize;}

#endif
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
OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef RENDERER_M
#define RENDERER_M

#include "Renderer.h"

Image * Renderer::currentImage;

Renderer::Renderer(void)
{
	dimensions = Pos2D(900, 900);
	viewPortPosition = Pos2D(0,0);
	viewPortSize = Pos2D(900, 900);
	maxNodes = 0;
	currentPointer = 0;
	basicBuffer = new FV3[Uint(maxNodes)];
	pixels = new IV2[Uint(maxNodes)];

}

void Renderer::addTile(Pos2D position, Pos2D size, bool canPass)
{
	if(currentPointer >= maxNodes)
	{
		cout << "Maximum number of triangles reached" << endl;
#ifdef OS_WINDOWS
		SetLastError(0xAAAA0001);
#endif
		return;
	}
	Pos2D * points = new Pos2D[4];
	//The position is the center of the tiles that are being drawn
	//Therefore the the starts at position - 1/2 size
	//and the corners exist as follows
	//[3] [2]
	//[0] [1]
	points[0].x = points[3].x = position.x;
	points[0].y = points[1].y = position.y;
	points[1].x = points[2].x = position.x + size.x, 
	points[2].y = points[3].y = position.y + size.y;
#ifdef _DEBUG
	points[0] = points[0] + IV2(1, 1);
	points[1] = points[1] + IV2(-1, 1);
	points[2] = points[2] + IV2(-1, -1);
	points[3] = points[3] + IV2(1, -1);
#endif

	FV3 color;
	if(canPass) color = FV3(1, 1, 1);
	else color = FV3(1, 0, 0);

	pixels[currentPointer] = points[0];
	basicBuffer[currentPointer] = color;
	currentPointer++;
	pixels[currentPointer] = points[3];
	basicBuffer[currentPointer] = color;
	currentPointer++;
	pixels[currentPointer] = points[2];
	basicBuffer[currentPointer] = color;
	currentPointer++;
	pixels[currentPointer] = points[2];
	basicBuffer[currentPointer] = color;
	currentPointer++;
	pixels[currentPointer] = points[1];
	basicBuffer[currentPointer] = color;
	currentPointer++;
	pixels[currentPointer] = points[0];
	basicBuffer[currentPointer] = color;
	currentPointer++;

	delete[] points;
}

void Renderer::linkSprite(Sprite * sprite)
{
	FV3 testColor;
	if(sprite -> getType() == ActorToken) testColor = FV3(0, 0, 1);
	else if(sprite->getType() == PlayerToken) testColor = FV3(0, 1, 0);
	for(int i = 0; i < 6; i++) basicBuffer[currentPointer + i] = testColor;
	sprite->setLocationPointer(&pixels[currentPointer]);
	currentPointer += 6;
}

void Renderer::setNewMax(ULong nodes)
{
	maxNodes = nodes*6;
	currentPointer = 0;
	basicBuffer = new FV3[Uint(maxNodes)];
	pixels = new IV2[Uint(maxNodes)];

}

void Renderer::render(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glColorPointer(3,GL_DOUBLE, 0, basicBuffer);
	glVertexPointer(2, GL_INT, 0, pixels);
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(currentPointer));

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Renderer::display(void) {}

/*
	Populate arrays is where all of the
	render information is set up

	each image will have its own set of
	position information, texture coordinates
	and color information

	Therefore the render will be broken down into
	the total number of textures that exist in the
	scene that is being drawn
*/
void Renderer::populateArrays(World2D *world)
{
	//Set the number of render nodes
	setNewMax(world->getTileNumber() +
			  world -> getActorNumber() + 
			  world -> getObjectNumber() + 1);
	Pos2D mapDims = world -> getDimensions();
	Pos2D tileSize = world -> getDefaultTileSize();

#ifdef _VERBOSE
	cout << "Tile dimensions: " << tileSize.x << " " << tileSize.y << endl;
#endif

	for(ULong i = 0; i < mapDims.y; i++)
	{
		for(ULong j = 0; j < mapDims.x; j++)
		{
			bool isPassable = world->isPassable(Pos2D(i, j));
			addTile(Pos2D(i*tileSize.y, j*tileSize.x), tileSize, isPassable);
		}
	}
	linkSprite(world -> getPlayer());
	for(Uint i = 0; i < world -> getActorNumber(); i++)
		linkSprite(world -> getActor(i));
}

void Renderer::initWindow(int argc, char *argv[]) {}
void Renderer::addImage(Image image)
{
	image.changeId(imageFiles.size());
	imageFiles.push_back(image);
}

/*
	loadImage is used to populate the list of textures that are
	contained in the system, the process is as follows
	
	1.	check if the image name matches on in the system
		a.	if it exists already do not reload the image
			and link the image ID to the id if that image
	2.	if the image name does not matching an existing image
		begin the image load
		a.	If the image exists link the image id to the 
			object that loaded it
*/
bool Renderer::loadImage(string imageName, string desc, Pos2D chunkSize, ULong *linkId)
{
#ifdef _DEBUG
	cout << endl << "Loading image: " << imageName << endl;
#endif
	for(Uint i = 0; i < imageFiles.size(); i++)
	{
		if(imageName == imageFiles.at(i).getName())
		{
#ifdef OS_WINDOWS
			SetLastError(0xAAAA0002);
	#ifdef _DEBUG
				cout << "Image: " << imageName << " already in system with ID: " << imageFiles.at(i).getId() << endl;
	#endif
#endif	
			*linkId = i;
			linkTextureId(linkId);
			return true;
		}
	}
	currentImage = new Image(imageName, desc, chunkSize);
	if(!currentImage -> isAvailable())
	{
#ifdef OS_WINDOWS
		SetLastError(0xAAAA0003);
	#ifdef _DEBUG
			cout << "Image: " << imageName << " does not exist" << endl;
	#endif
#endif
		return false;
	}
	addImage(*currentImage);
	*linkId = imageFiles.size() - 1;
	linkTextureId(linkId);
	return true;
}
/*
	linkTextureId links the addresses of the ids
	to the id of the requested image, this allows
	all images that share an image to stay synchronized
	if the ids need to be changed later, i.e. during a 
	sort
*/
void Renderer::linkTextureId(ULong *baseId) 
{ 
	for(Uint i = 0; i < textureIds.size(); i++)
	{
		if(*baseId == *textureIds.at(i))
		{
#ifdef _DEBUG
			cout << "Id matches: " << i << endl;
#endif
			baseId = &textureIds.at(i)[0];
			return;
		}
	}
	textureIds.push_back(new ULong());
	*textureIds.at(textureIds.size() - 1) = *baseId;
	baseId = &textureIds.at(textureIds.size() - 1)[0];
#ifdef _DEBUG
	cout << "Base id for the loaded texture is now: " << *baseId << endl;
#endif
}
#endif 
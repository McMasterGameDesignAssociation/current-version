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
#ifndef RENDERER_M
#define RENDERER_M

#include "Renderer.h"

Image * Renderer::currentImage;

Renderer::Renderer(void) : dimensions(Pos2D(900, 900)),
viewPortPosition(Pos2D(0, 0)), viewPortSize(Pos2D(900, 900)) {}

void Renderer::addTileToDraw(Pos2D position, Pos2D size, bool canPass, ULong id)
{
	if(currentPointer.empty() || maxNodes.empty())
	{
		cout << "current list of values is empty" << endl;
#ifdef OS_WINDOWS
		SetLastError(0xAAAA0001);
#endif
		return;
	}
	if(currentPointer.at(Uint(id)) >= maxNodes.at(Uint(id)))
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

	pixels.at(Uint(id))[currentPointer.at(Uint(id))] = points[0]; //0//3
	basicBuffer.at(Uint(id))[currentPointer.at(Uint(id))] = color;
	currentPointer.at(Uint(id))++;
	pixels.at(Uint(id))[currentPointer.at(Uint(id))] = points[3]; //3//2
	basicBuffer.at(Uint(id))[currentPointer.at(Uint(id))] = color;
	currentPointer.at(Uint(id))++;
	pixels.at(Uint(id))[currentPointer.at(Uint(id))] = points[2]; //2//2
	basicBuffer.at(Uint(id))[currentPointer.at(Uint(id))] = color;
	currentPointer.at(Uint(id))++;
	pixels.at(Uint(id))[currentPointer.at(Uint(id))] = points[2]; //2//1
	basicBuffer.at(Uint(id))[currentPointer.at(Uint(id))] = color;
	currentPointer.at(Uint(id))++;
	pixels.at(Uint(id))[currentPointer.at(Uint(id))] = points[1]; //1//0
	basicBuffer.at(Uint(id))[currentPointer.at(Uint(id))] = color;
	currentPointer.at(Uint(id))++;
	pixels.at(Uint(id))[currentPointer.at(Uint(id))] = points[0]; //0//0
	basicBuffer.at(Uint(id))[currentPointer.at(Uint(id))] = color;
	currentPointer.at(Uint(id))++;

	delete[] points;
}

void Renderer::linkSprite(Sprite * sprite)
{
	ULong index = sprite -> getTextureValue();
	FV3 testColor;
	if(sprite -> getType() == ActorToken) testColor = FV3(0, 0, 1);
	else if(sprite->getType() == PlayerToken) testColor = FV3(0, 1, 0);
	for(int i = 0; i < 6; i++) basicBuffer.at(Uint(index))[currentPointer.at(Uint(index)) + i] = testColor;
	sprite->setLocationPointer(&pixels.at(Uint(index))[currentPointer.at(Uint(index))]);
	currentPointer.at(Uint(index)) += 6;
}

void Renderer::setNewMax(ULong nodes, ULong id)
{
	maxNodes.at(Uint(id)) = nodes*6;
	currentPointer.at(Uint(id)) = 0;
	basicBuffer.at(Uint(id)) = new FV3[Uint(maxNodes.at(id))];
	pixels.at(Uint(id)) = new IV2[Uint(maxNodes.at(id))];
	textureCoords.at(Uint(id)) = new FV2[Uint(maxNodes.at(id))];
}

void Renderer::render(void)
{
	if(!verifyLists())
	{
#ifdef OS_WINDOWS
		SetLastError(0xAAAA0004);
	#ifdef _DEBUG
		std::cout << "List lengths do not match for rendering" << endl;
	#endif
		return;
#endif
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	for(Uint i = imageFiles.size(); i--;)
	{
		imageFiles.at(i).enableSetUp();
		glVertexPointer(2, GL_INT, 0, pixels.at(i));
		glTexCoordPointer(2, GL_DOUBLE, 0, textureCoords.at(i));
		glColorPointer(3, GL_DOUBLE, 0, basicBuffer.at(i));
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(currentPointer.at(i)));
		imageFiles.at(i).disableSetUp();
	}
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
#ifdef _DEBUG
	cout << "Beginning the array population" << endl << "setting maximums..." << endl;
#endif
	/*
		This is where the main loop will go,
		it will scan through each image find
		the relevant ids and populate the arrays
		and the texture coordinates
	*/
	ULong * maximums = new ULong[imageFiles.size()];
	for(Uint i = 0; i < imageFiles.size(); i++)
		maximums[i] = 0;
	//Add the tile map to the count
	maximums[world->getTextureValue()] += world -> getSize().x * world -> getSize().y;
	//Add the player to the count
	if(world->verifyPlayer()) maximums[world->getPlayerTexture()]++;
	//Next loop through all of the actors
	for(ULong i = 0; i < world->getActorNumber(); i++)
			maximums[world->getActorTexture(Uint(i))]++;
	for(Uint i = 0; i < imageFiles.size(); i++)
			setNewMax(maximums[i], i);
#ifdef _DEBUG
	cout << "maximums set" << endl;
#endif
	Pos2D mapDims = world -> getDimensions();
	Pos2D tileSize = world -> getDefaultTileSize();

#ifdef _VERBOSE
	cout << "Tile dimensions: " << tileSize.x << " " << tileSize.y << endl;
#endif
	
#ifdef _DEBUG
	cout << "building tile lists" << endl;
#endif
	
	/*
	All tiles are passive, and there is a lot more
	address in the renderer's texture coordinates
	array than there is in the tile data, so we
	first update the tile texture coordinates
	and then assign the corrected data to the
	renderer afterwards
	*/
	world->updateTileTexture(this);

	for(ULong i = 0; i < mapDims.y; i++)
	{
		for(ULong j = 0; j < mapDims.x; j++)
		{
			Pos2D currentLocation(i,j);
			bool isPassable = world->isPassable(currentLocation);
#ifdef _VERBOSE
			cout << "Address for the current tile: " << &world->getTileTextureCoords(currentLocation)[0] << endl;
#endif
			FV2 * temp = world->getTileTextureCoords(currentLocation);
			linkTextureCoords(temp,
							  world->getTextureValue(), 
							  world->getTileTexture(currentLocation));
#ifdef _VERBOSE
			cout << endl << "New address for current tile after linkage: " << &world->getTileTextureCoords(currentLocation)[0] << endl;
#endif
			addTileToDraw(Pos2D(i*tileSize.y, j*tileSize.x), tileSize, isPassable, world->getTextureValue());
		}
	}

	if(world->verifyPlayer()) linkSprite(world->getPlayer());
	for(Uint i = 0; i < world -> getActorNumber(); i++)
		linkSprite(world -> getActor(i));

}

/*
	THIS NEEDS TO BE MODIFIED SO THAT 
	ONLY ONE TILE IS DRAWN AT A TIME
*/
void Renderer::linkTextureCoords(FV2 *linkage, ULong imageId, Uint tileNumber)
{
#ifdef _VERBOSE
	cout << endl << "RUNNING FUNCTION linkTextureCoords" << endl;
	cout << "linkage address: " << &linkage[0] << endl;
	cout << "current textureCoords address: " << &textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId))];
	cout << endl << "Linking texture for image with ID: " << imageId << endl;
	cout << "With current pointer location: " <<
	textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId))].x << " " <<
	textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId))].y << endl <<
	" with address: " << &textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId))] << 
	" and current pointer at " << currentPointer.at(Uint(imageId)) << endl;
#endif
	if(textureCoords.empty()) return;
	//linkage = &textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId))];
	/*
	texture pattern
	0,0
	0,1
	1,1

	1,1
	1,0
	0,0

	also positive y is at the bottom, make a note of this
	*/
	textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId))] = linkage[0];	
	textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId)) + 1] = linkage[1];
	textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId)) + 2] = linkage[2];
	
	textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId)) + 3] = linkage[3];
	textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId)) + 4] = linkage[4];
	textureCoords.at(Uint(imageId))[currentPointer.at(Uint(imageId)) + 5] = linkage[5];
#ifdef _VERBOSE
	cout << "The new address of linkage: " << &linkage[0] << endl << endl;
#endif
}

Pos2D Renderer::getImageSize(Uint index) const
{
	/*First error check, if the image does not exist
	then return a division size of 0, and report in
	verbose mode and debug mode that the image does
	not exist*/
	if(imageFiles.size() <= index)
	{
#ifdef _DEBUG
		cout << "Image with ID: " << index << " does not currently exist in the system" << endl;
#endif
		return Pos2D(0, 0);
	}
	//else return the division size of the image at
	//that location
	return imageFiles.at(index).getSize();
}

Pos2D Renderer::getImageDivisions(Uint index) const
{
	/*First error check, if the image does not exist
	then return a division size of 0, and report in
	verbose mode and debug mode that the image does
	not exist*/
	if(imageFiles.size() <= index)
	{
#ifdef _DEBUG
		cout << "Image with ID: " << index << " does not currently exist in the system" << endl;
#endif
		return Pos2D(0, 0);
	}
	//else return the division size of the image at
	//that location
	return imageFiles.at(index).getDivisionSize();
}

void Renderer::initWindow(int argc, char *argv[]) 
{
	glutInitDisplayMode(GL_DEPTH | GL_DOUBLE | GL_RGBA);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	gluOrtho2D(0, 900, 0, 900);
	glViewport(0, 0, 900, 900);
}
void Renderer::addImage(Image image)
{
	image.changeId(imageFiles.size());
	//Now push back all of the 
	//information needed for the lists
	imageFiles.push_back(image);
	currentPointer.push_back(0);
	textureCoords.push_back(new FV2());
	textureCoords.at(textureCoords.size() - 1) = new FV2[6];
	FV2 * temp = &textureCoords.at(textureCoords.size() - 1)[0];
	temp[0] = FV2(0, 0), temp[1] = FV2(0, 1), temp[2] = FV2(1, 1);
	temp[3] = FV2(1, 1), temp[4] = FV2(1, 0), temp[5] = FV2(0, 0);
	maxNodes.push_back(0);
	basicBuffer.push_back(new FV3());
	pixels.push_back(new IV2());
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
	std::cout << endl << "Loading image: " << imageName << endl;
#endif
	for(Uint i = 0; i < imageFiles.size(); i++)
	{
		if(imageName == imageFiles.at(i).getName())
		{
#ifdef OS_WINDOWS
			SetLastError(0xAAAA0002);
	#ifdef _DEBUG
				std::cout << "Image: " << imageName << " already in system with ID: " << imageFiles.at(i).getId() << endl;
	#endif
#endif	
			*linkId = i;
			linkTextureId(linkId);
			return true;
		}
	}
#ifdef _VERBOSE
	cout << "loading image: " << imageName <<endl;
#endif
	currentImage = new Image(imageName, desc, chunkSize);
#ifdef _VERBOSE
	cout << "image: " << imageName << " loaded" << endl;
#endif
	if(!currentImage -> isAvailable())
	{
#ifdef OS_WINDOWS
		SetLastError(0xAAAA0003);
	#ifdef _DEBUG
			std::cout << "Image: " << imageName << " does not exist" << endl;
	#endif
#endif
		return false;
	}
#ifdef _VERBOSE
	cout << "adding image: " << imageName << " to image library" << endl;
#endif
	addImage(*currentImage);
	*linkId = imageFiles.size() - 1;
	linkTextureId(linkId);
#ifdef _VERBOSE
	cout << "image: " << imageName << " linked to reference: " << &linkId << endl;
#endif
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
			std::cout << "Id matches: " << i << endl;
#endif
			baseId = &textureIds.at(i)[0];
			return;
		}
	}
	textureIds.push_back(new ULong());
	*textureIds.at(textureIds.size() - 1) = *baseId;
	baseId = &textureIds.at(textureIds.size() - 1)[0];
#ifdef _DEBUG
	std::cout << "Base id for the loaded texture is now: " << *baseId << endl;
#endif
}

bool Renderer::verifyLists(void)
{
	bool check;
	(textureCoords.size() == textureIds.size() &&
	 basicBuffer.size() == textureIds.size() && 
	 pixels.size() == textureIds.size() &&
	 maxNodes.size() == textureIds.size() &&
	 imageFiles.size() == textureIds.size())
		? check = true : check = false;
#ifdef _DEBUG
	if(!check)
	{
		cout << "One or more list is missing a value, the read out is as follows: " << endl;
		cout << "TextureCoords size: " << textureCoords.size() << endl;
		cout << "TextureIds size: " << textureIds.size() << endl;
		cout << "Basic buffer size: " << basicBuffer.size() << endl;
		cout << "Pixels size: " << pixels.size() << endl;
		cout << "maxNodes size: " << maxNodes.size() << endl;
		cout << "ImageFiles size: " << imageFiles.size() << endl;
		cout << endl;
	}
#endif
	return check;
}
#endif 
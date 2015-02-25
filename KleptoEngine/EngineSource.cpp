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
#ifndef MAIN_CONSOLE
#define MAIN_CONSOLE
 
#include "engineStdafx.h"
#include "GameHeaders.h"

void idle(void);

Renderer testRender = Renderer();
World2D testWorld;
bool keys[4];

void keyStateMachine(void)
{
	if(keys[0]) testWorld.changePlayerDirection(0);
	else if(keys[1]) testWorld.changePlayerDirection(180);
	else if(keys[2]) testWorld.changePlayerDirection(270);
	else if(keys[3]) testWorld.changePlayerDirection(90);
	if(keys[0] || keys[1] || keys[2] || keys[3])
	{
		testWorld.changePlayerSpeed(1);
		testWorld.makePlayerDirty();
	}
}

void keyboardDown(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'W':
		case 'w':
			keys[0] = true;
			break;
		case 'S':
		case 's':
			keys[1] = true;
			break;
		case 'A':
		case 'a':
			keys[2] = true;
			break;
		case 'D':
		case 'd':
			keys[3] = true;
			break;
		case 'f':
		case 'F':
			testRender.setToFullScreen();
			break;
		case 'g':
		case 'G':
			testRender.setToWindowed();
			break;
		case 'm':
		case 'M':
			testRender.setToMinimized();
			break;
		default:;
	}
}

void entryFunc(int a)
{
	testRender.setToWindowed();
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'W':
		case 'w':
			keys[0] = false;
			break;
		case 'S':
		case 's':
			keys[1] = false;
			break;
		case 'A':
		case 'a':
			keys[2] = false;
			break;
		case 'D':
		case 'd':
			keys[3] = false;
			break;
		default:;
	}
}

void idle(void) 
{
	if(testRender.getWindowState() == Minimized) return;
	testWorld.updateWorld();
	keyStateMachine();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	testRender.render();

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

void reshape(int x, int y) 
{ 
	if(testRender.getWindowState() != FullScreen) 
		testRender.rebuildWindow(); 
}

int main(int argc, char* argv[])
{
#ifdef _VERBOSE
	cout << "Verbose mode" << endl << endl;
#elif defined(_DEBUG)
	cout << "Debug mode" << endl << endl; 
#endif
	testWorld.setDimensions(Pos2D(30, 30));
	testWorld.setTileSize(Pos2D(64, 64));
	
#ifdef _DEBUG
	cout << "generating tile lists" << endl;
#endif

	testWorld.addTileToSet("testTile0", "Just a test", 0, false);
	testWorld.addTileToSet("testTile1", "Just a test", 12, true);
	testWorld.addTileToSet("testTile2", "Just a test", 13, false);
	testWorld.addTileToSet("testTile3", "Just a test", 33, true);
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			testWorld.setTile(1, Pos2D(1 + i, 1 + j));
	
	testWorld.setTile(2, Pos2D(4, 4));
	testWorld.setTile(2, Pos2D(7, 4));
	testWorld.setTile(2, Pos2D(4, 7));
	testWorld.setTile(3, Pos2D(7, 7));

#ifdef _DEBUG
	cout << endl << endl << "Reading tile map" << endl;
#endif
	testWorld.setTileMap("Assets\\Tile_set_1.png", Pos2D(8,8), &testRender);
#ifdef _DEBUG
	cout << "tile map loaded successfully" << endl << endl;
#endif

	testWorld.addSprite("Player 0", "The current Player", Pos2D(96, 96), 
						FV2(190, 190), 10, "Assets\\test_subject_2.png", 
						&testRender, Pos2D(8, 4), true, PlayerToken);
	testWorld.addSprite("Actor 0", "The first actor", Pos2D(64, 64), 
						 FV2(96, 70), 10, "Assets\\test_subject_3.png", 
						 &testRender, Pos2D(8, 4), false, ActorToken);
	testWorld.addSprite("Actor 1", "The second actor", Pos2D(64, 64), 
					     FV2(96, 90), 10, "Assets\\Charactersforreal.png", 
						 &testRender, Pos2D(8, 4), false, ActorToken);
	testWorld.addSprite("Actor 2", "The third actor", Pos2D(64, 64), 
						 FV2(96, 150), 10, "Assets\\Charactersforreal.png", 
						 &testRender, Pos2D(8, 4), true, ActorToken);

#ifdef _DEBUG
	cout << "Finished image load in" << endl;
#endif

#ifdef _VERBOSE
	testWorld.printToConsole();
#endif
	testRender.populateArrays(&testWorld);
#ifdef _DEBUG
	cout << "arrays populated" << endl;
#endif
	testWorld.changePlayerDirection(60);
	testWorld.updateWorld();

#ifdef _DEBUG
	cout << "texture value of the world tileset: " << testWorld.getTextureValue() << endl;
#endif
	
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutCreateWindow("RenderTest");
	testRender.initWindow(argc, argv);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutReshapeFunc(reshape);
	glutEntryFunc(entryFunc);
	glutMainLoop();

	return 0;
}

#endif
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
 
#include "stdafx.h"
#include "GameHeaders.h"

Renderer testRender = Renderer();
World2D testWorld;
bool keys[4];
void keyStateMachine(void)
{
	if(keys[0]) testWorld.changePlayerDirection(0);
	else if(keys[1]) testWorld.changePlayerDirection(180);
	else if(keys[2]) testWorld.changePlayerDirection(270);
	else if(keys[3]) testWorld.changePlayerDirection(90);
	if(keys[0] || keys[1] || keys[2] || keys[3]) testWorld.changePlayerSpeed(1);
	else testWorld.changePlayerSpeed(0);
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
		default:;
	}
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
	testWorld.updateWorld();
	testRender.render();
	keyStateMachine();
}

void display(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glViewport(0, 0, 900, 900);
	gluOrtho2D(0, 900, 0, 900);
	testRender.render();
	glFlush();
	glutPostRedisplay();
	
}

int main(int argc, char* argv[])
{
#ifdef _VERBOSE
	cout << "Verbose mode" << endl << endl;
#elif defined(_DEBUG)
	cout << "Debug mode" << endl << endl; 
#endif
	testWorld.setDimensions(Pos2D(30, 30));
	testWorld.setTileSize(Pos2D(32, 32));
	testWorld.addTile("testTile0", "Just a test", false);
	testWorld.addTile("testTile1", "Just a test", true);
	testWorld.addTile("testTile2", "Just a test", true);
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			testWorld.setTile(1, Pos2D(1 + i, 1 + j));
	testWorld.setTile(0, Pos2D(4, 4));
	testWorld.setTile(0, Pos2D(7, 4));
	testWorld.setTile(0, Pos2D(4, 7));
	testWorld.setTile(0, Pos2D(7, 7));

	testWorld.setTileMap("Assets\\Tile_set_1.png", Pos2D(8,8), &testRender);
	testWorld.addSprite("Player 0", "The current Player", Pos2D(45, 45), FV2(82, 82), 10, "Assets\\Charactersforreal.png", &testRender, Pos2D(7, 4), PlayerToken);
	testWorld.addSprite("Actor 0", "The first actor", Pos2D(30, 30), FV2(30, 70), 10, "Assets\\test_subject_2.png", &testRender, Pos2D(7, 4), ActorToken);
	testWorld.addSprite("Actor 1", "The second actor", Pos2D(30, 30), FV2(30, 140), 10, "Assets\\Charactersforreal.png", &testRender, Pos2D(7, 4), ActorToken);
	testWorld.addSprite("Actor 2", "The third actor", Pos2D(30, 30), FV2(50, 250), 10, "Assets\\Charactersforreal.png", &testRender, Pos2D(7, 4), ActorToken);

#ifdef _VERBOSE
	testWorld.printToConsole();
#endif

	testRender.populateArrays(&testWorld);
	testWorld.changePlayerDirection(60);
	testWorld.updateWorld();
	testWorld.testRayTrace(Pos2D(9, 0), Pos2D(0, 0));
	testWorld.testRayTrace(Pos2D(0, 0), Pos2D(9, 0));
	testWorld.testRayTrace(Pos2D(0, 9), Pos2D(0, 2));
	testWorld.testRayTrace(Pos2D(0, 0), Pos2D(0, 9));
	testWorld.testRayTrace(Pos2D(0, 0), Pos2D(9, 9));
	testWorld.testRayTrace(Pos2D(9, 9), Pos2D(0, 0));
	testWorld.testRayTrace(Pos2D(0, 9), Pos2D(9, 0));
	testWorld.testRayTrace(Pos2D(9, 0), Pos2D(0, 9));
#ifdef _DEBUG
	cout << "texture value of the world tileset: " << testWorld.getTextureValue() << endl;
#endif
	glutInit(&argc, argv);
	glutInitWindowSize(900, 900);
	glutCreateWindow("RenderTest");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutMainLoop();
	
	return 0;
}

#endif
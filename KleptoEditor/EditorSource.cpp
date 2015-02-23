/*
COPYRIGHT BENJAMIN ISHERWOOD 15/02/2015
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
#ifndef KLEPTO_EDITOR_MAIN
#define KLEPTO_EDITOR_MAIN

#include "editorStdafx.h"
#include "Window.h"

Window * mainWindow;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0, 512, 0, 512);
	glViewport(0, 0, 512, 512);

	glPointSize(mainWindow -> getPanel(0) -> getDimensions().x);
	glColor3dv((GLdouble*)&mainWindow -> getPanel(0) -> getBgColor());

	glBegin(GL_POINTS);
	
	glVertex2i(mainWindow->getPanel(0)->getPosition().x, 
		mainWindow->getPanel(0)->getPosition().y);

	glEnd();

	glPointSize(mainWindow->getPanel(1)->getDimensions().x);
	glColor3dv((GLdouble*)&mainWindow->getPanel(1)->getBgColor());

	glBegin(GL_POINTS);

	glVertex2i(mainWindow->getPanel(1)->getPosition().x,
		mainWindow->getPanel(1)->getPosition().y);

	glEnd();

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

void main(int argc, char *argv[])
{
#ifdef _VERBOSE 
	cout << "Running in VERBOSE mode" << endl;
#elif defined(_DEBUG)
	cout << "Running in DEBUG mode" << endl;
#endif
	mainWindow = new Window();
	Panel * testPanel = new Panel(Pos2D(100, 100), Pos2D(100, 100), 1);
	testPanel -> changePanelPriority(activePriority);
	testPanel -> setBgColor(FV3(0.75, 0.5, 0.5));
	mainWindow->addPanel(testPanel);
	mainWindow->addPanel(new Panel(Pos2D(100, 100), Pos2D(200, 100), 1));
	mainWindow->getPanel(1)->setBgColor(FV3(1, 0, 0));
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Klepto Editor");
	
	glutDisplayFunc(display);

	glutMainLoop();
}

#endif
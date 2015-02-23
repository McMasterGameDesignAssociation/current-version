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
#ifndef WINDOW_M
#define WINDOW_M

#include "Window.h"

Window::Window() :
windowName("uninitiated window"),
windowDescription("This is an empty window"), windowID(0) {}

Window::Window(string name, string desc) :
windowName(name), windowDescription(desc),
windowID(0) {}

void Window::addPanel(Panel * panel) 
{ 
	//First get the current number of panels, this will
	//act as the ID for the new panel that you are going
	//to add to the list of panels
	Uint currentSize = windowPanels.size();

	//If the array of drawData hasn't been set
	//yet then build the array
	if(windowPanels.size() == 0) panelCorners = new IV2[6];
	//else resize the current array
	else panelCorners = (IV2*)realloc(panelCorners, sizeof(IV2)*((currentSize + 1) * 6));
	//set the id so it can be tracked later
	panel->setId(currentSize);
	windowPanels.push_back(panel); 
	//then link the new panel to the draw array
	windowPanels.at(currentSize) -> linkPanelCoords(&panelCorners[(currentSize + 1)*6]);
}

void Window::addBlankPanel(void) 
{ 
	//First get the current number of panels, this will
	//act as the ID for the new panel that you are going
	//to add to the list of panels
	Uint currentSize = windowPanels.size();

	//If the array of drawData hasn't been set
	//yet then build the array
	if(windowPanels.size() == 0) panelCorners = new IV2[6];
	//else resize the current array
	else panelCorners = (IV2*)realloc(panelCorners, sizeof(IV2)*((currentSize + 1) * 6));
	windowPanels.push_back(new Panel(windowPanels.size()));
	//set the id so it can be tracked later
	windowPanels.at(currentSize) -> 
		setId(currentSize);
	//then link the new panel to the draw array
	windowPanels.at(currentSize) -> 
		linkPanelCoords(&panelCorners[(currentSize + 1) * 6]);
}

void Window::setWindowName(string name) { windowName = name; }
void Window::setWindowDescription(string desc) { windowDescription = desc; }
void Window::setWindowId(Uint id) { windowID = id; }

Panel * Window::getPanel(Uint id) { return windowPanels.at(id); }
Uint Window::getWindowID(void) const { return windowID; }
string Window::getWindowName(void) const { return windowName; }
string Window::getWindowDescription(void) const { return windowDescription; }

#endif
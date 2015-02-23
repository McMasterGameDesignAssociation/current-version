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
#ifndef WINDOW_H
#define WINDOW_H

#include "editorStdafx.h"
#include "Panel.h"

class Window
{
	private:
		Uint windowID;
		vector<Panel*> windowPanels;
		string windowName;
		string windowDescription;

		//WindowCorners is the draw array for
		//the panels contained in the current
		//window
		IV2 * panelCorners;

	public:
		Window();
		Window(string name, string desc);

		void addPanel(Panel * panel);
		void addBlankPanel(void);

		void setWindowName(string name);
		void setWindowDescription(string desc);
		void setWindowId(Uint id);

		Panel * getPanel(Uint panelId);
		
		Uint getWindowID(void) const;
		string getWindowName(void) const;
		string getWindowDescription(void) const;

		//updateRenderCoords keeps track of which
		//window is in which order, i.e. moves
		//the windows in the render pipeline
		//so that the windows render in the
		//correct z-index
		void updateRenderCoords(void);

		//populateArrays is the preprocess function
		//that is responsible for the initial 
		//generation of the draw array
		void populateArrays(void);
};

#endif
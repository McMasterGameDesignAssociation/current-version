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
#ifndef PANEL_H
#define PANEL_H

#include "editorStdafx.h"

class Panel
{
	private:
		//panelDimensions is the width and
		//height of the panel
		Pos2D panelDimensions;

		//panelCenter is the x y window position
		//of a window, this will be used to manage
		//where windows are
		Pos2D panelCenter;

		//zIndex is a relative number that ranks
		//each panel, panels with a higher z index
		//appear over top of windows with a lower z index
		Uint zIndex = 0;

		//Window priority is used to identify if a window
		//is active or background, any system can have at most
		//one active panel.
		PanelPriority currentPriority;

		//Every panel needs an id to manage 
		//which panels belong to which windows
		Uint panelId = 0;

		//window color is the non-texture color
		FV3 windowColor;

		//textureId is linked directly to the
		//render pipeline, and identifies 
		//which texture file belongs to the
		//panel that is being rendered
		Uint * textureId;

		//panel description is used as a
		//brief description of the use of
		//the current panel
		string description;

		//panel name is used as a single
		//word regex identifier
		string name;

	public:
		Panel();
};

#endif
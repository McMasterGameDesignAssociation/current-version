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
#ifndef PANEL_M
#define PANEL_M

#include "Panel.h"

Panel::Panel() :
currentPriority(backgroundPriority), zIndex(0),
description("Empty Panel"), name("Panel 0"),
panelDimensions(Pos2D(10, 10)), panelCenter(Pos2D(10, 10)),
panelId(0), panelColor(FV3(0, 0, 0))
{}

Panel::Panel(Uint id) :
currentPriority(backgroundPriority), zIndex(0),
description("Empty Panel"), name("Panel 0"),
panelDimensions(Pos2D(10, 10)), panelCenter(Pos2D(10, 10)),
panelId(id), panelColor(FV3(0, 0, 0))
{}

Panel::Panel(Pos2D dims, Pos2D center, Uint zDex) :
currentPriority(backgroundPriority), zIndex(zDex),
description("Empty Panel"), name("Panel 0"),
panelDimensions(dims), panelCenter(center),
panelId(0), panelColor(FV3(0, 0, 0))
{}

Panel::Panel(Pos2D dims, Pos2D center, Uint zDex, Uint ID,
	string desc, string setName) :
	currentPriority(backgroundPriority), panelId(ID),
	description(desc), name(setName), panelDimensions(dims),
	panelCenter(center), zIndex(zDex), panelColor(FV3(0, 0, 0))
{}

void Panel::setBgColor(FV3 color) { panelColor = color; }
void Panel::setPosition(Pos2D center) { panelCenter = center; }
void Panel::setDimensions(Pos2D dims) { panelDimensions = dims; }
void Panel::setZIndex(Uint zDex) { zIndex = zDex; };
void Panel::changePanelPriority(PanelPriority updatePriority)
{
	currentPriority = updatePriority;
}

void Panel::setDescription(string desc) { description = desc; }
void Panel::setName(string setName) { name = setName; }
void Panel::setId(Uint id) { panelId = id; }
void Panel::linkPanelCoords(IV2 * linkage) { panelCorners = &linkage[0]; }

void Panel::setUpSquare(void) 
{
	/*
	[3] [2]
	[0] [1]

	[1] [2]
	[0]

	[5]
	[3] [4]

	[3]
	[5] [4]
	*/

	panelCorners[0].x = panelCorners[5].x = panelCorners[1].x = int(panelCenter.x - panelDimensions.x);
	panelCorners[0].y = panelCorners[5].y = panelCorners[4].y = int(panelCenter.y - panelDimensions.y);
	panelCorners[2].x = panelCorners[4].x = panelCorners[3].x = int(panelCenter.x + panelDimensions.x);
	panelCorners[1].y = panelCorners[2].y = panelCorners[3].y = int(panelCenter.y + panelDimensions.y);
}

FV3 Panel::getBgColor(void) const { return panelColor; }
Pos2D Panel::getPosition(void) const { return panelCenter; }
Pos2D Panel::getDimensions(void) const { return panelDimensions; }
Uint Panel::getZIndex(void) const { return zIndex; }
PanelPriority Panel::getPriority(void) const { return currentPriority; }
string Panel::getDescription(void) const { return description; }
string Panel::getName(void) const { return name; }

#endif
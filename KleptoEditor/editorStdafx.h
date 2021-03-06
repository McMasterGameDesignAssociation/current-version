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
#ifndef EDITOR_STADFX
#define EDITOR_STADFX

#include <iostream>
#include <string>
#include <vector>

#include "VectorTypes.h"
#include "freeglut.h"
#include "IEEECodex.h"

class Window;

enum PanelPriority 
{ 
	//background priority means that
	//the panel is currently not being
	//used by the user
	backgroundPriority,
	//active priority means that the
	//panel is currently being used
	//by the user
	activePriority 
};

using namespace std;

#endif
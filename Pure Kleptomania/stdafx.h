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
OTHER DEALINGS IN THE SOFTWARsE.
*/
#ifndef STDAFX_H
#define STDAFX_H

//Important system libraries
#include <iostream>
#include <ctime>
#include <queue>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <signal.h>

//OS selection and verification
#if (defined(_WIN32) || defined(_WIN64))
	#define OS_WINDOWS
#elif (defined(__unix))
	#define OS_UNIX
#endif

//OS dependant preprocessor macros
#ifdef OS_WINDOWS
	#include <Windows.h>
#endif

#ifdef OS_UNIX
	#include <unistd.h>
#endif

#include "VectorTypes.h"
#include "freeglut.h"
#include "png.h"

#ifndef PI
	#define PI	atan(1)*4
#endif
#ifndef PI2
	#define PI2	atan(1)*2
#endif
#ifndef PI4
	#define PI4	atan(1)
#endif
#ifndef TO_RAD
	#define TO_RAD(x) (x*PI)/180
#endif

class World2D;
class Tile;
class Player;
class Actor;
class Item;
class Inventory;
class Renderer;
class Storage;

typedef void(*AI)(void);
typedef void(*subRoutine)(void* args);

struct atomicChoice
{
	subRoutine routine;
	void * args;
};

enum ItemType { Clothing, Electronics, Jewelry, SportsEquipment, Novelty };
enum SpriteType { PlayerToken, ActorToken };
using namespace std;

#endif
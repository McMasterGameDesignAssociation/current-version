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
#ifndef ACTOR_H
#define ACTOR_H

#include "stdafx.h"
#include "Sprite.h"
#include "World2D.h"

class Actor : public Sprite
{
	private:
		queue<atomicChoice> moves; 
		AI actorAI;
		string routineName;

	protected:
		friend World2D;
		friend Renderer;
		Actor();
		Actor(AI startAI);
		Actor(Uint id, string name, string desc, FV2 initPos, Pos2D size, Uint topSpeed, AI startAI);
		Actor(Uint id, string name, string desc, FV2 initPos, Pos2D size, Uint topSpeed);
		Actor(Uint id, string name, string desc, FV2 initPos, Pos2D size, Uint topSpeed, bool isAnimated);

		void queueMove(atomicChoice nextMove);
		void changeBehavior(string routineName, AI ai);

		void * newDirection(void * args);
		void * setSpeed(void * args);

};

#endif
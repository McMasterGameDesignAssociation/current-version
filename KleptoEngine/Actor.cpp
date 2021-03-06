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
#ifndef ACTOR_M
#define ACTOR_M

#include "Actor.h"

Actor::Actor(void) : Sprite(ActorToken) {}

Actor::Actor(Uint id, string name, string desc, FV2 initPos, Pos2D size, Uint topSpeed, AI startAI) :
Sprite(id, name, desc, size, initPos, topSpeed, ActorToken), actorAI(startAI) {}

Actor::Actor(Uint id, string name, string desc, FV2 initPos, Pos2D size, Uint topSpeed) :
Sprite(id, name, desc, size, initPos, topSpeed, ActorToken) {}

Actor::Actor(Uint id, string name, string desc, FV2 initPos, Pos2D size, Uint topSpeed, bool isAnimated) :
Sprite(id, name, desc, size, initPos, topSpeed, isAnimated, ActorToken) {}


#endif
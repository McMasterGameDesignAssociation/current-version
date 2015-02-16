/*
COPYRIGHT BENJAMIN ISHERWOOD 11/02/2015
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
#ifndef ANIMATION_SEQUENCE_M
#define ANIMATION_SEQUENCE_M

#include "AnimationSequence.h"

AnimationSequence::AnimationSequence() : BasicEntity() {}

AnimationSequence::AnimationSequence(string desc, string name, ULong objectID) : 
BasicEntity(objectID, name, desc) {}

Uint AnimationSequence::getNumberOfFrames(void) const { return frames.size(); }

void AnimationSequence::addFrame(SquareFV nextFrame) {frames.push_back(nextFrame);}
void AnimationSequence::addFrame(FV2 * nextFrame)
{
	if(&nextFrame[0] == nullptr) return;
}

#endif
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
#ifndef ANIMATION_SEQUENCE_H
#define ANIMATION_SEQUENCE_H

#include "engineStdafx.h"
#include "BaseEntity.h"

class AnimationSequence : public BasicEntity
{
	private:
		vector<SquareFV> frames;

	protected:
		friend AnimationSequence;
		AnimationSequence();
		AnimationSequence(string desc, string name, ULong objectID);


		void addFrame(SquareFV nextFrame);
		void addFrame(FV2 * nextFrame);
		void resetSequence(void);
		Uint getNumberOfFrames(void) const;
};

#endif
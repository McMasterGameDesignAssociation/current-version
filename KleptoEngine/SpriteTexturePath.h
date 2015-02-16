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
#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include "engineStdafx.h"
#include "AnimationSequence.h"


/*
	AnimationPath is used as a way to
	generate custom animations for the
	texture maps that are used to texture
	the sprites and objects in the world
*/
class AnimationPath : public BasicEntity
{
	private:
		/*
			animationNumber describes how many
			separate animations are contained in
			one texture map, i.e. forward, left, right,
			down, etc.
		*/
		Uint animationNumber;

		/*
			AnimationPaths stores all of the available
			animation sequences that are available to
			be used with a specific game object
		*/
		vector<AnimationSequence> animationPaths;

		/*
			factorySequence is sequence factory
			used to manage the generation of the
			animation sequences
		*/
		static AnimationSequence * factorySequence;
	public:
		AnimationPath();

		/*
			generate sequence builds an empty sequence
			that the user can use to populate. The 
			ID of the sequence that is generated is
			returned, and 0xFFFFFFFF is returned in
			case of an error. This is a factory function
		*/
		Uint generateNewSequence();

		/*
			addNewSequence adds a newSequence to the
			animationPaths and returns the index value
			of the new sequence
		*/
		Uint addNewSequence(AnimationSequence nextSequence);

		/*
			addFrame adds a singular animation frame to
			a specific sequence
		*/
		void addFrame(SquareFV framePosition, Uint sequenceID);
};

#endif
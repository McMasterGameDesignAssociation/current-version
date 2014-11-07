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
#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include "stdafx.h"

class BasicEntity
{
	private:
		ULong	objectId;
		string	objectName;
		string	description;
		Pos2D	entitySize;
	
	protected:
		BasicEntity(void);
		BasicEntity(ULong id, string name, string desc);
		BasicEntity(ULong id, string name, string desc, Pos2D size);
		BasicEntity(string name, string desc);

		void changeId(ULong id);
		void changeName(string name);
		void changeDesc(string desc);
		void changeObjectSize(Pos2D size);

		ULong getId(void)		const;
		string getName(void)	const;
		string getDesc(void)	const;
		Pos2D getSize(void)		const;
};

#endif
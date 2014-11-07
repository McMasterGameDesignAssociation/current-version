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
#ifndef BASE_ENTITY_M
#define BASE_ENTITY_M

#include "BaseEntity.h"

BasicEntity::BasicEntity(void)
{
	objectId = 0;
	objectName = description = "Not Initiated";
}

BasicEntity::BasicEntity(ULong id, string name, string desc)
{
	objectId = id;
	objectName = name;
	description = desc;
}

BasicEntity::BasicEntity(ULong id, string name, string desc, Pos2D size)
{
	objectId = id;
	objectName = name;
	description = desc;
	entitySize = size;
}

BasicEntity::BasicEntity(string name, string desc)
{
#ifdef _DEBUG
	cout << "Constructing entity with name " << name << endl;
#endif
	objectName = name;
	description = desc;
}

ULong	BasicEntity::getId(void)		const { return objectId; }
string	BasicEntity::getName(void)		const { return objectName; }
string	BasicEntity::getDesc(void)		const { return description; }
Pos2D	BasicEntity::getSize(void)		const { return entitySize; }

void BasicEntity::changeId(ULong id) { objectId = id; }
void BasicEntity::changeName(string name) 
{ 
#ifdef _DEBUG
	cout << "setting entity name to: " << name << endl;
#endif
	objectName = name;
}
void BasicEntity::changeDesc(string desc) { description = desc; }
void BasicEntity::changeObjectSize(Pos2D size) { entitySize = size; }

#endif
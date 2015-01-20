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
#ifndef ITEM_H
#define ITEM_H

#include "stdafx.h"
#include "GameHeaders.h"

class Item : public BasicObject
{
	private:
		double value;
		Pos2D location;
		ItemType classification;
	protected:
		friend Inventory;
		
		Item(void);
		Item(double itemValue, Pos2D itemSize, ItemType itemClass);
		
		void changeValue(double itemValue);
		void changeType(ItemType type);
		void changeSize(Pos2D itemSize);
		void changeLocation(Pos2D itemLocation);

		double getValue(void) const;
		Pos2D getSize(void) const;
		Pos2D getLocation(void) const;
		ItemType getClass(void) const;
};

#endif
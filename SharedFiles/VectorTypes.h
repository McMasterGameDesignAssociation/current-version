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
#ifndef VECTOR_TYPES
#define VECTOR_TYPES

//Type defintions of the types we need
typedef unsigned short Ushort;
typedef unsigned int Uint;
typedef unsigned long long ULong;
typedef long long ILong;
struct Pos2D;
//These structs have a longer descriptive name for readability
struct IntVector3
{
	int x, y, z;
	IntVector3(void) { x = y = z = 0; }
	IntVector3(int X, int Y, int Z) { x = X, y = Y, z = Z; }
};

struct DoubleVector3
{
	double x, y, z;
	DoubleVector3(void) { x = y = z = 0; }
	DoubleVector3(double X, double Y, double Z) { x = X, y = Y, z = Z; }
};

struct IntVector4
{
	int x, y, z, w;
	IntVector4(void) { x = y = z = w = 0; }
	IntVector4(int X, int Y, int Z, int W) { x = X, y = Y, z = Z, w = W; }
};

struct DoubleVector4
{
	double x, y, z, w;
	DoubleVector4(void) { x = y = z = w = 0; }
	DoubleVector4(double X, double Y, double Z, double W)
	{
		x = X, y = Y, z = Z, w = W;
	}
};

struct IntVector2
{
	int x, y;
	IntVector2(int X, int Y) { x = X, y = Y; }
	IntVector2(void) { x = y = 0; }
	IntVector2 &operator+(IntVector2 value)
	{
		this->x += value.x;
		this->y += value.y;
		return *this;
	}
	IntVector2 &operator*(int scalar)
	{
		this->x *= scalar, this->y *= scalar;
		return *this;
	}
	IntVector2 &operator/(int scalar)
	{
		this->x /= scalar, this->y /= scalar;
		return *this;
	}
	IntVector2 &operator*(ULong scalar)
	{
		this->x *= int(scalar), this->y *= int(scalar);
		return *this;
	}

	IntVector2 &operator/(ULong scalar)
	{
		this->x /= int(scalar), this->y /= int(scalar);
		return *this;
	}
	IntVector2 &operator=(Pos2D vector);
};

struct DoubleVector2
{
	double x, y;
	DoubleVector2(double X, double Y) {x = X, y = Y;}
	DoubleVector2(void) {x = y = 0;}
	DoubleVector2 &operator+(DoubleVector2 value)
	{
		this -> x += value.x;
		this -> y += value.y;
		return *this;
	}
	DoubleVector2 &operator+=(DoubleVector2 value)
	{
		this->x += value.x;
		this->y += value.y;
		return *this;
	}
	DoubleVector2 &operator-=(DoubleVector2 value)
	{
		this->x -= value.x;
		this->y -= value.y;
		return *this;
	}
	DoubleVector2 &operator*(double scalar)
	{
		this -> x *= scalar, this -> y *= scalar;
		return *this;
	}

	DoubleVector2 &operator*=(double scalar)
	{
		this->x *= scalar, this->y *= scalar;
		return *this;
	}

	DoubleVector2 &operator/(double scalar)
	{
		this -> x /= scalar, this -> y /= scalar;
		return *this;
	}

	DoubleVector2 &operator*(DoubleVector2 multicand)
	{
		this -> x *= multicand.x, this -> y *= multicand.y;
		return *this;
	}

	DoubleVector2 &operator/(DoubleVector2 dividend)
	{
		this -> x /= dividend.x, this -> y /= dividend.y;
		return *this;
	}
};

struct Pos2D
{
	ULong x, y;
	Pos2D(ULong X, ULong Y) { x = X, y = Y; }
	Pos2D(void) { x = y = 0; }
	bool operator!=(Pos2D value)
	{
		bool check;
		this->x != value.x ? check = true : (this->y != value.y ? check = true : check = false);
		return check;
	}
	bool operator==(Pos2D value)
	{
		bool check;
		this->x == value.x ? (this->y == value.y ? check = true : check = false) : check = false;
		return check;
	}

	Pos2D operator+(Pos2D value)
	{
		this->x += value.x, this->y += value.y;
		return *this;
	}
	Pos2D operator-(Pos2D value)
	{
		this->x -= value.x, this->y -= value.y;
		return *this;
	}
	Pos2D operator*(Pos2D pos)
	{
		this->x *= pos.x, this->y *= pos.y;
		return *this;
	}
	Pos2D operator/(Pos2D pos)
	{
		this->x /= pos.x, this->y /= pos.y;
		return *this;
	}
	Pos2D operator*(DoubleVector2 pos)
	{
		this->x *= ULong(pos.x), this->y *= ULong(pos.y);
		return *this;
	}
	Pos2D operator/(DoubleVector2 pos)
	{
		this->x /= ULong(pos.x), this->y /= ULong(pos.y);
		return *this;
	}
	Pos2D operator*(int scalar)
	{
		this->x *= ULong(scalar), this->y *= ULong(scalar);
		return *this;
	}
	Pos2D operator/(int scalar)
	{
		this->x /= ULong(scalar), this->y /= ULong(scalar);
		return *this;
	}
	Pos2D operator*(ULong scalar)
	{
		this->x *= scalar, this->y *= scalar;
		return *this;
	}
	Pos2D operator/(ULong scalar)
	{
		this->x /= scalar, this->y /= scalar;
		return *this;
	}
	Pos2D operator+(IntVector2 pos)
	{
		this->x += pos.x, this->y += pos.y;
		return *this;
	}
	Pos2D operator-(IntVector2 pos)
	{
		this->x -= pos.x, this->y -= pos.y;
		return *this;
	}
};

struct Pos3D
{
	ULong x, y, z;
	Pos3D(ULong X, ULong Y, ULong Z) { x = X, y = Y, z = Z; }
	Pos3D(void) { x = y = z = 0; }
};

typedef IntVector2 IV2;
typedef IntVector3 IV3;
typedef IntVector4 IV4;
typedef DoubleVector2 FV2;
typedef DoubleVector3 FV3;
typedef DoubleVector4 FV4;

struct SquareFV 
{
	FV2 x, y, z, w;
	SquareFV() { x = y = z = w = FV2(0, 0); }
	SquareFV(FV2 X, FV2 Y, FV2 Z, FV2 W) {x = X, y = Y, z = Z, w = W;}
	SquareFV(FV2 * nextFrame)
	{
		if(&nextFrame[0] == nullptr || &nextFrame[1] == nullptr|| &nextFrame[2] == nullptr || &nextFrame[3] == nullptr)
			x = y = z = w = FV2(0, 0);
		else
		{
			x = nextFrame[0];
			y = nextFrame[1];
			z = nextFrame[2];
			w = nextFrame[3];
		}
	}
};

#endif
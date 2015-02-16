/*
The encoder namespace is used as a way to convert values that are
stored in the GEOC file format, this namespace uses IEEE 754 standard
for 32 and 64 bit numbers.
*/
#ifndef IEEE_CODEX_H
#define IEEE_CODEX_H

#include <bitset>
#include <cmath>

#include <iostream>

using namespace std;

#define _NAN 0xFFFFFFFF
#define LEAD_REMOVAL 0x7F

typedef unsigned char * ucharp;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned long long ulonglong;

using namespace std;

void toValue_32(float &value, ucharp input);
void toValue_64(double &size, ucharp input);
void toBinary_32(float value, ucharp charValue);
void toBinary_64(double value, ucharp charValue);

void toBinary(ushort Value, ucharp charValue);
void toBinary(short Value, ucharp charValue);
void toBinary(uint Value, ucharp charValue);
void toBinary(int Value, ucharp charValue);
void toBinary(ulong Value, ucharp charValue);
void toBinary(long Value, ucharp charValue);
void toBinary(ulonglong Value, ucharp charValue);
void toBinary(long long Value, ucharp charValue);

void toValue(ucharp Value, ushort &size);
void toValue(ucharp Value, short &size);
void toValue(ucharp Value, int &size);
void toValue(ucharp Value, uint &size);
void toValue(ucharp Value, ulong &size);
void toValue(ucharp Value, long &size);
void toValue(ucharp Value, ulonglong &size);
void toValue(ucharp Value, long long &size);

bool testCodex(void);

#endif
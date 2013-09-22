#ifndef __PACKET_H__
#define __PACKET_H__

#include <stdio.h>
#include <string>
using std::string;
#include "ezSockets.h"
typedef unsigned char byte;

#ifndef null
#define null 0
#endif

class Packet{
public:
	bool Valid;
	ezSockets* Sock;
	unsigned int OutPos;
	unsigned int OutSize;
	unsigned int InPos;
	unsigned int InSize;
	byte* InBuffer;
	byte* OutBuffer;

	Packet();
	Packet(ezSockets*);
	~Packet();

	void Clear();

	void WriteByte(byte);
	void WriteBytes(byte*, int, bool);
	void WriteShort(short);
	void WriteUShorts(unsigned short*, int, bool);
	void WriteInt(int);
	void WriteLong(long);
	void WriteFloat(float);
	void WriteDouble(double);
	void WriteString(const char*);

	byte ReadByte();
	byte* ReadBytes(int len);
	short ReadShort();
	int ReadInt();
	long ReadLong();
	float ReadFloat();
	double ReadDouble();
	string ReadString();

	bool CanRead(int);
	bool HasDataLeft();
	int DataLeft();

	void Send();

	void CheckSpaceOut(int);
	void AllocateMoreSpaceOut(int);
	void CheckSpaceIn(int);
	void AllocateMoreSpaceIn(int);
};

#endif // __PACKET_H__

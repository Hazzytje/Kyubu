//ezSockets.h - Easy-to-use multiplatform sockets class
//	Designed by Joshua Allen
//	UDP Supporty by Adam Lowman
//	Editor for EzSockets 1, 2 and 3: Charles Lohr
//	Supports: OSX, Linux, Windows (NT 3.5+ or 95+), PS2, XBOX

#ifndef EZSOCKETS_H
#define EZSOCKETS_H

#include <fcntl.h>
#include <ctype.h>
#include "MercuryString.h"

#if defined(_XBOX)
// Summary : WinsockX is bad, XTL is good.
// Explained : WinsockX may rely on some declares
//			   that are present in XTL. Also, using
//			   XTL includes some files maybe needed
//			   for other operations on Xbox.
#include <xtl.h>
#elif defined(WIN32)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

const int ezSocketsBuffersize = 16384;

class ezSocketsPacket
{
public:
	ezSocketsPacket();
	~ezSocketsPacket();

	void CopyPacket( ezSocketsPacket & pPacketToCopy );
	void ClearPacket();
	void SetupPacket( unsigned int iSize );
	void DestroyTree();

	char * Data;
	int Position;
	unsigned int PositionTAG;	//Special -- for internal use unless UDP mode, then it's the address
	int Size;
	unsigned short  Port;	//The remote port;

	ezSocketsPacket * Next;	//Only used if in a linked list.

	//Commands used to operate on NetPackets
	unsigned char	Read1();
	unsigned short	Read2();
	unsigned long	Read4();
	MString			ReadNT();
	int				ReadData( int Bytes, char * ToReplace );

	void Write1( unsigned char Info );
	void Write2( unsigned short Info );
	void Write4( unsigned long Info );
	void WriteNT( const MString& Info );
	void WriteData( const char * Info, unsigned int Length );
private:
	void Grow( unsigned long iSizeTo );
};

class ezSockets
{
public:
	ezSockets();
	~ezSockets();

	//Crate the socket
	bool Create(int Protocol = IPPROTO_TCP, int Type = SOCK_STREAM);

	//Bind Socket to local port
	bool Bind(unsigned short port);

	//Listen with socket
	bool Listen(unsigned long depth = 5);

	//Accept incomming socket
	bool Accept(ezSockets &socket);
	ezSockets * Accept();

	//Connect
	bool Connect(const MString& host, unsigned short port);

	//Kill socket
	void Close();

	//see if socket has been created
	bool Check();

	bool CanRead();
	bool DataAvailable();
	bool IsError();
	bool CanWrite();

	//Raw data system
	void SendData(const MString& outData);
	void SendData(const char *data, unsigned int bytes);
	int ReadData(char *data, unsigned int bytes);
	int PeekData(char *data, unsigned int bytes);
	bool ReadLine( MString & str );
	void WriteLine( const MString & str );

	//Read arbitrary amount of data
	int ReadLeftover( char * data, unsigned int maxbytes );

	//Packet system (for structures and classes)
	void SendPack( const ezSocketsPacket &pPack );

	//Read
	int ReadPack( ezSocketsPacket &pPack );
	int PeekPack( ezSocketsPacket &pPack );

	bool bBlocking;

	enum SockState
	{
		skDISCONNECTED = 0,
		skUNDEF1, //Not implemented
		skLISTENING,
		skUNDEF3, //Not implemented
		skUNDEF4, //Not implemented
		skUNDEF5, //Not implemented
		skUNDEF6, //Not implemented
		skCONNECTED,
		skERROR
	} state;

	int lastCode;	//Used for debugging purposes

	enum TransportMode
	{
		skPackets = 0,	//The normal packet system we are used to (Transmit/Receive Size)
		skGeneral,		//Flow the data into a general use buffer, or out of a buffer.
		skUDP,			//Move data from UDP on a per-packet basis. The first 4 bytes will be the UDP Address. (Transmit is the same)
	} mode;

	MString GetAddress() { return address; }

	MString address;
	ezSocketsPacket * pDataInHead;
	ezSocketsPacket * pDataInTail;
	int iBytesInPending;
	int iReadInPending;

	//Shift all data from the socket, into pDataInHead.
	//bForceBlock will block if in packet mode, until a full packet has been received.
	void ReceiveGeneralData( bool bForceBlock = false );

    struct sockaddr_in fromAddr;

	int maxcon;
	int sock;
	struct sockaddr_in addr;

	//Used for Select() command
	fd_set  *scks;
	timeval *times;

	int pReadData(char* data, int maxsize );
	int pWriteData(const char* data, int dataSize);

	//Only necessiary in windows, xbox
#if defined(_WIN32) || defined(_XBOX)
	WSADATA wsda;
#endif
	char Buffer[ezSocketsBuffersize];
};

#endif

/*
 * (c) 2003-2006 Josh Allen, Charles Lohr, and Adam Lowman
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*******************************************************************\
| ezsockets.cpp: EzSockets Class Source                             |
|   Designed by Josh Allen, Charles Lohr and Adam Lowman.           |
|   Socket programming methods based on Charles Lohr's EZW progam.  |
|   Modified by Charles Lohr for use with Windows-Based OSes.       |
|   UDP/NON-TCP Support by Adam Lowman.                             |
|                                                                   |
|   Primary Editor: Charles Lohr									|
\*******************************************************************/

#include "ezSockets.h"

#if defined(_MSC_VER) && !defined(_XBOX) // We need the WinSock32 Library on Windows
#pragma comment(lib,"wsock32.lib")
#elif !defined(__MINGW32__) && !defined(_XBOX)
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#if !defined(SOCKET_ERROR)
#define SOCKET_ERROR -1
#endif

//There are cases where 0 isn't a proper socket
#if !defined(SOCKET_NONE)
#define SOCKET_NONE 0
#endif

#if !defined(INVALID_SOCKET)
#define INVALID_SOCKET -1
#endif

#if defined(WIN32)
typedef int socklen_t;
#endif

int iPackets = 0;
#include <iostream>
#include <cstring>
ezSocketsPacket::ezSocketsPacket()
{
	Data = NULL;
	Next = NULL;
	iPackets++;
	Port = 0;
	ClearPacket();
}

ezSocketsPacket::~ezSocketsPacket()
{
	iPackets--;
	if ( Data )
		free( Data );
}

void ezSocketsPacket::CopyPacket( ezSocketsPacket & pPacketToCopy )
{
	Size = pPacketToCopy.Size;
	if ( Data )
		delete Data;
	Data = (char*)malloc( Size );
	memcpy( Data, pPacketToCopy.Data, Size );
	Position = 0;
	Next = NULL;
}

void ezSocketsPacket::DestroyTree()
{
	if ( Next )
	{
		Next->DestroyTree();
		delete Next;
		Next = NULL;
	}
	if ( Data )
	{
		delete Data;
		Data = NULL;
	}
}

void ezSocketsPacket::ClearPacket()
{
	if ( Data )
		free( Data );
	Data = NULL;
	Position = 0;
	Size = 0;
}

void ezSocketsPacket::SetupPacket( unsigned int iSize )
{
	if ( Data )
		free( Data );
	Data = (char*)malloc( iSize );
	Position = 0;
	Size = iSize;
}

unsigned char	ezSocketsPacket::Read1()
{
	if ( Position < Size )
		return Data[Position++];
	else
		return 0;
}

unsigned short	ezSocketsPacket::Read2()
{
	if ( Position < (Size-1) )
		return ntohs( *((unsigned short*)&(Data[(Position+=2)-2])) );
	else
		return 0;
}

unsigned long	ezSocketsPacket::Read4()
{
	if ( Position < (Size-3) )
		return ntohl( *((unsigned long*)&(Data[(Position+=4)-4])) );
	else
		return 0;
}

MString			ezSocketsPacket::ReadNT()
{
	if ( Position >= Size )
		return MString("");

	char * base = &Data[Position];

	while ( Data[Position++] != '\0' );

	return MString(base);
}

void ezSocketsPacket::Write1( unsigned char Info )
{
	Grow( Position + 1 );
	Data[Position++] = Info;
}

void ezSocketsPacket::Write2( unsigned short Info )
{
	Grow( Position + 2 );
	*((short*)(&Data[Position])) = htons(Info);
	Position+=2;
}

void ezSocketsPacket::Write4( unsigned long Info )
{
	Grow( Position + 4 );
	*((unsigned long*)(&Data[Position])) = htonl(Info);
	Position += 4;
}

void ezSocketsPacket::WriteNT( const MString& Info )
{
	Grow( Position + Info.length() + 1 );
	memcpy( &Data[Position], Info.c_str(), Info.length()+1 );
	Position += Info.length()+1;
}

int ezSocketsPacket::ReadData( int Bytes, char * ToReplace )
{
	if ( Bytes + Position >= Size )
		Bytes = Size - Position;
	if ( Bytes > 0 )
		memcpy( ToReplace, &Data[Position], Bytes );
	else
		return 0;
	Position += Bytes;
	return Bytes;
}

void ezSocketsPacket::WriteData( const char * Info, unsigned int Length )
{
	Grow( Length + Position );
	memcpy( &Data[Position], Info, Length );
	Position += Length;
}

void ezSocketsPacket::Grow( unsigned long iSizeTo )
{
	if ( iSizeTo > (unsigned long)(Size) )
	{
		char * OldData = Data;
		Data = (char*)malloc( iSizeTo );
		if ( OldData )
		{
			memcpy( Data, OldData, Size );
			free( OldData );
		}
		Size = iSizeTo;
	}
}

ezSockets::ezSockets()
{
	memset (&addr,0,sizeof(addr)); //Clear the sockaddr_in structure

#if defined(_WIN32) || defined(_XBOX) // Windows REQUIRES WinSock Startup
	WSAStartup( MAKEWORD(1,1), &wsda );
#endif

	sock = INVALID_SOCKET;
	bBlocking = true;
	scks = new fd_set;
	times = new timeval;
	times->tv_sec = 0;
	times->tv_usec = 0;
	state = skDISCONNECTED;

	iBytesInPending = 0;
	pDataInTail = NULL;
	pDataInHead = NULL;
}

ezSockets::~ezSockets()
{
	Close();
	delete scks;
	delete times;
}

bool ezSockets::Check()
{
#if !defined(XBOX)
	return sock > SOCKET_NONE;
#else
	return sock != INVALID_SOCKET;
#endif
}

bool ezSockets::Create(int Protocol, int Type)
{
	state = skDISCONNECTED;
	sock = socket(AF_INET, Type, Protocol);
	lastCode = sock;
#if !defined(XBOX)
	return sock > SOCKET_NONE;	//Socket must be Greater than 0
#else
	return sock != INVALID_SOCKET;
#endif
}

bool ezSockets::Bind(unsigned short port)
{
	if(!Check())
		return false;

	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port        = htons(port);
	lastCode = ::bind(sock,(struct sockaddr*)&addr, sizeof(addr));
	return !lastCode;
}

bool ezSockets::Listen( unsigned long depth )
{
	lastCode = ::listen(sock, depth);
	if (lastCode == SOCKET_ERROR)
		return false;

	state = skLISTENING;
	return true;
}

bool ezSockets::Accept(ezSockets& socket)
{
	if (!bBlocking && !CanRead())
		return false;

	int length = sizeof(socket);

	socket.sock = ::accept(sock,(struct sockaddr*) &socket.addr,
						   (socklen_t*) &length);

	#if !( defined (_WIN32) || defined( _XBOX ) )
		char buf[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &socket.addr.sin_addr, buf, INET_ADDRSTRLEN);
		address = buf;
	#else
		address = inet_ntoa(socket.addr.sin_addr);
	#endif

	lastCode = socket.sock;

	if ( socket.sock == SOCKET_ERROR )
		return false;

	socket.state = skCONNECTED;
	return true;
}

ezSockets * ezSockets::Accept()
{
	if (!bBlocking && !CanRead())
		return NULL;

	sockaddr_in local_addr;
	int length = sizeof(local_addr);

	int localsock = ::accept(sock,(struct sockaddr*) &local_addr,
						   (socklen_t*) &length);

	if ( localsock == SOCKET_ERROR )
		return NULL;

	ezSockets * sNew = new ezSockets;

	#if !( defined (_WIN32) || defined( _XBOX ) )
		char buf[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &local_addr.sin_addr, buf, INET_ADDRSTRLEN);
		sNew->address = buf;
	#else
		sNew->address = inet_ntoa(local_addr.sin_addr);
	#endif

	lastCode = localsock;


	sNew->bBlocking = bBlocking;
	sNew->lastCode = 0;
	sNew->mode = mode;
	sNew->pDataInHead = NULL;
	sNew->sock = localsock;
	sNew->state = skCONNECTED;
	return sNew;
}

void ezSockets::Close()
{
	state = skDISCONNECTED;

	if ( pDataInHead )
	{
		pDataInHead->DestroyTree();
		delete pDataInHead;
		pDataInHead = NULL;
	}

#if defined(WIN32) // The close socket command is different in Windows
	::closesocket(sock);
#else
	::close(sock);
#endif
}

bool ezSockets::Connect(const MString& host, unsigned short port)
{
	if(!Check())
		return false;

#if defined(_XBOX)
	if(!isdigit(host[0])) // don't do a DNS lookup for an IP address
	{
		XNDNS *pxndns = NULL;
		XNetDnsLookup(host.c_str(), NULL, &pxndns);
		while (pxndns->iStatus == WSAEINPROGRESS)
		{
			// Do something else while lookup is in progress
		}

		if (pxndns->iStatus == 0)
			memcpy(&addr.sin_addr, &pxndns->aina[0], sizeof(struct in_addr));
		else
			return false;

		XNetDnsRelease(pxndns);
	}
	else
		addr.sin_addr.s_addr = inet_addr(host.c_str());
#else
	struct hostent* phe;
	phe = gethostbyname(host.c_str());
	if (phe == NULL)
		return false;
	memcpy(&addr.sin_addr, phe->h_addr, sizeof(struct in_addr));
#endif
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(port);

	if(::connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		return false;

	state = skCONNECTED;
	return true;
}

bool ezSockets::CanRead()
{
	FD_ZERO(scks);
	FD_SET((unsigned)sock, scks);

	return select(sock+1,scks,NULL,NULL,times) > 0;
}

bool ezSockets::DataAvailable()
{
	if ( CanRead() )
		return true;

	//I think this is wrong, and is only the case with skGeneral
	if ( pDataInHead )
		if ( pDataInHead->PositionTAG != (unsigned int)(pDataInHead->Position) )
			return true;
	return false;
}

bool ezSockets::IsError()
{
	if (state == skERROR)
		return true;

	FD_ZERO(scks);
	FD_SET((unsigned)sock, scks);

	if (select(sock+1, NULL, NULL, scks, times) >=0 )
		return false;

	state = skERROR;
	return true;
}

bool ezSockets::CanWrite()
{
	FD_ZERO(scks);
	FD_SET((unsigned)sock, scks);

	return select(sock+1, NULL, scks, NULL, times) > 0;
}


void ezSockets::SendData(const MString& outData)
{
	if ( mode == skGeneral )
		pWriteData( outData.c_str(), outData.length() );
}

void ezSockets::SendData(const char *data, unsigned int bytes)
{
	if ( mode == skGeneral )
		pWriteData( data, bytes );
}

int ezSockets::ReadData(char *data, unsigned int bytes)
{
	if ( mode != skGeneral )
		return -1;
	int iRL = ReadLeftover( data, bytes );
	while ( (unsigned int)(iRL) < bytes )
	{
		int iGot = pReadData( data + iRL, bytes - iRL );
		if ( iGot <= 0 )
			return iRL;
		iRL += iGot;
	}
	return iRL;
}

int ezSockets::PeekData(char *data, unsigned int bytes)
{
	if ( mode != skGeneral )
		return -1;

	int iRL = 0;
	ReceiveGeneralData();
	ezSocketsPacket * tmp = pDataInHead;

	while ( tmp )
	{
		int iPotential = tmp->Size - tmp->Position;
		if ( (unsigned int)(iPotential) > bytes-iRL )
			iPotential = bytes-iRL;
		memcpy( data + iRL, tmp->Data + tmp->Position, iPotential );
		iRL += iPotential;
		if ( (unsigned int)(iRL) >= bytes )
			return iRL;
		tmp = tmp->Next;
	}
	return iRL;
}

//Yuck, this algorithm is kind of slow...
bool ezSockets::ReadLine( MString & str )
{
	if ( mode != skGeneral )
		return false;

	str = "";
	ReceiveGeneralData();

	ezSocketsPacket * tmp = pDataInHead;

	bool bFound = false;
	int i = 0;

	while ( tmp )
	{
		for ( i = tmp->Position; i < (int)(tmp->PositionTAG); i++ )
			if ( tmp->Data[i] == '\n' )
			{
				bFound = true;
				break;
			}
		if ( !bFound )
			tmp = tmp->Next;
		else
			break;
	}

	if ( !bFound && !bBlocking )
		return false;

	if ( bFound )
		while ( pDataInHead != tmp )
		{
			str.append( pDataInHead->Data, pDataInHead->Size-pDataInHead->Position );
			pDataInHead = pDataInHead->Next;
		}

	if ( pDataInHead )
		if ( i != pDataInHead->Position )
		{
			str.append( pDataInHead->Data + pDataInHead->Position, i - pDataInHead->Position );
			pDataInHead->Position = i + 1;
		}

	//Keep waiting for more data
	if ( !bFound && bBlocking )
	{
		if ( pDataInHead )
			pDataInHead->Position = pDataInHead->PositionTAG;

		char tmp = '\0';
		while ( tmp != '\n' )
		{
			if ( ReadLeftover( &tmp, 1 ) != 1 )
				return false;
			if ( tmp != '\n' )
				str.append( &tmp, 1 );
		}
	}

	if ( str.length() > 0 )
		if ( str.c_str()[str.length()-1] == '\r' )
			str.resize ( str.length() - 1 );

	return true;
}

void ezSockets::WriteLine( const MString & str )
{
	SendData( str.c_str(), str.length() );
	SendData( "\r\n", 1 );
}

int ezSockets::ReadLeftover( char * data, unsigned int maxbytes )
{
	if ( mode != skGeneral )
		return -1;

	ReceiveGeneralData();

	int iPos = 0;

	while ( pDataInHead )
	{
		int iMemToRead = pDataInHead->PositionTAG - pDataInHead->Position;
		if ( iMemToRead == 0 )
			break;
		if ( iMemToRead > ( (int)(maxbytes) - iPos ) )
			iMemToRead = maxbytes - iPos;

		memcpy( data + iPos, pDataInHead->Data + pDataInHead->Position, iMemToRead );

		pDataInHead->Position += iMemToRead;
		iPos += iMemToRead;

		if ( pDataInHead->Position == pDataInHead->Size )
		{
			if ( pDataInHead == pDataInTail )
			{
				delete pDataInHead;
				pDataInHead = 0;
				pDataInTail = 0;
				return iPos;
			} else {
				ezSocketsPacket * tmp = pDataInHead;
				pDataInHead = pDataInHead->Next;
				delete tmp;
			}
		}

		if ( (unsigned int)(iPos) >= maxbytes )
			return iPos;
	}

	if ( bBlocking )
	{
		int iRead = pReadData( Buffer, (ezSocketsBuffersize<maxbytes)?ezSocketsBuffersize:maxbytes );
		if ( iRead > 0 )
		{
			memcpy( data+iPos, Buffer, iRead );
			return iRead+iPos;
		}
		else
			return iPos;
	} else
		return iPos;
}

void ezSockets::ReceiveGeneralData( bool bForceBlock )
{
	if ( mode == skGeneral )
	{
		while( CanRead() )
		{
			if( pDataInTail )
			{
				if ( pDataInTail->Size <= (int)(pDataInTail->PositionTAG) )
				{
					pDataInTail->Next = new ezSocketsPacket;
					pDataInTail = pDataInTail->Next;
					pDataInTail->Next = NULL;
					pDataInTail->SetupPacket( ezSocketsBuffersize );
					pDataInTail->PositionTAG = 0;
				} else {
					int rec = pReadData( pDataInTail->Data + pDataInTail->PositionTAG, pDataInTail->Size - pDataInTail->PositionTAG );
					if ( rec>0 )
						pDataInTail->PositionTAG += rec;
					else
						break;
				}
			}
			else
			{
				pDataInTail = new ezSocketsPacket;
				pDataInHead = pDataInTail;
				pDataInHead->SetupPacket( ezSocketsBuffersize );
				pDataInHead->PositionTAG = 0;
			}
		}
		return;
	} else if ( mode == skPackets )
	{
		bool bGotPack = false;
		if ( pDataInTail )
			if ( pDataInTail->PositionTAG >= 4 && pDataInTail->Position == pDataInTail->Size )
				bGotPack = true;
		while ( CanRead() || ( bForceBlock && !bGotPack ) )
		{
			if ( !pDataInTail )
			{
				pDataInHead = new ezSocketsPacket;
				pDataInTail = pDataInHead;
				pDataInTail->Next = NULL;
				pDataInTail->PositionTAG = 0;	//0: Nothing received yet.
			}
			if ( pDataInTail->PositionTAG == 4 && pDataInTail->Size == pDataInTail->Position )
			{
				pDataInTail->Next = new ezSocketsPacket;
				pDataInTail = pDataInTail->Next;
				pDataInTail->Next = NULL;
				pDataInTail->PositionTAG = 0;
			}
			if ( pDataInTail->PositionTAG < 4 )
			{
				int recv = pReadData( Buffer, 4 - pDataInTail->PositionTAG );
				int cpos = 0;
				unsigned int oldpos = pDataInTail->PositionTAG;
				pDataInTail->PositionTAG+=recv;
				if ( recv > 0 )
				{
					for ( ; oldpos < pDataInTail->PositionTAG; oldpos++ )
					{
						unsigned long j = (unsigned char)Buffer[cpos];
						pDataInTail->Size += j<<(8*(3-oldpos));
						cpos++;
					}
				} else
					return;
				if ( pDataInTail->PositionTAG == 4 )
				{
					pDataInTail->SetupPacket( pDataInTail->Size );
					pDataInTail->Position = 0;
				}
			} else
			{
				int recv = pReadData( pDataInTail->Data + pDataInTail->Position, pDataInTail->Size - pDataInTail->Position );
				if ( recv > 0 )
					pDataInTail->Position += recv;
				else
					return;
			}

			if ( pDataInTail )
				if ( pDataInTail->PositionTAG >= 4 && pDataInTail->Position == pDataInTail->Size )
					bGotPack = true;
		}
	} else if ( mode == skUDP )
	{
		if ( CanRead() || bForceBlock )
		{
			int iRead = pReadData( Buffer, ezSocketsBuffersize );
			int pAddy = ntohl(fromAddr.sin_addr.s_addr);
			int pPort = ntohs(fromAddr.sin_port);

			if ( iRead>0 )
			{
				if ( !pDataInTail )
				{
					pDataInHead = new ezSocketsPacket;
					pDataInTail = pDataInHead;
					pDataInTail->PositionTAG = 0;	//0: Nothing received yet.
				} else {
					ezSocketsPacket * tmp = pDataInTail;
					pDataInTail = new ezSocketsPacket;
					tmp->Next = pDataInTail;
				}

				pDataInTail->SetupPacket( iRead );
				memcpy( pDataInTail->Data, Buffer, iRead );
				pDataInTail->PositionTAG = pAddy;
				pDataInTail->Port = pPort;
			}
		}
	}
}

void ezSockets::SendPack( const ezSocketsPacket &pPack )
{
	//While this seems kind of dumb, re-copying the data and all
	//it does save us a potential extra packet.
	//
	//Overall, it actually saves CPU.
	char * sToSend = (char*)malloc( pPack.Size + 4 );
	unsigned int nBytes = htonl( pPack.Size );
	memcpy( sToSend, &nBytes, 4 );
	memcpy( sToSend + 4, pPack.Data, pPack.Size );
	if( pPack.Port != 0 && mode == skUDP )
	{
		sockaddr_in to;
		to.sin_port = htons( pPack.Port );
		to.sin_family = AF_INET;
		to.sin_addr.s_addr = htonl( pPack.PositionTAG );
		memset( to.sin_zero, 0, 8 );
		sendto( sock, sToSend, pPack.Size + 4 , 0, (sockaddr*)&to, sizeof( sockaddr_in ) );
	}
	else
		pWriteData( sToSend, pPack.Size + 4 );
	free( sToSend );
}

int ezSockets::ReadPack( ezSocketsPacket &pPack )
{
	ReceiveGeneralData();
	if( pDataInHead )
	{
		if ( ( pDataInHead->Position == pDataInHead->Size || mode == skUDP ) && pDataInHead->PositionTAG >= 4 )
		{
			//Free up any necessiary data
			pPack.~ezSocketsPacket();
			pPack = (*pDataInHead);

			ezSocketsPacket * g = pDataInHead;

			pPack.Position = 0;

			if ( pDataInHead == pDataInTail )
			{
				pDataInHead = NULL;
				pDataInTail = NULL;
			} else
				pDataInHead = pDataInHead->Next;

			free( g );

			return 1;
		}
	}
	//Otherwise, wait until the packet is ready.
	if ( bBlocking )
	{
		ReceiveGeneralData( true );
		if( pDataInHead )
		{
			if ( ( pDataInHead->Position == pDataInHead->Size || mode == skUDP ) && pDataInHead->PositionTAG >= 4 )
			{
				//Free up any necessiary data
				pPack.~ezSocketsPacket();
				pPack = (*pDataInHead);

				ezSocketsPacket * g = pDataInHead;
				pPack.Position = 0;

				if ( pDataInHead == pDataInTail )
				{
					pDataInHead = NULL;
					pDataInTail = NULL;
				} else
					pDataInHead = pDataInHead->Next;

				free( g );

				return 1;
			}
		}
	}

	return 0;
}

int ezSockets::PeekPack( ezSocketsPacket &pPack )
{
	ReceiveGeneralData();
	if( pDataInHead )
	{
		if ( pDataInHead->Position == pDataInHead->Size )
		{
			pPack.CopyPacket( *pDataInHead );
			pPack.Position = 0;
			return 1;
		}
	}
	return 0;
}

int ezSockets::pReadData(char* data, int maxsize )
{
	if(state == skCONNECTED || state == skLISTENING)
		return recv(sock, data, maxsize, 0);

	int saddrinlen = sizeof(sockaddr_in);

	return recvfrom(sock, data, maxsize, 0, (sockaddr*)&fromAddr,
					(socklen_t*)&saddrinlen);
}

int ezSockets::pWriteData(const char* data, int dataSize)
{
	return send(sock, data, dataSize, 0);
}

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


/*

Nicholas LaRosa
CSE 30264, Program 4
IP agnostic Server

usage: server6

*/

#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

#include <netdb.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 1400
#define PORT "9979"
#define DEBUG 0

int main( int argc, char** argv )
{
	int sockfd, inputBytes, outputBytes, isHost, err, i, j;
	
	struct addrinfo hints, *server;
	struct sockaddr_storage serverAddress, clientAddress;
	struct addrinfo *hostInfo, *p;
	struct timeval timer;
	struct tm * timeFormat;

	socklen_t socketSize;
	time_t currentTime;
	
	char sendLine[ BUFFER + 1 ];
	char recvLine[ BUFFER + 1 ];
	char timeStamp[ 16 ];
	char ipstr[ INET6_ADDRSTRLEN ];

	void * addr;

	if( argc != 1 )
	{
		printf( "\nusage: server6\n\n" );
		exit( 1 );
	}

	memset( &clientAddress, 0, sizeof( clientAddress ) );
	memset( &hints, 0, sizeof( hints ) );	// secure enough memory for the hints addrinfo
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if( ( err = getaddrinfo( NULL, PORT, &hints, &server ) < 0 ) )
	{
		perror( "getaddrinfo(): " );
		exit( 1 );
	}	
	
	for( p = server; p != NULL; p = p->ai_next )
	{
		memcpy( &serverAddress, p->ai_addr, p->ai_addrlen );

		if( p->ai_family == AF_INET )
		{
			addr = &(((struct sockaddr_in *)&serverAddress)->sin_addr);
		}
		else
		{
			addr = &(((struct sockaddr_in6 *)&serverAddress)->sin6_addr);
		}

		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0 ) 
		{
			perror("Server: socket()");
			continue;
		}

		if( bind( sockfd, p->ai_addr, p->ai_addrlen ) < 0 )
		{
			perror( "Server - bind() error" );
			continue;
		}

		if(DEBUG) 	printf( "Bind() complete...\n" );

		break;
	}

	freeaddrinfo( server );

	if( p == NULL )
	{
		printf( "Failure to bind to socket.\n" );
		exit( 1 );
	}		
	
	while( 1 )
	{
		socketSize = sizeof( clientAddress );

		memset( recvLine, 0, BUFFER );
		memset( sendLine, 0, BUFFER );

		if(DEBUG)	printf( "Ready for a new client...\n" );

		if( ( inputBytes = recvfrom( sockfd, recvLine, BUFFER, 0, ( struct sockaddr * )&clientAddress, &socketSize ) ) < 0 )
		{
			perror( "Server - recvfrom() error" );
			exit( 1 );
		}

		if(DEBUG)	printf( "Recieved...\n" );	

		recvLine[ inputBytes ] = '\0';	

		for( i = 0; i < strlen( recvLine ); i++ )
		{
			j = strlen( recvLine ) - i - 1;
			sendLine[ i ] = recvLine[ j ];				// invert the input buffer
		}

		sendLine[ inputBytes ] = '\0';					// end the string just in case

		gettimeofday( &timer, NULL );
		timeFormat = localtime( &timer.tv_sec );
		sprintf( timeStamp, " Timestamp: %02d:%02d:%02d.%06d\n\0", timeFormat->tm_hour, timeFormat->tm_min, timeFormat->tm_sec, timer.tv_usec );

		strcat( sendLine, timeStamp );

		if( ( outputBytes = sendto( sockfd, sendLine, strlen( sendLine ), 0, ( struct sockaddr * )&clientAddress, sizeof( clientAddress ) ) ) < 0 )
		{
			perror( "Server - sendto() error" );
			exit( 1 );
		}
	}
}


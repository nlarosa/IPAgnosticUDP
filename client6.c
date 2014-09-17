
/*

Nicholas LaRosa
CSE 30264, Program 4
IP agnostic Client

usage: client6 <IP_Address> <Port_Number> <File_to_Send>

*/

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 1400
#define DEBUG 0

int main( int argc, char** argv )
{
	int sockfd, inputBytes, outputBytes, isHost, i;
	struct sockaddr_storage serverAddress;
	struct addrinfo hints, *server, *j;
	char sendLine[ BUFFER + 1 ];
	char recvLine[ BUFFER + 1 ];
	int err;
	in_port_t port;

	void * addr;
	char ipstr[INET6_ADDRSTRLEN];

	if( argc != 4 )
	{
		printf("\nusage: client6 <IP_Address> <Port_Number> <File_to_Send>\n\n");
		exit( 1 );
	}
	
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	memset( &serverAddress, 0, sizeof( serverAddress ) );	

	if( ( err = getaddrinfo( argv[1], argv[2], &hints, &server ) != 0 ) )
	{
		perror( "getaddrinfo(): " );
		exit( 1 );
	}

	for( j = server; j != NULL; j = j->ai_next )		// loop to find valid
	{
		if(DEBUG) 	printf( "Finding valid address...\n" );

		if( ( sockfd = socket( j->ai_family, j->ai_socktype, j->ai_protocol ) ) < 0 )		// open datagram socket, address family internet
		{
			perror( "Client - socket() error" );
			exit( 1 );
		}
		//bzero( &serverAddress, sizeof( serverAddress ) );

		memcpy( &serverAddress, j->ai_addr, j->ai_addrlen );

		if( j->ai_family == AF_INET )		// IPv4 
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)&serverAddress;
			addr = &(ipv4->sin_addr);
			port = (ipv4->sin_port);
		}
		else 					// IPv6
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)&serverAddress;
			addr = &(ipv6->sin6_addr);
			port = (ipv6->sin6_port);
		}

		break;
	}

	inet_ntop(j->ai_family, addr, ipstr, sizeof ipstr);		
			
	if(DEBUG) 	printf( "IP address: %s\n", ipstr );
	if(DEBUG)	printf( "IP address: %s\n", argv[1] );
	if(DEBUG)	printf( "Port number: %d\n", ntohs(port) );
	if(DEBUG) 	printf( "Address established...\n" );

	FILE *filePtr = fopen( argv[3], "r" );				// prepare storing of file to buffer
	if( filePtr != NULL )						// credit to Michael, StackOverflow
	{
		unsigned int length = fread( sendLine, sizeof( char ), BUFFER, filePtr );
		if( length == 0 )
		{
			fputs( "Error reading file.", stderr );
		}
		else
		{
			sendLine[ ++length ] = '\0';
		}

		fclose( filePtr );
	}
	else
	{
		fputs( "No such file exists.", stderr );
	}

	if( strlen( sendLine ) > BUFFER )
	{
		printf( "File sent cutoff at 1400 bytes.\n" );
		sendLine[ BUFFER - 1 ] = '\0';
	}

	if(DEBUG)	printf( "Sending...\n" );

	if( ( outputBytes = sendto( sockfd, sendLine, strlen( sendLine ), 0, ( struct sockaddr * )&serverAddress, sizeof( struct sockaddr_storage ) ) ) < 0 )
	{
		perror( "Client - sendto() error" );
		exit( 1 );
	}

	if(DEBUG)	printf( "Sending...\n" );
	
	if( ( inputBytes = recvfrom( sockfd, recvLine, BUFFER, 0, NULL, NULL ) ) < 0 )
	{
		perror( "Client - recvfrom() error" );
		exit( 1 );
	}

	recvLine[ inputBytes ] = '\0';	
	
	fputs( recvLine, stdout);	

	if( close( sockfd ) != 0 )					// close the socket
	{
		printf( "Client - sockfd closing failed!\n" );
	}		

	return 0;
}


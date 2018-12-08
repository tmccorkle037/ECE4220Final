#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <wiringPi.h>
#include <pthread.h>

#define BUTTON1 27		
#define BUTTON2 0
#define LED1 8
#define LED2 9
#define LED3 7

struct logData{
	double timestamp;
	char ip[50];
	int status;
	double volt;
	char type[50];
}Log;

void* checkB1( void* info)
{
	
}

void setup()
{
	wiringPiSetup();

	pinMode(BUTTON1, INPUT);
	pinMode(BUTTON2, INPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);

	pullUpDnControl(BUTTON1, PUD_DOWN);
	pullUpDnControl(BUTTON2, PUD_DOWN);
	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);
	digitalWrite(LED3, LOW);
}

int main(int argc, char *argv[])
{
	setup();
	
	//--------------CREATE SOCKET TO COMM W OPERTOR-------------//
	int sock, length, n;
	int boolval = 1;			// for a socket option
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in addr;

	if (argc < 2)
	{
		printf("usage: %s port\n", argv[0]);
		exit(0);
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
	if (sock < 0)
		error("Opening socket");

	length = sizeof(server);			// length of structure
	bzero(&server,length);			// sets all values to zero. memset() could be used
	server.sin_family = AF_INET;		// symbol constant for Internet domain
	server.sin_addr.s_addr = INADDR_ANY;		// IP address of the machine on which
	// the server is running
	server.sin_port = htons(atoi(argv[1]));	// port number



	// binds the socket to the address of the host and the port number
	if (bind(sock, (struct sockaddr *)&server, length) < 0)
		error("binding");

	// change socket permissions to allow broadcast
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
	{
		printf("error setting socket options\n");
		exit(-1);
	}

	addr.sin_addr.s_addr = inet_addr("128.206.19.255");

	fromlen = sizeof(struct sockaddr_in);	// size of structure
	//---------------------------------------------------------------------//

	//get IP address
	struct ifreq myReq;
	myReq.ifr_addr.sa_family = AF_INET;
	snprintf( myReq.ifr_name, IFNAMSIZ, "wlan0" );
	ioctl( sock, SIOCGIFADDR, &myReq );

	char* myIP = inet_ntoa( ((struct sockaddr_in *)&myReq.ifr_addr)->sin_addr );
	
	pthread_t b1_thread;
	
	pthread_create(&b1_thread, NULL, checkB1, (void*)&myIP);
}











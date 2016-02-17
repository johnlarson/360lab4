/*

To do:
receive responses
-d
print info

*/
#include "download.h"

using namespace std;

#define SOCKET_ERROR	-1
#define MAX_HTTP_SIZE	8192
#define BUFFER_SIZE	10000

int getSocket(char* host, int port);
void sendRequest(int sock, char* host, char* path);
void buildRequest(char result[], char* host, char* path);
void printDebug(char* request, char* headers);

void webtest(char* host, int port, char* path, int count, bool debug) {
	int socks[count];
	int epollFD = epoll_create(1);
	int startFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(close(startFD) == SOCKET_ERROR) {
		errorAndExit("Could not close socket");
	}
	struct timeval start[count + startFD];
	struct timeval finish[count + startFD];
	double delta[count + startFD];
	for(int i = 0; i < count; i++) {
		socks[i] = getSocket(host, port);
		sendRequest(socks[i], host, path);
		gettimeofday(&start[socks[i]], NULL);
		struct epoll_event event;
		event.data.fd = socks[i];
		event.events = EPOLLIN;
		int ret = epoll_ctl(epollFD,EPOLL_CTL_ADD, socks[i], &event);
		if(ret) {
			errorAndExit("epoll_ctl");
		}
	}
	for(int i = 0; i < count; i++) {
		struct epoll_event event;
		int rval = epoll_wait(epollFD, &event, 1, -1);
		if(rval < 0) {
			errorAndExit("epoll_wait");
		}
		char pBuffer[BUFFER_SIZE];
		rval = read(event.data.fd, pBuffer, BUFFER_SIZE);
		gettimeofday(&finish[event.data.fd], NULL);
		epoll_ctl(epollFD, EPOLL_CTL_DEL, event.data.fd, &event);
	}
	for(int i = 0; i < count; i++) {
		if(close(socks[i]) == SOCKET_ERROR) {
			errorAndExit("Could not close socket");
		}
	}
	double total = 0;
	for(int i = startFD; i < count + startFD; i++) {
		int delta_sec = finish[i].tv_sec - start[i].tv_sec;
		int delta_usec_remaining = finish[i].tv_usec - start[i].tv_usec;
		double delta_usec = delta_sec * (double)1000000 + delta_usec_remaining;
		double delta_time = delta_usec / 1000000;
		if(debug) {
			cout << i << ": " << delta_time << " sec" << endl;
		}
		total += delta_time;
		delta[i] = delta_time;
	}
	double avg = total / count;
	cout << "Average: " << avg << " sec" << endl;
}

int getSocket(char* host, int port) {
	const int READ_AMOUNT = 2000;
	int socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketHandle == SOCKET_ERROR) {
		errorAndExit("Could not make socket");
	}
	struct hostent* hostInfo = gethostbyname(host);
	if(hostInfo == NULL) {
		errorAndExit("Could not resolve host name");
	}
	long hostIp;
	memcpy(&hostIp, hostInfo->h_addr, hostInfo->h_length);
	struct sockaddr_in address;
	address.sin_addr.s_addr = hostIp;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	if(connect(socketHandle, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		printf("!!!!!!!");
		close(socketHandle);
		errorAndExit("Could not connect to host");
	}
	return socketHandle;
}

void sendRequest(int sock, char* host, char* path) {
	char request[MAX_HTTP_SIZE];
	buildRequest(request, host, path);
	write(sock, request, strlen(request));
}

void buildRequest(char result[], char* host, char* path) {
	strcat(result, "GET ");
	strcat(result, path);
	strcat(result, " HTTP/1.0");
	strcat(result, "\r\nHost: ");
	strcat(result, host);
	strcat(result, "\r\nAccept: */*");
	strcat(result, "\r\nContent-Type: text/html");
	strcat(result, "\r\nContent-Length: 0");
	strcat(result, "\r\n\r\n");
}

void printDebug(char* request, char* headers) {
	printf(request);
	printf(headers);
}

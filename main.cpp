#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <stdexcept>
#include "download.h"

using namespace std;

void usage();
void parseFlags(int argc, char* argv[], bool &debug);
char* getArg(int argc, char* argv[], int &i);

int main(int argc, char* argv[]) {
	if(argc < 4) {
		usage();
	} else {
		int i = 1;
		char* host = getArg(argc, argv, i);
		char* portStr = getArg(argc, argv, i);
		int port = atoi(portStr);
		if(port == 0) {
			errorAndExit("Port must be an integer greater than zero");
		}
		char* path = getArg(argc, argv, i); 
		char* countStr = getArg(argc, argv, i);
		int count = atoi(countStr);
		if(count == 0) {
			errorAndExit("Count must be an integer greater than zero");
		}
		bool debug = false;
		parseFlags(argc, argv, debug);
		webtest(host, port, path, count, debug);
		//download(host, port, path, count, debug);
	}
}

void usage() {
	errorAndExit("Usage: webtest host-name port path count\n");
}

void parseFlags(int argc, char* argv[], bool &debug) {
	int f;
	char* fvalue = NULL;
	int index;
	while((f = getopt(argc, argv, "c:d")) != -1) {
		switch(f) {
			case 'd':
				debug = true;
				break;
			case '?':
				usage();
				break;
		}
	}
}

char* getArg(int argc, char* argv[], int &i) {
	while(i < argc) {
		if(argv[i][0] != '-') {
			char* result =  argv[i];
			i++;
			return result;
		} 
		i++;
	}
}

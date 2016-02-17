#ifndef DOWNLOAD_H
#define ADD_H

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include "utils.h"

using namespace std;

void webtest(char* host, int port, char* path, int count, bool debug);

#endif

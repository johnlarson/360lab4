#include "utils.h"

using namespace std;

void errorAndExit(char* msg) {
	perror(msg);
	exit(EXIT_SUCCESS);
}

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <time.h>
#include <pthread.h>
#include <vector>

#include "Hospital.h"

using namespace std;

struct department{
		string dep_name;
		int lower, upper;
	};
struct Hospital_data{
	string name;
	department d[3]; // Assumption 1
};

class Healthcenter_server{
	struct arg_struct{
	    int* arg1;
	    Healthcenter_server* arg2;
	};
	vector<Hospital_data*> hospital_list;
	// vector<int*> student_list;
public:
	~Healthcenter_server();
	int setup();
	void get_hospital_data(int socket);
};
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <netdb.h>
using namespace std;

class Hospital{
	struct department{
		string dep_name;
		int lower, upper;
	};
public:
	string name;
	department d[3]; // Assumption 1
	Hospital();
	Hospital(char* name);
	~Hospital();
	int DISCOVERY();
	void send_hospital_data(char *h);

};
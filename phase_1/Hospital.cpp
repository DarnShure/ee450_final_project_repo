#include "Hospital.h"

using namespace std;

#define PORT "8080"
#define MAXDATASIZE 100 // max number of bytes we can get at once 

int sockfd;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) // From beej. See source 2
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Hospital::Hospital(){
}
Hospital::~Hospital(){
	close(sockfd);
}
Hospital::Hospital(char* hospital_file_name){ // Load datasturcutre from filename
	int length, count = 0; 
	string buffer;
	ifstream infile;
	infile.open(hospital_file_name);
	while(getline(infile,buffer)){ // Parse severity and departments
		this->name = buffer.substr(0,1);
		this->d[count].dep_name= buffer.substr(0,2);
		this->d[count].lower = stoi(buffer.substr(3, buffer.find(',')));
		this->d[count].upper = stoi(buffer.substr(buffer.find(',')+1, buffer.find("\n")));
		count++;
	} 
	infile.close();
}

int Hospital::DISCOVERY(){ //from example code provided
	int numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	const char* hostname = "ee250-VirtualBox"; // Assumption 2: Manual hostname

	if ((rv = getaddrinfo(hostname, PORT, &hints, &servinfo)) != 0) { //  Get server info from PORT and hostname
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) { // Checking for possibility of multiple sockets at desired port
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	// Printing IP address soluton from stack exchange. See source 5 in readme.txt
	getsockname(sockfd, p->ai_addr, &p->ai_addrlen);
	fprintf(stderr, "Hospital %s has TCP Port %d and IP address %s\n", name.c_str(),ntohs(((sockaddr_in *)p->ai_addr)->sin_port), 
            inet_ntoa(((sockaddr_in *)p->ai_addr)->sin_addr));
	freeaddrinfo(servinfo); // all done with this structure
	cout << "Hospital " << name << " is now connected to the health center"<<endl;
};

void Hospital::send_hospital_data(char *h){
	int count = 0;
	int length;
	// Adapted from stack exchange source 4 -see readme.txt
	ifstream infile;
	infile.open(h);
	infile.seekg(0, std::ios::end);    // go to the end
	length = infile.tellg();           // report location (this is the length)
	infile.seekg(0, std::ios::beg);    // go back to the beginning
	char buffer[length];    // allocate memory for a buffer of appropriate dimension
	infile.read(buffer, length);       // read the whole file into the buffer
	
	while(count < 3){ // Printing based on format.
		printf("Hospital %s has sent department %s to the health center\n", name.c_str(), (this->d[count].dep_name).c_str());
		count++;
	}
	send(sockfd,buffer, length, 0);
	cout << "Updating the admission office(Health Center?) is done for " << name << endl;
	cout << "End of Phase 1 for Hospital " << name << endl;
	infile.close(); 
}

int main(int argc, char const *argv[]){
	char* hospital_file[3]; // Array to store hospital file names.
	hospital_file[0] = "HospitalA.txt";
	hospital_file[1] = "HospitalB.txt";
	hospital_file[2] = "HospitalC.txt";
	for(int i = 0; i < 3; i++){
		if(fork() == 0){ // Run this section only in child process (3x).
			Hospital a(hospital_file[i]); // Construct hospital data.		
			a.DISCOVERY(); // Connect to server.
			a.send_hospital_data(hospital_file[i]); // Send data.
			break;
		}
		else{
			continue;
		}
	}
}

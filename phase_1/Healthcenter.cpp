#include "Healthcenter.h"

using namespace std;

#define PORT "8080"
#define BACKLOG 100

int *new_socket;
int Healthcenter_server_fd;
int count;

void *get_in_addr(struct sockaddr *sa) // Adapted from simple server example. See readme.txt source 2
{
	if (sa->sa_family == AF_INET) {
	return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void sigchld_handler(int s) // Adapted from simple server example. See readme.txt source 2
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}	

Healthcenter_server::~Healthcenter_server(){ 
    while(!hospital_list.empty()) { // Dellaocating and popping linked list
        delete hospital_list.back();
        hospital_list.pop_back();
    }
}


int Healthcenter_server::setup(){ // Adapted from simple server example. See readme.txt source 2
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) { // Get servinfo
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) { // Iterate through P looking for first viable address
		if ((sockfd = socket(p->ai_family, p->ai_socktype, //  Create socket
				p->ai_protocol)) == -1) {
			perror("server: socket"); 
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) { // Bind socket to address from getaddr
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	cout << "The health center has TCP port 8080 and IP address 120.0.0.1 for Phase 1"<<endl;

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) { // Listen
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); // Accept
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		
		// inet_ntop(their_addr.ss_family, // Not required in project message format but useful for debug
		// 	get_in_addr((struct sockaddr *)&their_addr),
		// 	s, sizeof s);
		// printf("server: got connection from %s\n", s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			get_hospital_data(new_fd); // Get hospital data. Assuming they have sent it.
			close(new_fd);
			exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}



}

void Healthcenter_server::get_hospital_data(int socket){
	
	count = 0;
	char recieved[1024] = {0};
	read(socket, recieved, 1024);
	string buffer(recieved);
	Hospital_data *new_entry = new Hospital_data; // Using hospital class a data container
	// Parse buffer into new_entry
	while(count < 3){ // count < 3 : Assumption 1. See readme.txt
		new_entry->name = buffer.substr(0,1); 
		new_entry->d[count].dep_name = buffer.substr(0,2);
		new_entry->d[count].lower = stoi(buffer.substr(3, buffer.find(',')-3));
		new_entry->d[count].upper = stoi(buffer.substr(buffer.find(',')+1, buffer.find("\n")-buffer.find(',')-1));
		// Enable to debug:
		// cout << new_entry->d[count].dep_name<< " : "<< new_entry->d[count].lower << " , "<< new_entry->d[count].upper << endl;
		buffer.erase(0,buffer.find("\n")+1);
		count++;
	}
	hospital_list.push_back(new_entry); // Adding to vector hospital_list
	cout << "Received the department list from Hospital " << new_entry->name << endl;
	cout << "End of Phase 1 for the health center." << endl;

}

int main(int argc, char const *argv[]){
	Healthcenter_server b;
	b.setup();
}
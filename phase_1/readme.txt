Darren Hsu | 8787302072
June, 22, 2020
EE450
				Final Project Phase 1: TCP connection between Hospital and Health Center

Description: 
	Run Healthcenter first, then Hospital. Hospital spawns 3 different processes that send the department data to the server Healthcenter on TCP connection. Healthcenter stores data in a dynamically allocated linked list of Hospital_data structs.

Usage: 
	g++ Healthcenter.cpp -o healthcenter 
	./healthcenter
	(New terminal)
	g++ Hospital.cpp -o hospital
	./hospital

Notes:
	Files included: Hospital.cpp, Hospital.h, Healthcenter.cpp, Healthcenter.h
	Input file are: HospitalA.cpp, HospitalB.cpp, HospitalC.cpp

Problems:

Assumptions:
	1.)Each hospital only has three departments.
	2.)Manual hardcoded "hostname" used in getaddrinfo() in Hospital.cpp in Discovery().
	3.) Input files format must have "\n" at the very end. Cannot end on other character due to parsing rules.

Sources:
	1.)Example code provided by class (Can be found in samples folder)
	2.)Beej's guide to network programming: pg 27-30 (Simple server and client)
	3.)Building on code from lab 2.
	4.)https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
	5.)https://stackoverflow.com/questions/5759031/getsockname-always-returning-0-0-0-0/16162840
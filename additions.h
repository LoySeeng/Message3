#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MESSAGE_BUFFER 4096
#define PORT 7777

using namespace std;

char buffer[MESSAGE_BUFFER];
char line[MESSAGE_BUFFER];
int socket_descriptor, message_size;;
struct sockaddr_in serveraddress;

string username;
string Answer;

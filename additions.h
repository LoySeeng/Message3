#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MESSAGE_BUFFER 4096
#define PORT 7777

char buffer[MESSAGE_BUFFER];
char message[MESSAGE_BUFFER];
int socket_file_descriptor, message_size;
socklen_t length;
struct sockaddr_in serveraddress, client;


char choice1[] = "1";
char choice2[] = "2";
char choice4[] = "4";
char choice5[] = "5";
char choice6[] = "6";
char choice3[] = "3";

string login;
string pass;
string start_query;
string between_query = "', '";
string end_query = "')";
string where_query;
string query;


string USER;
string ID_USER;

string find_id(string user) {
	string start = "SELECT id FROM user WHERE login = '";
	string end = "')";
	string question;
	question += start;
	question += user;
	question += end;
	int chech = mysql_query(&mysql, question.c_str()); 
	if (check == 0) {
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		string ID = row[0];
		return ID;
	}
	else {
		string False = "False";
		return False;
	}
};


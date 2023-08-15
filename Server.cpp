#include "additions.h"

using namespace std;


int main() {

	cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << endl;

	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if (&mysql == nullptr) {
		cout << "ERROR: Can't create MySQL-descriptor" << endl;
	}

	if (!mysql_real_connect(&mysql, "localhost", "root", "HanJisung_13", "messenger", NULL, NULL, 0)) {
		cout << "ERROR: Can't connect to database " << mysql_error(&mysql) << endl;
	}

	socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddress.sin_port = htons(PORT);
	serveraddress.sin_family = AF_INET;
	bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));

	while (1) {

		length = sizeof(client);
		memset(buffer, 0, MESSAGE_BUFFER);
		message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
		buffer[message_size] = '\0';

		// Sign Up
		if (strcmp(buffer, choice1) == 0) {
			// Получаем логин
			memset(buffer, 0, MESSAGE_BUFFER);
			message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
			buffer[message_size] = '\0';
			login.clear();
			login = buffer;
			start_query.clear();
			start_query = "INSERT INTO user(id_user, login) VALUES(default, '";
			query.clear();
			query += start_query;
			query += login;
			query += end_query;
			int CHECK = mysql_query(&mysql, query.c_str());
			if (CHECK == 0) {
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "True");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				// После проверки логина подгружаем пароль
				memset(buffer, 0, MESSAGE_BUFFER);
				message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
				buffer[message_size] = '\0';
				pass.clear();
				pass = buffer;
				start_query.clear();
				start_query = "UPDATE user_protected SET pass_hash = '";
				where_query = "WHERE login_protected = '";
				query += start_query;
				query += pass;
				query += "' ";
				query += where_query;
				query += login;
				query += "'";
				CHECK = mysql_query(&mysql, query.c_str());
				if (CHECK == 0) {
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, "True");
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				}
				else {
					cout << "Ошибка MySql номер " << mysql_error(&mysql);
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, "False");
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				}
			}
			else {
				cout << "Ошибка MySql номер " << mysql_error(&mysql);
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "False");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
			}
		}

		// Sign In
		if (strcmp(buffer, choice2) == 0) {
			// Получаем логин
			memset(buffer, 0, MESSAGE_BUFFER);
			message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
			buffer[message_size] = '\0';
			login.clear();
			login = buffer;
			start_query.clear();
			start_query = "SELECT pass_hash FROM user_protected WHERE login_protected = '";
			query.clear();
			query += start_query;
			query += login;
			query += "'";
			int CHECK = mysql_query(&mysql, query.c_str());
			if (CHECK == 0) {
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "True");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				// Получаем пароль после проверки логина
				res = mysql_store_result(&mysql);
				row = mysql_fetch_row(res);
				string pass_hash = row[0];
				memset(buffer, 0, MESSAGE_BUFFER);
				message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
				buffer[message_size] = '\0';
				pass.clear();
				pass = buffer;
				if (pass_hash == pass) {
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, "True");
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
					USER = login;
				}
				else {
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, "False");
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				}
			}
			else {
				cout << "Ошибка MySql номер " << mysql_error(&mysql);
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "False");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
			}
		}

		// Check mailbox
		if (strcmp(buffer, choice4) == 0) {
			// Находим id получателя 
			string id_reciver = find_id(USER);
			// Собираем id отправителей сообщений
			start_query.clear();
			start_query = "SELECT id_sender FROM message WHERE id_reciver = '";
			query.clear();
			query += start_query;
			query += id_reciver;
			query += "'";
			int CHECK = mysql_query(&mysql, query.c_str());
			// Проверка есть ли сообщения
			if (CHECK == 0) {
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "True");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				res = mysql_store_result(&mysql);
				vector <string> ALL_Senders_ID;
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < mysql_num_fields(res); i++) {
						ALL_Senders_ID.push_back(row[i]);
					}
				}
				// Собираем логины отправителей
				start_query.clear();
				start_query = "SELECT login FROM user WHERE id_user = '";
				query.clear();
				query += start_query;
				query += id_reciver;
				query += "'";
				vector <string> ALL_Senders_login;
				res = mysql_store_result(&mysql);
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < mysql_num_fields(res); i++) {
						ALL_Senders_login.push_back(row[i]);
					}
				}
				// Собираем сообщения
				start_query.clear();
				start_query = "SELECT message FROM message WHERE id_reciver = '";
				query.clear();
				query += start_query;
				query += id_reciver;
				query += "'";
				mysql_query(&mysql, query.c_str());
				res = mysql_store_result(&mysql);
				vector <string> ALL_SMS;
				int mes_count;
				mes_count = mysql_num_fields(res);
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < mysql_num_fields(res); i++) {
						ALL_SMS.push_back(row[i]);
					}
				}
				// Отправляем количесво сообщений
				memset(message, 0, MESSAGE_BUFFER);
				itoa(mes_count, message, 10);
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				// Отправляем связку отправитель + сообщение
				for (int i = 0; i < mysql_num_fields(res); i++) {
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, ALL_Senders_login[i].c_str());
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, ALL_SMS[i].c_str());
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				}
			}
			else {
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "False");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
			}
		}

		// Send a message to one peron
		if (strcmp(buffer, choice5) == 0) {
			// Получение id отправителя
			string id_sender = find_id(USER);
			// Получение id получателя
			memset(buffer, 0, MESSAGE_BUFFER);
			message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
			buffer[message_size] = '\0';
			string reciver = buffer;
			string id_reciver = find_id(reciver);
			if (id_reciver == "False") {
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "False");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
			}
			else {
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "True");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				// Получаем сообщение
				memset(buffer, 0, MESSAGE_BUFFER);
				message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
				buffer[message_size] = '\0';
				string one_SMS = buffer;
				start_query.clear();
				start_query = "INSERT INTO message(id_mes, id_sender, id_reciver, message) VALUES(default, '";
				query.clear();
				query += start_query;
				query += id_sender;
				query += between_query;
				query += id_reciver;
				query += between_query;
				query += one_SMS;
				query += end_query;
				int CHECK = mysql_query(&mysql, query.c_str());
				if (CHECK == 0) {
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, "True");
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				}
				else {
					cout << "Ошибка MySql номер " << mysql_error(&mysql);
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, "False");
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				}
			}
		}

		// Send a message to everyone
		if (strcmp(buffer, choice6) == 0) {
			// Получение id отправителя
			string id_sender = find_id(USER);
			// Получение id всех
			start_query.clear();
			start_query = "SELECT id_user FROM user WHERE id_user != '";
			query.clear();
			query += start_query;
			query += id_sender;
			query += "'";
			mysql_query(&mysql, query.c_str());
			if (res = mysql_store_result(&mysql)) {
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "True");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				vector <string> ALL_Users;
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < mysql_num_fields(res); i++) {
						ALL_Users.push_back(row[i]);
					}
				}
				// Вставляем сообщения для всех
				memset(buffer, 0, MESSAGE_BUFFER);
				message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
				buffer[message_size] = '\0';
				string one_SMS = buffer;
				start_query.clear();
				start_query = "INSERT INTO message(id_mes, id_sender, id_reciver, message) VALUES(default, '";
				query.clear();
				int Error_Count = 0;
				for (int i = 0; i < mysql_num_fields(res); i++) {
					query += start_query;
					query += id_sender;
					query += between_query;
					query += ALL_Users[i];
					query += between_query;
					query += one_SMS;
					query += end_query;
					int CHECK = mysql_query(&mysql, query.c_str());
					if (CHECK != 0) {
						cout << "Ошибка MySql номер " << mysql_error(&mysql);
						Error_Count++;
						cout << Error_Count << " errors occurred" << endl;
					}
				}
				if (Error_Count == 0) {
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, "True");
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				}
				else {
					memset(message, 0, MESSAGE_BUFFER);
					strcat(message, "False");
					sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				}
			}
			else {
				cout << "Ошибка MySql номер " << mysql_error(&mysql);
				memset(message, 0, MESSAGE_BUFFER);
				strcat(message, "False");
				sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
			}
		}

		// Exit
		if (strcmp(buffer, choice3) == 0) {
			mysql_close(&mysql);
			close(socket_file_descriptor);
			exit(0);
		}

	}

	return 0;
}


#include "additions.h"

void page1() {
    int flag = 0;
    int choice;

    std::cout << "\t \n \n Welcome to the Messenger '666'! \n" << std::endl;

    while (1) {
        std::cout << "\nWhat do you want to do:" << std::endl;
        std::cout << "1 - Sign Up, 2 - Sign In, 3 - Run away \n" << std::endl;
        std::cin >> choice;
        std::cout << "\n";
        switch (choice) {
        // Sign Up
        case 1: {
            memset(line, 0, MESSAGE_BUFFER);
            itoa(choice, line, 10);
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            // Отправляем логин
            std::string username;
            std::cout << "Create your login: ";
            std::cin >> username;
            memset(line, 0, MESSAGE_BUFFER);
            strcat(line, username.c_str());
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            // Получаем сообщение с сервера с проверкой логина
            memset(buffer, 0, MESSAGE_BUFFER);
            message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
            buffer[message_size] = '\0';
            Answer.clear();
            Answer = buffer;
            if (Answer == "True") {
                // Отправляем пароль
                std::string pass;
                std::cout << "Create your password: ";
                std::cin >> pass;
                memset(line, 0, MESSAGE_BUFFER);
                strcat(line, pass.c_str());
                sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
                // Принимаем сообщение от сервера, что всё хорошо
                memset(buffer, 0, MESSAGE_BUFFER);
                message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
                buffer[message_size] = '\0';
                Answer.clear();
                Answer = buffer;
                if (Answer == "True") {
                    cout << "You've signed up successfully." << endl;
                    break;
                }
                else {
                    cout << "Someting's went wrong. Please, try again later." << endl;
                    break;
                }
            }
            else {
                cout << "Sorry. The login is already occupied. Try with another one." << endl;
                break;
            }
        }
        // Sign In
        case 2: {
            memset(line, 0, MESSAGE_BUFFER);
            itoa(choice, line, 10);
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            // Отправляем логин
            std::string log_username;
            std::cout << "Enter your login: ";
            std::cin >> log_username;
            memset(line, 0, MESSAGE_BUFFER);
            strcat(line, log_username.c_str());
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            // Получаем ответ от сервера о логине
            memset(buffer, 0, MESSAGE_BUFFER);
            message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
            buffer[message_size] = '\0';
            Answer.clear();
            Answer = buffer;
            if (Answer == "True") {
                // Отправляем пароль
                std::cout << "Enter your password: ";
                std::string log_pass;
                std::cin >> log_pass;
                memset(line, 0, MESSAGE_BUFFER);
                strcat(line, log_pass.c_str());
                sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
                // Получаем ответ от сервера о правильности пароля
                memset(buffer, 0, MESSAGE_BUFFER);
                message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
                buffer[message_size] = '\0';
                Answer.clear();
                Answer = buffer;
                if (Answer == "True") {
                    username = log_username;
                    cout << "\nWelcome " << username << "!! You've logged in successfully." << endl;
                    flag = 1;
                    page2(username);
                    break;
                }
                else {
                    cout << "Password is wrong. Try again." << endl;
                    break;
                }
            }
            else {
                cout << "There is no user with this username. Try again." << endl;
                break;
            }
        }
        // Exit
        case 3: {
            memset(line, 0, MESSAGE_BUFFER);
            itoa(choice, line, 10);
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            close(socket_descriptor);
            system("cls");
            std::cout << "\nYou could never run away... Just kidding. Goodbye, User!" << std::endl;
            exit(666);
            break;
        }
        default: {
            std::cout << "Please, choose a number between 1 - 3" << std::endl;
            break;
        }
        }
        
        if (flag == 1) {
            break;
        }
    }
}


void page2(std::string uname) {
    system("cls");

    int choice2;

    do {
        std::cout << "\n \n \n \n \t \t \t Welcome, " << uname << "! \n" << std::endl;

        std::cout << "What do you want to do:\n" << std::endl;
        std::cout << "4 - Check mailbox \n5 - Send a message to the person \n6 - Send a message to everyone \n7 - Log Out \n" << std::endl;

        std::cin >> choice2;
        switch (choice2) {
        // Check mailbox
        case 4: {
            memset(line, 0, MESSAGE_BUFFER);
            itoa(choice2, line, 10);
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            // Узнаём от сервера, есть ли сообщения
            memset(buffer, 0, MESSAGE_BUFFER);
            message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
            buffer[message_size] = '\0';
            Answer.clear();
            Answer = buffer;
            if (Answer == "True") {
                // Получаем сообщения от сервера
                memset(buffer, 0, MESSAGE_BUFFER);
                message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
                buffer[message_size] = '\0';
                Answer.clear();
                Answer = buffer;
                int mes_count = stoi(Answer);
                for (int i = 0; i < mes_count; i++) {
                    memset(buffer, 0, MESSAGE_BUFFER);
                    message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
                    buffer[message_size] = '\0';
                    cout << i << "\t\a. From: " << buffer << endl;
                    memset(buffer, 0, MESSAGE_BUFFER);
                    message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
                    buffer[message_size] = '\0';
                    cout << "Message: " << buffer << "\n" << endl;
                }
                break;
            }
            else {
                cout << "Your mailbox is empty." << endl;
                break;
            }
        }
        // Send a message to one person
        case 5: {
            memset(line, 0, MESSAGE_BUFFER);
            itoa(choice2, line, 10);
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            // Отправляем логин получателя
            std::cout << "\nWho do you want to send a message to? To ";
            std::string reciepient;
            std::cin >> reciepient;
            memset(line, 0, MESSAGE_BUFFER);
            strcat(line, reciepient.c_str());
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            // Проверяем есть ли такой пользователь
            memset(buffer, 0, MESSAGE_BUFFER);
            message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
            buffer[message_size] = '\0';
            Answer.clear();
            Answer = buffer;
            if (Answer == "True") {
                // Отправляем сообщение
                std::string message;
                std::cout << "\nEnter your message: \n" << std::endl;
                std::cout << "\t";
                std::cin.ignore();
                std::cin >> message;
                memset(line, 0, MESSAGE_BUFFER);
                strcat(line, message.c_str());
                sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
                // Проверяем всё ли успешно
                memset(buffer, 0, MESSAGE_BUFFER);
                message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
                buffer[message_size] = '\0';
                Answer.clear();
                Answer = buffer;
                if (Answer == "True") {
                    cout << "\nYour message has been sent successfully." << endl;
                    break;
                }
                else {
                    cout << "Something's went wrong. Try again later." << endl;
                    break;
                }
            }
            else {
                cout << "This user is not found, please, check the login." << endl;
                break;
            }
        }
        // Send a message to everyone
        case 6: {
            memset(line, 0, MESSAGE_BUFFER);
            itoa(choice2, line, 10);
            sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            // Проверяем есть ли вообще другие пользователи на сервере
            // memset(buffer, 0, MESSAGE_BUFFER);
            message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
            buffer[message_size] = '\0';
            Answer.clear();
            Answer = buffer;
            if (Answer == "True") {
                // Отправляем сообщение на сервер
                std::string message;
                std::cout << "\nEnter your message:" << std::endl;
                std::cout << "\t";
                std::cin.ignore();
                std::getline(std::cin, message);
                memset(line, 0, MESSAGE_BUFFER);
                strcat(line, message.c_str());
                sendto(socket_descriptor, line, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
                // Проверяем удалось ли доставить
                memset(buffer, 0, MESSAGE_BUFFER);
                message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
                buffer[message_size] = '\0';
                Answer.clear();
                Answer = buffer;
                if (Answer == "True") {
                    cout << "\nYour message has been sent successfully to everyone." << endl;
                    break;
                }
                else {
                    cout << "\nErrors occurred during message delivery." << endl;
                    break;
                }
            }
            else {
                cout << "There are no other users on server. Please, try later." << endl;
                break;
            }
        }
        // Log out
        case 7: {
            system("cls");
            std::cout << "You have been logged out" << std::endl;
            page1();
            break;
        }
        default: {
            std::cout << "Please, choose a number between 1 - 4" << std::endl;
            break;
        }
        }
    } while (choice2 != 4);
}



int main() {
    std::cout << "CLIENT IS ESTABLISHING A CONNECTION WITH SERVER THROUGH PORT: " << PORT << std::endl;
    
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (connect(socket_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) < 0) {
        cout << endl << "Something went wrong: Connection Failed" << endl;
        exit(1);
    }

    page1();

    return 0;
}

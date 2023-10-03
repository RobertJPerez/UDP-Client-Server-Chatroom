
#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>

constexpr int BUFF_SIZE = 1024;
constexpr int PORT = 8080;
constexpr char SERVER_IP[] = "127.0.0.1";

// Function to receive messages from the server
void receive_messages(int client_socket) {
    char buffer[BUFF_SIZE];

    while (true) {
        int bytes_received = recv(client_socket, buffer, BUFF_SIZE - 1, 0);
        if (bytes_received < 0) {
            std::cerr << "Error receiving data" << std::endl;
            continue;
        }

        buffer[bytes_received] = '\0';
        std::cout << buffer << std::endl;
    }
}

int main() {
    // Creates the client socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Sets up the server address
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // While that registers the client with a unique username
    while (true) {
        std::string username;
        std::cout << "Enter your username: ";
        std::cin >> username;

        std::string register_msg = "C" + username;
        sendto(client_socket, register_msg.c_str(), register_msg.size(), 0, (sockaddr*)&server_addr, sizeof(server_addr));

        char buffer[BUFF_SIZE];
        int bytes_received = recv(client_socket, buffer, BUFF_SIZE - 1, 0);
        buffer[bytes_received] = '\0';

        if (std::string(buffer) != "Username Existed") {
            std::cout << buffer << std::endl;
            break;
        } else {
            std::cout << "Username already exists, please try again." << std::endl;
        }
    }

    std::thread(receive_messages, client_socket).detach();

    std::string input;
    // Loop to send messages to the server
    while (true) {
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        std::string msg = "M" + input;
        send(client_socket, msg.c_str(), msg.size(), 0);
    }

    return 0;
}

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <sys/socket.h>

constexpr int BUFF_SIZE = 1024;
constexpr int MAX_MESSAGE_SIZE = 80;
constexpr int PORT = 8080;

std::unordered_map<std::string, sockaddr_in> clients;
std::mutex clients_mutex;


// Function to handle messages from clients
void handle_client(int server_socket, sockaddr_in client_addr, char buffer[]) {
    std::string msg(buffer);
    std::string username = msg.substr(0, msg.find(':'));
    std::string message = msg.substr(msg.find(':') + 1);

    clients_mutex.lock();

    if (message[0] == 'U') { // Unicast
        std::string recipient = message.substr(1, message.find(':') - 1);
        message = message.substr(message.find(':') + 1);
        
        // If recipient is in clients, send the message
        if (clients.find(recipient) != clients.end()) {
            std::string formatted_msg = "From " + username + " (private): " + message;
            sendto(server_socket, formatted_msg.c_str(), formatted_msg.size(), 0, (sockaddr*)&clients[recipient], sizeof(client_addr));
        } else {
            std::string error_msg = "User " + recipient + " not exist";
            sendto(server_socket, error_msg.c_str(), error_msg.size(), 0, (sockaddr*)&client_addr, sizeof(client_addr));
        }
    } else { // Broadcast
        std::string formatted_msg = "From " + username + ": " + message;
        // Send the message to all clients except the sender
        for (const auto& [name, addr] : clients) {
            if (name != username) {
                sendto(server_socket, formatted_msg.c_str(), formatted_msg.size(), 0, (sockaddr*)&addr, sizeof(client_addr));
            }
        }
    }

    clients_mutex.unlock();
}

int main() {
    // Create the server socket
    int server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);


    // Bind the server socket to the address
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    char buffer[BUFF_SIZE];
    while (true) {
        sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        int bytes_received = recvfrom(server_socket, buffer, BUFF_SIZE - 1, 0, (sockaddr*)&client_addr, &addr_len);
        if (bytes_received < 0) {
            std::cerr << "Error receiving data" << std::endl;
            continue;
        }

        buffer[bytes_received] = '\0';

        if (buffer[0] == 'C') { // New client
            std::string username(buffer + 1);
            clients_mutex.lock();
            // Add new client if the username doesn't exist
            if (clients.find(username) == clients.end()) {
                clients[username] = client_addr;

                std::string welcome_msg = "Username: " + username + "\nList of users:\n";
                for (const auto& [name, _] : clients) {
                    welcome_msg += name + "\n";
                }

                sendto(server_socket, welcome_msg.c_str(), welcome_msg.size(), 0, (sockaddr*)&client_addr, sizeof(client_addr));
for (const auto& [name, addr] : clients) {
if (name != username) {
std::string joined_msg = "User " + username + " joined the chatroom";
sendto(server_socket, joined_msg.c_str(), joined_msg.size(), 0, (sockaddr*)&addr, sizeof(client_addr));
}
}
            clients_mutex.unlock();
        } else {
            std::string error_msg = "Username Existed";
            sendto(server_socket, error_msg.c_str(), error_msg.size(), 0, (sockaddr*)&client_addr, sizeof(client_addr));
            clients_mutex.unlock();
        }
    } else { // Send Message
        std::thread(handle_client, server_socket, client_addr, buffer).detach();
    }
}

return 0;
}

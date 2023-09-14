#include "header.h"

int connectTcpServer() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Server's port number
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server's IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server." << std::endl;
        return 1;
    }

    std::cout << "Connected to the server." << std::endl;

    int choice;
    // Send and receive messages
    while (true) {
        std::cout << "Enter 1 to send information packet " << std::endl;
        std::cout << "Enter 2 to send login packet " << std::endl;
        std::cin >> choice;
        uint16_t packetId;
        if (choice == 1) packetId = 0;
        else packetId = 2;

        // Create packet
        Packet* packet = PacketFactory::GetPacket(packetId);
        packet->FillInformation();
        packet->packetId = packetId;
        std::vector<uint8_t> serializedPacket = packet->Serialize();

        // Send packet
        ssize_t bytesSent = send(clientSocket, serializedPacket.data(), serializedPacket.size(), 0);
        if (bytesSent < 0) {
            perror("Sending failed");
            return 1;
        }
        else {
            std::cout << "Sent information packet successfully" << std::endl;
            std::cout << "Sent " << bytesSent << " bytes" << std::endl;
        }
        
        // Receive the response packet
        std::vector<uint8_t> receiveBuffer(1024);
        // memset(receiveBuffer.data(), 0, receiveBuffer.size());
        ssize_t bytesRead = recv(clientSocket, receiveBuffer.data(), receiveBuffer.size(), 0);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving data" << std::endl;
            break;
        }
        receiveBuffer.resize(static_cast<size_t>(bytesRead));

         // Process the response packet 
        Packet* responsePacket;
        responsePacket = PacketFactory::GetPacket(packetId + 1);
        responsePacket->Deserialize(receiveBuffer);
        responsePacket->PrintInformation();

        // Free memory
        delete packet;
        packet = nullptr;
        delete responsePacket;
        responsePacket = nullptr;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}

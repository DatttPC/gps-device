#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/md5.h>

int connectTcpServer();
uint16_t CalculateChecksum(const std::vector<uint8_t>& buffer);
bool ValidateChecksum(std::vector<uint8_t> buffer, uint16_t& checksum);


class Packet{
public:
    uint16_t startMarker;
    uint16_t packetId;
    std::string deviceId;
    uint16_t packetOrderIndex;
    uint16_t checksum;
    uint16_t endMarker;

    void SerializeUInt16(std::vector<uint8_t>& buffer, uint16_t value) const;
    void SerializeUInt32(std::vector<uint8_t>& buffer, uint32_t value) const;
    void SerializeString(std::vector<uint8_t>& buffer, const std::string& value) const;
    bool DeserializeUInt16(const std::vector<uint8_t>& buffer, size_t& offset, uint16_t& value) const;
    bool DeserializeUInt32(const std::vector<uint8_t>& buffer, size_t& offset, uint32_t& value) const;
    bool DeserializeString(const std::vector<uint8_t>& buffer, size_t& offset, std::string& value) const;
    
    void SerializePacketStarting(std::vector<uint8_t>& buffer) const;
    virtual void SerializePacketBody(std::vector<uint8_t>& buffer) const {};
    void SerializePacketEnding(std::vector<uint8_t>& buffer) const;
    virtual std::vector<uint8_t> Serialize() const {};

    bool DeserializePacketStarting(const std::vector<uint8_t>& buffer, size_t& offset);
    virtual bool DeserializePacketBody(const std::vector<uint8_t>& buffer, size_t& offset) {};
    bool DeserializePacketEnding(const std::vector<uint8_t>& buffer, size_t& offset);
    virtual bool Deserialize(const std::vector<uint8_t>& buffer) {};

    void FillStartingInformation();
    virtual void FillBodyInformation() {};
    void FillEndingInformation();
    virtual void FillInformation() {};
    
    virtual void PrintInformation() {};
};

class InformationPacket : public Packet{
    uint32_t latitude;
    uint32_t longitude;

    void SerializePacketBody(std::vector<uint8_t>& buffer) const;
    std::vector<uint8_t> Serialize() const;
    void FillBodyInformation();
    void FillInformation();
};

class InformationResponsePacket : public Packet{
    uint16_t receivedPacketIndex;

    bool DeserializePacketBody(const std::vector<uint8_t>& buffer, size_t& offset);
    bool Deserialize(const std::vector<uint8_t>& buffer);
    void PrintInformation();
};

class LoginPacket : public Packet{
public:
    std::string username;
    std::string password;

    void SerializePacketBody(std::vector<uint8_t>& buffer) const;
    std::vector<uint8_t> Serialize() const;
    void FillBodyInformation();
    void FillInformation();
};

class LoginResponsePacket : public Packet {
public:
    uint16_t receivedPacketIndex;

    bool DeserializePacketBody(const std::vector<uint8_t>& buffer, size_t& offset);
    bool Deserialize(const std::vector<uint8_t>& buffer);
    void PrintInformation();
};

class PacketFactory{
public:
    enum PacketType
    {
        informationPacket = 0,
        informationResponsePacket = 1,
        loginPacket = 2,
        loginResponsePacket = 3
    };
    static Packet* GetPacket(uint16_t packetId);
};

#endif
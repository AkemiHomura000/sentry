/*
*@brief只作为暂时方案，因此只对tcp进行封装，由tcp通信获取对方烧饼状态，注意自瞄开启服务端后再开启
*/
#ifndef ARMOR_RELAY_H
#define ARMOR_RELAY_H

#include <ros/ros.h>
#include <tf2/utils.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_listener.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <sstream>

#include "robot_msg/Armor.h"
class TCPClient
{
private:
    int sock;
    std::string address;
    int port;

public:
    TCPClient(std::string address, int port) : address(address), port(port)
    {
        sock = -1;
    }
    bool setup()
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            std::cerr << "Could not create socket" << std::endl;
            return false;
        }
        struct sockaddr_in server;
        server.sin_addr.s_addr = inet_addr(address.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        // 连接到服务器
        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            std::cerr << "Connection failed" << std::endl;
            return false;
        }
        std::cout << "Connected" << std::endl;
        return true;
    }
    bool send(const std::string &data)
    {
        if (::send(sock, data.c_str(), data.size(), 0) < 0)
        {
            std::cerr << "Send failed" << std::endl;
            return false;
        }
        return true;
    }
    std::string receive()
    {
        char buffer[1024] = {0};
        std::string receivedData;
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0)
        {
            std::cerr << "Receive failed" << std::endl;
        }
        else
        {
            receivedData = std::string(buffer, bytesReceived);
        }
        return receivedData;
    }
    std::vector<std::string> splitString(const std::string &input, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(input);
        std::string token;
        while (std::getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
    ~TCPClient()
    {
        close(sock);
    }
};

#endif
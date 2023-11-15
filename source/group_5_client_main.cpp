/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A client-side networking main to prototype communication
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include "Interfaces/NetWorth/client/ClientInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Agents/PacingAgent.hpp"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Must have an argument for server IP and port\nUsage: ./client [IP] [port]" << std::endl;
        return 1;
    }

    std::string ip_string(argv[1]);
    unsigned short port = stoi(std::string(argv[2]));

    // will probably need to find a way to receive the world from the server first
    // this will be confusing
    // receive initial world, set of agents, etc?? This will have to be separate from ClientInterface
    sf::UdpSocket socket;
    sf::Packet send_pkt, recv_pkt;
    std::optional<sf::IpAddress> ip_addr = sf::IpAddress::resolve(ip_string);
    std::string serialized;

    send_pkt << "Requesting connection";
    if (socket.send(send_pkt, ip_addr.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
        return 1;
    }
    if (socket.receive(recv_pkt, ip_addr, port) !=  sf::Socket::Status::Done) {
        std::cerr << "Failed to receive" << std::endl;
        return 1;
    }
    recv_pkt >> serialized;
    cse491::MazeWorld world(serialized);
    port = 55002;

    world.AddAgent<netWorth::ClientInterface>("Interface", "ip", ip_string, "port", port).SetProperty("symbol", '@');
    //world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    //world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);

    world.Run();
    return 0;
}

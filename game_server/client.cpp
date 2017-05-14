/**
 * client.cpp
 *
 * Jigar and Chris worked on this
 */

#include "client.hpp"

#define MAX_RECV_LENGTH 100000

void UDPClient::send(const std::string &message) {
    std::cout << message.size() << std::endl;

    socket_.send_to(boost::asio::buffer(message, message.size()), receiver_endpoint_);

    size_t len = socket_.receive_from(boost::asio::buffer(recv_buf), sender_endpoint_);

    std::cout.write(recv_buf.data(), len);
}

void UDPClient::send_keystrokes(std::vector<int> keystrokes, std::string ship_id) {
    std::cout << ship_id << std::endl;
}

/**
 * client.cpp
 *
 * Jigar and Chris worked on this
 */

#include "client.hpp"

#define MAX_RECV_LENGTH 100000

size_t UDPClient::send(const std::string &message) {
    socket_.send_to(boost::asio::buffer(message, message.size()), receiver_endpoint_);
    size_t len = socket_.receive_from(boost::asio::buffer(recv_buf), sender_endpoint_);
    return len;
}

void UDPClient::send_keystrokes(std::vector<int> keystrokes, int ship_id) {
    keystrokes_obj keystrokes_to_send(ship_id, keystrokes);
    const char *serialized_keystrokes = serialize_keystrokes(keystrokes_to_send).c_str();
    socket_.send_to(boost::asio::buffer(serialized_keystrokes, strlen(serialized_keystrokes)), receiver_endpoint_);
}

void UDPClient::get_gamestate() {
    size_t gamestate_len = UDPClient::send("gamestate");
    std::string serialized_gamestate(recv_buf.data(), gamestate_len);
    std::cout << serialized_gamestate << std::endl;
    // return deserilize_gamestate(serialized_gamestate, false);
}

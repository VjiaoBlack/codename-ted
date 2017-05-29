/**
 * client.cpp
 *
 * Chris worked on this
 */

#include "client.hpp"


int TCPClient::register_player() {
    const std::string message = "register";
    socket_.send(boost::asio::buffer(message, message.size()));

    for (;;) {
        size_t len = socket_.read_some(boost::asio::buffer(recv_buf), error);
        if (len) {
            std::string registration_id_as_str(recv_buf.data(), len);
            std::cout << "Registration id" << std::endl;
        }

        if (error == boost::asio::error::eof) {
            break; // Connection closed cleanly by peer.
        } else if (error){
            throw boost::system::system_error(error); // Some other error.
        }
    }

    return 1;
}

size_t UDPClient::send(const std::string &message) {
    socket_.send_to(boost::asio::buffer(message, message.size()), receiver_endpoint_);
    size_t len = socket_.receive_from(boost::asio::buffer(recv_buf), sender_endpoint_);
    return len;
}

void UDPClient::send_keystrokes(std::vector<int> keystrokes, int ship_id) {
    keystrokes_obj keystrokes_to_send(ship_id, keystrokes);
    std::string message = serialize_keystrokes(keystrokes_to_send);
    // const char* lolll = serialize_keystrokes(keystrokes_to_send).c_str();
    // socket_.send_to(boost::asio::buffer(lolll, strlen(lolll)), receiver_endpoint_);
    socket_.send_to(boost::asio::buffer(message), receiver_endpoint_);
}

PiGameState UDPClient::get_gamestate() {
    size_t gamestate_len = UDPClient::send("gamestate");
    std::string serialized_gamestate(recv_buf.data(), gamestate_len);
    std::cout << "Client received: " << serialized_gamestate << std::endl;
    return deserialize_gamestate(serialized_gamestate, false);
}

/**
 * client.cpp
 *
 * Chris worked on this
 */

#include "client.hpp"

// Must have called register_player before start_game
void TCPClient::start_game(int player_id) {
    const std::string message = "start, " + to_string(player_id);
    socket_.send(boost::asio::buffer(message, message.size()));
}


int TCPClient::register_player() {
    boost::asio::connect(socket_, endpoint_iterator_);
    const std::string message = "register";
    socket_.send(boost::asio::buffer(message, message.size()));

    int registration_id;

    for (;;) {
        size_t len = socket_.read_some(boost::asio::buffer(recv_buf), error);
        if (len) {
            registration_id = (recv_buf.data()[0] - '0');
            std::cout << registration_id << std::endl;
            return registration_id;
        }

        if (error == boost::asio::error::eof) {
            break; // Connection closed cleanly by peer.
        } else if (error){
            throw boost::system::system_error(error); // Some other error.
        }
    }

    // If we get to the end somehow then return -1 to show something is very wrong
    return -1;
}

size_t UDPClient::send(const std::string &message) {
    socket_.send_to(boost::asio::buffer(message, message.size()), receiver_endpoint_);
    size_t len = socket_.receive_from(boost::asio::buffer(recv_buf), sender_endpoint_);
    return len;
}

void UDPClient::send_keystrokes(std::vector<int> keystrokes) {
    keystrokes_obj keystrokes_to_send(player_id_, keystrokes);
    std::string message = serialize_keystrokes(keystrokes_to_send);
    // const char* lolll = serialize_keystrokes(keystrokes_to_send).c_str();
    // socket_.send_to(boost::asio::buffer(lolll, strlen(lolll)), receiver_endpoint_);
    socket_.send_to(boost::asio::buffer(message), receiver_endpoint_);
}

PiGameState UDPClient::get_gamestate() {
    size_t gamestate_len = UDPClient::send("gamestate");
    std::string serialized_gamestate(recv_buf.data(), gamestate_len);
    std::cout << serialized_gamestate << std::endl;
    return deserialize_gamestate(serialized_gamestate, false);
}

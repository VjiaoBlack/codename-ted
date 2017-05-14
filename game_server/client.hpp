/**
 * client.hpp
 *
 * worked on by Jigar, Chris 
 */
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

#define MAX_RECV_LENGTH 100000

// Inspiration from https://gist.github.com/kaimallea/e112f5c22fe8ca6dc627
class UDPClient {
public:
    UDPClient(boost::asio::io_service &io_service,
              const std::string &host,
              const std::string &port)
        : io_service_(io_service)
        , socket_(io_service, udp::endpoint(udp::v4(), 0)) {
        udp::resolver resolver(io_service_);
        udp::resolver::query query(udp::v4(), host, port);
        udp::resolver::iterator iter = resolver.resolve(query);
        receiver_endpoint_ = *iter;
    }

    ~UDPClient() {
        socket_.close();
    }

    void send(const std::string &message);
    void send_keystrokes(std::vector<int> keystrokes, std::string ship_id);

private:
    boost::asio::io_service &io_service_;
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    udp::endpoint receiver_endpoint_;
    boost::array<char, MAX_RECV_LENGTH> recv_buf;
};


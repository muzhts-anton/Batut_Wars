#include "Client.h"

#include "Network.h"

namespace network {

Client::Client() : _is_connected(false) {}

Client::Client(std::pair<sf::IpAddress, uint16_t> const &adress) {
    set_adress(adress);
}

size_t Client::get_id() const {
    return _id;
}

void Client::send_actions() {
    sf::Packet output_packet;
    output_packet << _actions;
    _server.send(output_packet);
}

std::queue<Player::Action>& Client::get_actions() {
    return _actions;
}

void Client::set_adress(std::pair<sf::IpAddress, uint16_t> const &adress) {
    _server.connect(adress.first, adress.second);
    _server.setBlocking(true);
    sf::Packet input_packet;
    _server.receive(input_packet);
    int tmp;
    input_packet >> tmp;
    _id = static_cast<size_t>(tmp);
    _server.setBlocking(false);
    _is_connected = true;
}

std::vector<std::vector<Status>> Client::get_status() {
    sf::Packet input_packet;
    std::vector<std::vector<Status>> status;
    _server.receive(input_packet);
    input_packet >> status;
    return status;
}

bool Client::is_connected() const {
    return _is_connected;
}

}  // namespace network
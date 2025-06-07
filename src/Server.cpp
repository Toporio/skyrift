#include "Server.hpp"
#include "Stage.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Network.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <iostream>
sf::UdpSocket socket;
sf::Clock game_clock;
sf::Time time_since_last_broadcast = sf::Time::Zero;
const sf::Time broadcast_interval = sf::seconds(1.f / 20.f);

Server::Server(unsigned short port, const GameSettings &settings)
    : stage({4, 2, 2}), port(port), game_settings({4, 2, 2}) {
  socket.bind(port);
  socket.setBlocking(false);
}
void Server::run() {
  is_running = true;
  std::cout << "Server started on port: " << port << std::endl;
  sf::Clock clock;
  while (is_running) {
    sf::Time delta_time = clock.restart();
    receive_packets(delta_time.asSeconds());
    stage.update(delta_time.asSeconds());
    time_since_last_broadcast += delta_time;
    if (time_since_last_broadcast >= broadcast_interval) {
      broadcast_snapshot();
      time_since_last_broadcast -= broadcast_interval;
    }
    check_timeouts(delta_time.asSeconds());
    sf::sleep(sf::milliseconds(1));
  }
}
void Server::receive_packets(float delta_time) {
  sf::Packet packet;
  std::optional<sf::IpAddress> sender_ip;
  unsigned short sender_port;
  while (socket.receive(packet, sender_ip, sender_port) ==
         sf::Socket::Status::Done) {
    if (sender_ip) {
      std::string sender_key =
          sender_ip->toString() + ":" + std::to_string(sender_port);
      auto it = players_by_address.find(sender_key);
      if (it != players_by_address.end()) {
        handle_player_input(packet, it->second, delta_time);
      } else {
        handle_new_connection(*sender_ip, sender_port);
      }
    }
  }
}
void Server::handle_player_input(sf::Packet &packet, int player_id,
                                 float delta_time) {
  PlayerInputState input_state;
  packet >> input_state;
  stage.handle_player_input(player_id, input_state, delta_time);
  clients[player_id].time_since_last_packet = 0.f;
}
void Server::handle_new_connection(const sf::IpAddress &sender_ip,
                                   unsigned short sender_port) {
  if (clients.size() >= game_settings.max_players) {
    std::cout << "Connection rejected from " << sender_ip << ":" << sender_port
              << ". Server is full." << std::endl;
    return;
  }
  int new_player_id = next_player_id++;
  stage.add_player(new_player_id, {600.f, 100.f});

  ConnectedClient new_client = {sender_ip, sender_port, new_player_id,
                                "Player" + std::to_string(new_player_id), 0.f};
  clients[new_player_id] = new_client;
  std::string sender_key =
      sender_ip.toString() + ":" + std::to_string(sender_port);
  players_by_address[sender_key] = new_player_id;
  sf::Packet first_packet;
  first_packet << new_player_id;
  StageSnapshot current_state = stage.get_stage_snapshot();
  first_packet << current_state;
  socket.send(first_packet, sender_ip, sender_port);
  std::cout << new_client.name << " connected." << std::endl;
}
void Server::broadcast_snapshot() {
  if (clients.empty())
    return;
  sf::Packet current_stage_packet;
  StageSnapshot current_stage_snapshot = stage.get_stage_snapshot();
  current_stage_packet << current_stage_snapshot;
  for (auto &pair : clients) {
    socket.send(current_stage_packet, pair.second.ip_address, pair.second.port);
  }
}
void Server::check_timeouts(float delta_time) {
  std::vector<int> clients_to_remove;
  for (auto &pair : clients) {
    pair.second.time_since_last_packet += delta_time;
    if (pair.second.time_since_last_packet >= Config::PLAYER_TIMEOUT) {
      clients_to_remove.push_back(pair.first);
    }
  }

  for (int player_id : clients_to_remove) {
    std::cout << "Player ID " << player_id << " timed out. Disconnecting."
              << std::endl;
    std::string address_key_to_remove;
    for (const auto &pair : players_by_address) {
      if (pair.second == player_id) {
        address_key_to_remove = pair.first;
        break;
      }
    }
    if (!address_key_to_remove.empty())
      players_by_address.erase(address_key_to_remove);
    clients.erase(player_id);
    stage.remove_player(player_id);
  }
}

#pragma once
#include "Stage.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Network.hpp>
#include <SFML/Network/IpAddress.hpp>

struct ConnectedClient {
  sf::IpAddress ip_address;
  unsigned short port;
  int player_id = -1;
  std::string name;
  float time_since_last_packet = 0.f;
};

class Server {
public:
  Server(unsigned short port, const GameSettings &settings);
  void run();

private:
  void receive_packets(float delta_time);
  void handle_player_input(sf::Packet &packet, int player_id, float delta_time);
  void handle_new_connection(const sf::IpAddress &sender_ip,
                             unsigned short sender_port);
  void broadcast_snapshot();
  void check_timeouts(float delta_time);

  Stage stage;
  GameSettings game_settings;
  sf::UdpSocket socket;
  unsigned short port;
  bool is_running;

  int next_player_id = 0;
  std::map<int, ConnectedClient> clients;
  std::map<std::string, int> players_by_address;
  sf::Clock clock;
  const sf::Time broadcast_interval = sf::seconds(1.f / 20.f);
};

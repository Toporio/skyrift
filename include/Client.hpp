#pragma once
#include "Stage.hpp"
#include "common.hpp"
#include "config.hpp"
#include <Map.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network/UdpSocket.hpp>

class Client {
public:
  Client();
  bool connect(const sf::IpAddress &server_ip, unsigned short server_port_,
               const std::string &player_name_ = "Player");
  void run(sf::RenderWindow &window);

private:
  void receive_snapshot();
  void send_input();
  void get_input();

  void run();
  void update_local_stage(float delta_time);
  void render(sf::RenderWindow &window);

  sf::UdpSocket socket;
  sf::IpAddress server_address;
  unsigned short server_port;
  bool is_connected = false;

  int player_id = -1;
  std::string player_name;
  PlayerInputState input_state;
  GameSettings game_settings;
  Stage stage;
  StageSnapshot last_stage_snapshot;
  sf::Clock clock;
  sf::Time time_since_last_input_sent;
  const sf::Time input_send_interval = sf::seconds(1.f / 30.f);
  sf::RenderWindow window;
};

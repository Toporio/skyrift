#include "Client.hpp"
#include "Map.hpp"
#include "Stage.hpp"
#include "Tile.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Client::Client()
    : window(sf::RenderWindow(
          sf::VideoMode({Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}),
          Config::WINDOW_TITLE)),
      game_settings{}, stage({game_settings}), server_address(0) {

  if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) {
    std::cout << "Failed to bind socket to port." << std::endl;
  } else {
    std::cout << "socket bound to " << socket.getLocalPort() << std::endl;
  }
  socket.setBlocking(false);
  window.setFramerateLimit(Config::FPS_LIMIT);
};
bool Client::connect(const sf::IpAddress &server_ip,
                     unsigned short server_port_,
                     const std::string &player_name_) {
  server_address = server_ip;
  server_port = server_port_;
  player_name = player_name_;
  sf::Packet request_packet;
  request_packet << "JEBAC DISA";

  if (socket.send(request_packet, server_address, server_port) !=
      sf::Socket::Status::Done) {
    std::cout << "Failed to send connection request" << std::endl;
    return false;
  }
  sf::Packet first_packet;
  std::optional<sf::IpAddress> received_ip;
  unsigned short received_port;
  socket.setBlocking(true);
  if (socket.receive(first_packet, received_ip, received_port) !=
      sf::Socket::Status::Done) {
    std::cout << "server didnt respond" << std::endl;
    socket.setBlocking(false);
    return false;
  }

  socket.setBlocking(false);
  if (first_packet >> player_id >> last_stage_snapshot) {
    is_connected = true;
    stage.apply_stage_snapshot(last_stage_snapshot, sf::Time::Zero, player_id);
    //  std::cout << stage.get_players().size() << std::endl;
    return true;
  }
  return false;
}

void Client::run() {
  if (!is_connected) {
    std::cout << "not connected to a server" << std::endl;
  } else {
    std::cout << " connected to a server" << std::endl;
  }
  const sf::Time INTERPOLATION_DELAY = sf::milliseconds(50);
  clock.restart();
  socket.setBlocking(false);
  while (window.isOpen()) {
    sf::Time delta_time = clock.restart();
    get_input();
    for (auto &pair : stage.get_players()) {
      if (pair.first == player_id) {
        pair.second->handle_input(input_state, delta_time.asSeconds());
        pair.second->update(delta_time.asSeconds());
      }
    }
    // stage.handle_player_input(player_id, input_state,
    // delta_time.asSeconds());
    //  stage.update(delta_time.asSeconds());
    send_input(delta_time);

    receive_snapshot();
    // stage.apply_stage_snapshot(last_stage_snapshot, clock.getElapsedTime());
    sf::Time render_time = clock.getElapsedTime() - INTERPOLATION_DELAY;
    stage.update_interpolation(render_time, player_id);
    stage.render(window);
  }
}
void Client::get_input() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
    input_state.move_left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    input_state.move_right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    input_state.jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    input_state.attack_melee = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K);
    input_state.attack_ranged =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J);
    input_state.block = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L);
  }
}
void Client::send_input(sf::Time delta_time) {
  time_since_last_input_sent += delta_time;
  if (time_since_last_input_sent >= input_send_interval) {
    sf::Packet input_packet;
    input_packet << input_state;
    socket.send(input_packet, server_address, server_port);
    time_since_last_input_sent -= input_send_interval;
  }
}
void Client::receive_snapshot() {
  sf::Packet new_stage_packet;
  std::optional<sf::IpAddress> server_ip;
  unsigned short server_port;
  while (socket.receive(new_stage_packet, server_ip, server_port) ==
         sf::Socket::Status::Done) {
    if (server_ip && *server_ip == server_address) {
      StageSnapshot snapshot;
      if (new_stage_packet >> snapshot) {
        stage.apply_stage_snapshot(snapshot, clock.getElapsedTime(), player_id);
      }
    }
  }
}

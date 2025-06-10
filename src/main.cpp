#include "Client.hpp"
#include "Game.hpp"
#include "Server.hpp"
#include "common.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <cerrno>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <system_error>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return -1;
  }
  std::string mode = argv[1];
  if (mode == "server") {
    GameSettings settings = {4, 2, 2};
    try {
      Server server(Config::SERVER_PORT, settings);
      server.run();

    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return EXIT_FAILURE;
    }
  } else if (mode == "client") {
    if (argc < 3) {
      std::cerr << "ip address is not passed" << std::endl;
      return 1;
    }
    std::string_view ip_to_string = argv[2];
    auto server_ip = sf::IpAddress::resolve(ip_to_string);
    Client client;
    if (client.connect(*server_ip, Config::SERVER_PORT, "player1")) {
      client.run();
    } else {
      std::cout << "connection to server failed" << std::endl;
      return 1;
    }
  }
  return 0;
}

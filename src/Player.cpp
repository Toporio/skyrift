#include "Player.hpp"
#include "config.hpp"
#include <SFML/System/Vector2.hpp>

Player::Player(int id, const sf::Texture &texture, const sf::Vector2f &position,
               const sf::Vector2f &velocity, unsigned int lives)
    : Entity(id, texture, position, velocity), lives(lives),
      status(PlayerStatus::IDLE), current_jumps(0), dir_x(1) {}
void Player::jump() { velocity.y = -jump_speed; };
void Player::apply_gravity(float delta_time) {
  velocity.y += Config::GRAVITY * delta_time;
}
void Player::move(float direction) {
  velocity.x += direction * move_speed;
  dir_x = direction;
  if (direction != 0.0f)
    status = PlayerStatus::RUNNING;
  else
    status = PlayerStatus::IDLE;
}
void Player::block(bool start_blocking) { return; }

Projectile Player::attack_ranged() {
    return Projectile(id, dir_x, sprite.getPosition());
}

void Player::attack_melee(Player& enemy, float dir_x) {
    this->sprite.move(sf::Vector2f ((dir_x > 0 ? 10.f : -10.f), 0.f));
    if (this->check_collision(enemy)) {
        enemy.health--;
    }
    this->sprite.move(sf::Vector2f((dir_x > 0 ? -10.f : 10.f), 0.f));
}


void Player::handle_input(const PlayerInputState &input_state,
                          float delta_time) {
  float move_direction = 0.0f;
  if (input_state.move_left) {
    move_direction -= 1.0f;
  }
  if (input_state.move_right) {
    move_direction += 1.0f;
  }
  move(move_direction);
  if (input_state.attack_melee) {
    attack_melee();
  }
  if (input_state.attack_ranged) {
    // attack_ranged(std::vector<Projectile> & projectiles,
    //             sf::Vector2f & direction);
  }
  if (input_state.jump) {
    jump();
  }
  if (input_state.block) {
    block(true);
  }
}

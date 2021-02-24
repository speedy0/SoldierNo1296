#pragma once
#include "pch.h"
#include "wolf.h"

//Wolf is ferocious and will attack both, zombie and players on sight. 
wolf::wolf() {};
wolf::~wolf() {};

void wolf::initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward) {
	m_wolf = object;
	m_wolf->set_forward(forward);
	m_wolf->set_position(position);

	Set_Health(50);
}

void wolf::on_update(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3& zombie_position) {
	//Measures the distance between a wolf and a player.
	float dist_to_player = glm::distance(m_wolf->position(), player_position);
	float dist_to_zombie = glm::distance(m_wolf->position(), zombie_position);

	//Checks which behaviour is wolf currently in.
	if (m_state == state::idle) {
		//If player is not in the near the wolf's detection radius, wolf will just sniff around.
		patrol(time_step);

		//If player is in wolf's detection radius, wolf will face the player and prepare to attack. 
		if (dist_to_player < m_detection_radius)
			m_state = state::patrol;
		else if (dist_to_zombie < m_detection_radius)
			m_state = state::patrol;
	}
	else if (m_state == state::patrol) {
		face_player(time_step, player_position, zombie_position);

		//This is a wolf that is guarding the zone, if player leaves the area, wolf will leave the player alone.
		if (dist_to_player > m_detection_radius)
			m_state = state::idle;
		else if (dist_to_zombie > m_detection_radius)
			m_state = state::idle;
		//however if player will not back away and move towards the wolf, wolf will start chasing and attack. 
		else if (dist_to_player < m_trigger_radius)
			m_state = state::attack;
		else if (dist_to_zombie < m_trigger_radius)
			m_state = state::attack;
	}
	else {
		chase_player(time_step, player_position, zombie_position);
	}
}

void wolf::patrol(const engine::timestep& time_step) {
	//When wolf wonders around, how long does it take him to turn around and change direction.
	m_speed = 1.f;
	m_switch_directions -= (float)time_step;

	if (m_switch_directions < 0.f) {
		m_wolf->set_forward(m_wolf->forward() * -1.f);
		m_switch_directions = m_default_time;
	}

	m_wolf->set_position(m_wolf->position() + m_wolf->forward() * m_speed * (float)time_step);
	m_wolf->set_rotation_amount(atan2(m_wolf->forward().x, m_wolf->forward().z));
}

void wolf::face_player(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3 zombie_position) {
	//Measures the distance between a wolf and a player.
	float dist_to_player = glm::distance(m_wolf->position(), player_position);
	float dist_to_zombie = glm::distance(m_wolf->position(), zombie_position);

	if (dist_to_player < m_detection_radius) {
		m_wolf->set_forward(player_position - m_wolf->position());
		m_wolf->set_rotation_amount(atan2(m_wolf->forward().x, m_wolf->forward().z));
	}
	else if (dist_to_zombie < m_detection_radius) {
		m_wolf->set_forward(zombie_position - m_wolf->position());
		m_wolf->set_rotation_amount(atan2(m_wolf->forward().x, m_wolf->forward().z));
	}
	
}

void wolf::chase_player(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3 zombie_position) {
	m_speed = 3.f;

	//Measures the distance between a wolf and a player.
	float dist_to_player = glm::distance(m_wolf->position(), player_position);
	float dist_to_zombie = glm::distance(m_wolf->position(), zombie_position);

	if (dist_to_player < m_trigger_radius) {
		m_wolf->set_forward(player_position - m_wolf->position());
		m_wolf->set_position(m_wolf->position() + m_wolf->forward() * m_speed * (float)time_step);
		m_wolf->set_rotation_amount(atan2(m_wolf->forward().x, m_wolf->forward().z));
	}
	else if (dist_to_zombie < m_trigger_radius) {
		m_wolf->set_forward(zombie_position - m_wolf->position());
		m_wolf->set_position(m_wolf->position() + m_wolf->forward() * m_speed * (float)time_step);
		m_wolf->set_rotation_amount(atan2(m_wolf->forward().x, m_wolf->forward().z));
	}
	
}

void wolf::Set_Health(int a_health) {
	health = a_health;
}

int wolf::Get_Health() {
	return health;
}

int wolf::Damage_Taken(int a_health) {
	health -= a_health;
	return health;
}

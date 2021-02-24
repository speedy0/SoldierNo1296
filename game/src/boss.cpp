#pragma once
#include "pch.h"
#include "boss.h"

//Boss is a modified robot that was designed to attack zombies mainly but can attack the player if he gets too close. 
boss::boss() {};
boss::~boss() {};

void boss::initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward) {
	m_boss = object;
	m_boss->set_forward(forward);
	m_boss->set_position(position);

	Set_Health(500);
}

void boss::on_update(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3& zombie_position) {
	//Measures the distance between a boss and a player. Then it measures the distance between zombie and a boss.
	float dist_to_player = glm::distance(m_boss->position(), player_position);
	float dist_to_zombie = glm::distance(m_boss->position(), zombie_position);


	//Checks which behaviour is zombie currently in.
	if (m_state == state::idle) {
		search(time_step);

		if (dist_to_player < m_trigger)
			m_state = state::search;
		else if (dist_to_zombie < m_trigger)
			m_state = state::search;
	}
	else if (m_state == state::search) {
		attack(time_step, player_position, zombie_position);

		if (dist_to_player > m_trigger)
			m_state = state::idle;
		else if (dist_to_zombie > m_trigger)
			m_state = state::idle;

		else if (dist_to_player < m_trigger)
			m_state = state::kill;
		else if (dist_to_zombie < m_trigger)
			m_state = state::kill;
	}
	else {
		attack(time_step, player_position, zombie_position);
	}
}

void boss::search(const engine::timestep& time_step) {
	//When a boss wonders around, how long does it take him to turn around and change direction.
	m_speed = 0.005f;
	m_stop_timer -= (float)time_step;

	if (m_stop_timer < 0.f) {
		m_boss->set_forward(m_boss->forward() * 0.1f);
		m_stop_timer = m_default_time;
	}

	m_boss->set_position(m_boss->position() + m_boss->forward() * m_speed * (float)time_step);
	m_boss->set_rotation_amount(atan2(m_boss->forward().x, m_boss->forward().z));
}

void boss::attack(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3 zombie_position) {
	m_speed = 0.005f;

	//Measures the distance between a boss and a player.
	float dist_to_player = glm::distance(m_boss->position(), player_position);
	float dist_to_zombie = glm::distance(m_boss->position(), zombie_position);

	if (dist_to_player < m_trigger) {
		m_boss->set_forward(player_position - m_boss->position());
		m_boss->set_position(m_boss->position() + m_boss->forward() * m_speed * (float)time_step);
		m_boss->set_rotation_amount(atan2(m_boss->forward().x, m_boss->forward().z));
	}
	else if (dist_to_zombie < m_trigger) {
		m_boss->set_forward(zombie_position - m_boss->position());
		m_boss->set_position(m_boss->position() + m_boss->forward() * m_speed * (float)time_step);
		m_boss->set_rotation_amount(atan2(m_boss->forward().x, m_boss->forward().z));
	}

}

void boss::Set_Health(int a_health) {
	health = a_health;
}

int boss::Get_Health() {
	return health;
}

int boss::Damage_Taken(int a_health) {
	health -= a_health;
	return health;
}

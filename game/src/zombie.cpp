#pragma once
#include "pch.h"
#include "zombie.h"

zombie::zombie() {};
zombie::~zombie() {};

void zombie::initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward) {
	m_zom = object;
	m_zom->set_forward(forward);
	m_zom->set_position(position);

	m_zom->animated_mesh()->set_default_animation(2);
	isWalking = false;

	Set_Health(35);
}

void zombie::on_update(const engine::timestep& time_step, const glm::vec3& player_position) {
	//Measures the distance between a zombie and a player.
	float dist_to_player = glm::distance(m_zom->position(), player_position);

	if (isWalking == true && m_speed == 0.f) {
		isWalking = false;
		m_zom->animated_mesh()->switch_animation(m_zom->animated_mesh()->default_animation());
	}
	else if (isWalking != true && m_speed > 0.f) {
		isWalking = true;
		m_zom->animated_mesh()->switch_animation(1);
	}

	//Checks which behaviour is zombie currently in.
	if (m_state == state::idle) {
		//If player is not in the near the zombie's detection radius, zombie will just wonder around.
		wonder(time_step);

		//If player is in zombie's detection radius, zombie will face the player and prepare to attack. 
		if (dist_to_player < m_detection_radius)
			m_state = state::prepare;
	}
	else if (m_state == state::prepare) {
		confirm_player(time_step, player_position);

		//This is a slow zombie, if player leaves the area when zombie is preparing, the zombie will leave the player alone.
		if (dist_to_player > m_detection_radius)
			m_state = state::idle;
		//however if player will not back away and move towards the zombie, zombie will start chasing and attack. 
		else if (dist_to_player < m_trigger_radius)
			m_state = state::attack;
	}
	else {
		attack_player(time_step, player_position);
	}
}

void zombie::wonder(const engine::timestep& time_step) {
	//When zombie wonders around, how long does it take him to turn around and change direction.
	m_speed = 0.8f;
	m_switch_directions -= (float)time_step;

	if (m_switch_directions < 0.f) {
		m_zom->set_forward(m_zom->forward() * -1.f);
		m_switch_directions = m_default_time;
	}

	m_zom->set_position(m_zom->position() + m_zom->forward() * m_speed * (float)time_step);
	m_zom->set_rotation_amount(atan2(m_zom->forward().x, m_zom->forward().z));
}

void zombie::confirm_player(const engine::timestep& time_step, const glm::vec3& player_position) {
	m_zom->set_forward(player_position - m_zom->position());
	m_zom->set_rotation_amount(atan2(m_zom->forward().x, m_zom->forward().z));
}

void zombie::attack_player(const engine::timestep& time_step, const glm::vec3& player_position) {
	m_speed = 1.f;
	m_zom->set_forward(player_position - m_zom->position());
	m_zom->set_position(m_zom->position() + m_zom->forward() * m_speed * (float)time_step);
	m_zom->set_rotation_amount(atan2(m_zom->forward().x, m_zom->forward().z));
}

void zombie::Set_Health(int a_health) {
	health = a_health;
}

int zombie::Get_Health() {
	return health;
}

int zombie::Damage_Taken(int a_health) {
	health -= a_health;
	return health;
}

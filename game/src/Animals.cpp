#pragma once
#include "pch.h"
#include "Animals.h"

//Wolf is ferocious and will attack both, zombie and players on sight. 
Animals::Animals() {};
Animals::~Animals() {};

void Animals::initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward) {
	m_animal = object;
	m_animal->set_forward(forward);
	m_animal->set_position(position);

	Set_Health(25);
}

void Animals::on_update(const engine::timestep& time_step) {

	//Checks which behaviour an animal currently is in.
	if (m_state == state::idle) {
		wander(time_step);
	}
}

void Animals::wander(const engine::timestep& time_step) {
	//When wolf wonders around, how long does it take him to turn around and change direction.
	m_speed = 0.3f;
	m_switch_directions -= (float)time_step;

	if (m_switch_directions < 0.f) {
		m_animal->set_forward(m_animal->forward() * -0.5f);
		m_switch_directions = m_default_time;
	}

	m_animal->set_position(m_animal->position() + m_animal->forward() * m_speed * (float)time_step);
	m_animal->set_rotation_amount(atan2(m_animal->forward().x, m_animal->forward().z));
}

void Animals::Set_Health(int a_health) {
	health = a_health;
}

int Animals::Get_Health() {
	return health;
}

int Animals::Damage_Taken(int a_health) {
	health -= a_health;
	return health;
}

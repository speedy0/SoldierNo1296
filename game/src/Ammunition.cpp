#pragma once
#include "Ammunition.h"

Ammunition::Ammunition() {}

Ammunition::~Ammunition() {}

void Ammunition::initialise(engine::ref<engine::game_object> object) {
	m_ammunition = object;
}

void Ammunition::on_render(const engine::ref<engine::shader>& shader) {
	//Sets specifics for the ammunition. 
	glm::mat4 ammunition_trans(1.f);
	ammunition_trans = glm::translate(ammunition_trans, m_ammunition->position());
	ammunition_trans = glm::rotate(ammunition_trans, m_ammunition->rotation_amount(), m_ammunition->rotation_axis());
	ammunition_trans = glm::scale(ammunition_trans, m_ammunition->scale());
	engine::renderer::submit(shader, ammunition_trans, m_ammunition);
}

void Ammunition::on_update(const engine::timestep& time_step) {
	//Controls update of ammunition movement.
	m_ammunition->set_position(m_ammunition->position() + m_ammunition->forward() * (float)time_step * m_speed);
}

void Ammunition::fire(const engine::perspective_camera& camera, float speed) {
	//Defines ammunition's position on camera's position and its forward.
	m_ammunition->set_position(camera.position());
	m_ammunition->set_forward(camera.front_vector());
	m_speed = speed;
}

#include "pch.h"
#include "SpecialAmmunition.h"

specialamm::specialamm()
{}

specialamm::~specialamm()
{}

void specialamm::initialise(engine::ref<engine::game_object> ammunition, glm::vec3 forward)
{
	m_ammunition = ammunition;
	m_rotational_inertia = (2.f / 3.f) * ammunition->mass() * (m_ammunition->bounding_shape().y / 2.f) * (m_ammunition->bounding_shape().y / 2.f);
	m_contact_time = 0.05f;
	m_ammunition->set_forward(forward);

	isTargeting = false;
}

void specialamm::shoot(const engine::perspective_camera& camera, float shoot)
{
	m_ammunition->set_velocity(glm::vec3(0.f));
	m_ammunition->set_acceleration(glm::vec3(0.f, 0.f, 0.f));
	m_ammunition->set_torque(glm::vec3(0.f));
	m_ammunition->set_rotation_amount(0.f);
	m_ammunition->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
	m_ammunition->set_angular_velocity(glm::vec3(0.f));
	m_contact_time = 0.0f;

	// Set the ammunition's position.
	m_ammunition->set_position(camera.position());
	m_ammunition->set_velocity(25.0f * camera.front_vector());

	// Determine rotation angles of camera
	m_theta = engine::PI / 2.f - acos(camera.front_vector().y);
	m_phi = atan2(camera.front_vector().x, camera.front_vector().z);
}

void specialamm::on_update(const engine::timestep& time_step, glm::vec3& zombie_position, glm::vec3& wolf_position, glm::vec3& wolf1_position)
{
	float dist_to_zombie = glm::distance(m_ammunition->position(), zombie_position);
	float dist_to_wolf = glm::distance(m_ammunition->position(), wolf_position);
	float dist_to_wolf1 = glm::distance(m_ammunition->position(), wolf1_position);

	if (isTargeting == false) {
		// Update physical quanitities
		m_last_position = m_ammunition->position();

		m_ammunition->set_position(m_ammunition->position() + m_ammunition->velocity() * (float)time_step);

		glm::vec3 angle = m_ammunition->rotation_axis() * m_ammunition->rotation_amount();
		angle += m_ammunition->angular_velocity() * (float)time_step;
		m_ammunition->set_rotation_amount(glm::length(angle));
		if (glm::length(angle) > 0.f)
		{
			m_ammunition->set_rotation_axis(glm::normalize(angle));
		}

		// Turn off instantaneous forces if contact time is surpassed
		if (glm::length(m_instantaneous_acceleration) > 0 && m_contact_time > 0.05) {
			m_instantaneous_acceleration = glm::vec3(0.f);
			m_instantaneous_angular_acceleration = glm::vec3(0.f);
			m_contact_time = 0.f;
		}
		m_contact_time += time_step;

		// Check for collision detection
		float y_plane = 0.5f;
		if (collision_detection(y_plane)) {
			collision_response(y_plane);
		}

		if (dist_to_zombie < m_trigger_radius) {
			m_state = state::attack;
			isTargeting = true;
		}
		else if (dist_to_wolf < m_trigger_radius) {
			m_state = state::attack;
			isTargeting = true;
		}
		else if (dist_to_wolf1 < m_trigger_radius) {
			m_state = state::attack;
			isTargeting = true;
		}	
	}
	else if (isTargeting == true) {
		attack(time_step, zombie_position, wolf_position, wolf1_position);
	}
}

void specialamm::attack(const engine::timestep& time_step, const glm::vec3& zombie_position, glm::vec3& wolf_position, glm::vec3& wolf1_position) {
	m_speed = 1.f;
	float dist_to_zombie = glm::distance(m_ammunition->position(), zombie_position);
	float dist_to_wolf = glm::distance(m_ammunition->position(), wolf_position);
	float dist_to_wolf1 = glm::distance(m_ammunition->position(), wolf1_position);

	if (dist_to_zombie < m_trigger_radius) {
		m_ammunition->set_forward(zombie_position - m_ammunition->position());
		m_ammunition->set_position(m_ammunition->position() + m_ammunition->forward() * m_speed * (float)time_step);
		m_ammunition->set_rotation_amount(atan2(m_ammunition->forward().x, m_ammunition->forward().z));
	}
	else if (dist_to_wolf < m_trigger_radius) {
		m_ammunition->set_forward(wolf_position - m_ammunition->position());
		m_ammunition->set_position(m_ammunition->position() + m_ammunition->forward() * m_speed * (float)time_step);
		m_ammunition->set_rotation_amount(atan2(m_ammunition->forward().x, m_ammunition->forward().z));
	}
	else if (dist_to_wolf1 < m_trigger_radius) {
		m_ammunition->set_forward(wolf1_position - m_ammunition->position());
		m_ammunition->set_position(m_ammunition->position() + m_ammunition->forward() * m_speed * (float)time_step);
		m_ammunition->set_rotation_amount(atan2(m_ammunition->forward().x, m_ammunition->forward().z));
	}
}

void specialamm::on_render(const engine::ref<engine::shader>& shader)
{
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, m_ammunition->position());
	transform = glm::rotate(transform, m_phi, glm::vec3(0.f, 1.f, 0.f));
	transform = glm::rotate(transform, -glm::pi<float>() / 2.0f, glm::vec3(-1.f, 0.f, 0.f));
	transform = glm::scale(transform, glm::vec3(0.05f));
	engine::renderer::submit(shader, transform, m_ammunition);
}

bool specialamm::collision_detection(float y_plane)
{
	if (m_ammunition->position().y - m_ammunition->bounding_shape().y < y_plane && m_ammunition->velocity().y < 0) {
		return true;
	}
	return false;
}

void specialamm::collision_response(float y_plane)
{
	float convergenceThreshold = 0.5f;
	if (glm::length(m_ammunition->velocity()) > convergenceThreshold) {
		m_ammunition->set_velocity(glm::vec3(m_ammunition->velocity().x, -m_ammunition->velocity().y, m_ammunition->velocity().z));
	}
	else {
		m_ammunition->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
		m_ammunition->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
		m_ammunition->set_position(glm::vec3(m_ammunition->position().x, m_ammunition->bounding_shape().y + y_plane, m_ammunition->position().z));
		m_ammunition->set_angular_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

bool specialamm::Target(int a_int) {
	if (a_int == 1) {
		isTargeting = false;
		return isTargeting;
	}
	else {
		isTargeting = true;
		return isTargeting;
	}
	

}

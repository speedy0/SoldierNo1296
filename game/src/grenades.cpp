#pragma once
#include "pch.h"
#include "grenades.h"

//Important informations regarding m67 grenade that will be basis for this one. 
//Average mass for the M67 grenade is 400g = 0.4kg.
//Average distance applied when grenade is thrown by average male is 30-35metres with 4-5seconds defuse time. For those calculations we will assume 35metres and 5seconds.
//Average velocity that grenade is traveling at is: 50km/h (1/3rd of a baseball which is known as fastball and thrown on average 150km/h).

/// <summary>
/// Maths for F=ma
/// 
/// </summary>
grenades::grenades(){}
grenades::~grenades() {};

void grenades::initialise(engine::ref<engine::game_object> grenade) {
	//Initialises grenade and sets rotational interia for the grenade.
	m_grenades = grenade;
	m_rotational_interia = (2.f / 3.f) * grenade->mass() * (m_grenades->bounding_shape().y / 2.f) * (m_grenades->bounding_shape().y / 2.f);
	m_contact_time = 0.05f;
}

void grenades::on_update(const engine::timestep& time_step) {
	m_last_known_position = m_grenades->position();
}

void grenades::on_render(const engine::ref<engine::shader>& shader){
	//Spawns the grenade when it is called.
	glm::mat4 grenade_transform(1.f);
	grenade_transform = glm::translate(grenade_transform, m_grenades->position());
	grenade_transform = glm::rotate(grenade_transform, m_grenades->rotation_amount(), m_grenades->rotation_axis());
	grenade_transform = glm::scale(grenade_transform, m_grenades->scale());
	engine::renderer::submit(shader, grenade_transform, m_grenades);
}

void grenades::throwing(const engine::perspective_camera& camera, float throwing) {
	//Required criteria for the grenade will now be set for the player to be able to throw the grenade.
	m_grenades->set_velocity(glm::vec3(0.f));
	m_grenades->set_acceleration(glm::vec3(0.f, 0.f, 0.f));
	m_grenades->set_torque(glm::vec3(0.f));
	m_grenades->set_rotation_amount(0.f);
	m_grenades->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
	m_grenades->set_angular_velocity(glm::vec3(0.f));
	m_contact_time = 0.f;

	//Grenade should spawn in the player's hand and therefore, player's position.

	if (camera.position().y <= 2.f) {
		m_grenades->set_position(glm::vec3(camera.position().x, camera.position().y + 0.9f, camera.position().z));
	}
	else {
		m_grenades->set_position(glm::vec3(camera.position().x, camera.position().y, camera.position().z));
	}
	m_grenades->set_acceleration(3.f * throwing * camera.front_vector());

	m_theta = engine::PI / 2.f - acos(camera.front_vector().y); //
	m_phi = atan2(camera.front_vector().x, camera.front_vector().z); //

	
}

bool grenades::collision_det(float y_plane) {
	//Checks for the collision of the grenade and the ground. 
	if (m_grenades->position().y - m_grenades->bounding_shape().y < y_plane && m_grenades->velocity().y < 0) {
		return true;
	}
	return false;
}
void grenades::collision_response(float y_plane) {
	float ConvergenceThreshold = 0.5f;
	if (glm::length(m_grenades->velocity()) > ConvergenceThreshold) {
		//Grenade will bounce off as it will be based on a real-life grenade m67.
		//According to Bohemia Interactive (2020), 'Effective use of grenades', available at: https://community.bistudio.com/wiki/Effective_use_of_grenades#:~:text=types%20of%20grenades%3A-,M67%20(US)%20Grenade,time%20to%20evade%20the%20blast.
		//The m67 grenade can bounce off the wall.
		m_grenades->set_velocity(glm::vec3(m_grenades->velocity().x, -m_grenades->velocity().y , m_grenades->velocity().z));
	}
	else {
		//If the grenade falls below the ground, it will be stopped with following code.
		m_grenades->set_velocity(glm::vec3(0.f, 0.f, 0.f));
		m_grenades->set_acceleration(glm::vec3(0.f, 0.f, 0.f));
		m_grenades->set_position(glm::vec3(m_grenades->position().x, m_grenades->bounding_shape().y + y_plane, m_grenades->position().z));
		m_grenades->set_angular_velocity(glm::vec3(0.f, 0.f, 0.f));
	}
}

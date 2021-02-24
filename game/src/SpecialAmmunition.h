#pragma once
#include <engine.h>

class specialamm
{
	//Sets possible states for the wolf character.
	enum class state
	{
		idle,
		attack
	};
public:
	specialamm();
	~specialamm();

	void initialise(engine::ref<engine::game_object> ammunition, glm::vec3 forward);
	void shoot(const engine::perspective_camera& camera, float shoot);
	void on_update(const engine::timestep& time_step, glm::vec3& zombie_position, glm::vec3& wolf_position, glm::vec3& wolf1_position);
	void on_render(const engine::ref<engine::shader>& shader);
	void attack(const engine::timestep& time_step, const glm::vec3& zombie_position, glm::vec3& wolf_position, glm::vec3& wolf1_position);

	bool Target(int a_int);

	glm::vec3 last_position() const { return m_last_position; }

	engine::ref<engine::game_object> object() const { return m_ammunition; }

private:
	engine::ref<engine::game_object> m_ammunition;

	glm::vec3 m_instantaneous_acceleration{ 0.f };
	glm::vec3 m_instantaneous_angular_acceleration{ 0.f };

	float m_contact_time = 0.0f;

	float m_rotational_inertia = 0.f;

	float m_phi, m_theta;

	float m_trigger_radius{ 14.f };

	float m_speed = 0.f;

	bool isTargeting;

	glm::vec3 m_last_position{ 0.f };

	// Methods to handle collisions with the ground
	bool collision_detection(float y_plane);
	void collision_response(float y_plane);

	state m_state = state::idle;
};

#pragma once
#include <engine.h>

class grenades
{
public:
	grenades();
	~grenades();

	void initialise(engine::ref<engine::game_object> grenade);
	void on_update(const engine::timestep& time_step);
	void on_render(const engine::ref<engine::shader>& shader);
	void throwing(const engine::perspective_camera& camera, float throwing);
	
	glm::vec3 last_known_position() const { return m_last_known_position; }
	engine::ref<engine::game_object> grenade() const { return m_grenades; }

private:
	bool collision_det(float y_plane);
	void collision_response(float y_plane);

	engine::ref<engine::game_object> m_grenades;
	glm::vec3 m_instantenous_acceleration{ 0.f };
	glm::vec3 m_instantenous_angular_acceleration{ 0.f };

	float m_rotational_interia = 0.f;
	float m_contact_time = 0.f;
	float m_theta;
	float m_phi;

	glm::vec3 m_last_known_position{ 0.f };
};

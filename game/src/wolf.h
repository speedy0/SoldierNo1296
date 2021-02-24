#pragma once
#include <engine.h>

class wolf
{
	//Sets possible states for the wolf character.
	enum class state
	{
		idle,
		patrol,
		attack
	};

public:
	wolf();
	~wolf();

	void initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward);
	void on_update(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3& zombie_position);

	//States:
	void patrol(const engine::timestep& time_step);
	void face_player(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3 zombie_position);
	void chase_player(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3 zombie_position);

	void Set_Health(int a_health);
	int Get_Health();
	int Damage_Taken(int a_health);

	engine::ref<engine::game_object> object() const { return m_wolf; }

private:
	float m_speed;
	int health;

	//timer controls the direction switch and reset value
	float m_default_time{ 10.f };
	float m_switch_directions = m_default_time;

	//Specifics regarding detection
	float m_detection_radius{ 20.f }; 
	float m_trigger_radius{ 14.f };

	engine::ref<engine::game_object> m_wolf;

	state m_state = state::idle;
};

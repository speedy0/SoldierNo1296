#pragma once
#include <engine.h>

class boss
{
	//Sets possible states for the Boss character.
	enum class state
	{
		idle,
		search,
		kill
	};

public:
	boss();
	~boss();

	void initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward);
	void on_update(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3& zombie_position);

	//States:
	void search(const engine::timestep& time_step);
	void attack(const engine::timestep& time_step, const glm::vec3& player_position, glm::vec3 zombie_position);

	void Set_Health(int a_health);
	int Get_Health();
	int Damage_Taken(int a_health);

	engine::ref<engine::game_object> object() const { return m_boss; }

private:
	float m_speed;
	int health;

	//timer controls the direction switch and reset value
	float m_default_time{ 10.f };
	float m_stop_timer = m_default_time;

	//Specifics regarding detection
	float m_trigger{ 25.f };

	engine::ref<engine::game_object> m_boss;

	state m_state = state::idle;
};

#pragma once
#include <engine.h>

class Animals
{
	//Sets possible states for the wolf character.
	enum class state
	{
		idle,
		wander,
	};

public:
	Animals();
	~Animals();

	void initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward);
	void on_update(const engine::timestep& time_step);

	//States:
	void wander(const engine::timestep& time_step);

	void Set_Health(int a_health);
	int Get_Health();
	int Damage_Taken(int a_health);

	engine::ref<engine::game_object> object() const { return m_animal; }

private:
	float m_speed;
	int health;

	//timer controls the direction switch and reset value
	float m_default_time{ 5.f };
	float m_switch_directions = m_default_time;

	engine::ref<engine::game_object> m_animal;

	state m_state = state::idle;
};

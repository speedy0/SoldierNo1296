#pragma once
#include <engine.h>

class zombie
{
	//Sets possible states for the zombie character.
	enum class state
	{
		idle,
		prepare,
		attack
	};

public:
	zombie();
	~zombie();

	void initialise(engine::ref<engine::game_object> object, glm::vec3 position, glm::vec3 forward);
	void on_update(const engine::timestep& time_step, const glm::vec3& player_position);

	//States:
	void wonder(const engine::timestep& time_step);
	void confirm_player(const engine::timestep& time_step, const glm::vec3& player_position);
	void attack_player(const engine::timestep& time_step, const glm::vec3& player_position);

	void Set_Health(int a_health);
	int Get_Health();
	int Damage_Taken(int a_health);

	engine::ref<engine::game_object> object() const { return m_zom; }

private:
	float m_speed{ 1.f };
	int health;
	bool isWalking;

	//timer controls the direction switch and reset value
	float m_default_time{ 15.f };
	float m_switch_directions = m_default_time;

	//Specifics regarding detection
	float m_detection_radius{ 10.f };
	float m_trigger_radius{ 9.f };

	engine::ref<engine::game_object> m_zom;

	state m_state = state::idle;
};

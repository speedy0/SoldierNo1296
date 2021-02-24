#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"

class Player {
public:
	Player();
	~Player();

	void Initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);

	engine::ref<engine::game_object> object() const { return m_gbp; }

	void update_camera(engine::perspective_camera& camera);

	//Health
	void Set_Health(int a_health);
	int Get_Health();
	int Damage_Taken(int a_health);
	int Heal(int a_health);

	//Grenades
	void Set_Grenades(int w_grenades);
	int Get_Grenades();
	int Throw_Grenade(int w_grenades);
	int Collect_Grenades(int w_grenades);

	//Special Ammunition
	void Set_SpecialAmm(int a_amm);
	int Get_SpecialAmm();
	int Throw_SpecialAmm(int a_amm);
	int Collect_SpecialAmm(int a_amm);

	//Normal Ammunition
	void Set_Ammunition(int a_ammunition);
	int Collect_Ammunition(int a_ammunition);
	int Shoot_Ammunition(int a_ammunition);
	int Get_Ammunition();

	//Player's actions
	void Turn(float angle);
	void Jump();

	//Player's stats
	int Pscore() { return score; };
	bool PlayerAlive() { return isPlayerAlive; }

	//Tests whether player has weapons.
	bool carriesStaff() { return IsCarryingStaff; }
	bool carriesShuriken() { return IsCarryingShuriken; }
	bool carriesSword() { return IsCarryingSword; }
	bool carriesSniperRifle() { return IsCarryingSniperRifle; }
	//int Equipment();
	/*virtual void update(double dt) {}

protected:
	void move(glm::vec3 accelerate, double dt);*/

	
private:
	//Declares the player's stats:
	int score = 0;
	int health;
	int grenades;
	int specialamm;
	int ammunition;
	bool isPlayerAlive;
	float m_speed{ 0.f };
	float m_timer;

	//Declares player's animations:
	bool isWalking = false;
	bool isJumping = true;

	//Declares player's camera's point of views.
	bool FP = false;
	bool TP = true;

	//Declares whether the player owns equipment.
	bool ownastaff;
	bool ownashuriken;
	bool ownasword;
	bool ownasniperrifle;

	//Declares whether the player equips his equipment.
	bool IsCarryingStaff;
	bool IsCarryingShuriken;
	bool IsCarryingSword;
	bool IsCarryingSniperRifle;

	engine::ref<engine::game_object> m_gbp;
};

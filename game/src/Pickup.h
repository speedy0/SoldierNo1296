#pragma once
#include <engine.h>

class Pickup: public engine::game_object {
public:
	Pickup(const engine::game_object_properties props);
	~Pickup();

	void Initialise();
	void update(glm::vec3 a, float det);

	bool Staff() { return isStaff; }
	bool Shuriken() { return isShuriken; }
	bool Sword() { return isSword; }
	bool SniperRifle() { return isSniperRifle; }
	bool SmallHealth() { return isSmallHealth; }
	bool MediumHealth() { return isMediumHealth; }
	bool LargeHealth() { return isLargeHealth; }
	void SetScrap(int a_scrap);
	int ScrapScore();
	int AddScrap(int a_scrap);
	int RemoveScrap(int a_scrap);
	void PickupCollect(glm::vec3 a, float det);

	static engine::ref<Pickup> create(const engine::game_object_properties& props);

private:
	int scrapscore; 
	bool isStaff; //Checks whether Staff is active. 
	bool isShuriken; //Checks whether Shuriken is active.
	bool isSword; //Checks whether Sword is active. 
	bool isSniperRifle; //Checks whether Sniper Rifle is active.
	bool isSmallHealth; //Checks whether Small Healthpack is active.
	bool isMediumHealth; //Checks whether Medium Healthpack is active.
	bool isLargeHealth; //Checks whether Large Healthpack is active.

	std::string equipment[4];
};

#pragma once
#include "Pickup.h"

Pickup::Pickup(const engine::game_object_properties props) : engine::game_object(props) {}
Pickup::~Pickup(){}

void Pickup::Initialise()
{
	//We initialise at the start for the objective to be on.
	isStaff = true;
	isShuriken = true;
	isSword = true;
	isSniperRifle = true;
	isSmallHealth = true;
	isMediumHealth = true;
	isLargeHealth = true;

	///EQUIPMENT INITIALISATION
	//Initialises Equipment of the player. 
	equipment[0] = "Hand";
	equipment[1] = "Null";
	equipment[2] = "Null";
	equipment[3] = "Null";

	SetScrap(0);
}

void CheckEquipment(std::string variable[]) {
	std::cout << variable[1];
}

void Pickup::update(glm::vec3 b, float sped)
{
	glm::vec3 a = b;
	float det = sped;
	PickupCollect(a, det);
}

void Pickup::PickupCollect(glm::vec3 a, float det) {
	set_rotation_amount(rotation_amount() + det * 0.5f);

	glm::vec3 b = position() - a;

	if (glm::length(b) < 1.f) {
		isStaff = false; //Staff
		AddScrap(5);
	}

	if (glm::length(b) < 1.f) {
		isShuriken = false; //Shuriken
		AddScrap(2);
	}

	if (glm::length(b) < 1.f) {//&& isActive2 == true <-- code in order to replace. 
		isSword = false; //Sword
		AddScrap(8);
	}

	if (glm::length(b) < 1.f) {
		isSniperRifle = false; //Sniper Rifle
		AddScrap(24);
	}

	if (glm::length(b) < 1.f) {
		isSmallHealth = false; //25 Healthpack
	}

	if (glm::length(b) < 1.f) {
		isMediumHealth = false; //50 Healthpack
	}

	if (glm::length(b) < 1.f) {
		isLargeHealth = false; //100 Healthpack.
	}
}

engine::ref<Pickup> Pickup::create(const engine::game_object_properties& props)
{
	return std::make_shared<Pickup>(props);
}

void Pickup::SetScrap(int a_scrap) {
	scrapscore = a_scrap;
}

int Pickup::AddScrap(int a_scrap) {
	scrapscore += a_scrap;
	return scrapscore;
}

int Pickup::RemoveScrap(int a_scrap) {
	scrapscore -= a_scrap;
	return scrapscore;
}

int Pickup::ScrapScore() {
	return scrapscore;
}

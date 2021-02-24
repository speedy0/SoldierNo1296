#pragma once
#include "pch.h"
#include "Player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"

Player::Player() : health(100), grenades(10), m_timer(0.f), m_speed(0.f) {
	;
}

Player::~Player(){}

void Player::Initialise(engine::ref<engine::game_object> object) {
	//We assign m_gbp to the object that will become a player. 
	m_gbp = object;
	m_gbp->set_forward(glm::vec3(0.f, 0.f, 1.0f));
	m_gbp->set_position(glm::vec3(-35.f, 0.5f, -5.f));

	//As the object that player will take the role of is animated, the default animation will be (2) as it is idle. 
	m_gbp->animated_mesh()->set_default_animation(2);

	///CAMERA INITIALISATION
	//Initialises Third person view camera at the start with possibility to switch to a first person view.
	FP = false;
	TP = true;

	///PLAYER'S STATISTICS INITIALISATION
	//Initialises player's health and sets the condition that the player is alive to true.
	Set_Health(100);
	Set_Grenades(10);
	Set_SpecialAmm(0);
	Set_Ammunition(30);
	isPlayerAlive = true;

	//Clears out the player's equipment.
	ownastaff = true;
	ownashuriken = true;
	ownasword = true;
	ownasniperrifle = true;

	//Ensures that player does not have anything equippped at the beginning.
	IsCarryingStaff = false;
	IsCarryingShuriken = false;
	IsCarryingSword = false;
	IsCarryingSniperRifle = false;
}

//Most likely will be applied in the future part of the game and m_speed will be removed.
/*
void Player::move(glm::vec3 accelerate, double dt)
{
	m_gbp->set_acceleration(accelerate);
}*/


void Player::on_update(const engine::timestep& time_step) {
	m_gbp->set_position(m_gbp->position() += m_gbp->forward() * m_speed * (float)time_step);
	m_gbp->set_rotation_amount(atan2(m_gbp->forward().x, m_gbp->forward().z));

	///Animations of the player.
	//If character is not moving, Idle animation will be introduced, if Character is walking, Walking animation will be implied.
	if (isWalking == true && m_speed == 0.f) {
		isWalking = false;
		m_gbp->animated_mesh()->switch_animation(m_gbp->animated_mesh()->default_animation());
	}
	else if (isWalking != true && m_speed > 0.f) {
		isWalking = true;
		m_gbp->animated_mesh()->switch_animation(1);
	}
	//If player is walking and presses Left Shift, the character will sprint. 
	else if (m_speed == 2.f && engine::input::key_pressed(engine::key_codes::KEY_LEFT_SHIFT)) {
		m_gbp->animated_mesh()->switch_animation(4);
		m_speed = 4.f;
	}

	//Movement keys for the player.
	if (engine::input::key_pressed(engine::key_codes::KEY_W)) //Allows the player to move straight
		m_speed = 2.f;
	else if (engine::input::key_pressed(engine::key_codes::KEY_S)) //Allows the player to stop.
		m_speed = 0.f;
	else if (engine::input::key_pressed(engine::key_codes::KEY_A)) //Allows the player to move left.
		Turn(1.f * time_step);
	else if (engine::input::key_pressed(engine::key_codes::KEY_D)) //Allows the player to move right.
		Turn(-1.f * time_step);
	else if (engine::input::key_pressed(engine::key_codes::KEY_SPACE)) //Allows the player to perform task (jumping).
		Jump();
	else if (engine::input::key_pressed(engine::key_codes::KEY_F)) {
		TP = false;
		FP = true;
	}
	/*else if (engine::input::key_pressed(engine::key_codes::KEY_Q)) {
		std::cout << equipment[0] << "\n";
		std::cout << equipment[1] << "\n";
		std::cout << equipment[2] << "\n";
		std::cout << equipment[3] << "\n";
	}*/
	//CAMERA choices:
	if (engine::input::key_pressed(engine::key_codes::KEY_F1)) { //First person camera view.
		FP = true;
		TP = false;
	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_F2)) { //Third person camera view.
		IsCarryingStaff = false;
		IsCarryingShuriken = false;
		IsCarryingSword = false;
		IsCarryingSniperRifle = false;
		FP = false;
		TP = true;
	}

	//Equipment Keys for the player:
	if (engine::input::key_pressed(engine::key_codes::KEY_1) && ownastaff) {
		TP = false;
		FP = true;
		IsCarryingStaff = true;
		IsCarryingShuriken = false;
		IsCarryingSword = false;
		IsCarryingSniperRifle = false;
	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_2) && ownasword) {
		TP = false;
		FP = true;
		IsCarryingStaff = false;
		IsCarryingShuriken = false;
		IsCarryingSword = true;
		IsCarryingSniperRifle = false;
	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_3) && ownashuriken) {
		TP = false;
		FP = true;
		IsCarryingStaff = false;
		IsCarryingShuriken = true;
		IsCarryingSword = false;
		IsCarryingSniperRifle = false;
	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_4) && ownasniperrifle) {
		TP = false;
		FP = true;
		IsCarryingStaff = false;
		IsCarryingShuriken = false;
		IsCarryingSword = false;
		IsCarryingSniperRifle = true;
	}

	//test
	if (engine::input::key_pressed(engine::key_codes::KEY_Z)) {
		ownashuriken = !ownashuriken;
	}
		
	///Player Mechanics.
	//If player's health is equal to 0, player will die and the game will end.
	if (health <= 0) {
		isPlayerAlive = false;
	}
	//Ensures that player does not exceeds 100% of their health.
	if (health > 100)
		health = 100;
	//Ensures that player does not have less health than 0%.
	if (health < 0)
		health = 0;
	//Ensures that player does not exceed allowed amount for grenades.
	if (grenades > 10)
		grenades = 10;
	//Ensures that player does not have less grenades than 0 if bug would occur.
	if (grenades < 0)
		grenades = 0;
	
	//Jump Method. Player will stop and perform a jump task. 
	if (m_timer > 0.f) {
		m_timer -= (float)time_step;
		if (m_timer < 0.f) {
			m_gbp->animated_mesh()->switch_root_movement(false);
			m_gbp->animated_mesh()->switch_animation(m_gbp->animated_mesh()->default_animation());
			m_speed = 0.f;
			//Bool added for extra activities that will be implented into the game in future to be implemented.
			isJumping = false;
		}
	}

	m_gbp->animated_mesh()->on_update(time_step);
}

void Player::Turn(float angle){
	m_gbp->set_forward(glm::rotate(m_gbp->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
}

void Player::Jump(){
	//root movement added in order to stop the player from moving. Will be required in future activities. 
	m_gbp->animated_mesh()->switch_root_movement(true);
	m_gbp->animated_mesh()->switch_animation(3);
	m_timer = m_gbp->animated_mesh()->animations().at(3)->mDuration;
	isJumping = true;
}

void Player::update_camera(engine::perspective_camera& camera) {

	float a, b, c;

	//Implements the choice between First person and Third person camera view. 
	if (FP == true && TP == false) {
		a = 1.1f;
		b = 0.001f;
		c = 6.f;
	}
	else if (FP == false && TP == true) {
		a = 2;
		b = 3;
		c = 6.f;
	}

	glm::vec3 camera_pos = m_gbp->position() - glm::normalize(m_gbp->forward()) * b;
	glm::vec3 camera_lookat = m_gbp->position() + glm::normalize(m_gbp->forward()) * c;

	camera_pos.y += a;
	camera_lookat.y = 1.f;

	camera.set_view_matrix(camera_pos, camera_lookat);
}

//Health
void Player::Set_Health(int a_health) {
	health = a_health;
}

int Player::Damage_Taken(int a_health) {
	health -= a_health;
	return health;
}

int Player::Heal(int a_health) {
	health += a_health;
	return health;
}

int Player::Get_Health() { return health; }

//Grenades
int Player::Get_Grenades() { return grenades; }

void Player::Set_Grenades(int w_grenades) {
	grenades = w_grenades;
}

int Player::Throw_Grenade(int w_grenades) {
	grenades -= w_grenades;
	return grenades;
}

int Player::Collect_Grenades(int w_grenades) {
	grenades += w_grenades;
	return grenades;
}

//Special AMmunition
int Player::Get_SpecialAmm() { return specialamm; }

void Player::Set_SpecialAmm(int a_specialamm) {
	specialamm = a_specialamm;
}

int Player::Throw_SpecialAmm(int a_amm) {
	specialamm -= a_amm;
	return specialamm;
}

int Player::Collect_SpecialAmm(int a_amm) {
	specialamm += a_amm;
	return specialamm;
}

//Ammunition
void Player::Set_Ammunition(int a_ammunition) {
	ammunition = a_ammunition;
}

int Player::Get_Ammunition() { return ammunition; }

int Player::Collect_Ammunition(int a_ammunition) {
	ammunition += a_ammunition;
	return ammunition;
}

int Player::Shoot_Ammunition(int a_ammunition) {
	ammunition -= a_ammunition;
	return ammunition;
}


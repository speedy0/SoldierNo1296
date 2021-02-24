#pragma once
#include <engine.h>
#include "Player.h"
#include "Animals.h"
#include "zombie.h"
#include "wolf.h"
#include "boss.h"
#include "Pickup.h"
#include "CrossF.h"
#include "billboard.h"
#include "grenades.h"
#include "SpecialAmmunition.h"
#include "Ammunition.h"
#include "engine/entities/bounding_box.h"

class Pickup;

class example_layer : public engine::layer
{
public:
	example_layer();
	~example_layer();

	void on_update(const engine::timestep& time_step) override;
	void on_render() override;
	void on_event(engine::event& event) override;



private:
	void check_bounce();
	void platforms();
	int platform_score();
	int secret_score();
	void Alive();

	//Game Objects
	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_cow{};
	engine::ref<engine::game_object>	m_cow1{};
	engine::ref<engine::game_object>	m_tree{};
	engine::ref<engine::game_object>	m_tree1{};
	engine::ref<engine::game_object>	m_tree2{};
	engine::ref<engine::game_object>	m_grenad{};
	engine::ref<engine::game_object>	m_ammun{};
	engine::ref<engine::game_object>	m_wolf{};
	engine::ref<engine::game_object>	m_wolf1{};
	engine::ref<engine::game_object>	m_bot{};
	engine::ref<engine::game_object>	m_robot{};
	engine::ref<engine::game_object>	m_zombie{};
	//engine::ref<engine::game_object>	m_zombie1{};
	//engine::ref<engine::game_object>	m_zombie2{};
	//engine::ref<engine::game_object>	m_zombie3{};
	//engine::ref<engine::game_object>	m_zombie4{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_tetrahedron{};
	engine::ref<engine::game_object>	m_pentahedron{};
	engine::ref<engine::game_object>	m_pentahedron1{};
	engine::ref<engine::game_object>	m_pentahedron2{};
	engine::ref<engine::game_object>	m_pentahedron3{};
	engine::ref<engine::game_object>	m_pentahedron4{};
	engine::ref<engine::game_object>	m_pentahedron5{};
	engine::ref<engine::game_object>	m_pentahedron6{};
	engine::ref<engine::game_object>	m_pentahedron7{};
	engine::ref<engine::game_object>	m_pentahedron8{};
	engine::ref<engine::game_object>	m_platform{};
	engine::ref<engine::game_object>	m_platform2{};
	engine::ref<engine::game_object>	m_platform3{};
	engine::ref<engine::game_object>	m_platform4{};
	engine::ref<engine::game_object>	m_platform5{};
	//engine::ref<engine::game_object>	m_skull{};
	engine::ref<engine::game_object>	m_street{};
	engine::ref<engine::game_object>	m_wall{};
	engine::ref<engine::game_object>	m_house1{};
	engine::ref<engine::game_object>	m_house12{};
	engine::ref<engine::game_object>	m_house2{};
	engine::ref<engine::game_object>	m_house3{};
	engine::ref<engine::game_object>	m_jeep{};
	engine::ref<engine::game_object>	m_jeep1{};
	engine::ref<engine::game_object>	m_rock{};
	engine::ref<engine::game_object>	m_staffo{};
	engine::ref<engine::game_object>	m_shurikeno{};
	engine::ref<engine::game_object>	m_swordo{};
	engine::ref<engine::game_object>	m_sniperrifleo{};

	//Weapons
	grenades							m_grenade{};
	engine::bounding_box				m_grenade_box{};
	bool isGrenade = true;
	specialamm							m_bulletshot{};
	bool isSpecialAmm = true;
	Ammunition							m_ammunition{};
	engine::bounding_box				m_ammunition_box{};

	//Player
	Player								m_player{};
	engine::bounding_box				m_player_box{};
	bool isJumping = true;

	//Animals
	Animals								m_cowa{};
	Animals								m_cowa1{};
	engine::bounding_box				m_cow_box{};
	engine::bounding_box				m_cow_box1{};
	bool isCow = true;
	bool isCow1 = true;


	//Enemies
	zombie								m_zomb{};
	//zombie								m_zomb1{};
	//zombie								m_zomb2{};
	//zombie								m_zomb3{};
	//zombie								m_zomb4{};
	engine::bounding_box				m_zombie_box{};
	//engine::bounding_box				m_zombie1_box{};
	//engine::bounding_box				m_zombie2_box{};
	//engine::bounding_box				m_zombie3_box{};
	//engine::bounding_box				m_zombie4_box{};
	bool isZombie1 = true;
	//bool isZombie2 = true;
	//bool isZombie3 = true;
	//bool isZombie4 = true;
	//bool isZombie5 = true;
	wolf								m_wol{};
	wolf								m_wol1{};
	engine::bounding_box				m_wolf_box{};
	engine::bounding_box				m_wolf1_box{};
	bool isWolf1 = true;
	bool isWolf2 = true;
	boss								m_boss{};
	engine::bounding_box				m_bot_box{};
	bool isBoss = true;
	//robot								m_rob{};
	engine::bounding_box				m_robot_box{};
	bool isRobot = true;

	//AABB for objects.
	engine::bounding_box				m_jeep_box{};
	engine::bounding_box				m_jeep1_box{};
	//First row of trees from the player
	engine::bounding_box				m_tree_box{};
	engine::bounding_box				m_tree2_box{};
	engine::bounding_box				m_tree3_box{};
	engine::bounding_box				m_tree4_box{};
	engine::bounding_box				m_tree5_box{};
	engine::bounding_box				m_tree6_box{};
	engine::bounding_box				m_tree7_box{};
	engine::bounding_box				m_tree8_box{};
	engine::bounding_box				m_tree9_box{};
	//Second row of trees from the player
	engine::bounding_box				m_tree10_box{};
	engine::bounding_box				m_tree11_box{};
	engine::bounding_box				m_tree12_box{};
	engine::bounding_box				m_tree13_box{};
	engine::bounding_box				m_tree14_box{};
	engine::bounding_box				m_tree15_box{};
	engine::bounding_box				m_tree16_box{};
	engine::bounding_box				m_tree17_box{};
	//Third row of trees from the player.
	engine::bounding_box				m_tree18_box{};
	engine::bounding_box				m_tree19_box{};
	engine::bounding_box				m_tree20_box{};
	engine::bounding_box				m_tree21_box{};
	engine::bounding_box				m_tree22_box{};
	engine::bounding_box				m_tree23_box{};
	engine::bounding_box				m_tree24_box{};
	//Walls & Houses
	engine::bounding_box				m_wall_box{};
	engine::bounding_box				m_wall1_box{};
	engine::bounding_box				m_house1_box{};
	//Rocks
	engine::bounding_box				m_rock_box{};
	engine::bounding_box				m_rock1_box{};
	engine::bounding_box				m_rock2_box{};
	engine::bounding_box				m_rock3_box{};
	engine::bounding_box				m_rock4_box{};
	engine::bounding_box				m_rock5_box{};
	engine::bounding_box				m_rock6_box{};
	engine::bounding_box				m_rock7_box{};
	engine::bounding_box				m_rock8_box{};
	engine::bounding_box				m_rock9_box{};
	engine::bounding_box				m_rock10_box{};
	engine::bounding_box				m_rock11_box{};
	engine::bounding_box				m_rock12_box{};
	engine::bounding_box				m_rock13_box{};

	//Pickups
	engine::ref<Pickup>					m_staff{};
	engine::ref<Pickup>					m_shuriken{};
	engine::ref<Pickup>					m_sword{};
	engine::ref<Pickup>					m_sniper{};
	engine::ref<Pickup>					m_SmallHealth{};
	engine::ref<Pickup>					m_MediumHealth{};
	engine::ref<Pickup>					m_LargeHealth{};

	//Materials
	engine::ref<engine::material>		m_material{};
	engine::ref<engine::material>		m_tetrahedron_material{};
	engine::ref<engine::material>		m_pentahedron_material{};
	engine::ref<engine::material>		m_bulletshape_material{};
	engine::ref<engine::material>		m_platform_material{};
	engine::ref<engine::material>		m_lightsource_material{};
	engine::ref<engine::material>		m_ammunition_material{};

	//Lights
	engine::DirectionalLight            m_directionalLight;
	engine::SpotLight					m_spotlight{};
	engine::ref<engine::game_object>	m_light{};
	uint32_t							num_spotlights = 1;
	engine::PointLight					m_pointlight{};
	uint32_t							num_pointlights = 1;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;

	//engine::ref<engine::heightmap>		m_heightmap;
	//float								m_physical_terrain_height = 1.f;

	//Adds fog into the game.
	bool m_fog;
	glm::vec3 m_fog_colour;
	int32_t m_fog_factor_type;

	//Bools for the platforms.
	bool isPlatform1 = true;
	bool isPlatform2 = true;
	bool isPlatform3 = true;
	bool isPlatform4 = true;
	bool isPlatform5 = true;

	//Bool to turn on AABB shapes.
	bool isAABB = false; //will be initialised when game starts.

	//Adds CrossFade into the game
	engine::ref<CrossFade>				m_crossfade{};
	engine::ref<CrossFade>				m_crossfadeheal{};
	engine::ref<CrossFade>				m_crossfadeend{};
	engine::ref<CrossFade>				m_crossfadecomplete{};

	//Adds billboard onto the game.
	engine::ref<billboard>				m_billboard{};

	//Bool to start a game.
	bool StartG = false;
	bool EndGame = false;
	bool FinishLevel = false;
	bool Pause = false;

	int platformscore = 0;
	int secretscore = 0;

	//Strings for the HUD Display.
	std::string healthhud; //Displays Health
	std::string platformhud; //Displays collected points.
	std::string secretscorehud; //Displays secret score points, those could be turn into scrap in the future.
	std::string grenadeshud; //Displays ammunition of grenades.
	std::string scrapamount; //Displays amount of scrap.
	std::string specialammhud; //Displays amount of special ammunition held by a player.
	std::string ammunitionhud; //Displays amount of normal ammunition held by a player.

	//timers 
	float m_timer1, m_timer2, m_timer3, m_timer4, m_timer5;
	float m_wolf1_timer;

	//Enemy attacks
	bool Wolf1Bitten = false;

	//Sound system:
	float m_freq_high;
	float m_freq_low;

	//Sets the level of the game:
	bool isHard = false;
	bool isEasy = false;
};

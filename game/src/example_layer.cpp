#pragma once
#include "example_layer.h"
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"

example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())


{
	//Initialises the fog onto the scene.
	//Fog will be turned on throughout the game, but for testing purposes it can be turned off using button 6.
	m_fog = true;
	glm::vec3 m_fog_colour = glm::vec3(0.5f);
	int m_fog_factor_type = 3;
	
	m_freq_high = 0.f;
	m_freq_low = 20000.f;

	// Hide the mouse and lock it inside the window
	//engine::input::anchor_mouse(true);
	engine::application::window().hide_mouse_cursor();

	// Initialises audio and plays background music
	//	Bounce file is provided by the lecturer and obtained from: Royalty free sound from freesound.org
	//		Background music has been obtained from The Mirages https://www.youtube.com/watch?v=IokQDPrFZ8E Created by: Aleksander Napieralski
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	//Man screaming has been sourced from: https://freesound.org/people/alexaxy/sounds/278177/
	//m_audio_manager->load_sound("assets/audio/effects/screams/man_screaming.wav", engine::sound_type::spatialised, "manscreaming");
	m_audio_manager->load_sound("assets/audio/effects/screams/man_screaming.wav", engine::sound_type::event, "manscreaming");
	//Car explosion has been sourced from: https://freesound.org/people/Iwiploppenisse/sounds/156031/
	//m_audio_manager->load_sound("assets/audio/effects/car_explosion.mp3", engine::sound_type::spatialised, "carexplosion");
	m_audio_manager->load_sound("assets/audio/effects/car_explosion.mp3", engine::sound_type::event, "carexplosion");
	//attack has been sourced from: https://freesound.org/people/soykevin/sounds/213509/
	//m_audio_manager->load_sound("assets/audio/effects/wave/zombie-attack.wav", engine::sound_type::spatialised, "zom_attack");
	m_audio_manager->load_sound("assets/audio/effects/wave/zombie-attack.wav", engine::sound_type::event, "zom_attack");
	//random zombie noises around has been sourced from: https://freesound.org/people/Under7dude/sounds/163440/
	m_audio_manager->load_sound("assets/audio/effects/wave/zombie-idle.wav", engine::sound_type::event, "idlezombie");
	//Boss' arrival sound has been sourced from: https://freesound.org/people/timtube/sounds/61258/
	//m_audio_manager->load_sound("assets/audio/effects/wave/boss_arrival.wav", engine::sound_type::spatialised, "bossarrival");
	m_audio_manager->load_sound("assets/audio/effects/wave/boss_arrival.wav", engine::sound_type::event, "bossarrival");
	//Gunshots that do appear in the distance are sourced from: https://freesound.org/people/16FThumaF/sounds/505264/
	//m_audio_manager->load_sound("assets/audio/effects/guns/shots_in_the_distance.wav", engine::sound_type::spatialised, "shots_in_the_distance");
	m_audio_manager->load_sound("assets/audio/effects/guns/shots_in_the_distance.wav", engine::sound_type::event, "shots_in_the_distance");
	//Background music has been obtained from The Mirages https://www.youtube.com/watch?v=IokQDPrFZ8E Created by: Aleksander Napieralski
	m_audio_manager->load_sound("assets/audio/Track1-SilentDream.mp3", engine::sound_type::track, "music");
	m_audio_manager->play("music");
	//m_audio_manager->loop("music", true);

	//m_audio_manager->create_high_pass_filter();
	//m_audio_manager->set_high_pass_filter(m_freq_high);
	//m_audio_manager->create_low_pass_filter();
	//m_audio_manager->set_low_pass_filter(m_freq_low);


	// Initialises shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

	m_directionalLight.Color = glm::vec3(0.3f, 0.3f, 0.7f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	m_spotlight.Color = glm::vec3(1.f, 0.f, 1.f);
	m_spotlight.AmbientIntensity = 0.25f;
	m_spotlight.DiffuseIntensity = 0.6f;
	m_spotlight.Position = glm::vec3(-17.0462f, 1.f, -14.2095f);
	m_spotlight.Direction = glm::vec3(-1.f, -0.1f, 0.f);
	m_spotlight.Cutoff = 0.1f;
	m_spotlight.Attenuation.Constant = 1.f;
	m_spotlight.Attenuation.Linear = 0.1f;
	m_spotlight.Attenuation.Exp = 0.1f;

	m_pointlight.Color = glm::vec3(1.f, 1.f, 1.f);
	m_pointlight.AmbientIntensity = 0.25f;
	m_pointlight.DiffuseIntensity = 0.6f;
	m_pointlight.Position = glm::vec3(-25.38f, 3.5f, -12.95f);

	// Seting color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity", 0.8f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
			(float)engine::application::window().height()));

	//Sets RGB for Materials.
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.f);
	m_lightsource_material = engine::material::create(1.f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f), 1.f);
	m_ammunition_material = engine::material::create(1.f, glm::vec3(0.f, 1.f, 1.f), glm::vec3(0.f, 1.f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f), 1.f);
	m_tetrahedron_material = engine::material::create(32.f, glm::vec3(1.f, 0.5f, 0.f), glm::vec3(1.f, 0.5f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f), 0.3f);
	m_pentahedron_material = engine::material::create(32.f, glm::vec3(1.f, 0.5f, 0.f), glm::vec3(1.f, 0.5f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f), 0.3f);
	m_bulletshape_material = engine::material::create(32.f, glm::vec3(1.f, 0.5f, 0.f), glm::vec3(1.f, 0.5f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f), 0.3f);
	m_platform_material = engine::material::create(32.f, glm::vec3(1.f, 0.5f, 0.f), glm::vec3(1.f, 0.5f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f), 0.3f);


	// Skybox sourced from https://gamebanana.com/textures/6031 / it has also been converted from vtf file onto bmp file using VTFEdit.
	m_skybox = engine::skybox::create(55.f,
		{ engine::texture_2d::create("assets/textures/sb/right.bmp", true),
		  engine::texture_2d::create("assets/textures/sb/front.bmp", true),
		  engine::texture_2d::create("assets/textures/sb/left.bmp", true),
		  engine::texture_2d::create("assets/textures/sb/back.bmp", true),
		  engine::texture_2d::create("assets/textures/sb/up.bmp", true),
		  engine::texture_2d::create("assets/textures/sb/down.bmp", true)
		});

	//Animated Mesh sourced from:  https://www.turbosquid.com/3d-models/free-mannequin-male-3d-model/1005602.
	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae"); //-(0)-
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae"); // -Switch_animation(1) -> walking-
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");		//-Switch_animation(2) -> idle-
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");		//-Switch_animation(3) -> jumping-
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");	//-Switch_animation(4) -> running-
	m_skinned_mesh->switch_root_movement(false);

	//Initialises properties for the mannequin that will be used as a player.
	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(3.0f, 0.5f, -5.0f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 2.f, m_skinned_mesh->size().y, m_skinned_mesh->size().x / 2.f);
	m_mannequin = engine::game_object::create(mannequin_props);
	m_player.Initialise(m_mannequin);
	m_player_box.set_box(mannequin_props.bounding_shape.x* mannequin_props.scale.x, mannequin_props.bounding_shape.y* mannequin_props.scale.x, mannequin_props.bounding_shape.z* mannequin_props.scale.x, mannequin_props.position);

	//Using the mannequin, Zombie will be created.
	engine::ref<engine::skinned_mesh> m_zombie_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_zombie_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_zombie_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");		
	m_zombie_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");	
	m_zombie_mesh->switch_root_movement(false);

	//Initialises zombies that will be same as mannequin.
	engine::game_object_properties zombie_props;
	zombie_props.animated_mesh = m_zombie_mesh;
	zombie_props.scale = glm::vec3(1.f / glm::max(m_zombie_mesh->size().x, glm::max(m_zombie_mesh->size().y, m_zombie_mesh->size().z)));
	zombie_props.position = glm::vec3(3.0f, 0.5f, 0.f);
	zombie_props.type = 0;
	zombie_props.bounding_shape = glm::vec3(m_zombie_mesh->size().x / 2.f, m_zombie_mesh->size().y, m_zombie_mesh->size().x / 2.f);
	m_zombie = engine::game_object::create(zombie_props);
	m_zombie_box.set_box(zombie_props.bounding_shape.x * zombie_props.scale.x, zombie_props.bounding_shape.y * zombie_props.scale.x, zombie_props.bounding_shape.z * zombie_props.scale.x, zombie_props.position);
	m_zomb.initialise(m_zombie, zombie_props.position, glm::vec3(0.f, 0.f, 1.f));

	//Loads in the Wolves and initialises them.
	//Wolf sourced from: https://free3d.com/dl-files.php?p=566e1dcb26be8bdb4a8b4567&f=0
	engine::ref<engine::skinned_mesh> m_wolf_mesh = engine::skinned_mesh::create("assets/models/animated/wolf/Wolf_dae.dae");
	m_wolf_mesh->LoadAnimationFile("assets/models/animated/wolf/Wolf_dae.dae");
	m_wolf_mesh->switch_root_movement(false);
	engine::game_object_properties wolf_props;
	wolf_props.animated_mesh = m_wolf_mesh;
	wolf_props.scale = glm::vec3(150.f, 150.f, 150.f);
	wolf_props.position = glm::vec3(8.18f, 0.5f, 22.21f);
	wolf_props.type = 0;
	wolf_props.bounding_shape = glm::vec3(m_wolf_mesh->size().x / 2.f, m_wolf_mesh->size().y, m_wolf_mesh->size().x / 2.f);
	m_wolf = engine::game_object::create(wolf_props);
	m_wolf_box.set_box(1.2f, 1.2f, 1.2f, wolf_props.position);
	m_wol.initialise(m_wolf, wolf_props.position, glm::vec3(0.f, 0.f, 1.f));
	
	engine::game_object_properties wolf_props1;
	wolf_props1.animated_mesh = m_wolf_mesh;
	wolf_props1.scale = glm::vec3(150.f, 150.f, 150.f);
	wolf_props1.position = glm::vec3(8.43f, 0.5f, -33.51f);
	wolf_props1.type = 0;
	wolf_props1.bounding_shape = glm::vec3(m_wolf_mesh->size().x / 2.f, m_wolf_mesh->size().y, m_wolf_mesh->size().x / 2.f);
	m_wolf1 = engine::game_object::create(wolf_props1);
	m_wolf1_box.set_box(1.2f, 1.2f, 1.2f, wolf_props1.position);
	m_wol1.initialise(m_wolf1, wolf_props1.position, glm::vec3(0.f, 0.f, 1.f));

	//Loads gun_bot into the game and initialises it. This mesh will be a considered as a final boss.
	//Gun_bot sourced from: https://free3d.com/3d-model/gun-bot-78928.html
	engine::ref<engine::skinned_mesh> m_bot_mesh = engine::skinned_mesh::create("assets/models/animated/boss/boss.dae");
	m_bot_mesh->LoadAnimationFile("assets/models/animated/boss/boss.dae");
	m_bot_mesh->switch_root_movement(false);
	engine::game_object_properties bot_props;
	bot_props.animated_mesh = m_bot_mesh;
	bot_props.scale = glm::vec3(550.f, 550.f, 550.f);
	bot_props.position = glm::vec3(33.94f, 0.5f, -13.81f);
	bot_props.type = 0;
	bot_props.bounding_shape = m_bot_mesh->size();
	m_bot = engine::game_object::create(bot_props);
	m_bot_box.set_box(12.f, 12.f, 12.f, bot_props.position);
	m_boss.initialise(m_bot, bot_props.position, glm::vec3(-1.f, 0.f, 0.f));

	/// Unused heightmap due to performance issues. Heightmap was sourced from: https://miro.medium.com/max/8192/1*NRAKXcQCD4m-f-QP9crdCg.png
	/*
	m_heightmap = engine::heightmap::create("assets/textures/terrainHe.bmp", "assets/textures/darkroad.jpg", 50.f, 50.f, glm::vec3(0.f, 0.f, 0.f), 10.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { m_heightmap->mesh() };
	terrain_props.textures = { m_heightmap->texture() };
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(m_heightmap->terrain_size().x, m_physical_terrain_height, m_heightmap->terrain_size().y);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);
	*/
	///

	// Loads Terrain's texture and creates terrain mesh.
	//	Texture has been found on https://www.stockvault.net/data/2017/11/30/240399/preview16.jpg -
	//		Terrain has been resized to smaller as the level will need much less space. - 
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/darkroad.jpg", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(50.f, 0.5f, 50.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(50.f, 0.5f, 50.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	// Load the cow model. Wanders around the base. Cow model has been given during the labs.
	engine::ref <engine::model> cow_model = engine::model::create("assets/models/static/cow4.3ds");
	engine::game_object_properties cow_props;
	cow_props.meshes = cow_model->meshes();
	cow_props.textures = cow_model->textures();
	float cow_scale = 1.f / glm::max(cow_model->size().x, glm::max(cow_model->size().y, cow_model->size().z));
	cow_props.position = { -24.f,0.5f, 2.8f };
	cow_props.scale = glm::vec3(cow_scale);
	cow_props.bounding_shape = cow_model->size() / 2.f * cow_scale;
	m_cow = engine::game_object::create(cow_props);
	m_cow->set_offset(cow_model->offset());
	m_cow_box.set_box(1.1f, 1.f, 1.1f, cow_props.position);
	m_cowa.initialise(m_cow, m_cow->position(), glm::vec3(0.5f, 0.f, 0.5f));

	engine::game_object_properties cow_props1;
	cow_props1.meshes = cow_model->meshes();
	cow_props1.textures = cow_model->textures();
	cow_props1.position = { 0.48f, 0.5f, -22.75f };
	cow_props1.scale = glm::vec3(cow_scale);
	cow_props1.bounding_shape = cow_model->size() / 2.f * cow_scale;
	m_cow1 = engine::game_object::create(cow_props1);
	m_cow1->set_offset(cow_model->offset());
	m_cow_box1.set_box(1.1f, 1.f, 1.1f, cow_props1.position);
	m_cowa1.initialise(m_cow1, m_cow1->position(), glm::vec3(1.f, 0.f, 0.2f));
	
	//Load the rock, sourced from: https://free3d.com/3d-model/low-poly-rock-4631.html
	engine::ref <engine::model> rock_model = engine::model::create("assets/models/static/Rock.obj");
	std::vector<engine::ref<engine::texture_2d>> rock_textures = { engine::texture_2d::create("assets/textures/rock.jpg", false) };
	engine::game_object_properties rock_props;
	rock_props.meshes = rock_model->meshes();
	rock_props.textures = { rock_textures };
	float rock_scale = 1.f / glm::max(rock_model->size().x, glm::max(rock_model->size().y, rock_model->size().z));
	rock_props.position = { -2.53f,0.5f, -2.36f };
	rock_props.scale = glm::vec3(rock_scale);
	rock_props.bounding_shape = rock_model->size() / 2.f * rock_scale;
	m_rock = engine::game_object::create(rock_props);
	m_rock_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-11.9768f, 0.5f, -2.76f));
	m_rock1_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-11.9768f, 0.5f, -2.76f + 3.605f));
	m_rock2_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-11.9768f, 0.5f, -2.76f + 7.21f));
	m_rock3_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-11.9768f, 0.5f, -2.76f + 10.815f));
	m_rock4_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-11.9768f, 0.5f, -2.76f + 14.42f));
	m_rock5_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-11.9768f, 0.5f, -2.76f - 3.605));
	m_rock6_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-11.9768f, 0.5f, -2.76f - 7.21f));
	m_rock7_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-11.9768f, 0.5f, -2.76f - 10.815f)); 
	m_rock8_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-0.220715f, 0.5f, -16.6155f));
	m_rock9_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-2.5109f, 0.5f, -2.37599f));
	m_rock10_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-0.302047f, 0.5f, -0.00996378f));
	m_rock11_box.set_box(1.f, 0.5f, 1.f, glm::vec3(5.25425f, 0.5f, 3.54644f));
	m_rock12_box.set_box(1.f, 0.5f, 1.f, glm::vec3(10.1825f, 0.5f, 22.8756f));
	m_rock13_box.set_box(1.f, 0.5f, 1.f, glm::vec3(-6.07815f, 0.5f, 23.1256f));
	

	//Load the Jeeps, sourced from the labs.
	//Only one jeep is created, even though three are rendered, it is because second and third does not need a bounding box as it will be out of the playing scence for both, AI and a player.
	engine::ref <engine::model> jeep_model = engine::model::create("assets/models/static/jeep1.3ds");
	engine::game_object_properties jeep_props;
	jeep_props.meshes = jeep_model->meshes();
	jeep_props.textures = jeep_model->textures();
	float jeep_scale = 0.18f;
	jeep_props.position = { 3.09f, 0.5f, -13.f };
	jeep_props.scale = glm::vec3(jeep_scale);
	jeep_props.bounding_shape = jeep_model->size();
	m_jeep = engine::game_object::create(jeep_props);
	m_jeep->set_offset(jeep_model->offset()); //offset has to be created in order to fit AABB correctly. 
	m_jeep_box.set_box(jeep_props.bounding_shape.x * jeep_props.scale.x * 1.5f, jeep_props.bounding_shape.y * jeep_props.scale.x / 2.f, jeep_props.bounding_shape.z * jeep_props.scale.x * 2.5f, jeep_props.position);

	//Loads and creates Street model.
	//Sourced mesh from: https://free3d.com/3d-model/old-road-93173.html
	//Tesxture is sourced from: https://cdna.artstation.com/p/assets/images/images/014/040/290/large/jordan-holbrook-rox.jpg?1542198581
	//Road is purposely rescaled in order to cut half of the street for the purpose of the game, as cars would enter the road from the houses.
	engine::ref <engine::model> street_mod = engine::model::create("assets/models/static/road.obj");
	std::vector<engine::ref<engine::texture_2d>> street_text = { engine::texture_2d::create("assets/textures/street.jpg", true) };
	engine::game_object_properties street_props;
	street_props.meshes = street_mod->meshes();
	street_props.textures = { street_text };
	street_props.is_static = true;
	float street_scale = 2.f / glm::max(street_mod->size().x, glm::max(street_mod->size().y, street_mod->size().z));
	street_props.position = { -4.28f, 0.5f, -3.65f };
	street_props.scale = glm::vec3(street_scale);
	m_street = engine::game_object::create(street_props);

	//Loads roof of the main building inside the game (Current Army defending camp). Tetrahedron was repositioned and created much bigger due to the size of a building.
	//Roof is purposely made larger in order to be outside of the walls to make it look more realistic.
	//Vertices can be set to (18.5, 0.f, 50.095f) with correct (positive, negative) sign to make it align with the walls.
	//Texture is sourced from: https://lh3.googleusercontent.com/proxy/iN9liLZDNTBKhfgHVCBCrqoVsRMnAmz-hNLoLMwQIk4SJowSNNk-CjW8O1vccRY4whNuNgCBhGKqVKRYAampjRiI8KGc1nT0PeG3fqDBfV0BWna12e1AKy2F14oY3l0LySyMb_U
	std::vector<glm::vec3> pentahedron_vertices;
	std::vector<engine::ref<engine::texture_2d>> pentahedron_textures = { engine::texture_2d::create("assets/textures/roofcamp.jpg", false) };
	pentahedron_vertices.push_back(glm::vec3(0.f, 10.f, 0.f)); //Top vertex
	pentahedron_vertices.push_back(glm::vec3(20.f, 0.f, 52.f)); //Right front vertex 
	pentahedron_vertices.push_back(glm::vec3(20.f, 0.f, -52.f)); //Right back vertex
	pentahedron_vertices.push_back(glm::vec3(-20.f, 0.f, -52.f)); //Left back vertex
	pentahedron_vertices.push_back(glm::vec3(-20.f, 0.f, 52.f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape = engine::pentahedron::create(pentahedron_vertices);
	engine::game_object_properties pentahedron_props;
	pentahedron_props.position = { -31.6f, 4.07f, 0.f };
	pentahedron_props.meshes = { pentahedron_shape->mesh() };
	pentahedron_props.textures = pentahedron_textures;
	m_pentahedron = engine::game_object::create(pentahedron_props);

	//Roofs for all of the house_2 will now be created.
	//Texture is sourced from: https://www.filterforge.com/filters/4914.jpg
	std::vector<glm::vec3> pentahedron_vertices1;
	std::vector<engine::ref<engine::texture_2d>> pentahedron_textures1 = { engine::texture_2d::create("assets/textures/woodt.jpg", false) };
	pentahedron_vertices1.push_back(glm::vec3(0.f, 4.f, 0.f)); //Top vertex
	pentahedron_vertices1.push_back(glm::vec3(4.1213f, 0.f, 4.1213f)); //Right front vertex 
	pentahedron_vertices1.push_back(glm::vec3(4.1213f, 0.f, -4.1213f)); //Right back vertex
	pentahedron_vertices1.push_back(glm::vec3(-4.1213f, 0.f, -4.1213f)); //Left back vertex
	pentahedron_vertices1.push_back(glm::vec3(-4.1213f, 0.f, 4.1213f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape1 = engine::pentahedron::create(pentahedron_vertices1);
	engine::game_object_properties pentahedron_props1;
	pentahedron_props1.position = { 31.7f, 4.7f, -33.5f };
	pentahedron_props1.meshes = { pentahedron_shape1->mesh() };
	pentahedron_props1.textures = pentahedron_textures1;
	m_pentahedron1 = engine::game_object::create(pentahedron_props1);

	std::vector<glm::vec3> pentahedron_vertices2;
	pentahedron_vertices2.push_back(glm::vec3(0.f, 3.f, 0.f)); //Top vertex
	pentahedron_vertices2.push_back(glm::vec3(3.1213f, 0.f, 3.1213f)); //Right front vertex 
	pentahedron_vertices2.push_back(glm::vec3(3.1213f, 0.f, -3.1213f)); //Right back vertex
	pentahedron_vertices2.push_back(glm::vec3(-3.1213f, 0.f, -3.1213f)); //Left back vertex
	pentahedron_vertices2.push_back(glm::vec3(-3.1213f, 0.f, 3.1213f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape2 = engine::pentahedron::create(pentahedron_vertices2);
	engine::game_object_properties pentahedron_props2;
	pentahedron_props2.position = { 43.6f, 4.7f, 34.17f };
	pentahedron_props2.meshes = { pentahedron_shape2->mesh() };
	pentahedron_props2.textures = pentahedron_textures1;
	m_pentahedron2 = engine::game_object::create(pentahedron_props2);

	std::vector<glm::vec3> pentahedron_vertices3;
	pentahedron_vertices3.push_back(glm::vec3(0.f, 3.f, 0.f)); //Top vertex
	pentahedron_vertices3.push_back(glm::vec3(3.1213f, 0.f, 3.1213f)); //Right front vertex 
	pentahedron_vertices3.push_back(glm::vec3(3.1213f, 0.f, -3.1213f)); //Right back vertex
	pentahedron_vertices3.push_back(glm::vec3(-3.1213f, 0.f, -3.1213f)); //Left back vertex
	pentahedron_vertices3.push_back(glm::vec3(-3.1213f, 0.f, 3.1213f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape3 = engine::pentahedron::create(pentahedron_vertices3);
	engine::game_object_properties pentahedron_props3;
	pentahedron_props3.position = { 12.2f, 4.7f, 39.5f };
	pentahedron_props3.meshes = { pentahedron_shape3->mesh() };
	pentahedron_props3.textures = pentahedron_textures1;
	m_pentahedron3 = engine::game_object::create(pentahedron_props3);

	//Roofs of all the m_house1 will now be created.
	//Texture sourced from: https://www.textures.com/system/gallery/photos/Roofing/Wood/67521/RooftilesWood0047_4_download600.jpg
	std::vector<glm::vec3> pentahedron_vertices4;
	std::vector<engine::ref<engine::texture_2d>> pentahedron_textures2 = { engine::texture_2d::create("assets/textures/wdt1.jpg", false) };
	pentahedron_vertices4.push_back(glm::vec3(0.f, 2.f, 0.f)); //Top vertex
	pentahedron_vertices4.push_back(glm::vec3(2.1213f, 0.f, 2.1213f)); //Right front vertex 
	pentahedron_vertices4.push_back(glm::vec3(2.1213f, 0.f, -2.1213f)); //Right back vertex
	pentahedron_vertices4.push_back(glm::vec3(-2.1213f, 0.f, -2.1213f)); //Left back vertex
	pentahedron_vertices4.push_back(glm::vec3(-2.1213f, 0.f, 2.1213f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape4 = engine::pentahedron::create(pentahedron_vertices4);
	engine::game_object_properties pentahedron_props4;
	pentahedron_props4.position = { 15.3f, 3.5f, -3.2f };
	pentahedron_props4.meshes = { pentahedron_shape4->mesh() };
	pentahedron_props4.textures = pentahedron_textures2;
	m_pentahedron4 = engine::game_object::create(pentahedron_props4);

	std::vector<glm::vec3> pentahedron_vertices5;
	pentahedron_vertices5.push_back(glm::vec3(0.f, 2.f, 0.f)); //Top vertex
	pentahedron_vertices5.push_back(glm::vec3(2.1213f, 0.f, 2.1213f)); //Right front vertex 
	pentahedron_vertices5.push_back(glm::vec3(2.1213f, 0.f, -2.1213f)); //Right back vertex
	pentahedron_vertices5.push_back(glm::vec3(-2.1213f, 0.f, -2.1213f)); //Left back vertex
	pentahedron_vertices5.push_back(glm::vec3(-2.1213f, 0.f, 2.1213f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape5 = engine::pentahedron::create(pentahedron_vertices5);
	engine::game_object_properties pentahedron_props5;
	pentahedron_props5.position = { 10.f, 3.5f, -20.8f };
	pentahedron_props5.meshes = { pentahedron_shape5->mesh() };
	pentahedron_props5.textures = pentahedron_textures2;
	m_pentahedron5 = engine::game_object::create(pentahedron_props5);

	//All of the roofs for m_house3 will now be created.
	//Texture sourced from: https://www.the3rdsequence.com/texturedb/download/158/texture/jpg/1024/wood+roof+planks-1024x1024.jpg
	std::vector<glm::vec3> pentahedron_vertices6;
	std::vector<engine::ref<engine::texture_2d>> pentahedron_textures3 = { engine::texture_2d::create("assets/textures/wdt2.jpg", false) };
	pentahedron_vertices6.push_back(glm::vec3(0.f, 2.f, 0.f)); //Top vertex
	pentahedron_vertices6.push_back(glm::vec3(2.1213f, 0.f, 2.1213f)); //Right front vertex 
	pentahedron_vertices6.push_back(glm::vec3(2.1213f, 0.f, -2.1213f)); //Right back vertex
	pentahedron_vertices6.push_back(glm::vec3(-2.1213f, 0.f, -2.1213f)); //Left back vertex
	pentahedron_vertices6.push_back(glm::vec3(-2.1213f, 0.f, 2.1213f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape6 = engine::pentahedron::create(pentahedron_vertices6);
	engine::game_object_properties pentahedron_props6;
	pentahedron_props6.position = { 26.7f, 3.5f, -9.9f };
	pentahedron_props6.meshes = { pentahedron_shape6->mesh() };
	pentahedron_props6.textures = pentahedron_textures3;
	m_pentahedron6 = engine::game_object::create(pentahedron_props6);

	std::vector<glm::vec3> pentahedron_vertices7;
	pentahedron_vertices7.push_back(glm::vec3(0.f, 2.f, 0.f)); //Top vertex
	pentahedron_vertices7.push_back(glm::vec3(2.1213f, 0.f, 2.1213f)); //Right front vertex 
	pentahedron_vertices7.push_back(glm::vec3(2.1213f, 0.f, -2.1213f)); //Right back vertex
	pentahedron_vertices7.push_back(glm::vec3(-2.1213f, 0.f, -2.1213f)); //Left back vertex
	pentahedron_vertices7.push_back(glm::vec3(-2.1213f, 0.f, 2.1213f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape7 = engine::pentahedron::create(pentahedron_vertices7);
	engine::game_object_properties pentahedron_props7;
	pentahedron_props7.position = { 38.8f, 3.5f, 6.7f };
	pentahedron_props7.meshes = { pentahedron_shape7->mesh() };
	pentahedron_props7.textures = pentahedron_textures3;
	m_pentahedron7 = engine::game_object::create(pentahedron_props7);

	std::vector<glm::vec3> pentahedron_vertices8;
	pentahedron_vertices8.push_back(glm::vec3(0.f, 2.f, 0.f)); //Top vertex
	pentahedron_vertices8.push_back(glm::vec3(2.1213f, 0.f, 2.1213f)); //Right front vertex 
	pentahedron_vertices8.push_back(glm::vec3(2.1213f, 0.f, -2.1213f)); //Right back vertex
	pentahedron_vertices8.push_back(glm::vec3(-2.1213f, 0.f, -2.1213f)); //Left back vertex
	pentahedron_vertices8.push_back(glm::vec3(-2.1213f, 0.f, 2.1213f)); //Left front vertex
	engine::ref<engine::pentahedron> pentahedron_shape8 = engine::pentahedron::create(pentahedron_vertices8);
	engine::game_object_properties pentahedron_props8;
	pentahedron_props8.position = { 24.5f, 3.5f, 16.4f };
	pentahedron_props8.meshes = { pentahedron_shape8->mesh() };
	pentahedron_props8.textures = pentahedron_textures3;
	m_pentahedron8 = engine::game_object::create(pentahedron_props8);

	//Spawns a bullet shape on the map.
	//Those bullets will be also used as an ammunition for the final game as special bullets. 
	std::vector<glm::vec3> bullet_vertices;
	bullet_vertices.push_back(glm::vec3(0.f, 0.f, 0.f));
	bullet_vertices.push_back(glm::vec3(0.f, 0.f, 1.f));
	bullet_vertices.push_back(glm::vec3(1.f, 0.f, 1.f));
	bullet_vertices.push_back(glm::vec3(1.f, 0.f, 0.f));
	bullet_vertices.push_back(glm::vec3(1.f, 2.f, 0.f));
	bullet_vertices.push_back(glm::vec3(1.f, 2.f, 1.f));
	bullet_vertices.push_back(glm::vec3(0.f, 2.f, 1.f));
	bullet_vertices.push_back(glm::vec3(0.f, 2.f, 0.f));
	bullet_vertices.push_back(glm::vec3(0.5f, 3.f, 0.5f));
	engine::ref<engine::bulletshape> bullet_shape = engine::bulletshape::create(bullet_vertices);
	engine::game_object_properties bullet_props;
	bullet_props.position = { 100.5f, 1.5f, 0.4f };
	float bullet_scale = 0.05f;
	bullet_props.scale = glm::vec3(bullet_scale);
	bullet_props.meshes = { bullet_shape->mesh() };
	//bullet_props.textures = bullet_textures; //Unsure whether bullets will just use simple RGB colours or a texture.
	m_robot = engine::game_object::create(bullet_props);
	m_robot_box.set_box(0.2f, 0.1f, 0.2f, bullet_props.position);

	//Initialises and creates platform that are spawned on_render method.
	std::vector<engine::ref<engine::texture_2d>> platform_textures = { engine::texture_2d::create("assets/textures/platform.jpg", false) };
	std::vector<glm::vec3> platform_vertices;
	platform_vertices.push_back(glm::vec3(0.f, 0.f, 0.f)); //0
	platform_vertices.push_back(glm::vec3(0.f, 0.f, -1.f)); //1
	platform_vertices.push_back(glm::vec3(1.f, 0.f, -2.f)); //2
	platform_vertices.push_back(glm::vec3(2.f, 0.f, -1.f)); //3
	platform_vertices.push_back(glm::vec3(2.f, 0.f, 0.f)); //4
	platform_vertices.push_back(glm::vec3(1.f, 0.f, 1.f)); //5
	platform_vertices.push_back(glm::vec3(0.f, 0.5f, 0.f)); //6
	platform_vertices.push_back(glm::vec3(1.f, 0.5f, 1.f)); //7
	platform_vertices.push_back(glm::vec3(2.f, 0.5f, 0.f)); //8
	platform_vertices.push_back(glm::vec3(2.f, 0.5f, -1.f)); //9
	platform_vertices.push_back(glm::vec3(1.f, 0.5f, -2.f)); //10
	platform_vertices.push_back(glm::vec3(0.f, 0.5f, -1.f)); //11
	engine::ref<engine::platform> platform_shape = engine::platform::create(platform_vertices);
	engine::game_object_properties platform_props;
	platform_props.position = { -33.4041f, 0.5f, -4.62949f };
	float platform_scale = 1.0f; //0.05f;
	platform_props.scale = glm::vec3(platform_scale);
	platform_props.meshes = { platform_shape->mesh() };
	platform_props.textures = platform_textures;
	m_platform = engine::game_object::create(platform_props);

	engine::ref<engine::platform> platform_shape2 = engine::platform::create(platform_vertices);
	engine::game_object_properties platform_props2;
	platform_props2.position = { -11.76f, 0.5f, -7.46f };
	platform_props2.scale = glm::vec3(platform_scale);
	platform_props2.meshes = { platform_shape2->mesh() };
	platform_props2.textures = platform_textures;
	m_platform2 = engine::game_object::create(platform_props2);

	engine::ref<engine::platform> platform_shape3 = engine::platform::create(platform_vertices);
	engine::game_object_properties platform_props3;
	platform_props3.position = { 7.25f, 0.5f, 23.11f };
	platform_props3.scale = glm::vec3(platform_scale);
	platform_props3.meshes = { platform_shape3->mesh() };
	platform_props3.textures = platform_textures;
	m_platform3 = engine::game_object::create(platform_props3);

	engine::ref<engine::platform> platform_shape4 = engine::platform::create(platform_vertices);
	engine::game_object_properties platform_props4;
	platform_props4.position = { 3.3741f, 0.5f, 9.79632f };
	platform_props4.scale = glm::vec3(platform_scale);
	platform_props4.meshes = { platform_shape4->mesh() };
	platform_props4.textures = platform_textures;
	m_platform4 = engine::game_object::create(platform_props4);

	engine::ref<engine::platform> platform_shape5 = engine::platform::create(platform_vertices);
	engine::game_object_properties platform_props5;
	platform_props5.position = { 7.12175f, 0.5f, -16.0302f };
	platform_props5.scale = glm::vec3(platform_scale);
	platform_props5.meshes = { platform_shape5->mesh() };
	platform_props5.textures = platform_textures;
	m_platform5 = engine::game_object::create(platform_props5);

	//Tetrahedron --> most likely will be deleted by the final version.
	/*
	std::vector<glm::vec3> tetrahedron_vertices;
	tetrahedron_vertices.push_back(glm::vec3(0.f, 10.f, 0.f)); //Top vertex
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.f, 10.f)); //1
	tetrahedron_vertices.push_back(glm::vec3(-10.f, 0.f, -10.f)); //2
	tetrahedron_vertices.push_back(glm::vec3(10.f, 0.f, -10.f)); //3
	engine::ref<engine::tetrahedron> tetrahedron_shape = engine::tetrahedron::create(tetrahedron_vertices);
	engine::game_object_properties tetrahedron_props;
	tetrahedron_props.position = { -32.1025f, 3.5f, -0.005f };
	tetrahedron_props.meshes = { tetrahedron_shape->mesh() };
	m_tetrahedron = engine::game_object::create(tetrahedron_props); */

	// Loads and creates tree model and an object. 
	engine::ref <engine::model> tree_model = engine::model::create("assets/models/static/elm.3ds");
	engine::game_object_properties tree_props;
	tree_props.meshes = tree_model->meshes();
	tree_props.textures = tree_model->textures();
	float tree_scale = 3.f / glm::max(tree_model->size().x, glm::max(tree_model->size().y, tree_model->size().z));
	tree_props.position = { 4.f, 0.5f, -5.f };
	tree_props.bounding_shape = tree_model->size() / 2.f * tree_scale;
	tree_props.scale = glm::vec3(tree_scale);
	m_tree = engine::game_object::create(tree_props);
	m_tree_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f)); //First row of trees from the player.
	m_tree2_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f + 5.5f));
	m_tree3_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f + 11.f));
	m_tree4_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f + 16.5f));
	m_tree5_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f + 22.f));
	m_tree6_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f + 27.5f));
	m_tree7_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f + 33.f));
	m_tree8_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f + 38.5f));
	m_tree9_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(4.73651f, 0.5f, -21.0198f + 44.f));
	m_tree10_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(6.62455f, 0.5f, -20.0665f)); //Second row of trees from the player.
	m_tree11_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(6.62455f, 0.5f, -20.0665f + 5.5f));
	m_tree12_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(6.62455f, 0.5f, -20.0665f + 11.f));
	m_tree13_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(6.62455f, 0.5f, -20.0665f + 16.5f));
	m_tree14_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(6.62455f, 0.5f, -20.0665f + 22.f));
	m_tree15_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(6.62455f, 0.5f, -20.0665f + 27.5f));
	m_tree16_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(6.62455f, 0.5f, -20.0665f + 33.f));
	m_tree17_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(6.62455f, 0.5f, -20.0665f + 38.5f));
	m_tree18_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(10.315f, 0.5f, -12.7911f)); //Third row of trees from the player.
	m_tree19_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(10.315f, 0.5f, -12.7911f + 5.5f));
	m_tree20_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(10.315f, 0.5f, -12.7911f + 11.f));
	m_tree21_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(10.315f, 0.5f, -12.7911f + 16.5f));
	m_tree22_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(10.315f, 0.5f, -12.7911f + 22.f));
	m_tree23_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(10.315f, 0.5f, -12.7911f + 27.5f));
	m_tree24_box.set_box(0.5f, 2.75f, 0.5f, glm::vec3(10.315f, 0.5f, -12.7911f + 33.f));

	// Loads and creates grenade model.
	//Texture has been sourced from: https://i1.wp.com/nnn.ng/wp-content/uploads/2020/10/nnn-dark-featured-image.jpg
	engine::ref<engine::sphere> grenade_shape = engine::sphere::create(10, 20, 0.05f);
	engine::ref<engine::texture_2d> grenade_texture = engine::texture_2d::create("assets/textures/m67.jpg", false);
	engine::game_object_properties grenade_props;
	//Spawns the grenade onto the level outside of the player's view. 
	grenade_props.position = { -30.f, 5.f, -30.f };
	grenade_props.meshes = { grenade_shape->mesh() };
	grenade_props.textures = { grenade_texture };
	grenade_props.type = 1;
	grenade_props.bounding_shape = glm::vec3(0.5f);
	grenade_props.restitution = 0.92f;
	grenade_props.mass = 0.4f; //0.4kg
	grenade_props.RollingGroundFriction = 0.4f;
	m_grenad = engine::game_object::create(grenade_props);
	m_grenade_box.set_box(grenade_props.bounding_shape.x* grenade_props.scale.x / 2.f, grenade_props.bounding_shape.y * grenade_props.scale.x / 4.f, grenade_props.bounding_shape.z * grenade_props.scale.x / 2.f, grenade_props.position);

	//Creates sphere that will be used as ammunition:
	engine::game_object_properties ammunition_props;
	ammunition_props.position = { -100.f, 0.f, 5.f };
	ammunition_props.meshes = { grenade_shape->mesh() };
	ammunition_props.type = 1;
	ammunition_props.bounding_shape = glm::vec3(0.5f);
	ammunition_props.restitution = 0.92f;
	ammunition_props.mass = 0.0000001f;
	m_ammun = engine::game_object::create(ammunition_props);
	m_ammunition.initialise(engine::game_object::create(ammunition_props));
	m_ammunition_box.set_box(0.1f, 0.1f, 0.1f, glm::vec3(m_ammun->position()));
	m_light = engine::game_object::create(ammunition_props);

	// Medkit texture has been created by me and applied to the created cuboid.
	// Loads in Medkits that are used as a pickup that will heal the player.
	engine::ref < engine::cuboid> SmallHealth_shape = engine::cuboid::create(glm::vec3(0.5), false);
	engine::ref<engine::texture_2d> SmallHealth_text = engine::texture_2d::create("assets/textures/medpacks/25.jpg", false);
	engine::game_object_properties SmallHealth_props;
	SmallHealth_props.textures = { SmallHealth_text };
	SmallHealth_props.position = { -18.53f, 1.f, 9.56f };
	SmallHealth_props.meshes = { SmallHealth_shape->mesh() };
	m_SmallHealth = Pickup::create(SmallHealth_props);
	m_SmallHealth->Initialise();

	engine::ref < engine::cuboid> MediumHealth_shape = engine::cuboid::create(glm::vec3(0.5), false);
	engine::ref<engine::texture_2d> MediumHealth_text = engine::texture_2d::create("assets/textures/medpacks/50.jpg", false);
	engine::game_object_properties MediumHealth_props;
	MediumHealth_props.textures = { MediumHealth_text };
	MediumHealth_props.position = { -18.53f, 1.f, 13.48f };
	MediumHealth_props.meshes = { MediumHealth_shape->mesh() };
	MediumHealth_props.textures = { MediumHealth_text };
	m_MediumHealth = Pickup::create(MediumHealth_props);

	engine::ref < engine::cuboid> LargeHealth_shape = engine::cuboid::create(glm::vec3(0.5), false);
	engine::ref<engine::texture_2d> LargeHealth_text = engine::texture_2d::create("assets/textures/medpacks/100.jpg", false);
	engine::game_object_properties LargeHealth_props;
	LargeHealth_props.textures = { LargeHealth_text };
	LargeHealth_props.position = { -18.53f, 1.f, 17.48f };
	LargeHealth_props.meshes = { LargeHealth_shape->mesh() };
	LargeHealth_props.textures = { LargeHealth_text };
	m_LargeHealth = Pickup::create(LargeHealth_props);

	//All the weapons below are sourced from: https://free3d.com/3d-model/ninja-weapons-59065.html
	//Loads the weapon: Staff
	engine::ref <engine::model> staff_model = engine::model::create("assets/models/static/staff.obj");
	engine::game_object_properties staff_props;
	staff_props.meshes = staff_model->meshes();
	std::vector<engine::ref<engine::texture_2d>> staff_text = { engine::texture_2d::create("assets/textures/woodweap.jpg", false) };
	staff_props.textures = { staff_text };
	float staff_scale = 0.01f;
	staff_props.scale = glm::vec3(staff_scale);
	staff_props.position = { -19.37f, 1.f, -17.16f };
	staff_props.bounding_shape = staff_model->size();
	m_staff = Pickup::create(staff_props);
	m_staffo = engine::game_object::create(staff_props);

	//Loads shurikens into the game.
	engine::ref <engine::model> shuriken_model = engine::model::create("assets/models/static/shuriken.obj");
	engine::game_object_properties shuriken_props;
	shuriken_props.meshes = shuriken_model->meshes();
	shuriken_props.textures = { staff_text };
	float shuriken_scale = 0.025f;
	shuriken_props.scale = glm::vec3(shuriken_scale);
	shuriken_props.position = { -19.37f, 1.f, -13.16f };
	shuriken_props.bounding_shape = shuriken_model->size();
	m_shuriken = Pickup::create(shuriken_props);
	m_shurikeno = engine::game_object::create(shuriken_props);

	//Loads a sword into the game.
	engine::ref <engine::model> sword_model = engine::model::create("assets/models/static/sword.obj");
	engine::game_object_properties sword_props;
	sword_props.meshes = sword_model->meshes();
	sword_props.textures = { staff_text };
	float sword_scale = 0.01f;
	sword_props.scale = glm::vec3(sword_scale);
	sword_props.position = { -19.37f, 1.f, -21.16f };
	sword_props.bounding_shape = sword_model->size();
	m_sword = Pickup::create(sword_props);
	m_swordo = engine::game_object::create(sword_props);

	//Loads a sniper rifle into the game.
	//Sniper rifle was sourced from: https://free3d.com/3d-model/sniper-rifle-ksr-29-new-34178.html
	engine::ref <engine::model> sniper_model = engine::model::create("assets/models/static/sniper.obj");
	std::vector<engine::ref<engine::texture_2d>> sniper_text = { engine::texture_2d::create("assets/textures/sniper.jpg", false) };
	engine::game_object_properties sniper_props;
	sniper_props.meshes = sniper_model->meshes();
	sniper_props.textures = { sniper_text };
	float sniper_scale = 0.1f;
	sniper_props.scale = glm::vec3(sniper_scale);
	sniper_props.position = { -19.37f, 1.f, -10.16f };
	sniper_props.bounding_shape = sniper_model->size();
	m_sniper = Pickup::create(sniper_props);
	m_sniperrifleo = engine::game_object::create(sniper_props);

	//Creates cuboid to represent a houses form of 1, texture from https://a.rgbimg.com/users/s/su/sundstrom/600/mifnCNQ.jpg
	engine::ref<engine::cuboid> house_shape = engine::cuboid::create(glm::vec3(1.8f), false);
	engine::ref<engine::texture_2d> house_text = engine::texture_2d::create("assets/textures/houses/brickwall.jpg", true);
	engine::game_object_properties house_props;
	house_props.position = { 0.f, 0.5f, -8.f };
	house_props.meshes = { house_shape->mesh() };
	house_props.textures = { house_text };
	m_wall = engine::game_object::create(house_props);
	m_wall_box.set_box(3.6f, 3.6f, 43.5f, glm::vec3(-14.98f, 0.5f, -28.527f));
	m_wall1_box.set_box(3.6f, 3.6f, 43.5f, glm::vec3(-14.98f, 0.5f, 28.55f));

	//Creates cuboid to represent a house2, texture from https://lh3.googleusercontent.com/proxy/jYNxsRPLRxoS0K9g6apzGzEGg5dTtjq4ctmlQCCH9gdd5SKCpbweZCNehYYevlOEohVYExtT-GAOHQD7KVDmE3dbpa7WmQZkAD2Iagpo-OkTIa11rJWcPcwqEuXix9povZfKehV3fUZc8dky
	engine::ref<engine::cuboid> house_shape1 = engine::cuboid::create(glm::vec3(1.5f), false);
	engine::ref<engine::texture_2d> house_text1 = engine::texture_2d::create("assets/textures/houses/brickwall1.jpg", true);
	engine::game_object_properties house_props1;
	house_props1.position = { 15.3f, 2.f, -3.2f };
	house_props1.meshes = { house_shape1->mesh() };
	house_props1.textures = { house_text1 };
	m_house1 = engine::game_object::create(house_props1);

	engine::game_object_properties house_props12;
	house_props12.position = { 10.f, 2.f, -20.8f };
	house_props12.meshes = { house_shape1->mesh() };
	house_props12.textures = { house_text1 };
	m_house12 = engine::game_object::create(house_props12);
	m_house1_box.set_box(3.f, 3.f, 3.f, glm::vec3(house_props12.position.x, 0.5f, house_props12.position.z));

	//Creates cuboid to represent a house3, texture from https://res.cloudinary.com/twenty20/private_images/t_watermark-criss-cross-10/v1566472367000/photosp/baf9185c-11b9-4c62-af66-ee6fffba6d96/stock-photo-pattern-wall-house-brick-broken-abstract-concrete-texture-construct-baf9185c-11b9-4c62-af66-ee6fffba6d96.jpg
	engine::ref<engine::cuboid> house_shape2 = engine::cuboid::create(glm::vec3(2.1f), false);
	engine::ref<engine::texture_2d> house_text2 = engine::texture_2d::create("assets/textures/houses/brickwall2.jpg", true);
	engine::game_object_properties house_props2;
	house_props2.position = { 7.f, 0.5f, -14.f };
	house_props2.meshes = { house_shape2->mesh() };
	house_props2.textures = { house_text2 };
	m_house2 = engine::game_object::create(house_props2);

	//Creates cuboid to represent a house4, texture from https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcSn5XTp9rPjecoGh3U_-7HUqrmnxSJnRHx2Pg&usqp=CAU
	engine::ref<engine::cuboid> house_shape3 = engine::cuboid::create(glm::vec3(1.5f), false);
	engine::ref<engine::texture_2d> house_text3 = engine::texture_2d::create("assets/textures/houses/brickwall3.jpg", true);
	engine::game_object_properties house_props3;
	house_props3.position = { 14.f, 0.5f, 3.f };
	house_props3.meshes = { house_shape3->mesh() };
	house_props3.textures = { house_text3 };
	m_house3 = engine::game_object::create(house_props3);

	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_grenad);
	//m_game_objects.push_back(m_cow);
	//m_game_objects.push_back(m_tree);
	//m_game_objects.push_back(m_pickup);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	//Adds Crossfade into the game.
	//RB is made in paint and therefore, not sourced from anywhere.
	m_crossfade = CrossFade::create("assets/textures/RB.jpg", 2.f, 1.6f, 0.9f);
	m_crossfadeheal = CrossFade::create("assets/textures/heal.jpg", 2.f, 1.6f, 0.9f);
	m_crossfadeend = CrossFade::create("assets/textures/lose.jpg", 2.f, 1.6f, 0.9f);
	m_crossfadecomplete = CrossFade::create("assets/textures/complete.jpg", 2.f, 1.6f, 0.9f);

	m_billboard = billboard::create("assets/textures/explosion.tga", 4, 5, 16);

	//Sets default animations for the animated meshes.
	m_skinned_mesh->switch_animation(2);
	m_wolf_mesh->switch_animation(1);
	m_zombie_mesh->switch_animation(2);
	m_bot_mesh->switch_animation(1);

	//Initialises weapons
	if (isGrenade) { m_grenade.initialise(m_grenad); }
	if (isSpecialAmm) { m_bulletshot.initialise(m_robot, glm::vec3(1.f, 0.f, 0.f)); }
	}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step) 
{
	//Player must press P in order to start the game. Player's camera will then be initialised and actions allowed.
	if (StartG == true && EndGame == false && FinishLevel == false && Pause == false) {
			glm::vec3 player_position = m_player.object()->position();
			glm::vec3 zombie_position = m_zomb.object()->position();
			glm::vec3 wolf_position = m_wol.object()->position();
			glm::vec3 wolf1_position = m_wol1.object()->position();
			glm::vec3 boss_position = m_boss.object()->position();

			//Snaps m_3d_camera to m_player.
			m_player.update_camera(m_3d_camera);

			//Testing Camera
			//m_3d_camera.on_update(time_step);

			//Refers back to specific AI & object's class.
			m_player.on_update(time_step);
			if (isZombie1) { m_zomb.on_update(time_step, m_player.object()->position()); }
			if (isWolf1) { m_wol.on_update(time_step, m_player.object()->position(), m_zomb.object()->position()); }
			if (isWolf2) { m_wol1.on_update(time_step, m_player.object()->position(), m_zomb.object()->position()); }
			if (isBoss) { m_boss.on_update(time_step, m_player.object()->position(), m_zomb.object()->position()); }
			if (isCow) { m_cowa.on_update(time_step); }
			if (isCow1) { m_cowa1.on_update(time_step); }
			m_bulletshot.on_update(time_step, m_zomb.object()->position(), m_wol.object()->position(), m_wol1.object()->position());
			m_ammunition.on_update(time_step);
			m_grenade.on_update(time_step); 

			//Loads AI into the game.
			if (isWolf1) { m_wolf->animated_mesh()->on_update(time_step); }
			if (isWolf2) { m_wolf1->animated_mesh()->on_update(time_step); }
			if (isBoss) { m_bot->animated_mesh()->on_update(time_step); }
			if (isZombie1) { m_zombie->animated_mesh()->on_update(time_step); }

			// Pickup gets picked up by the player, isActive becomes false.
			m_staff->update(m_mannequin->position(), time_step);
			m_shuriken->update(m_mannequin->position(), time_step);
			m_sword->update(m_mannequin->position(), time_step);
			m_sniper->update(m_mannequin->position(), time_step);
			m_SmallHealth->update(m_mannequin->position(), time_step);
			m_MediumHealth->update(m_mannequin->position(), time_step);
			m_LargeHealth->update(m_mannequin->position(), time_step);

			m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

			//Updates crossfade and billboard.
			m_crossfade->on_update(time_step);
			m_crossfadeheal->on_update(time_step);
			m_billboard->on_update(time_step);

			//Axis-Aligned Bounding Boxes Method for the collission will be used within this game.
			m_player_box.on_update(m_player.object()->position());
			if (isWolf1) { m_wolf_box.on_update(m_wol.object()->position()); }
			if (isWolf2) { m_wolf1_box.on_update(m_wolf1->position()); }
			if (isBoss) { m_bot_box.on_update(m_boss.object()->position()); }
			if (isZombie1) { m_zombie_box.on_update(m_zomb.object()->position()); }
			if (isRobot) { m_robot_box.on_update(m_robot->position()); }
			if (isCow) { m_cow_box.on_update(m_cowa.object()->position()); }
			if (isCow1) { m_cow_box1.on_update(m_cowa1.object()->position()); }
			m_grenade_box.on_update(m_grenad->position());
			m_ammunition_box.on_update(m_ammunition.object()->position());
			m_robot_box.on_update(m_bulletshot.object()->position());

			//Collissions:
			//Wolves.
			if (isWolf1) {
				if (m_wolf_box.collision(m_player_box)) {
					m_player.object()->set_position(player_position);
					if (isEasy) { m_player.Damage_Taken(30 * time_step); }
					if (isHard) { m_player.Damage_Taken(60 * time_step); }
					//m_crossfade->activate();
					m_wol.object()->set_position(wolf_position);
				}
				else if (m_wolf_box.collision(m_zombie_box)) {
					m_zomb.object()->set_position(zombie_position);
					m_wol.object()->set_position(wolf_position);
					m_zomb.Damage_Taken(30 * time_step);
					m_wol.Damage_Taken(25 * time_step);
					//m_audio_manager->play_spatialised_sound("zom_attack", m_3d_camera.position(), m_zomb.object()->position());
				}
			}
			if (isWolf2) {
				if (m_wolf1_box.collision(m_player_box)) {
					m_player.object()->set_position(player_position);
					m_wol1.object()->set_position(wolf1_position);
					if (isEasy) { m_player.Damage_Taken(30 * time_step); }
					if (isHard) { m_player.Damage_Taken(60 * time_step); }
					//m_crossfade->activate();
				}
				else if (m_wolf1_box.collision(m_zombie_box)) {
					m_zomb.object()->set_position(zombie_position);
					m_wol1.object()->set_position(wolf_position);
					m_zomb.Damage_Taken(30 * time_step);
					m_wol1.Damage_Taken(25 * time_step);
					//m_audio_manager->play_spatialised_sound("zom_attack", m_3d_camera.position(), m_zomb.object()->position());
				}
			}
			//Zombies.
			if (isZombie1) {
				if (m_zombie_box.collision(m_player_box)) {
					m_player.object()->set_position(player_position);
					if (isEasy) { m_player.Damage_Taken(25 * time_step); }
					if (isHard) { m_player.Damage_Taken(50 * time_step); }
					m_zomb.object()->set_position(zombie_position);
					//m_audio_manager->play_spatialised_sound("zom_attack", m_3d_camera.position(), m_zomb.object()->position());
					m_audio_manager->play("zom_attack");
					//m_crossfade->activate();
				}
			}

			//Boss
			if (isBoss) {
				if (m_bot_box.collision(m_zombie_box)) {
					m_zomb.object()->set_position(zombie_position);
					m_boss.Damage_Taken(0.1 * time_step); //Armour of the boss is too big to be hurt by a zombie.
					m_zomb.Damage_Taken(2.5 * time_step); //Boss deals 250 damage to unarmoured targets.
					m_boss.object()->set_position(boss_position);
					//m_audio_manager->play_spatialised_sound("zom_attack", m_3d_camera.position(), m_zomb.object()->position());
				}
				else if (m_bot_box.collision(m_player_box)) {
					m_player.object()->set_position(player_position);
					if (isEasy) { m_player.Damage_Taken(125 * time_step); }//Boss deals 125 to armoured characters.
					if (isEasy) { m_boss.Damage_Taken(25 * time_step); } //Player's strength will allow to hurt boss with 25 damage if hit with a hand.
					if (isHard) { m_player.Damage_Taken(250 * time_step); }
					if (isHard) { m_boss.Damage_Taken(15 * time_step); }
					m_boss.object()->set_position(boss_position);
				}
			}

			//Grenade deals 100 damage to targets.
			if (m_grenade_box.collision(m_zombie_box)) {
				m_zomb.Damage_Taken(100);
				m_billboard->activate(glm::vec3(m_grenad->position()), 2.f, 2.f);
				isGrenade = false;
			}
			else if (m_grenade_box.collision(m_bot_box)) {
				m_boss.Damage_Taken(100);
				m_billboard->activate(glm::vec3(m_grenad->position()), 2.f, 2.f);
				isGrenade = false;
			}
			else if (m_grenade_box.collision(m_wolf_box)) {
				m_wol.Damage_Taken(100);
				m_billboard->activate(glm::vec3(m_grenad->position()), 2.f, 2.f);
				isGrenade = false;
			}
			else if (m_grenade_box.collision(m_wolf1_box)) {
				m_wol1.Damage_Taken(100);
				m_billboard->activate(glm::vec3(m_grenad->position()), 2.f, 2.f);
				isGrenade = false;
			}

			//If Ammunition is shot at the target.
			if (m_ammunition_box.collision(m_zombie_box)) {
				m_zomb.Damage_Taken(10);
			}
			else if (m_ammunition_box.collision(m_bot_box)) {
				m_boss.Damage_Taken(1); //Boss has an armour and therefore, ammunition is greatly reduced on an impact.
			}
			else if (m_ammunition_box.collision(m_wolf_box)) {
				m_wol.Damage_Taken(10);
			}
			else if (m_ammunition_box.collision(m_wolf1_box)) {
				m_wol1.Damage_Taken(10);
			}

			//Special Ammunition deals 150dmg to targets it also has its own AI built in for auto-aim.
			if (isZombie1) {
				if (m_robot_box.collision(m_zombie_box)) {
					m_zomb.Damage_Taken(150 * time_step);
					m_bulletshot.Target(1);
					m_billboard->activate(glm::vec3(m_robot->position()), 5.f, 5.f);
					isSpecialAmm = false;
					isRobot = false;
				}
			}
			else if (isWolf1) {
				if (m_robot_box.collision(m_wolf_box)) {
					m_wol.Damage_Taken(150 * time_step);
					m_bulletshot.Target(1);
					m_billboard->activate(glm::vec3(m_robot->position()), 5.f, 5.f);
					isSpecialAmm = false;
					isRobot = false;
				}
			}
			else if (isWolf2) {
				if (m_robot_box.collision(m_wolf1_box)) {
				m_wol1.Damage_Taken(150 * time_step);
				m_bulletshot.Target(1);
				m_billboard->activate(glm::vec3(m_robot->position()), 5.f, 5.f);
				isSpecialAmm = false;
				isRobot = false;
				}
			}

			//If the player collides with the objects.
			//Jeep collision, only one is considered as other are considered to be unplayable area.
			if (m_jeep_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_jeep_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_jeep_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_jeep_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			//House collision.
			if (m_house1_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_house1_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_house1_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_house1_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			//Wall Collisions, only two front sides are covered as the rest is unplayable area.
			if (m_wall_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_wall_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_wall_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_wall_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_wall1_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_wall1_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_wall1_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_wall1_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			//Rock collisions
			if (m_rock_box.collision(m_player_box)) //Tests for collisions against a player.
				m_player.object()->set_position(player_position);
			else if (m_rock1_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock2_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock3_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock4_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock5_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock6_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock7_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock8_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock9_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock10_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock11_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock12_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock13_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_rock_box.collision(m_zombie_box)) //Tests for collision against zombies.
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock1_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock2_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock3_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock4_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock5_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock6_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock7_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock8_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock9_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock10_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock11_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock12_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock13_box.collision(m_zombie_box))
				m_zomb.object()->set_position(zombie_position);
			else if (m_rock_box.collision(m_wolf_box)) //Tests for collision against wolves.
				m_wol.object()->set_position(wolf_position);
			else if (m_rock1_box.collision(m_wolf_box)) 
				m_wol.object()->set_position(wolf_position);
			else if (m_rock2_box.collision(m_wolf_box)) 
				m_wol.object()->set_position(wolf_position);
			else if (m_rock3_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock4_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock5_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock6_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock7_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock8_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock9_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock10_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock11_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock12_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock13_box.collision(m_wolf_box))
				m_wol.object()->set_position(wolf_position);
			else if (m_rock_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock1_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock2_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock3_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock4_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock5_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock6_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock7_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock8_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock9_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock10_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock11_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock12_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			else if (m_rock13_box.collision(m_wolf1_box))
				m_wol1.object()->set_position(wolf1_position);
			//Tree collisions
			if (m_tree_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree2_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree3_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree4_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree5_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree6_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree7_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree8_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree9_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree10_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree11_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree12_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree13_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree14_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree15_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree16_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree17_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree18_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree19_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree20_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree21_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree22_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree23_box.collision(m_player_box))
				m_player.object()->set_position(player_position);
			else if (m_tree24_box.collision(m_player_box))
				m_player.object()->set_position(player_position);

			//m_audio_manager->update_with_camera(m_3d_camera);

			//Stops the player from accessing parts of the map that are unaccessible to the player or leaving the map.
			//If the player will touch the sides of the map, gun shots will appear to represent an on-going battle in the distance.
			if (m_mannequin->position().z >= 22.8756f) { m_mannequin->set_position(glm::vec3(m_mannequin->position().x, m_mannequin->position().y, 22.8756f)); m_audio_manager->play("shots_in_the_distance"); } //Right side of the map.
			else if (m_mannequin->position().x >= 10.19f) { m_mannequin->set_position(glm::vec3(10.19f, m_mannequin->position().y, m_mannequin->position().z)); m_audio_manager->play("shots_in_the_distance"); } //Towards the forest. 
			else if (m_mannequin->position().z <= -22.75f) { m_mannequin->set_position(glm::vec3(m_mannequin->position().x, m_mannequin->position().y, -22.75f)); m_audio_manager->play("shots_in_the_distance"); } // Left side of the map.
			else if (m_mannequin->position().x <= -45.8831f) { m_mannequin->set_position(glm::vec3(-45.8831f, m_mannequin->position().y, m_mannequin->position().z)); m_audio_manager->play("shots_in_the_distance"); } //Back Side of the map.

			healthhud = std::to_string(m_player.Get_Health());
			platformhud = std::to_string(platform_score());
			grenadeshud = std::to_string(m_player.Get_Grenades());
			scrapamount = std::to_string(m_SmallHealth->ScrapScore());
			secretscorehud = std::to_string(secret_score());
			specialammhud = std::to_string(m_player.Get_SpecialAmm());
			ammunitionhud = std::to_string(m_player.Get_Ammunition());

			check_bounce();
			platforms();
			Alive();
	}

	//If player reaches 0 health, the game will end.
	if (m_player.Get_Health() == 0) {
		EndGame = true;
		StartG = false;
	}

	if (m_player.Pscore() == 5) {
		StartG = false;
		FinishLevel = true;
	}
	
}

void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();

	//const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
	//engine::renderer::begin_scene(m_3d_camera, textured_shader);

	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("fog_on", m_fog);

	// Positions and renders the skybox centred on the player.
	glm::mat4 skybox_transform(1.0f);
	skybox_transform = glm::translate(skybox_transform, m_3d_camera.position());
	//skybox_transform = glm::scale(skybox_transform, glm::vec3(55.f,55.f,55.f));
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_transform);

	engine::renderer::submit(textured_lighting_shader, m_terrain);

	m_grenade.on_render(textured_lighting_shader);

	//Renders all the roofs for the houses including the Army camp.
	engine::renderer::submit(textured_lighting_shader, m_pentahedron);
	engine::renderer::submit(textured_lighting_shader, m_pentahedron1);
	engine::renderer::submit(textured_lighting_shader, m_pentahedron2);
	engine::renderer::submit(textured_lighting_shader, m_pentahedron3);
	engine::renderer::submit(textured_lighting_shader, m_pentahedron4);
	engine::renderer::submit(textured_lighting_shader, m_pentahedron5);
	engine::renderer::submit(textured_lighting_shader, m_pentahedron6);
	engine::renderer::submit(textured_lighting_shader, m_pentahedron7);
	engine::renderer::submit(textured_lighting_shader, m_pentahedron8);

	//Starts to render the environment.
	//Renders the road.
	glm::mat4 road_trans(1.f);
	road_trans = glm::translate(road_trans, glm::vec3(-4.28f, 0.5f, -0.25f));
	road_trans = glm::rotate(road_trans, -glm::pi<float>() / 2.f, glm::vec3(0.f, 1.f, 0.f));
	road_trans = glm::scale(road_trans, glm::vec3(1.995f, m_street->scale().y, 0.15f));
	engine::renderer::submit(textured_lighting_shader, road_trans, m_street);

	//Renders tree lines.

	for (int i = 0; i < 16; ++i) {
		glm::mat4 tree_transform(1.0f);
		tree_transform = glm::translate(tree_transform, glm::vec3(4.71f, 0.5, -37.5f + (i * 5.5)));
		tree_transform = glm::rotate(tree_transform, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform = glm::scale(tree_transform, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform, m_tree);
	}

	for (int i = 0; i < 9; ++i) {
		glm::mat4 tree_transform2(1.0f);
		tree_transform2 = glm::translate(tree_transform2, glm::vec3(10.3f, 0.5, -12.83f + (i * 5.5)));
		tree_transform2 = glm::rotate(tree_transform2, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform2 = glm::scale(tree_transform2, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform2, m_tree);
	}

	for (int i = 0; i < 3; ++i) {
		glm::mat4 tree_transform3(1.0f);
		tree_transform3 = glm::translate(tree_transform3, glm::vec3(10.3f, 0.5, -42.64f + (i * 5.5)));
		tree_transform3 = glm::rotate(tree_transform3, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform3 = glm::scale(tree_transform3, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform3, m_tree);
	}

	glm::mat4 tree_transform4(1.0f);
	tree_transform4 = glm::translate(tree_transform4, glm::vec3(12.f, 0.5, 47.43f));
	tree_transform4 = glm::rotate(tree_transform4, m_tree->rotation_amount(), m_tree->rotation_axis());
	tree_transform4 = glm::scale(tree_transform4, m_tree->scale());
	engine::renderer::submit(textured_lighting_shader, tree_transform4, m_tree);

	for (int i = 0; i < 8; ++i) {
		glm::mat4 tree_transform5(1.0f);
		tree_transform5 = glm::translate(tree_transform5, glm::vec3(15.86f, 0.5, -45.31f + (i * 5.5)));
		tree_transform5 = glm::rotate(tree_transform5, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform5 = glm::scale(tree_transform5, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform5, m_tree);
	}

	for (int i = 0; i < 8; ++i) {
		glm::mat4 tree_transform6(1.0f);
		tree_transform6 = glm::translate(tree_transform6, glm::vec3(15.86f, 0.5, 4.3f + (i * 5.5)));
		tree_transform6 = glm::rotate(tree_transform6, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform6 = glm::scale(tree_transform6, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform6, m_tree);
	}

	for (int i = 0; i < 17; ++i) {
		glm::mat4 tree_transform7(1.0f);
		tree_transform7 = glm::translate(tree_transform7, glm::vec3(21.55f, 0.5, -42.95f + (i * 5.5)));
		tree_transform7 = glm::rotate(tree_transform7, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform7 = glm::scale(tree_transform7, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform7, m_tree);
	}

	for (int i = 0; i < 8; ++i) {
		glm::mat4 tree_transform8(1.0f);
		tree_transform8 = glm::translate(tree_transform8, glm::vec3(24.42f, 0.5, -46.68f + (i * 5.5)));
		tree_transform8 = glm::rotate(tree_transform8, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform8 = glm::scale(tree_transform8, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform8, m_tree);
	}

	for (int i = 0; i < 10; ++i) {
		glm::mat4 tree_transform9(1.0f);
		tree_transform9 = glm::translate(tree_transform9, glm::vec3(24.42f, 0.5, -2.38f + (i * 5.5)));
		tree_transform9 = glm::rotate(tree_transform9, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform9 = glm::scale(tree_transform9, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform9, m_tree);
	}

	for (int i = 0; i < 18; ++i) {
		glm::mat4 tree_transform10(1.0f);
		tree_transform10 = glm::translate(tree_transform10, glm::vec3(28.48f, 0.5, 49.45f - (i * 5.5)));
		tree_transform10 = glm::rotate(tree_transform10, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform10 = glm::scale(tree_transform10, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform10, m_tree);
	}

	for (int i = 0; i < 18; ++i) {
		glm::mat4 tree_transform11(1.0f);
		tree_transform11 = glm::translate(tree_transform11, glm::vec3(36.71f, 0.5, -48.75f + (i * 5.5)));
		tree_transform11 = glm::rotate(tree_transform11, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform11 = glm::scale(tree_transform11, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform11, m_tree);
	}

	for (int i = 0; i < 18; ++i) {
		glm::mat4 tree_transform12(1.0f);
		tree_transform12 = glm::translate(tree_transform12, glm::vec3(6.59f, 0.5, -47.56f + (i * 5.5)));
		tree_transform12 = glm::rotate(tree_transform12, m_tree->rotation_amount(), m_tree->rotation_axis());
		tree_transform12 = glm::scale(tree_transform12, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform12, m_tree);
	}

	//Sets the walls for the houses.
	for (int i = 0; i < 12; ++i) {
		glm::mat4 house_trans(1.f);
		house_trans = glm::translate(house_trans, glm::vec3(-15.f, 2.3f, 48.2f - (i * 3.6)));
		house_trans = glm::rotate(house_trans, m_wall->rotation_amount(), m_wall->rotation_axis());
		house_trans = glm::scale(house_trans, m_wall->scale());
		engine::renderer::submit(textured_lighting_shader, house_trans, m_wall);
	}

	for (int i = 0; i < 12; ++i) {
		glm::mat4 house_trans1(1.f);
		house_trans1 = glm::translate(house_trans1, glm::vec3(-15.f, 2.3f, -48.2f + (i * 3.6)));
		house_trans1 = glm::rotate(house_trans1, m_wall->rotation_amount(), m_wall->rotation_axis());
		house_trans1 = glm::scale(house_trans1, m_wall->scale());
		engine::renderer::submit(textured_lighting_shader, house_trans1, m_wall);
	}

	for (int i = 0; i < 27; ++i) {
		glm::mat4 house_trans2(1.f);
		house_trans2 = glm::translate(house_trans2, glm::vec3(-48.2f, 2.3f, 48.2f - (i * 3.6)));
		house_trans2 = glm::rotate(house_trans2, m_wall->rotation_amount(), m_wall->rotation_axis());
		house_trans2 = glm::scale(house_trans2, m_wall->scale());
		engine::renderer::submit(textured_lighting_shader, house_trans2, m_wall);
	}
	glm::mat4 house_trans8(1.f); //corner wall, inaccissable by the player. Terrain will be resized at the end to allow for extra space to accommodate AI. 
	house_trans8 = glm::translate(house_trans8, glm::vec3(-48.2f, 2.3f, -48.2f));
	house_trans8 = glm::rotate(house_trans8, m_wall->rotation_amount(), m_wall->rotation_axis());
	house_trans8 = glm::scale(house_trans8, m_wall->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans8, m_wall);

	for (int i = 0; i < 9; ++i) {
		glm::mat4 house_trans3(1.f);
		house_trans3 = glm::translate(house_trans3, glm::vec3(-18.6f - (i * 3.6), 2.3f, -48.2f));
		house_trans3 = glm::rotate(house_trans3, m_wall->rotation_amount(), m_wall->rotation_axis());
		house_trans3 = glm::scale(house_trans3, m_wall->scale());
		engine::renderer::submit(textured_lighting_shader, house_trans3, m_wall);
	}

	for (int i = 0; i < 9; ++i) {
		glm::mat4 house_trans9(1.f); //Additional wall placed at the end, The end of the left side which will not be accessible by the player.
		house_trans9 = glm::translate(house_trans9, glm::vec3(-18.6f - (i * 3.6), 2.3f, -25.f));
		house_trans9 = glm::rotate(house_trans9, m_wall->rotation_amount(), m_wall->rotation_axis());
		house_trans9 = glm::scale(house_trans9, m_wall->scale());
		engine::renderer::submit(textured_lighting_shader, house_trans9, m_wall);
	}

	for (int i = 0; i < 9; ++i) {
		glm::mat4 house_trans4(1.f);
		house_trans4 = glm::translate(house_trans4, glm::vec3(-18.6f - (i * 3.6), 2.3f, 25.f));
		house_trans4 = glm::rotate(house_trans4, m_wall->rotation_amount(), m_wall->rotation_axis());
		house_trans4 = glm::scale(house_trans4, m_wall->scale());
		engine::renderer::submit(textured_lighting_shader, house_trans4, m_wall);
	}

	for (int i = 0; i < 9; ++i) {
		glm::mat4 house_trans10(1.f); //Additional wall placed at the end, The end of the right side which will not be accessible by the player.
		house_trans10 = glm::translate(house_trans10, glm::vec3(-18.6f - (i * 3.6), 2.3f, -48.2f));
		house_trans10 = glm::rotate(house_trans10, m_wall->rotation_amount(), m_wall->rotation_axis());
		house_trans10 = glm::scale(house_trans10, m_wall->scale());
		engine::renderer::submit(textured_lighting_shader, house_trans10, m_wall);
	}

	for (int i = 0; i < 9; ++i) {
		glm::mat4 house_trans11(1.f);
		house_trans11 = glm::translate(house_trans11, glm::vec3(-18.6f - (i * 3.6), 2.3f, 48.2f));
		house_trans11 = glm::rotate(house_trans11, m_wall->rotation_amount(), m_wall->rotation_axis());
		house_trans11 = glm::scale(house_trans11, m_wall->scale());
		engine::renderer::submit(textured_lighting_shader, house_trans11, m_wall);
	}

	//Spawns houses around the map to make it look more realistic and random.
	glm::mat4 house_trans(1.0f);
	house_trans = glm::translate(house_trans, glm::vec3(31.7f, 2.6f, -33.5f));
	house_trans = glm::rotate(house_trans, m_house2->rotation_amount(), m_house2->rotation_axis());
	house_trans = glm::scale(house_trans, m_house2->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans, m_house2);


	glm::mat4 house_trans1(1.0f);
	house_trans1 = glm::translate(house_trans1, m_house1->position());
	house_trans1 = glm::rotate(house_trans1, m_house1->rotation_amount(), m_house1->rotation_axis());
	house_trans1 = glm::scale(house_trans1, m_house1->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans1, m_house1);


	glm::mat4 house_trans2(1.0f);
	house_trans2 = glm::translate(house_trans2, glm::vec3(26.7f, 2.f, -9.9f));
	house_trans2 = glm::rotate(house_trans2, m_house3->rotation_amount(), m_house3->rotation_axis());
	house_trans2 = glm::scale(house_trans2, m_house3->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans2, m_house3);

	glm::mat4 house_trans3(1.0f);
	house_trans3 = glm::translate(house_trans3, m_house12->position());
	house_trans3 = glm::rotate(house_trans3, m_house12->rotation_amount(), m_house12->rotation_axis());
	house_trans3 = glm::scale(house_trans3, m_house12->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans3, m_house12);

	glm::mat4 house_trans4(1.0f);
	house_trans4 = glm::translate(house_trans4, glm::vec3(38.8f, 2.f, 6.7f));
	house_trans4 = glm::rotate(house_trans4, m_house3->rotation_amount(), m_house3->rotation_axis());
	house_trans4 = glm::scale(house_trans4, m_house3->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans4, m_house3);

	glm::mat4 house_trans5(1.0f);
	house_trans5 = glm::translate(house_trans5, glm::vec3(43.6f, 2.6f, 34.17f));
	house_trans5 = glm::rotate(house_trans5, m_house2->rotation_amount(), m_house2->rotation_axis());
	house_trans5 = glm::scale(house_trans5, m_house2->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans5, m_house2);

	glm::mat4 house_trans6(1.0f);
	house_trans6 = glm::translate(house_trans6, glm::vec3(12.2f, 2.6f, 39.5f));
	house_trans6 = glm::rotate(house_trans6, m_house2->rotation_amount(), m_house2->rotation_axis());
	house_trans6 = glm::scale(house_trans6, m_house2->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans6, m_house2);

	glm::mat4 house_trans7(1.0f);
	house_trans7 = glm::translate(house_trans7, glm::vec3(24.5f, 2.f, 16.4f));
	house_trans7 = glm::rotate(house_trans7, m_house3->rotation_amount(), m_house3->rotation_axis());
	house_trans7 = glm::scale(house_trans7, m_house3->scale());
	engine::renderer::submit(textured_lighting_shader, house_trans7, m_house3);

	//Starts to render objects.

	//Renders jeeps in order to present crashed or abandoned vehicles after civilians tried to flee.
	glm::mat4 jeep_trans(1.f);
	jeep_trans = glm::translate(jeep_trans, m_jeep->position() - glm::vec3(m_jeep->offset().x, 0.f, -m_jeep->offset().z * m_jeep->scale().x * 0.6f));
	jeep_trans = glm::rotate(jeep_trans, -glm::pi<float>() / 2.f, glm::vec3(1.f, 0.f, 0.f));
	jeep_trans = glm::rotate(jeep_trans, -glm::pi<float>() / 4.f, glm::vec3(0.f, 0.f, 1.f));
	jeep_trans = glm::scale(jeep_trans, m_jeep->scale());
	engine::renderer::submit(textured_lighting_shader, jeep_trans, m_jeep);

	glm::mat4 jeep_trans1(1.f);
	jeep_trans1 = glm::translate(jeep_trans1, glm::vec3(-5.75646f, 2.f, -32.333f));
	jeep_trans1 = glm::rotate(jeep_trans1, glm::pi<float>() / 2.f, glm::vec3(1.f, 0.f, 0.f));
	jeep_trans1 = glm::rotate(jeep_trans1, glm::pi<float>() / 3.f, glm::vec3(0.f, 0.f, 1.f));
	jeep_trans1 = glm::scale(jeep_trans1, m_jeep->scale());
	engine::renderer::submit(textured_lighting_shader, jeep_trans1, m_jeep);

	glm::mat4 jeep_trans2(1.f);
	jeep_trans2 = glm::translate(jeep_trans2, glm::vec3(2.42f, 0.5f, 37.5f));
	jeep_trans2 = glm::rotate(jeep_trans2, -glm::pi<float>() / 2.f, glm::vec3(1.f, 0.f, 0.f));
	jeep_trans2 = glm::rotate(jeep_trans2, -glm::pi<float>() / 1.f, glm::vec3(0.f, 0.f, 1.f));
	jeep_trans2 = glm::scale(jeep_trans2, m_jeep->scale());
	engine::renderer::submit(textured_lighting_shader, jeep_trans2, m_jeep);

	//Placing random rocks across the map and rock wall to create an obstacles for AI from the Army.
	glm::mat4 rock_trans(1.f);
	rock_trans = glm::translate(rock_trans, glm::vec3(m_rock->position().x, 0.6f, m_rock->position().z));
	rock_trans = glm::rotate(rock_trans, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans = glm::scale(rock_trans, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans, m_rock);

	for (int i = 0; i < 8; ++i) {
		glm::mat4 rock_trans1(1.f);
		rock_trans1 = glm::translate(rock_trans1, glm::vec3(-12.02f, 0.6f, -13.53f + (i * 3.605)));
		rock_trans1 = glm::rotate(rock_trans1, m_rock->rotation_amount(), m_rock->rotation_axis());
		rock_trans1 = glm::scale(rock_trans1, m_rock->scale());
		engine::renderer::submit(textured_lighting_shader, rock_trans1, m_rock);
	}

	glm::mat4 rock_trans2(1.f);
	rock_trans2 = glm::translate(rock_trans2, glm::vec3(-0.32f, 0.6f, 0.f));
	rock_trans2 = glm::rotate(rock_trans2, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans2 = glm::scale(rock_trans2, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans2, m_rock);

	glm::mat4 rock_trans3(1.f);
	rock_trans3 = glm::translate(rock_trans3, glm::vec3(-5.99f, 0.6f, 23.2f));
	rock_trans3 = glm::rotate(rock_trans3, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans3 = glm::scale(rock_trans3, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans3, m_rock);

	glm::mat4 rock_trans4(1.f);
	rock_trans4 = glm::translate(rock_trans4, glm::vec3(-2.25f, 0.6f, 36.49f));
	rock_trans4 = glm::rotate(rock_trans4, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans4 = glm::scale(rock_trans4, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans4, m_rock);

	glm::mat4 rock_trans5(1.f);
	rock_trans5 = glm::translate(rock_trans5, glm::vec3(1.98f, 0.6f, 41.17f));
	rock_trans5 = glm::rotate(rock_trans5, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans5 = glm::scale(rock_trans5, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans5, m_rock);

	glm::mat4 rock_trans6(1.f);
	rock_trans6 = glm::translate(rock_trans6, glm::vec3(-12.f, 0.6f, 44.8f));
	rock_trans6 = glm::rotate(rock_trans6, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans6 = glm::scale(rock_trans6, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans6, m_rock);

	glm::mat4 rock_trans7(1.f);
	rock_trans7 = glm::translate(rock_trans7, glm::vec3(-0.07f, 0.6f, -16.6f));
	rock_trans7 = glm::rotate(rock_trans7, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans7 = glm::scale(rock_trans7, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans7, m_rock);

	glm::mat4 rock_trans8(1.f);
	rock_trans8 = glm::translate(rock_trans8, glm::vec3(-5.15f, 0.6f, -28.22f));
	rock_trans8 = glm::rotate(rock_trans8, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans8 = glm::scale(rock_trans8, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans8, m_rock);

	glm::mat4 rock_trans9(1.f);
	rock_trans9 = glm::translate(rock_trans9, glm::vec3(4.19f, 0.6f, -34.19f));
	rock_trans9 = glm::rotate(rock_trans9, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans9 = glm::scale(rock_trans9, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans9, m_rock);

	glm::mat4 rock_trans10(1.f);
	rock_trans10 = glm::translate(rock_trans10, glm::vec3(11.56f, 0.6f, -10.29f));
	rock_trans10 = glm::rotate(rock_trans10, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans10 = glm::scale(rock_trans10, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans10, m_rock);

	glm::mat4 rock_trans11(1.f);
	rock_trans11 = glm::translate(rock_trans11, glm::vec3(5.12f, 0.6f, 3.52f));
	rock_trans11 = glm::rotate(rock_trans11, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans11 = glm::scale(rock_trans11, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans11, m_rock);

	glm::mat4 rock_trans12(1.f);
	rock_trans12 = glm::translate(rock_trans12, glm::vec3(10.23f, 0.6f, 22.83f));
	rock_trans12 = glm::rotate(rock_trans12, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans12 = glm::scale(rock_trans12, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans12, m_rock);

	glm::mat4 rock_trans13(1.f);
	rock_trans13 = glm::translate(rock_trans13, glm::vec3(18.51f, 0.6f, 16.34f));
	rock_trans13 = glm::rotate(rock_trans13, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans13 = glm::scale(rock_trans13, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans13, m_rock);

	glm::mat4 rock_trans14(1.f);
	rock_trans14 = glm::translate(rock_trans14, glm::vec3(7.59f, 0.6f, 36.65f));
	rock_trans14 = glm::rotate(rock_trans14, m_rock->rotation_amount(), m_rock->rotation_axis());
	rock_trans14 = glm::scale(rock_trans14, m_rock->scale());
	engine::renderer::submit(textured_lighting_shader, rock_trans14, m_rock);

	//Start of rendering pickups. 
	//Renders a singular of each weapon and healthpack pickups.
	if (m_SmallHealth->SmallHealth()) {
		m_SmallHealth->textures().at(0)->bind();
		glm::mat4 SmallHealth_transform(1.f);
		SmallHealth_transform = glm::translate(SmallHealth_transform, m_SmallHealth->position());
		SmallHealth_transform = glm::rotate(SmallHealth_transform, m_SmallHealth->rotation_amount(), m_SmallHealth->rotation_axis());
		SmallHealth_transform = glm::scale(SmallHealth_transform, m_SmallHealth->scale());
		engine::renderer::submit(textured_lighting_shader, m_SmallHealth->meshes().at(0), SmallHealth_transform);
	}

	if (m_MediumHealth->MediumHealth()) {
		m_MediumHealth->textures().at(0)->bind();
		glm::mat4 MediumHealth_transform(1.f);
		MediumHealth_transform = glm::translate(MediumHealth_transform, m_MediumHealth->position());
		MediumHealth_transform = glm::rotate(MediumHealth_transform, m_MediumHealth->rotation_amount(), m_MediumHealth->rotation_axis());
		MediumHealth_transform = glm::scale(MediumHealth_transform, m_MediumHealth->scale());
		engine::renderer::submit(textured_lighting_shader, m_MediumHealth->meshes().at(0), MediumHealth_transform);
	}

	if (m_LargeHealth->LargeHealth()) {
		m_LargeHealth->textures().at(0)->bind();
		glm::mat4 LargeHealth_transform(1.f);
		LargeHealth_transform = glm::translate(LargeHealth_transform, m_LargeHealth->position());
		LargeHealth_transform = glm::rotate(LargeHealth_transform, m_LargeHealth->rotation_amount(), m_LargeHealth->rotation_axis());
		LargeHealth_transform = glm::scale(LargeHealth_transform, m_LargeHealth->scale());
		engine::renderer::submit(textured_lighting_shader, m_LargeHealth->meshes().at(0), LargeHealth_transform);
	}

	if (m_staff->Staff()) {
		glm::mat4 staff_transform(1.f);
		staff_transform = glm::translate(staff_transform, m_staff->position());
		staff_transform = glm::rotate(staff_transform, m_staff->rotation_amount(), m_staff->rotation_axis());
		staff_transform = glm::scale(staff_transform, m_staff->scale());
		engine::renderer::submit(textured_lighting_shader, staff_transform, m_staff);
	}

	if (m_shuriken->Shuriken()) {
		glm::mat4 shuriken_trans(1.f);
		shuriken_trans = glm::translate(shuriken_trans, m_shuriken->position());
		shuriken_trans = glm::rotate(shuriken_trans, m_shuriken->rotation_amount(), m_shuriken->rotation_axis());
		shuriken_trans = glm::scale(shuriken_trans, m_shuriken->scale());
		engine::renderer::submit(textured_lighting_shader, shuriken_trans, m_shuriken);
	}

	if (m_sword->Sword()) {
		glm::mat4 sword_trans(1.f);
		sword_trans = glm::translate(sword_trans, m_sword->position());
		sword_trans = glm::rotate(sword_trans, glm::pi<float>() / 2.f, glm::vec3(1.f, 0.f, 0.f));
		sword_trans = glm::scale(sword_trans, m_sword->scale());
		engine::renderer::submit(textured_lighting_shader, sword_trans, m_sword);
	}

	if (m_sniper->SniperRifle()) {
		glm::mat4 sniper_trans(1.f);
		sniper_trans = glm::translate(sniper_trans, m_sniper->position());
		sniper_trans = glm::rotate(sniper_trans, m_sniper->rotation_amount(), m_sniper->rotation_axis());
		sniper_trans = glm::scale(sniper_trans, m_sniper->scale());
		engine::renderer::submit(textured_lighting_shader, sniper_trans, m_sniper);
	}

	//Allows for the weapons to be spawned at the player's Camera if such are obtained. 
	if (m_player.carriesShuriken()) {
		glm::vec3 pos = m_3d_camera.position();
		glm::vec3 forward = glm::vec3(0.f, 0.f, 1.0f);
		glm::vec3 right = m_3d_camera.right_vector();
		float theta = engine::PI / 2.0f - acos(forward.y);
		float phi = atan2(forward.x, forward.z);
		glm::vec3 p = pos + 0.5f * forward + 0.2f * right;
		glm::mat4 shurikeno_transform(1.0f);
		shurikeno_transform = glm::translate(shurikeno_transform, p);
		shurikeno_transform = glm::rotate(shurikeno_transform, phi, glm::vec3(0.f, 1.f, 0.f));
		shurikeno_transform = glm::rotate(shurikeno_transform, -theta, glm::vec3(0.f, 0.f, 1.f));
		shurikeno_transform = glm::scale(shurikeno_transform, glm::vec3(0.025f, 0.025f, 0.025f));
		engine::renderer::submit(textured_lighting_shader, shurikeno_transform, m_shurikeno);
	}

	if (m_player.carriesStaff()) {
		glm::vec3 pos = m_3d_camera.position();
		glm::vec3 forward = glm::vec3(0.f, 0.f, 1.0f);
		glm::vec3 right = m_3d_camera.right_vector();
		float theta = engine::PI / 2.0f - acos(forward.y);
		float phi = atan2(forward.x, forward.z);
		glm::vec3 p = pos + 0.5f * forward + 0.2f * right;
		glm::mat4 cameras_transform(1.0f);
		glm::translate(cameras_transform, glm::vec3(1.f, 1.f, 2.f));
		glm::rotate(cameras_transform, phi, glm::vec3(0.f, 1.f, 0.f));
		glm::rotate(cameras_transform, -theta, glm::vec3(1.f, 0.f, 0.f));
		glm::scale(cameras_transform, glm::vec3(0.25, 0.25, 0.25));
		engine::renderer::submit(textured_lighting_shader, cameras_transform, m_staffo);
	}

	if (m_player.carriesSword()) {
		glm::vec3 pos = m_3d_camera.position();
		glm::vec3 forward = glm::vec3(0.f, 0.f, 1.0f);
		glm::vec3 right = m_3d_camera.right_vector();
		float theta = engine::PI / 2.0f - acos(forward.y);
		float phi = atan2(forward.x, forward.z);
		glm::vec3 p = pos + 0.5f * forward + 0.2f * right;
		glm::mat4 cameras_transform(1.0f);
		glm::translate(cameras_transform, glm::vec3(1.f, 1.f, 2.f));
		glm::rotate(cameras_transform, phi, glm::vec3(0.f, 1.f, 0.f));
		glm::rotate(cameras_transform, -theta, glm::vec3(1.f, 0.f, 0.f));
		glm::scale(cameras_transform, glm::vec3(0.25, 0.25, 0.25));
		engine::renderer::submit(textured_lighting_shader, cameras_transform, m_swordo);
	}

	if (m_player.carriesSniperRifle()) {
		glm::vec3 pos = m_3d_camera.position();
		glm::vec3 forward = m_3d_camera.front_vector();
		glm::vec3 right = m_3d_camera.right_vector();
		float theta = engine::PI / 2.0f - acos(forward.y);
		float phi = atan2(forward.x, forward.z);
		glm::vec3 p = pos + 0.5f * forward + 0.2f * right;
		glm::mat4 cameras_transform(1.0f);
		glm::translate(cameras_transform, glm::vec3(1.f, 1.f, 2.f));
		glm::rotate(cameras_transform, phi, glm::vec3(0.f, 1.f, 0.f));
		glm::rotate(cameras_transform, -theta, glm::vec3(1.f, 0.f, 0.f));
		glm::scale(cameras_transform, glm::vec3(0.05, 0.05, 0.05));
		engine::renderer::submit(textured_lighting_shader, cameras_transform, m_sniperrifleo);
	}

	//Renders Platforms that player can jump on and collect. 
	if (isPlatform1 == true) {
		glm::mat4 platform1_trans(1.f);
		platform1_trans = glm::translate(platform1_trans, m_platform->position());
		platform1_trans = glm::rotate(platform1_trans, m_platform->rotation_amount(), m_platform->rotation_axis());
		platform1_trans = glm::scale(platform1_trans, glm::vec3(0.1f, 0.1f, 0.1f));
		engine::renderer::submit(textured_lighting_shader, platform1_trans, m_platform);
	}

	if (isPlatform2 == true) {
		glm::mat4 platform2_trans(1.f);
		platform2_trans = glm::translate(platform2_trans, m_platform2->position());
		platform2_trans = glm::rotate(platform2_trans, m_platform->rotation_amount(), m_platform->rotation_axis());
		platform2_trans = glm::scale(platform2_trans, glm::vec3(0.1f, 0.1f, 0.1f));
		engine::renderer::submit(textured_lighting_shader, platform2_trans, m_platform2);
	}

	if (isPlatform3 == true) {
		glm::mat4 platform3_trans(1.f);
		platform3_trans = glm::translate(platform3_trans, m_platform3->position());
		platform3_trans = glm::rotate(platform3_trans, m_platform->rotation_amount(), m_platform->rotation_axis());
		platform3_trans = glm::scale(platform3_trans, glm::vec3(0.1f, 0.1f, 0.1f));
		engine::renderer::submit(textured_lighting_shader, platform3_trans, m_platform3);
	}

	if (isPlatform4 == true) {
		glm::mat4 platform4_trans(1.f);
		platform4_trans = glm::translate(platform4_trans, m_platform4->position());
		platform4_trans = glm::rotate(platform4_trans, m_platform->rotation_amount(), m_platform->rotation_axis());
		platform4_trans = glm::scale(platform4_trans, glm::vec3(0.1f, 0.1f, 0.1f));
		engine::renderer::submit(textured_lighting_shader, platform4_trans, m_platform4);
	}

	if (isPlatform5 == true) {
		glm::mat4 platform5_trans(1.f);
		platform5_trans = glm::translate(platform5_trans, m_platform5->position());
		platform5_trans = glm::rotate(platform5_trans, m_platform->rotation_amount(), m_platform->rotation_axis());
		platform5_trans = glm::scale(platform5_trans, glm::vec3(0.1f, 0.1f, 0.1f));
		engine::renderer::submit(textured_lighting_shader, platform5_trans, m_platform5);
	}

	if (isCow) {
		//Renders two cows
		glm::mat4 cow_trans(1.f);
		cow_trans = glm::translate(cow_trans, m_cow->position() - glm::vec3(m_cow->offset().x, 0.f, m_cow->offset().z) * m_cow->scale().x);
		cow_trans = glm::rotate(cow_trans, m_cow->rotation_amount(), m_cow->rotation_axis());
		cow_trans = glm::scale(cow_trans, m_cow->scale());
		engine::renderer::submit(textured_lighting_shader, cow_trans, m_cow);
	}
	if (isCow1) {
		glm::mat4 cow_trans1(1.f);
		cow_trans1 = glm::translate(cow_trans1, m_cow1->position() - glm::vec3(m_cow1->offset().x, 0.f, m_cow1->offset().z) * m_cow1->scale().x);
		cow_trans1 = glm::rotate(cow_trans1, m_cow1->rotation_amount(), m_cow1->rotation_axis());
		cow_trans1 = glm::scale(cow_trans1, m_cow1->scale());
		engine::renderer::submit(textured_lighting_shader, cow_trans1, m_cow1);
	}

	//Renders Axis-Aligned Bounding Boxes for the objects & players. 
	if (isAABB) {
		m_player_box.on_render(0.f, 1.f, 0.f, textured_lighting_shader);
		if (isZombie1) { m_zombie_box.on_render(1.f, 0.f, 0.f, textured_lighting_shader); }
		if (isWolf1) { m_wolf_box.on_render(1.f, 0.f, 0.f, textured_lighting_shader); }
		if (isWolf2) { m_wolf1_box.on_render(1.f, 0.f, 0.f, textured_lighting_shader); }
		if (isBoss) { m_bot_box.on_render(1.f, 0.f, 0.f, textured_lighting_shader); }
		if (isRobot) { m_robot_box.on_render(1.f, 0.f, 0.f, textured_lighting_shader); }
		if (isCow) { m_cow_box.on_render(0.f, 1.f, 0.f, textured_lighting_shader); }
		if (isCow1) { m_cow_box1.on_render(0.f, 1.f, 0.f, textured_lighting_shader); }
		m_jeep_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree2_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree3_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree4_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree5_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree6_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree7_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree8_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree9_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree10_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree11_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree12_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree13_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree14_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree15_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree16_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree17_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree18_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree19_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree20_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree21_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree22_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree23_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_tree24_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_house1_box.on_render(0.5f, 0.f, 1.f, textured_lighting_shader);
		m_wall_box.on_render(0.5f, 0.f, 1.f, textured_lighting_shader);
		m_wall1_box.on_render(0.5f, 0.f, 1.f, textured_lighting_shader);
		m_rock_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock1_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock2_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock3_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock4_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock5_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock6_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock7_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock8_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock9_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock10_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock11_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock12_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		m_rock13_box.on_render(0.f, 0.f, 1.f, textured_lighting_shader);
		if (isGrenade) { m_grenade_box.on_render(0.5f, 0.5f, 0.f, textured_lighting_shader); }
		m_ammunition_box.on_render(0.5f, 0.5f, 0.f, textured_lighting_shader);
	}

	//Pointlight
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumPointLights", (int)num_pointlights);
	m_pointlight.submit(textured_lighting_shader, 0);

	engine::renderer::end_scene();

	// Set up material shader. (does not render textures, renders materials instead)
	const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
	engine::renderer::begin_scene(m_3d_camera, material_shader);

	m_material->submit(material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("fog_on", m_fog);

	//Spotlight
	/*
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumSpotLights", (int)num_spotlights);
	m_spotlight.submit(material_shader, 0);
	engine::renderer::submit(material_shader, m_light->meshes().at(0), glm::translate(glm::mat4(1.f), m_spotlight.Position));
	*/

	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", false);

	m_lightsource_material->submit(material_shader);
	engine::renderer::submit(material_shader, m_light->meshes().at(0), glm::translate(glm::mat4(1.f), m_pointlight.Position));
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", true);

	//Renders the bullet.
	m_material->submit(material_shader);
	m_bulletshot.on_render(material_shader);

	//Renders the ammunition
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", false);
	m_ammunition_material->submit(material_shader);
	m_ammunition.on_render(material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", true);

	engine::renderer::end_scene();

	//Sets up animated mesh shader and initialises Player into the scene.
	const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
	engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("fog_on", m_fog);

	glm::mat4 aniTransform = glm::mat4(1.0f);

	//Loads the wolves into the game. (Enemies)
	if (isWolf1) {
		glm::mat4 wolf_trans(1.f);
		wolf_trans = glm::translate(wolf_trans, m_wolf->position());
		wolf_trans = glm::rotate(wolf_trans, -glm::pi<float>() / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		wolf_trans = glm::scale(wolf_trans, m_wolf->scale());
		engine::renderer::submit(animated_mesh_shader, wolf_trans, m_wol.object());
		if (m_wol.Get_Health() <= 0) { isWolf1 = false; }
	}
	if (isWolf2) {
		glm::mat4 wolf_trans2(1.f);
		wolf_trans2 = glm::translate(wolf_trans2, m_wolf1->position());
		wolf_trans2 = glm::rotate(wolf_trans2, -glm::pi<float>() / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		wolf_trans2 = glm::scale(wolf_trans2, m_wolf1->scale());
		engine::renderer::submit(animated_mesh_shader, wolf_trans2, m_wolf1);
	}

	//Loads the zombies into the game. (Enemies)
	if (isZombie1) {
		glm::mat4 zombie_trans(1.f);
		zombie_trans = glm::translate(zombie_trans, m_zombie->position());
		zombie_trans = glm::rotate(zombie_trans, m_zombie->rotation_amount(), m_zombie->rotation_axis());
		zombie_trans = glm::scale(zombie_trans, m_zombie->scale());
		engine::renderer::submit(animated_mesh_shader, zombie_trans, m_zomb.object());
		if (m_zomb.Get_Health() <= 0) {
			isZombie1 = false;
		}
	}

	//Renders boss into the game. (Enemy)
	if (isBoss) {
		glm::mat4 bot_trans(1.f);
		bot_trans = glm::translate(bot_trans, m_bot->position());
		bot_trans = glm::rotate(bot_trans, -glm::pi<float>() / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		bot_trans = glm::scale(bot_trans, m_bot->scale());
		engine::renderer::submit(animated_mesh_shader, bot_trans, m_bot);
	}

	//Renders the player into the game if player is alive.
	if (m_player.PlayerAlive()) {
		engine::renderer::submit(animated_mesh_shader, m_player.object());
	}

	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);
	m_billboard->on_render(m_3d_camera, textured_lighting_shader);
	engine::renderer::end_scene();

	// Renders the text
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	if (StartG == true && Pause == false) {
	m_text_manager->render_text(text_shader, "Health: " + healthhud + "%", 10.f, (float)engine::application::window().height() - 680.f, 0.4f, glm::vec4(1.f, 0.f, 0.f, 0.5f));
	m_text_manager->render_text(text_shader, "Ammunition: " + ammunitionhud + "/30" + "    Grenades: " + grenadeshud + "/10" + "    Special Ammunition: " + specialammhud + "/1", 10.f, (float)engine::application::window().height() - 700.f, 0.4f, glm::vec4(0.5f, 0.5f, 0.f, 0.5f));
	m_text_manager->render_text(text_shader, "Collected points: " + platformhud + "    Collected Secret Points: " + secretscorehud, 10.f, (float)engine::application::window().height() - 20.f, 0.4f, glm::vec4(0.f, 1.f, 1.f, 0.5f));
	m_text_manager->render_text(text_shader, "Owned scrap: " + scrapamount, 10.f, (float)engine::application::window().height() - 40.f, 0.4f, glm::vec4(0.f, 1.f, 1.f, 0.5f));
}

	//Applies Crossfade
	engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);
	m_crossfade->on_render(textured_lighting_shader);
	m_crossfadeheal->on_render(textured_lighting_shader);
	m_crossfadeend->on_render(textured_lighting_shader);

	//Intro screen:
	//One must press P in order for the boolean to start the game. 
	if (StartG == false && Pause == false) {
		m_text_manager->render_text(text_shader, "Game Rules:", 10.f, (float)engine::application::window().height() - 20.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "Player has to collect all platforms by jumping on them in order to win the game.", 10.f, (float)engine::application::window().height() - 40.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "Hungry wolves are looking for food, if player or a zombie comes across their path, they will hunt their prey!", 10.f, (float)engine::application::window().height() - 60.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "If Player comes across a zombie, zombie will try to sniff whether player is alive and if so, chase him down!", 10.f, (float)engine::application::window().height() - 80.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "Player can destroy zombie with well-placed grenade. Be careful! They are very hard to aim with and only few in your arsenal!", 10.f, (float)engine::application::window().height() - 100.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "Zombies will not attack wolves but will defend themselves if needed!", 10.f, (float)engine::application::window().height() - 120.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "Boss seeks body heat and destroys it. He will wander around the forest in order to find its prey.", 10.f, (float)engine::application::window().height() - 140.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "If player runs into a boss, or boss will defeat all the enemies on the game level, player will be found and killed.", 10.f, (float)engine::application::window().height() - 160.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "Collect all the weapons in order to scrap them for extra score! This will be useful in the next game levels!", 10.f, (float)engine::application::window().height() - 180.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "Easy mode will allow for the player to be attacked and not killed, while hard mode will make player die instantly!", 10.f, (float)engine::application::window().height() - 200.f, 0.4f, glm::vec4(1.f, 1.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "   To start the game in easy mode press G! (Recommended!!!)", 10.f, (float)engine::application::window().height() - 460.f, 0.8f, glm::vec4(0.f, 0.5f, 0.5f, 0.5f));
		m_text_manager->render_text(text_shader, "   To start the game in hard mode press press V!", 10.f, (float)engine::application::window().height() - 520.f, 1.f, glm::vec4(0.f, 0.5f, 0.5f, 0.5f));
		m_text_manager->render_text(text_shader, "Game Controls:", 10.f, (float)engine::application::window().height() - 600.f, 0.4f, glm::vec4(1.f, 0.f, 0.f, 0.5f));
		m_text_manager->render_text(text_shader, "Use C to kill cows and obtain secret points and health, it is impossible to catch all the cows! Will you risk it?", 10.f, (float)engine::application::window().height() - 620.f, 0.4f, glm::vec4(0.f, 1.f, 1.f, 0.5f));
		m_text_manager->render_text(text_shader, "F1 -> First Person Camera view, F2 - Third Person Camera view, F3 turn off background music, F4 turn it on.", 10.f, (float)engine::application::window().height() - 640.f, 0.4f, glm::vec4(0.f, 1.f, 1.f, 0.5f));
		m_text_manager->render_text(text_shader, "W -> Go forward, S -> Stop, A/D -> Turn", 10.f, (float)engine::application::window().height() - 660.f, 0.4f, glm::vec4(0.f, 1.f, 1.f, 0.5f));
		m_text_manager->render_text(text_shader, "Left Shift -> Run, Space -> Jump, G -> To throw a Heat-Stick-Grenade, T -> To shoot Auto-aim special ammunition, F -> Shoot!", 10.f, (float)engine::application::window().height() - 680.f, 0.4f, glm::vec4(0.f, 1.f, 1.f, 0.5f));
		m_text_manager->render_text(text_shader, "Win a game by collecting all the platforms and not getting killed!", 10.f, (float)engine::application::window().height() - 700.f, 0.4f, glm::vec4(0.f, 1.f, 1.f, 0.5f));
		engine::renderer::begin_scene(m_2d_camera, material_shader);
		if (engine::input::key_pressed(engine::key_codes::KEY_G)) { StartG = true; m_fog = true; isAABB = true; isEasy = true;  m_audio_manager->play("manscreaming"); m_audio_manager->play("carexplosion"); } //m_audio_manager->play_spatialised_sound("manscreaming", m_3d_camera.position(), m_jeep->position()); m_audio_manager->play_spatialised_sound("carexplosion", m_3d_camera.position(), m_jeep->position());
		if (engine::input::key_pressed(engine::key_codes::KEY_V)) { StartG = true; m_fog = true; isAABB = true; isHard = true; m_audio_manager->play("manscreaming"); m_audio_manager->play("carexplosion"); }
	}

	if (Pause == true) {
		//engine::renderer::end_scene();
		//const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
		engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);
		m_text_manager->render_text(text_shader, "   To unpause the game press P!", 15.f, (float)engine::application::window().height() - 480.f, 1.8f, glm::vec4(0.f, 0.5f, 0.5f, 0.5f));
		engine::renderer::begin_scene(m_2d_camera, material_shader);
		if (engine::input::key_pressed(engine::key_codes::KEY_P)) { StartG = true; m_audio_manager->play("idlezombie"); }
	}

	//Completed screen:
	if (FinishLevel == true) {
		engine::renderer::begin_scene(m_2d_camera, material_shader);
		m_crossfadecomplete->activate_short();
		//m_audio_manager->play_spatialised_sound("bossarrival", m_3d_camera.position(), m_boss.object()->position());
		m_audio_manager->play("bossarrival");
	}

	//Death screen:
	if (EndGame == true) {
		engine::renderer::begin_scene(m_2d_camera, material_shader);
		m_crossfadeend->activate_short();
		m_audio_manager->play("idlezombie");	
	}
} 

void example_layer::on_event(engine::event& event) 
{
	//Unless game is started, no events or actions are available.
	if (StartG == true) {
		
		if (event.event_type() == engine::event_type_e::key_pressed)
		{
			//Allows to see wireframe view if one presses TAB.
			auto& e = dynamic_cast<engine::key_pressed_event&>(event);
			if (e.key_code() == engine::key_codes::KEY_TAB)
			{
				engine::render_command::toggle_wireframe();
			}
			if (e.key_code() == engine::key_codes::KEY_6) {
				m_billboard->activate(glm::vec3(0.f, 5.f, 0.f), 4.f, 4.f);
			}
			//Allows to turn on and off fog.
			if (e.key_code() == engine::key_codes::KEY_7) {
				m_fog = !m_fog;
			}
			//Representation of getting hurt with crossfade.
			if (e.key_code() == engine::key_codes::KEY_8) {
				m_crossfade->activate_short();
			}
			if (e.key_code() == engine::key_codes::KEY_9) {
				m_crossfadeheal->activate_short();
			}
			if (e.key_code() == engine::key_codes::KEY_0) {
				if (isAABB)
					isAABB = false;
				else
					isAABB = true;
			}
			//Tests health -->  Heals the player for a 100health.
			if (e.key_code() == engine::key_codes::KEY_I) {
				m_player.Heal(100);
				m_crossfadeheal->activate();
			}
			//Tests damage, Health should be reduced by 25 and crossfade effect should appear.
			if (e.key_code() == engine::key_codes::KEY_U) {
				m_player.Damage_Taken(25);
				m_crossfade->activate();
			}
			// Allows the player to throw a grenade.
			if (e.key_code() == engine::key_codes::KEY_G) {
				if (m_player.Get_Grenades() > 0) {
					m_grenade.throwing(m_3d_camera, 100.f);
					m_player.Throw_Grenade(1);
				}
			}
			if (e.key_code() == engine::key_codes::KEY_T) {
				if (m_player.Get_SpecialAmm() > 0) {
					m_bulletshot.shoot(m_3d_camera, 100.f);
					m_player.Throw_SpecialAmm(1);
				}
			}
			if (e.key_code() == engine::key_codes::KEY_F) {
				if (m_player.Get_Ammunition() > 0) {
					m_player.Shoot_Ammunition(1);
					m_ammunition.fire(m_3d_camera, 30.f);
					isGrenade = true;
				}
			}
			//if (e.key_code() == engine::key_codes::KEY_F3) {
			//	m_audio_manager->pause("music");
			//}
			//if (e.key_code() == engine::key_codes::KEY_F4) {
			//	m_audio_manager->unpause("music");
			//}
			if (e.key_code() == engine::key_codes::KEY_SPACE) {
				isJumping = true;
			}
			if (e.key_code() == engine::key_codes::KEY_C && m_cow_box.collision(m_player_box)) {
				m_cowa.Damage_Taken(25); //Kills the cow and heals the player for 100.
				m_player.Heal(100);
				secretscore++;
			}
			if (e.key_code() == engine::key_codes::KEY_C && m_cow_box1.collision(m_player_box)) {
				m_cowa1.Damage_Taken(25);
				m_player.Heal(100);
				secretscore++;
			}
			// This code allows us to print out current position of the mannequin into the console. This is used to help find the position where new objects could be placed.
			if (e.key_code() == engine::key_codes::KEY_V) {
				std::cout << m_mannequin->position();
			}
		}
	}
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (e.key_code() == engine::key_codes::KEY_P) {
			Pause = !Pause;
		}
	}
}

void example_layer::check_bounce()
{
	//if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
		//m_audio_manager->play("bounce");
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}

int example_layer::platform_score() { return platformscore; }
int example_layer::secret_score() { return secretscore; }

void example_layer::platforms() {
	//Mechanics of the platforms.
	//Checks whether player is nearby the given platform and if yes, whether he is jumping. If both statements are true, platform will be deleted and player will increase his platforms' score. 
	glm::vec3 playersposition = m_mannequin->position();

	if (isPlatform1 == true && glm::length(playersposition - m_platform->position()) < 1.f && engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
		isPlatform1 = false;
		platformscore++;
		m_player.Heal(20);
		m_crossfadeheal->activate();
		m_timer1 = 5.f;
		m_audio_manager->play("manscreaming");
		m_player.Collect_SpecialAmm(1);
		//m_audio_manager->play_spatialised_sound("manscreaming", m_3d_camera.position(), glm::vec3(-19.37f, 1.f, -30.16f));
		return;
	}

	if (isPlatform2 == true && glm::length(playersposition - m_platform2->position()) < 1.f && engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
		isPlatform2 = false;
		platformscore++;
		m_timer2 = 5.f;
		m_crossfade->activate();
		m_player.Damage_Taken(30);
		return;
	}

	if (isPlatform3 == true && glm::length(playersposition - m_platform3->position()) < 1.f && engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
		isPlatform3 = false;
		platformscore++;
		m_player.Collect_Grenades(2);
		m_timer3 = 5.f;
		return;
	}

	if (isPlatform4 == true && glm::length(playersposition - m_platform4->position()) < 1.f && engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
		isPlatform4 = false;
		platformscore++;
		m_timer4 = 5.f;
		m_crossfade->activate();
		m_player.Damage_Taken(40);
		return;
	}

	if (isPlatform5 == true && glm::length(playersposition - m_platform5->position()) < 1.f && engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
		isPlatform5 = false;
		platformscore++;
		m_player.Heal(45);
		m_crossfadeheal->activate();
		m_timer5 = 5.f;
		return;
	}
}

void example_layer::Alive() {
	if (m_wol.Get_Health() <= 0) {
		isWolf2 = false;
	}

	if (m_wol1.Get_Health() <= 0) {
		isWolf1 = false;
	}

	if (m_zomb.Get_Health() <= 0) {
		isZombie1 = false;
	}

	if (m_boss.Get_Health() <= 0) {
		isBoss = false;
	}

	if (m_cowa.Get_Health() <= 0) {
		isCow = false;
	}

	if (m_cowa1.Get_Health() <= 0) {
		isCow1 = false;
	}
}

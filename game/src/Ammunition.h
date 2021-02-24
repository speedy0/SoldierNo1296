#pragma once
#include <engine.h>

class Ammunition {
public:
	Ammunition();
	~Ammunition();

	void initialise(engine::ref<engine::game_object> object);

	void fire(const engine::perspective_camera& camera, float speed);

	void on_update(const engine::timestep& time_step);
	void on_render(const engine::ref<engine::shader>& shader);

	engine::ref<engine::game_object> object() const { return m_ammunition; }

private:
	engine::ref<engine::game_object> m_ammunition;
	float m_speed = 0.f;
};

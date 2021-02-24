#pragma once
#include <engine.h>

class Cub;

class CrossFade {
public:
	CrossFade(const std::string& path, float maxt, float wdth, float hght);
	~CrossFade();

	void on_update(const engine::timestep& tsp);
	void on_render(engine::ref<engine::shader> shader);
	void activate();
	void activate_short();
	static engine::ref<CrossFade> create(const std::string& path, float maxt, float wdth, float hght);

private:
	bool cf_active;
	float m_timer;
	float m_maxt;
	engine::ref<engine::texture_2d> m_text;
	float m_transp;
	engine::ref<Cub> m_cub;
};

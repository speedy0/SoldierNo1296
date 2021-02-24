#pragma once
#include "pch.h"
#include "CrossF.h"
#include "Cub.h"

CrossFade::CrossFade(const std::string& path, float maxt, float wdth, float hght) {

	m_maxt = maxt;
	m_text = engine::texture_2d::create(path, true);
	m_transp = 0.f;
	m_cub = Cub::create(glm::vec2(wdth, hght));
	cf_active = false;
}

CrossFade::~CrossFade(){}

void CrossFade::on_update(const engine::timestep& tsp) {
	if (!cf_active) {
		return;
	}
	m_timer += (float)tsp;
	m_transp = 0.8f - 0.8f * m_timer / m_maxt;

	if (m_timer > m_maxt) { cf_active = false; }
}

void CrossFade::on_render(engine::ref<engine::shader> shader) {
	if (!cf_active) { return; }

	glm::mat4 cf_transform(1.f);
	cf_transform = glm::translate(cf_transform, glm::vec3(0.f, 0.f, 0.1f));

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transp);
	m_text->bind();
	engine::renderer::submit(shader, m_cub->mesh(), cf_transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.f);
}

void CrossFade::activate() {
	cf_active = true;
	m_transp = 1.f;
	m_timer = 0.f;
}

void CrossFade::activate_short() {
	cf_active = true;
	m_transp = 0.02f;
	m_timer = 0.0000001f;
}

engine::ref<CrossFade> CrossFade::create(const std::string& path, float maxt, float wdth, float hght) {
	return std::make_shared<CrossFade>(path, maxt, wdth, hght);
}

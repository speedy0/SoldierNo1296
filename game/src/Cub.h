#pragma once
#include <engine.h>

class Cub
{
public:
	//Constructor of the Quad
	Cub(glm::vec2 half_ext);
	~Cub();

	glm::vec2 half_extents() const { return m_half_ext; }
	engine::ref<engine::mesh> mesh() const { return m_cub; }
	static engine::ref<Cub> create(glm::vec2 half_ext);

private:
	glm::vec2 m_half_ext;
	engine::ref<engine::mesh> m_cub;

};

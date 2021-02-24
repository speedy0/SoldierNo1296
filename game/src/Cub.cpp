#pragma once
#include "pch.h"
#include "Cub.h"

Cub::Cub(glm::vec2 half_ex) : m_half_ext(half_ex){
	std::vector<engine::mesh::vertex> Cub_vertices
	{
		//Creation of the Cub's vertices also known as Quad in the lab.
		//Position													normals					Texture Coordinates
		{ {-1.f * m_half_ext.x, -1.f * m_half_ext.y,  0.0f },		{ 0.0f, 0.0f,  -1.0f },		{ 0.f, 0.f } },
		{ { 1.f * m_half_ext.x, -1.f * m_half_ext.y,  0.0f },		{ 0.0f, 0.0f,  -1.0f },		{ 1.f, 0.f } },
		{ { 1.f * m_half_ext.x,  1.f * m_half_ext.y,  0.0f },		{ 0.0f, 0.0f,  -1.0f },		{ 1.f, 1.f } },
		{ {-1.f * m_half_ext.x,  1.f * m_half_ext.y,  0.0f },		{ 0.0f, 0.0f,  -1.0f },		{ 0.f, 1.f } },
	};
	const std::vector<uint32_t> Cub_indices{
		0,1,2,
		0,2,3,
	};

	m_cub = engine::mesh::create(Cub_vertices, Cub_indices);
}

Cub::~Cub() {}

engine::ref<Cub> Cub::create(glm::vec2 half_ext){
	return std::make_shared<Cub>(half_ext);
}

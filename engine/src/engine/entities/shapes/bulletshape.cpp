#pragma once
#include "pch.h"
#include "bulletshape.h"
#include <engine.h>	

engine::bulletshape::bulletshape(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// bottom right
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
	//bottom left
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(0), vertices.at(2) - vertices.at(3)));
	//middle back left
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(2), vertices.at(3) - vertices.at(4)));
	//middle back right
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(2), vertices.at(4) - vertices.at(5)));
	//middle right left
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(2), vertices.at(5) - vertices.at(1)));
	//middle right right 
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) - vertices.at(6)));
	//middle front left
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(1), vertices.at(6) - vertices.at(0)));
	//middle front right 
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(0), vertices.at(6) - vertices.at(7)));
	//middle left left
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(0), vertices.at(7) - vertices.at(3)));
	//middle left right
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(3), vertices.at(7) - vertices.at(4)));
	//front top
	normals.push_back(glm::cross(vertices.at(7) - vertices.at(6), vertices.at(7) - vertices.at(8)));
	//right top
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(5), vertices.at(6) - vertices.at(8)));
	//back top
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) - vertices.at(8)));
	//left top
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(7), vertices.at(4) - vertices.at(8)));

	std::vector<mesh::vertex> bulletshape_vertices
	{
		//bottom right
		//  position			normal			      tex coord       
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(0),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(0),		{ 0.5f, 1.f } },

		//bottom left
		//  position			normal			      tex coord       
		{ vertices.at(2),		normals.at(1),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(1),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 0.5f, 1.f } },

		//middle back left
		//  position			normal			      tex coord       
		{ vertices.at(3),		normals.at(2),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(2),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(2),		{ 0.5f, 1.f } },

		//middle back right
		//  position			normal			      tex coord       
		{ vertices.at(4),		normals.at(3),		{ 0.f,  0.f } },
		{ vertices.at(5),		normals.at(3),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(3),		{ 0.5f, 1.f } },

		//middle right left
		//  position			normal			      tex coord       
		{ vertices.at(5),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(4),		{ 1.f,  0.f } },
		{ vertices.at(2),		normals.at(4),		{ 0.5f, 1.f } },

		//middle right right
		//  position			normal			      tex coord       
		{ vertices.at(5),		normals.at(5),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(5),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(5),		{ 0.5f, 1.f } },

		//middle front left
		//  position			normal			      tex coord       
		{ vertices.at(6),		normals.at(6),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(6),		{ 1.f,  0.f } },
		{ vertices.at(1),		normals.at(6),		{ 0.5f, 1.f } },

		//middle front right
		//  position			normal			      tex coord       
		{ vertices.at(6),		normals.at(7),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(7),		{ 1.f,  0.f } },
		{ vertices.at(0),		normals.at(7),		{ 0.5f, 1.f } },

		//middle left left
		//  position			normal			      tex coord       
		{ vertices.at(7),		normals.at(8),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(8),		{ 1.f,  0.f } },
		{ vertices.at(0),		normals.at(8),		{ 0.5f, 1.f } },

		//middle left right
		//  position			normal			      tex coord       
		{ vertices.at(7),		normals.at(9),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(9),		{ 1.f,  0.f } },
		{ vertices.at(3),		normals.at(9),		{ 0.5f, 1.f } },

		//front top
		//  position			normal			      tex coord       
		{ vertices.at(7),		normals.at(10),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(10),		{ 1.f,  0.f } },
		{ vertices.at(8),		normals.at(10),		{ 0.5f, 1.f } },

		//right top
		//  position			normal			      tex coord       
		{ vertices.at(6),		normals.at(11),		{ 0.f,  0.f } },
		{ vertices.at(5),		normals.at(11),		{ 1.f,  0.f } },
		{ vertices.at(8),		normals.at(11),		{ 0.5f, 1.f } },

		//back top
		//  position			normal			      tex coord       
		{ vertices.at(5),		normals.at(12),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(12),		{ 1.f,  0.f } },
		{ vertices.at(8),		normals.at(12),		{ 0.5f, 1.f } },

		//left top
		//  position			normal			      tex coord       
		{ vertices.at(4),		normals.at(13),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(13),		{ 1.f,  0.f } },
		{ vertices.at(8),		normals.at(13),		{ 0.5f, 1.f } },
	};

	const std::vector<uint32_t> bulletshape_indices
	{
		0, 1, 2, //bottom right
		3, 4, 5, //bottom left
		6, 7, 8, //middle back left
		9, 10, 11, //middle back right
		12, 13, 14, //middle right left
		15, 16, 17, //middle right right
		18, 19, 20, //middle front left
		21, 22, 23, //middle front right
		24, 25, 26, //middle left left
		27, 28, 29, //middle left right
		30, 31, 32, //front top
		33, 34, 35, //right top
		36, 37, 38, //back top
		39, 40, 41 //left top
	};

	m_mesh = engine::mesh::create(bulletshape_vertices, bulletshape_indices);
}

engine::bulletshape::~bulletshape() {}

engine::ref<engine::bulletshape> engine::bulletshape::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::bulletshape>(vertices);
}

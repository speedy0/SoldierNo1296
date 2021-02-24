#pragma once
#include "pch.h"
#include "platform.h"
#include <engine.h>	

engine::platform::platform(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// bottom down
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(2)));
	//bottom left
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(3)));
	//bottom right
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(4)));
	//bottom top
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(5)));
	//side right 1
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(0), vertices.at(5) - vertices.at(6)));
	//side left 1
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(6), vertices.at(5) - vertices.at(7)));
	//side right 2
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) - vertices.at(7)));
	//side left 2
	normals.push_back(glm::cross(vertices.at(5) - vertices.at(8), vertices.at(5) - vertices.at(7)));
	//side right 3
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(3), vertices.at(4) - vertices.at(9)));
	//side left 3
	normals.push_back(glm::cross(vertices.at(4) - vertices.at(9), vertices.at(4) - vertices.at(8)));
	//side right 4
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(2), vertices.at(3) - vertices.at(10)));
	//side left 4
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(10), vertices.at(3) - vertices.at(9)));
	//side right 5
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(1), vertices.at(2) - vertices.at(11)));
	//side left 5
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(11), vertices.at(2) - vertices.at(10)));
	//side right 6
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(0), vertices.at(1) - vertices.at(6)));
	//side left 6
	normals.push_back(glm::cross(vertices.at(1) - vertices.at(6), vertices.at(1) - vertices.at(11)));
	//top down
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(11), vertices.at(6) - vertices.at(10)));
	//top left
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(10), vertices.at(6) - vertices.at(9))); 
	//top right
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(9), vertices.at(6) - vertices.at(8)));
	//bottom top
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(8), vertices.at(6) - vertices.at(7)));



	std::vector<mesh::vertex> platform_vertices
	{
		//  position			normal			      tex coord       
		//bottom down
		{ vertices.at(0),		normals.at(0),		{ 0.f,  0.75f } },
		{ vertices.at(1),		normals.at(0),		{ 0.f,  0.25f } },
		{ vertices.at(2),		normals.at(0),		{ 0.5f, 0.f } },

		//bottom left
		{ vertices.at(0),		normals.at(1),		{ 0.f,  0.75f } },
		{ vertices.at(2),		normals.at(1),		{ 0.5f,  0.f } },
		{ vertices.at(3),		normals.at(1),		{ 1.f, 1.f } },

		//bottom right
		{ vertices.at(0),		normals.at(2),		{ 0.f,  0.75f } },
		{ vertices.at(3),		normals.at(2),		{ 1.f,  1.f } },
		{ vertices.at(4),		normals.at(2),		{ 1.f, 0.75f } },

		//bottom top
		{ vertices.at(0),		normals.at(3),		{ 0.f,  0.75f } },
		{ vertices.at(4),		normals.at(3),		{ 1.f,  0.75f } },
		{ vertices.at(5),		normals.at(3),		{ 0.5f, 1.f } },

		//side right 1
		{ vertices.at(5),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(4),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(4),		{ 0.f,  0.f } },

		//side left 1
		{ vertices.at(5),		normals.at(5),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(5),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(5),		{ 0.f,  0.f } },

		//side right 2
		{ vertices.at(5),		normals.at(6),		{ 0.f,  0.f } },
		{ vertices.at(4),		normals.at(6),		{ 0.f,  0.f } },
		{ vertices.at(8),		normals.at(6),		{ 0.f,  0.f } },

		//side left 2
		{ vertices.at(5),		normals.at(7),		{ 0.f,  0.f } },
		{ vertices.at(8),		normals.at(7),		{ 0.f,  0.f } },
		{ vertices.at(7),		normals.at(7),		{ 0.f,  0.f } },

		//side right 3
		{ vertices.at(4),		normals.at(8),		{ 0.f,  0.f } },
		{ vertices.at(3),		normals.at(8),		{ 0.f,  0.f } },
		{ vertices.at(9),		normals.at(8),		{ 0.f,  0.f } },

		//side left 3
		{ vertices.at(4),		normals.at(9),		{ 0.f,  0.f } },
		{ vertices.at(9),		normals.at(9),		{ 0.f,  0.f } },
		{ vertices.at(8),		normals.at(9),		{ 0.f,  0.f } },

		//side right 4
		{ vertices.at(3),		normals.at(10),		{ 0.f,  0.f } },
		{ vertices.at(2),		normals.at(10),		{ 0.f,  0.f } },
		{ vertices.at(10),		normals.at(10),		{ 0.f,  0.f } },

		//side left 4
		{ vertices.at(3),		normals.at(11),		{ 0.f,  0.f } },
		{ vertices.at(10),		normals.at(11),		{ 0.f,  0.f } },
		{ vertices.at(9),		normals.at(11),		{ 0.f,  0.f } },

		//side right 5
		{ vertices.at(2),		normals.at(12),		{ 0.f,  0.f } },
		{ vertices.at(1),		normals.at(12),		{ 0.f,  0.f } },
		{ vertices.at(11),		normals.at(12),		{ 0.f,  0.f } },

		//side left 5
		{ vertices.at(2),		normals.at(13),		{ 0.f,  0.f } },
		{ vertices.at(11),		normals.at(13),		{ 0.f,  0.f } },
		{ vertices.at(10),		normals.at(13),		{ 0.f,  0.f } },

		//side right 6
		{ vertices.at(1),		normals.at(14),		{ 0.f,  0.f } },
		{ vertices.at(0),		normals.at(14),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(14),		{ 0.f,  0.f } },

		//side left 6
		{ vertices.at(1),		normals.at(15),		{ 0.f,  0.f } },
		{ vertices.at(6),		normals.at(15),		{ 0.f,  0.f } },
		{ vertices.at(11),		normals.at(15),		{ 0.f,  0.f } },

		//top bottom
		{ vertices.at(6),		normals.at(16),		{ 0.f,  0.75f } },
		{ vertices.at(10),		normals.at(16),		{ 0.f,  0.25f } },
		{ vertices.at(11),		normals.at(16),		{ 0.5f, 0.f } },

		//top left
		{ vertices.at(6),		normals.at(17),		{ 0.f,  0.75f } },
		{ vertices.at(9),		normals.at(17),		{ 0.f,  0.5f } },
		{ vertices.at(10),		normals.at(17),		{ 1.f, 1.f } },

		//top right
		{ vertices.at(6),		normals.at(18),		{ 0.f,  0.75f } },
		{ vertices.at(8),		normals.at(18),		{ 1.f,  1.f } },
		{ vertices.at(9),		normals.at(18),		{ 1.f, 0.75f } },

		//top up
		{ vertices.at(6),		normals.at(19),		{ 0.f,  0.75f } },
		{ vertices.at(7),		normals.at(19),		{ 1.f,  0.75f } },
		{ vertices.at(8),		normals.at(19),		{ 0.5f, 1.f } },

	};

	const std::vector<uint32_t> platform_indices
	{
		0,  1,  2,	//bottom down
		3,  4,  5,	//bottom left
		6,	7,	8,	//bottom right
		9,	10,	11,	//bottom top
		12, 13, 14, //side right 1
		15, 16, 17, //side left 1
		18, 19, 20, //side right 2
		21, 22, 23, //side left 2
		24, 25, 26, //side right 3
		27, 28, 29, //side left 3
		30, 31, 32, //side right 4
		33, 34, 35, //side left 4
		36, 37, 38, //side right 5
		39, 40, 41, //side left 5
		42, 43, 44, //side right 6
		45, 46, 47, //side left 6
		48, 49, 50, //top down
		51, 52, 53, //top left
		54, 55, 56, //top right
		57, 58, 59 //top up
	};

	m_mesh = engine::mesh::create(platform_vertices, platform_indices);
}

engine::platform::~platform() {}

engine::ref<engine::platform> engine::platform::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::platform>(vertices);
}

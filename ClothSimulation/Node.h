#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Node
{
public: 
	Node() :pos(glm::vec3(0,0,0)), vel(glm::vec3(0,0,0)), acc(glm::vec3(0,0,0)), force(glm::vec3(0,0,0)), mass(1.0) {} 
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	glm::vec3 force;
	float mass;
};


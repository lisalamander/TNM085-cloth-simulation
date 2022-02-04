#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Node
{
public: 
	Node(const glm::vec3& p) : pos(p), vel(glm::vec3(0,0,0)), acc(glm::vec3(0,0,0)), force(glm::vec3(0,0,0)), mass(1.0), isFixed(false) {}
	Node() : Node(glm::vec3(0,0,0))  {} 
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	glm::vec3 force;
	float const mass;
	bool isFixed;


};

//  <(O.o<) <(O.o)> (>O.o)>
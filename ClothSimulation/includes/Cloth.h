#include "Node.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class Cloth {
public:
	Cloth(int x_size, int y_size, const glm::vec3& pos);
	~Cloth();
	void render();
	void updateSimulation(float time_step);
	void handleSphereIntersections(float radius, const glm::vec3& position);
	glm::vec3 getPos() const {	return position;} 
	
private:
#define IX(a,b) ((a) + (b) * cols);

	void createVertexData();
	void updateVertexData();
	void createIndexData();
	void sendVertexData();
	void sendIndexData();
	std::vector<Node> nodes;
	std::vector<float> vertices;
	std::vector<unsigned int> indicies;
	glm::vec3 position;

	int rows, cols;
	const float k1, k2, k3, damping_c;
	unsigned int VBO, EAO, VAO;

};
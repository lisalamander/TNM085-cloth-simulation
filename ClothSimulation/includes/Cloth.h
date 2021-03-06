#include "Node.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
class Cloth {
public:
	Cloth(int x_size, int y_size, const glm::vec3& pos);
	~Cloth();
	void render();
	void updateSimulation(float time_step);
	void updateBuffers();
	void handleSphereIntersections(float radius, const glm::vec3& position);
	void setConstants(float k1_, float k2_, float k3_, float damping_);
	glm::vec3 getPos() const {	return position;} 
	
private:
#define IX(a,b) ((a) + (b) * cols);

	void createVertexData();
	void updateVertexData();
	void createIndexData();
	void createNormalData();
	void createTextureData();
	void sendVertexData();
	void sendIndexData();
	void sendTextureData();
	void sendNormalData();
	std::vector<Node> nodes;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indicies;
	glm::vec3 position;
	

	int rows, cols;
	float k1, k2, k3, damping_c;
	unsigned int VBO, EAO, VAO, TBO, NBO;

};
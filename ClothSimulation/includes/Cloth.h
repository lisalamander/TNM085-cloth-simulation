#include "Node.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Cloth {
public:
	Cloth(int x_size, int y_size);
	~Cloth();
	void render();
	void updateSimulation(float time_step);
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

	int rows, cols;
	const float k1, k2, damping_c;
	unsigned int VBO, EAO, VAO;

};
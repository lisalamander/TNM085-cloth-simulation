#include "Node.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Cloth {
public:
	Cloth(int x_size, int y_size);
	~Cloth();
	void render();
private:
#define IX(a,b) ((a) + (b) * cols);

	void createVertexData();
	void createIndexData();
	std::vector<Node> nodes;
	std::vector<float> vertices; // test
	std::vector<unsigned int> indicies;

	int rows, cols;

	unsigned int VBO, EAO, VAO;

};
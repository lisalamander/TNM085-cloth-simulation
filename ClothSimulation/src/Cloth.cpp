#include "../includes/Cloth.h"
#include <iostream> // for debugging

Cloth::Cloth(int x_size, int y_size): cols(x_size), rows(y_size) {
	// Create buffers
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EAO);
	// Set vertex data
	createVertexData();
	// Send vertex data to GPU
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Set index data
	createIndexData();
	// Send index data to GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies.size(), indicies.data(), GL_STATIC_DRAW);


}

Cloth::~Cloth() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EAO);
	glDeleteVertexArrays(1, &VAO);
}


void Cloth::render() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);

}
// Fills vertex vector with data
void Cloth::createVertexData() {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			nodes.push_back(glm::vec3((float)c/cols, (float)r/rows, -5));
			Node n = nodes.back();
			vertices.push_back(n.pos.x);
			vertices.push_back(n.pos.y);
			vertices.push_back(n.pos.z);
			// Lock upper row
			if (r == rows - 1)
				n.isFixed = true;


		}
	}
	std::cout << "\nGenerated " << vertices.size() / 3 << " vertices:\n";
	int counter = 0;
	/*for (float f : vertices) {
		counter++;
		std::cout << f << ", " << (counter % 3 == 0 ? "\n" : "");
	}*/
}
// Fills index vector with data
void Cloth::createIndexData() {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (c < cols - 1 && r < rows - 1) {
				unsigned int index = IX(c, r);
				indicies.push_back(index);
				index = IX(c + 1, r);
				indicies.push_back(index);
				index = IX(c + 1, r + 1);
				indicies.push_back(index);

				index = IX(c, r);
				indicies.push_back(index);
				index = IX(c + 1, r + 1);
				indicies.push_back(index);
				index = IX(c, r + 1);
				indicies.push_back(index);
				
			}
		}
	}

	std::cout << "Generated " << indicies.size() << " indices:\n";
	/*for (unsigned int i : indicies) {
		std::cout << i << " ";
	}*/
}
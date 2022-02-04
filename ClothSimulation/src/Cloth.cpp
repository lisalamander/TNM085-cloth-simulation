#include "../includes/Cloth.h"
#include <iostream> // for debugging

Cloth::Cloth(int x_size, int y_size): cols(x_size), rows(y_size), k1(500), k2(200), damping_c(10) {
	// Create buffers
	glGenBuffers(1, &VBO);		// Vertex Buffer Object
	glGenVertexArrays(1, &VAO);	// Vertex Array Object
	glGenBuffers(1, &EAO);		// Element Array Object
	// Set vertex data
	createVertexData();
	// Send vertex data to GPU
	sendVertexData();
	// Set index data
	createIndexData();
	// Send index data to GPU
	sendIndexData();
	

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
			
			nodes.push_back(glm::vec3((float)c/cols, (float)r/rows, 0));
			Node& n = nodes.back();
			vertices.push_back(n.pos.x);
			vertices.push_back(n.pos.y);
			vertices.push_back(n.pos.z);
			// Lock upper row
			if (r == rows - 1 && (c == 0 || c == cols-1))
				n.isFixed = true;
		}
	}
	//std::cout << "\nGenerated " << vertices.size() / 3 << " vertices:\n";
	//int counter = 0;
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

	//std::cout << "Generated " << indicies.size() << " indices:\n";
	/*for (unsigned int i : indicies) {
		std::cout << i << " ";
	}*/
}


void Cloth::updateSimulation(float time_step) {
	// clear vector data
	vertices.clear();
	indicies.clear();
	// calculate forces
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			
			int thisNodesIndex = IX(c, r);
			if (nodes[thisNodesIndex].isFixed)
				continue;
			int left = c - 1;
			int right = c + 1;
			int up = r + 1;
			int down = r - 1;
			int downLeftNeighbour = IX(left, down);
			int leftNeighbour = IX(left, r);
			int upLeftNeighbour = IX(left, up);
			int upNeighbour = IX(c, up);
			int upRightNeighbour = IX(right, up);
			int rightNeighbour = IX(right, r);
			int downRightNeighbour = IX(right, down);
			int downNeighbour = IX(c, down);

			nodes[thisNodesIndex].force = glm::vec3(0, 0, 0);
			std::vector<glm::vec3> forces(8, glm::vec3(0, 0, 0));
			glm::vec3 gravitationalForce = glm::vec3(0, -9.82, 0)* nodes[thisNodesIndex].mass;
			const float L1 = (1.0f / cols);			// TODO: move to outside function
			const float L2 = sqrt(2 * pow(L1, 2));

			// down left 
			if (left >= 0 && down >= 0) {
				glm::vec3 X = nodes[thisNodesIndex].pos - nodes[downLeftNeighbour].pos;
				float X_magn = glm::length(X);
				forces[0] += -k2 * (X_magn - L2) * X / X_magn;
			}

			// left
			if (left >= 0) {
				glm::vec3 X = nodes[thisNodesIndex].pos - nodes[leftNeighbour].pos;
				float X_magn = glm::length(X);
				forces[1] += -k1 * (X_magn - L1) * X / X_magn;
			}

			// up left 
			if (left >= 0 && up < rows) {
				glm::vec3 X = nodes[thisNodesIndex].pos - nodes[upLeftNeighbour].pos;
				float X_magn = glm::length(X);
				forces[2] += -k2 * (X_magn - L2) * X / X_magn;
			}

			// up
			if (up < rows) {
				glm::vec3 X = nodes[thisNodesIndex].pos - nodes[upNeighbour].pos;
				float X_magn = glm::length(X);
				forces[3] += -k1 * (X_magn - L1) * X / X_magn;
			}

			// up right 
			if (right < cols && up < rows) {
				glm::vec3 X = nodes[thisNodesIndex].pos - nodes[upRightNeighbour].pos;
				float X_magn = glm::length(X);
				forces[4] += -k2 * (X_magn - L2) * X / X_magn;
			}
			// right
			if (right < cols) {
				glm::vec3 X = nodes[thisNodesIndex].pos - nodes[rightNeighbour].pos;
				float X_magn = glm::length(X);
				forces[5] += -k1 * (X_magn - L1) * X / X_magn;
			}
			
			// down right 
			if (down >= 0 && right < cols) {
				glm::vec3 X = nodes[thisNodesIndex].pos - nodes[downRightNeighbour].pos;
				float X_magn = glm::length(X);
				forces[6] += -k2 * (X_magn - L2) * X / X_magn;
			}
			// down
			if (down >= 0) {
				glm::vec3 X = nodes[thisNodesIndex].pos - nodes[downNeighbour].pos;
				float X_magn = glm::length(X);
				forces[7] += -k1 * (X_magn - L1) * X / X_magn;
			}
			glm::vec3 dampingForce = -nodes[thisNodesIndex].vel * damping_c;

			for (const glm::vec3& f : forces) {
				nodes[thisNodesIndex].force += f;
			}
			nodes[thisNodesIndex].force += (dampingForce + gravitationalForce);
			// calculate acceleration
			nodes[thisNodesIndex].acc = (1 / nodes[thisNodesIndex].mass) * nodes[thisNodesIndex].force;
			// numerical method
			nodes[thisNodesIndex].Euler(time_step);
		}
	}
	updateVertexData();
	createIndexData();
	sendVertexData();
	sendIndexData();
}

void Cloth::sendVertexData() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void Cloth::sendIndexData() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies.size(), indicies.data(), GL_STATIC_DRAW);
}


void Cloth::updateVertexData() {
	vertices.clear();
	for (int i = 0; i < nodes.size(); i++) {
		vertices.push_back(nodes[i].pos.x);
		vertices.push_back(nodes[i].pos.y);
		vertices.push_back(nodes[i].pos.z);
	}
}
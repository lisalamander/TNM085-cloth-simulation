#include "../includes/Cloth.h"
#include <iostream> // for debugging

Cloth::Cloth(int x_size, int y_size, const glm::vec3& pos): cols(x_size), rows(y_size), k1(2000), k2(5000), k3(200), damping_c(50), position(pos) {
	// Create buffers
	glGenBuffers(1, &VBO);		// Vertex Buffer Object
	glGenVertexArrays(1, &VAO);	// Vertex Array Object
	glGenBuffers(1, &EAO);		// Element Array Object
	glGenBuffers(1, &TBO);		// Texture Buffer Object
	glGenBuffers(1, &NBO);		// Normal Buffer Object
	// Set vertex data
	createVertexData();
	// Send vertex data to GPU
	sendVertexData();
	// Set index data
	createIndexData();
	// Send index data to GPU
	sendIndexData();
	// Set texture data
	createTextureData();
	// Send texture data to GPU
	sendTextureData();
	// Set normal data
	createNormalData();
	// Send normal data to GPU
	sendNormalData();
}

Cloth::~Cloth() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EAO);
	glDeleteBuffers(1, &TBO);
	glDeleteBuffers(1, &NBO);
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
			
			nodes.push_back(glm::vec3((float)c/cols+position.x, (float)r/rows+position.y, position.z));
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

void Cloth::createNormalData() {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			
				int thisNodeIndex = IX(c, r);
				int rightNeighbourIndex = IX(c + 1, r);
				int upRightNeighbourIndex = IX(c + 1, r + 1);
				int upNeighbourIndex = IX(c, r + 1);
				int downNeighbourIndex = IX(c, r - 1);

				glm::vec3 normal;
				// For all nodes, except last row and last column
				if (c < cols - 1 && r < rows - 1) {
					// Normal for first triangle
					glm::vec3 v1 = nodes[rightNeighbourIndex].pos - nodes[thisNodeIndex].pos;
					glm::vec3 v2 = nodes[upRightNeighbourIndex].pos - nodes[rightNeighbourIndex].pos;
					glm::vec3 normal1 = glm::normalize(glm::cross(v1, v2));
					// Normal for second triangle
					glm::vec3 v3 = nodes[thisNodeIndex].pos - nodes[upRightNeighbourIndex].pos;
					glm::vec3 v4 = nodes[upRightNeighbourIndex].pos - nodes[upNeighbourIndex].pos;
					glm::vec3 normal2 = glm::normalize(glm::cross(v3, v4));
					// Mean normal
					normal = glm::normalize((normal1 + normal2) / 2.0f);
					
				}
				// For last row
				else if (c < cols - 1) {
					// Use normal for second triangle of last row
					glm::vec3 v1 = nodes[downNeighbourIndex].pos - nodes[rightNeighbourIndex].pos;
					glm::vec3 v2 = nodes[rightNeighbourIndex].pos - nodes[thisNodeIndex].pos;
					glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
					
					
				}
				// For last column
				else {
					// Use normal for last vertex
					glm::vec3 normal = glm::vec3(normals.size() - 3, normals[normals.size() - 2], normals[normals.size() - 1]);
				}
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
			


		}
	}
	//std::cout << "Generated " << normals.size()/3 << " normals.\n";
}


void Cloth::createTextureData() {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			texCoords.push_back(c / (float)cols);
			texCoords.push_back(r / (float)rows);
		}
	}
}

void Cloth::updateSimulation(float time_step) {
	// clear vector data
	vertices.clear();
	indicies.clear();
	normals.clear();
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
			std::vector<glm::vec3> forces(12, glm::vec3(0, 0, 0));
			glm::vec3 gravitationalForce = glm::vec3(0, -9.82, 0)* nodes[thisNodesIndex].mass;
			const float L1 = (1.0f / cols);			// TODO: move to outside function
			const float L2 = sqrt(2 * pow(L1, 2));
			const float L3 = L1 * 2;
			
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
				if (left - 1 >= 0) {
					int leftleft = leftNeighbour - 1;
					glm::vec3 X = nodes[thisNodesIndex].pos - nodes[leftleft].pos;
					float X_magn = glm::length(X);
					forces[8] += -k3 * (X_magn - L3) * X / X_magn;
				}
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
				if (up + 1  < rows) {
					int upup = upNeighbour + cols;
					glm::vec3 X = nodes[thisNodesIndex].pos - nodes[upup].pos;
					float X_magn = glm::length(X);
					forces[9] += -k3 * (X_magn - L3) * X / X_magn;
				}
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
				if (right + 1 < cols) {
					int rightright = rightNeighbour + 1;
					glm::vec3 X = nodes[thisNodesIndex].pos - nodes[rightright].pos;
					float X_magn = glm::length(X);
					forces[10] += -k3 * (X_magn - L3) * X / X_magn;
				}
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
				if (down - 1 >= 0) {
					int downdown = downNeighbour - cols;
					glm::vec3 X = nodes[thisNodesIndex].pos - nodes[downdown].pos;
					float X_magn = glm::length(X);
					forces[11] += -k3 * (X_magn - L3) * X / X_magn;
				}
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
			//nodes[thisNodesIndex].Verlet(time_step);
			
		}
	}
	
}

void Cloth::updateBuffers() {
	updateVertexData();
	createIndexData();
	createNormalData();
	sendVertexData();
	sendIndexData();
	sendNormalData();
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

void Cloth::sendTextureData() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 2 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
}
void Cloth::sendNormalData() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * normals.size(), normals.data(), GL_STATIC_DRAW);
}

void Cloth::updateVertexData() {
	vertices.clear();
	for (const Node& n : nodes) {
		vertices.push_back(n.pos.x);
		vertices.push_back(n.pos.y);
		vertices.push_back(n.pos.z);
	}
}

// Sets the minimum node distance to SpherePos = sphereRadius
void Cloth::handleSphereIntersections(float sphereRadius, const glm::vec3& spherePos) {
	float threshold = 0.025f;
	for (Node& n : nodes) {
		if (n.isFixed)
			continue;
		glm::vec3 nodeToSphere = n.pos - spherePos;
		float distToSphere = glm::length(nodeToSphere);
	
		if (distToSphere < sphereRadius+threshold) {
			// Set node position to the border of the sphere
			glm::vec3 sphereBorder = nodeToSphere / distToSphere * (sphereRadius + threshold);
			n.pos = spherePos + sphereBorder;
			// Update velocity
			//n.vel = -n.vel;
			// Reflection of ingoing velocity
			n.vel = (-n.vel + (glm::dot(-n.vel, glm::normalize(nodeToSphere))* glm::normalize(nodeToSphere) - n.vel) * 2.0f)*0.25f;
		}
	}
	
}

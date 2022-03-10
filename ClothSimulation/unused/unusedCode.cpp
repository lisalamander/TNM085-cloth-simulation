bool Cloth::passConstraints(int thisNodesIndex, int column, int row) {
	int c = column;
	int r = row;
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
	const float L1 = (1.0f / cols);			// TODO: move to outside function
	const float L2 = sqrt(2 * pow(L1, 2));
	const float L3 = L1 * 2;
	float stiffness_threshold = 1.10f;
	std::vector<int> neighbours = { downLeftNeighbour, leftNeighbour, upLeftNeighbour, upNeighbour, upRightNeighbour, rightNeighbour, downRightNeighbour, downNeighbour };
	int counter = 1;
	for (int neighbour : neighbours) {
		if (neighbour < 0 || neighbour > nodes.size() - 1) {
			counter++;
			continue;
		}
		float distance = glm::length(nodes[thisNodesIndex].pos - nodes[neighbour].pos);
		// For shear springs
		if (counter % 2 == 1) {
			if (distance / L2 > stiffness_threshold)
				return false;
		}
		// For stretch springs
		else {
			if (distance / L1 > stiffness_threshold)
				return false;
		}
		counter++;
	}
	return true;
}
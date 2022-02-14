#include "../includes/Node.h"

void Node::Euler(float step_size) {
	vel = vel + step_size * acc;
	pos = pos + step_size * vel;
}

// @TODO: Implement real verlet integration method
void Node::Verlet(float step_size) {
	
	newPos = pos + pos - lastPos + (step_size * step_size * acc);
	lastPos = pos;
	pos = newPos;

	vel = 1 / (2*step_size) * (pos - lastPos);
	
}
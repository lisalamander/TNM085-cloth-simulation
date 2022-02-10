#include "../includes/Node.h"

void Node::Euler(float step_size) {
	vel = vel + step_size * acc;
	pos = pos + step_size * vel;
}

// @TODO: Implement real verlet integration method
void Node::Verlet(float step_size) {
	
	vel = vel + acc * step_size;
	pos = pos + vel * step_size + 0.5f * acc * step_size * step_size;
}
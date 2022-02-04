#include "../includes/Node.h"

void Node::Euler(float step_size) {
	vel = vel + step_size * acc;
	pos = pos + step_size * vel;
}
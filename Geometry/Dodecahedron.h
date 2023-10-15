#pragma once
#include "Polyhedron.h"

class Dodecahedron : public Polyhedron
{
public:
	using Polyhedron::Polyhedron;

	//constructs the shape.
	void construct(double spacing);
	void constructEdges(double spacing);

	void reconstruct(double spacing);
};
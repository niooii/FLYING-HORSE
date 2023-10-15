#include "Dodecahedron.h"

//construct a dodecahedron
void Dodecahedron::construct(double spacing)
{
	//golden ratio
	double gr = (1 + std::sqrt(5)) / 2;

	//initialize vertices vector first

	//orange
	vertices.emplace_back(1, 1, 1);//0
	vertices.emplace_back(1, 1, -1);//1
	vertices.emplace_back(1, -1, 1);//2
	vertices.emplace_back(1, -1, -1);//3

	vertices.emplace_back(-1, 1, 1);//4
	vertices.emplace_back(-1, 1, -1);//5
	vertices.emplace_back(-1, -1, 1);//6
	vertices.emplace_back(-1, -1, -1);//7

	//green
	vertices.emplace_back(0, gr, 1 / gr);//8
	vertices.emplace_back(0, gr, -1 / gr);//9
	vertices.emplace_back(0, -gr, 1 / gr);//10
	vertices.emplace_back(0, -gr, -1 / gr);//11

	//blue
	vertices.emplace_back(1 / gr, 0, gr);//12
	vertices.emplace_back(1 / gr, 0, -gr);//13
	vertices.emplace_back(-1 / gr, 0, gr);//14
	vertices.emplace_back(-1 / gr, 0, -gr);//15

	//pink
	vertices.emplace_back(gr, 1 / gr, 0);//16
	vertices.emplace_back(gr, -1 / gr, 0);//17
	vertices.emplace_back(-gr, 1 / gr, 0);//18
	vertices.emplace_back(-gr, -1 / gr, 0);//19

	points.insert(points.begin(), vertices.begin(), vertices.end());

	texture = textures::purpleStar;

	constructEdges(spacing);

	std::cout << points.size() << " points!" << '\n';
}

void Dodecahedron::constructEdges(double spacing)
{
	//KILLS MYSELF
	connectPoints(vertices[2], vertices[10], spacing);
	connectPoints(vertices[2], vertices[17], spacing);
	connectPoints(vertices[2], vertices[12], spacing);
	connectPoints(vertices[12], vertices[14], spacing);
	connectPoints(vertices[12], vertices[0], spacing);
	connectPoints(vertices[14], vertices[4], spacing);
	connectPoints(vertices[14], vertices[6], spacing);
	//connectPoints(vertices[2], vertices[17], spacing); duplicate
	connectPoints(vertices[6], vertices[19], spacing);
	connectPoints(vertices[19], vertices[7], spacing);
	connectPoints(vertices[7], vertices[15], spacing);
	connectPoints(vertices[15], vertices[13], spacing);
	connectPoints(vertices[13], vertices[1], spacing);
	connectPoints(vertices[1], vertices[16], spacing);
	connectPoints(vertices[16], vertices[0], spacing);
	connectPoints(vertices[16], vertices[17], spacing);
	connectPoints(vertices[0], vertices[8], spacing);
	connectPoints(vertices[8], vertices[4], spacing);
	//connectPoints(vertices[4], vertices[6], spacing); incorrect
	connectPoints(vertices[4], vertices[18], spacing);
	connectPoints(vertices[18], vertices[19], spacing);
	//connectPoints(vertices[15], vertices[13], spacing); duplicate
	connectPoints(vertices[6], vertices[10], spacing);
	connectPoints(vertices[10], vertices[11], spacing);
	connectPoints(vertices[11], vertices[7], spacing);
	connectPoints(vertices[5], vertices[15], spacing);
	connectPoints(vertices[5], vertices[9], spacing);
	connectPoints(vertices[9], vertices[8], spacing);
	connectPoints(vertices[5], vertices[18], spacing);
	connectPoints(vertices[3], vertices[11], spacing);
	connectPoints(vertices[3], vertices[17], spacing);
	connectPoints(vertices[3], vertices[13], spacing);
	connectPoints(vertices[1], vertices[9], spacing);
	connectPoints(vertices[19], vertices[18], spacing);
}

void Dodecahedron::reconstruct(double spacing)
{
	//reset vector of points
	points.clear();

	points.insert(points.begin(), vertices.begin(), vertices.end());

	constructEdges(spacing);
}
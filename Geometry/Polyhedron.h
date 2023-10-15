#pragma once
#include <iostream>
#include <vector>
#include <SDL.h>
#include "Vector3.h"
#include "Entities/Projectile.h"
#include "Utility/Utility.h"
#include "globals/global.h"
#include "globals/Textures.h"

struct Face {
	std::vector<int> vertexIndices;
};

class Polyhedron
{
protected:
	SDL_Renderer* renderer;
	winfo* info;

	SDL_Texture* texture;
	std::vector<Face> faces;
	double Scale{};
	//RENDER OFFSET
	double roX;
	//RENDER OFFSET
	double roY;
	//render scale
	double rScale;
	//update projectile positions
	void updatePr();

public:
	boolean pointsOnly{ false };
	boolean showTexture{ true };
	Polyhedron();
	Polyhedron(winfo* info, SDL_Renderer* renderer);
	void connectPoints(Vector3 start, Vector3 end, double interval);
	
	SDL_FPoint center;

	std::vector<Projectile> projectiles;
	std::vector<Vector3> vertices;
	std::vector<Vector3> points;
	//add vertex (IM NOT EVEN USING THIS)
	void vertex(Vector3 v);
	std::vector<Vector3>& getPoints();
	void constructEdges(double spacing);
	void render();
	void renderPoints(SDL_Color color);
	void rotate(double rX, double rY, double rZ);
	void setRotation(double rX, double rY, double rZ);
	void scale(double scale);
	void move(double x, double y);
};
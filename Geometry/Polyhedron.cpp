#include "Polyhedron.h"

Polyhedron::Polyhedron()
{
    this->info = nullptr;
    this->renderer = nullptr;
}

Polyhedron::Polyhedron(SingleplayerGame* inst, winfo* info, SDL_Texture* tex, SDL_Renderer* renderer)
{
    this->inst = inst;
    this->texture = tex;
    this->info = info;
    this->renderer = renderer;
}

void Polyhedron::connectPoints(Vector3 start, Vector3 end, double spacing)
{
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float dz = end.z - start.z;

    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    int numPoints = static_cast<int>(std::round(distance / spacing));

    float stepX = dx / numPoints;
    float stepY = dy / numPoints;
    float stepZ = dz / numPoints;

    for (int i = 0; i <= numPoints; ++i) {
        float x = start.x + i * stepX;
        float y = start.y + i * stepY;
        float z = start.z + i * stepZ;
        points.emplace_back(x, y, z);
    }
}

void Polyhedron::constructEdges(double spacing)
{
    for (const auto& face : faces) {
        int numVertices = static_cast<int>(face.vertexIndices.size());

        // Connect each pair of adjacent vertices on the face
        for (int i = 0; i < numVertices; ++i) {
            int v1 = face.vertexIndices[i];
            int v2 = face.vertexIndices[(i + 1) % numVertices];

            // Generate points along the edge using the connectPoints function
            connectPoints(vertices[v1], vertices[v2], spacing);
        }
    }
}

//rotation around x axis, y, z.
void Polyhedron::rotate(double rX, double rY, double rZ)
{
    for (Vector3& v : points)
    {
        v.rotate(rX, rY, rZ);
    }
}

void Polyhedron::setRotation(double rX, double rY, double rZ)
{
    for (Vector3& v : points)
    {
        v.setRotation(rX, rY, rZ);
    }
}

void Polyhedron::scale(double scale)
{
    this->rScale = scale;
}

void Polyhedron::move(double x, double y)
{
    this->roX = x;
    this->roY = y;
    center.x = roX;
    center.y = roY;
}

void Polyhedron::vertex(Vector3 v)
{
    vertices.emplace_back(v);
}

std::vector<Vector3>& Polyhedron::getPoints()
{
    return points;
}

void Polyhedron::render()
{
    updatePr();
    for (Projectile& projectile : projectiles)
    {
        if (showTexture)
        {
            projectile.render();
        }
        else
        {
            if (constants::renderHitboxes)
                SDL_RenderDrawRect(renderer, &projectile.hitbox);
        }
    }
}

void Polyhedron::renderPoints(SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (Vector3& p : points)
    {
        
        SDL_RenderDrawPoint(renderer, (p.x * rScale) + roX, (p.y * rScale) + roY);
    }
}
    

void Polyhedron::updatePr()
{
    projectiles.clear();
    Projectile pr = Projectile(inst, texture, renderer, info, 0, 0);
    pr.friction = false;
    pr.collisions = false;
    pr.acceleration = { 0, 0 };
    for (Vector3& p : points)
    {
        //std::cout << "initializing thing at x: " << (p.x * rScale) - pr.size.w / 2 + roX << ", y: " << (p.y * rScale) - pr.size.h / 2 + roY << ".\n";
        pr.x = (p.x * rScale) - pr.size.w / 2 + roX;
        pr.y = (p.y * rScale) - pr.size.h / 2 + roY;
        pr.updateBox();
        projectiles.emplace_back(pr);
    }
    /*pr.x = center.x - pr.size.w/2;
    pr.y = center.y - pr.size.h/2;
    projectiles.emplace_back(pr);*/
}
#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>
#include "plane.h"

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    Hit hit = {0, 0, 0};

    if (part < 0) {
        hit.dist = std::numeric_limits<double>::max();
        unsigned int i = 0;

        while (i < triangles.size()) {
            double temp_dist;
            bool doesIntersect = Intersect_Triangle(ray, i, temp_dist);

            if (doesIntersect && temp_dist < hit.dist) {
                hit.object = this;
                hit.dist = temp_dist;
                hit.part = i;
            }

            i += 1;
        }
    }
    else if (part >= 0) {
        bool doesIntersect = Intersect_Triangle(ray, part, hit.dist);
        if (doesIntersect) {
            hit.object = this;
            hit.part = part;
        }
    }

    return hit;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);

    ivec3 points = triangles[part];
    return cross(
        vertices[points[1]] - vertices[points[0]],
        vertices[points[2]] - vertices[points[0]]).normalized();
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    ivec3 points = triangles[tri];

    vec3 normal = Normal(vertices[points[0]], tri);
    Hit hit = Plane(vertices[points[0]], normal)
        .Intersection(ray, tri);

    if (hit.object != NULL) {
        vec3 u = ray.direction;
        vec3 v = vertices[points[1]] - vertices[points[0]];
        vec3 w = vertices[points[2]] - vertices[points[0]];
        vec3 y = ray.endpoint - vertices[points[0]];

        double gamma = dot(cross(u, v), y) / dot(cross(u, v), w);
        double beta = dot(cross(w, u), y) / dot(cross(u, v), w);
        double alpha = 1 - gamma - beta;

        if (dot(cross(u, v), w) && gamma > -weight_tol && beta > -weight_tol && alpha > -weight_tol) {
            dist = hit.dist;
            return true;
        }
    }

    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}

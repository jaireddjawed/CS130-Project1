#include <cmath>
#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    Hit hit;

    double a = ray.direction.magnitude_squared(),
           b = 2 * dot(ray.direction, ray.endpoint - center),
           c = (ray.endpoint - center).magnitude_squared() - (radius * radius);

    double desc = (b * b) - (4 * a * c);

    if (desc <= 0) {
        return hit;
    }

    double t2 = (-b - sqrt(desc))/(2 * a);

    if (t2 > 0) {
        hit.object = this;
        hit.dist = t2;
        return hit;
    }
    else {
        double t1 = (-b + sqrt(desc)/(2 * a));
        hit.object = this;
        hit.dist = t1;
        return hit;
    }
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    TODO; // compute the normal direction
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}

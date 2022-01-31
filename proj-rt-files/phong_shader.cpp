#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color,
         ambient,
         diffuse,
         specular;

    // calculate ambient
    ambient = world.ambient_color * world.ambient_intensity * color_ambient;

    for (unsigned int i = 0; i < world.lights.size(); i += 1) {
        vec3 l = world.lights[i]->position - intersection_point;

        Ray myRay;
        myRay.endpoint = intersection_point;
        myRay.direction = l;

        Hit hit = world.Closest_Intersection(myRay);

        if (world.enable_shadows) {
            if ((myRay.Point(hit.dist) - intersection_point).magnitude() < l.magnitude()) {
                continue;
            }
        }

        vec3 light_color = world.lights[i]->Emitted_Light(l);

        // calculate diffuse
        double diffuseIntensity = dot(l.normalized(), normal);
        if (diffuseIntensity < 0) diffuseIntensity = 0;

        diffuse += diffuseIntensity * color_diffuse * light_color;

        vec3 reflectedDirection = ((2 * dot(l, normal) * normal) - l).normalized();

        // calculate specular
        double specularIntensity = dot(reflectedDirection, -(ray.direction.normalized()));
        if (specularIntensity < 0) specularIntensity = 0;
        specularIntensity = pow(specularIntensity, specular_power);

        specular += specularIntensity * color_specular * light_color;
    }

    color = ambient + diffuse + specular;
    return color;
}

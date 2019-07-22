#include "ray_tracer.h"


int main() {

	vList r1;

	NewItem(r1, &Sphere(Vector3D(0, -100.5, 0), 100, new Metal(Vector3D(0.8, 0.8, 0), 1)));
	NewItem(r1, &Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(0.51, 0.7, 1.0))));
	//NewItem(r1, &Sphere(Vector3D(0,0,0), 1, new Dielectric(1.3)));
	//NewItem(r1, &Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))).BBox(r1, new Lambertian(Vector3D(1, 0, 0))));

	RayTracer({ 200, 100 }, 100, Camera({ 200, 100 }, Vector3D(10, 0, 0)), r1, "ray_tracer");

	return 0;
}
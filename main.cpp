#include "ray_tracer.h"

int main() {

	vList r1;

	 NewItem(r1, &Sphere(Vector3D(0), 1, new Metal(Vector3D(1,0,0), 1)));
	//NewItem(r1, &Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(1, 0, 0))));
	// NewItem(r1, &Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))).BBox(r1, new Dielectric(1.5)));

	SDim a = { 500, 250 };
	RayTracer(a, 10, Camera(a, Vector3D(5, 3, 0)), r1, "ray_tracer");

	return 0;
}
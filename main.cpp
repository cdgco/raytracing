#include "ray_tracer.h"

int main() {

	vList r1;

<<<<<<< Updated upstream
	// Sphere Only
	 NewItem(r1, &Sphere(Vector3D(5, 3, 0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))));

	// Box Only
	 NewItem(r1, &Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(1, 0, 0))));

	// Sphere with Box
	// NewItem(r1, &Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))).BBox(r1));

	SDim a = { 200, 100 };
	RayTracer(a, 10, Camera(a, Vector3D(20, 3, 0)), r1, "RayTracer");
=======
	//NewItem(r1, &Sphere(Vector3D(3,0,0), 1, new Metal(Vector3D(1,0,0), 1)));
	//NewItem(r1, &Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(0, 0, 0))));
	NewItem(r1, &Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))).BBox(r1, new Lambertian(Vector3D(1, 0, 0))));

	SDim a = { 100, 50 };
	RayTracer(a, 10, Camera(a, Vector3D(5, 3, 0)), r1, "ray_tracer");

>>>>>>> Stashed changes

	return 0;
}
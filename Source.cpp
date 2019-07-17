#include "RayTracer.h"

int main() {

	vArray r1;

	// Sphere Only
	// NewItem(r1, &Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))));

	// Box Only
	// NewItem(r1, &Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(1.0, 0.0, 0.0))));

	// Sphere with Box
	NewItem(r1, &Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))).BBox(r1, new Lambertian(Vector3D(1,0,0))));

	SDim a = { 200, 100 };
	RayTracer(a, 30, Camera(a, Vector3D(5, 3, 0)), r1, "RayTracer");

	return 0;
}
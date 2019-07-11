#include "pch.h"
#include "RayTracer.h"

int main() {

	Object *list[3];

	list[0] = new Sphere(Vector3D(0, 0, 0), 0.6, new Dielectric(1.7));
	list[1] = new Sphere(Vector3D(-1.1, 0, 0), 0.5, new Lambertian(Vector3D(0,0,1)));
	list[2] = new Sphere(Vector3D(0, -100.5, -1), 100, new Lambertian(Vector3D(0.8, 0.8, 0.0)));
	
	Object *world = new ObjectList(list, 3);

	Dims dims = { 1080, 540 };
	Camera cam(dims, Vector3D(0, 1, 5));
	RayTracer(dims, 10, cam, world);
	return 0;
}
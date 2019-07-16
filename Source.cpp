#include "RayTracer.h"

using namespace std;

int main() {

	vector<Object*> r1;

	NewItem(r1, &Sphere(Vector3D(0, 0, 0), 0.5, new Lambertian(Vector3D(0.8, 0.3, 0.3))));
	NewItem(r1, &Sphere(Vector3D(0, -100.5, 0), 100, new Lambertian(Vector3D(0.51, 0.7, 1.0))));
	NewItem(r1, &Sphere(Vector3D(1, 0, 0), 0.5, new Metal(Vector3D(0.8, 0.6, 0.2), 0.3)));
	NewItem(r1, &Sphere(Vector3D(-1, 0, 0), 0.5, new Dielectric(1.5)));
	NewItem(r1, &Sphere(Vector3D(-1, 0, 0), -0.45, new Dielectric(1.5)));

	Dim a = { 200, 100 };
	RayTracer(a, 30, Camera(a, Vector3D(4, 2, 2)), r1, "RayTracer");

	return 0;
}
#include "pch.h"
#include "RayTracer.h"

using namespace std;

int main() {

	vector<Object*> r1;

	NewItem(r1, &Sphere(Vector3D(0, 0, -1), 0.5, new Lambertian(Vector3D(0.8, 0.3, 0.3))));
	NewItem(r1, &Sphere(Vector3D(0, -100.5, -1), 100, new Lambertian(Vector3D(0.51, 0.7, 1.0))));
	NewItem(r1, &Sphere(Vector3D(1, 0, -1), 0.5, new Metal(Vector3D(0.8, 0.6, 0.2), 0.3)));
	NewItem(r1, &Sphere(Vector3D(-1, 0, -1), 0.5, new Dielectric(1.5)));
	NewItem(r1, &Sphere(Vector3D(-1, 0, -1), -0.45, new Dielectric(1.5)));

	Dim a = { 1080, 540 };
	RayTracer(a, 30, Camera(a, Vector3D(3, 1, 3)), r1);

	return 0;
}
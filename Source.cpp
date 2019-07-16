#include "RayTracer.h"

using namespace std;

int main() {

	vector<Object*> r1;

	NewItem(r1, &Box(Vector3D(-5, -5, -5), Vector3D(5, 5, 5), new Lambertian(Vector3D(1, 1, 1))));
	NewItem(r1, &Sphere(Vector3D(0, 0, 0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))));

	Dim a = { 1280, 640 };
	RayTracer(a, 30, Camera(a, Vector3D(4, 2, 2)), r1, "RayTracer");

	return 0;
}
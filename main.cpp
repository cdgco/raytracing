#include "ray_tracer.h"

int main() {

	RayTracer *ray_tracer = new RayTracer({ 2000, 1000 });
	
	ray_tracer->SetCamera(Vector3D(10, 3, -6));

	//ray_tracer->AddItem(new Sphere(Vector3D(0, -100.5, 0), 100, new Metal(Vector3D(0.8, 0.8, 0), 1)));
	//ray_tracer->AddItem(new Box(Vector3D(-2), Vector3D(2), new Lambertian(Vector3D(0))));
	//ray_tracer->AddItem(new Sphere(Vector3D(0,2.2,0), 1, new Lambertian(Vector3D(1, 0, 0))));

	ray_tracer->AddItem(new Sphere(Vector3D(0, -100.5, 0), 100, new Lambertian(Vector3D(0.8, 0.8, 0))));
	ray_tracer->AddItem(new Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(0))));
	ray_tracer->AddItem(new Sphere(Vector3D(0, 2, 0), 1, new Lambertian(Vector3D(1,0,0))));
	
	ray_tracer->Render("ray_tracer");

	delete ray_tracer;

	return 0;
}
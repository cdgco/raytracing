#include "ray_tracer.h"

int main() {

	RayTracer *ray_tracer = new RayTracer({ 200, 100 }, 100);
	
	ray_tracer->SetCamera(Vector3D(0, 0, 0));

	ray_tracer->AddItem(new Sphere(Vector3D(0, 0, -1), 0.5, 0, Vector3D(0, 0.8, 1)));
	//ray_tracer->AddItem(new Sphere(Vector3D(0, 0, -1), 0.5, new Lambertian(Vector3D(0, 0.8, 1))));
	//ray_tracer->AddItem(new Sphere(Vector3D(0, -100.5, 0), 100, 0, Vector3D(0.8, 0.8, 0)));
	//ray_tracer->AddItem(new Sphere(Vector3D(0, -100.5, 0), 100, new Lambertian(Vector3D(0.8, 0.8, 0))));
	//ray_tracer->AddItem(new Box(Vector3D(-2), Vector3D(2), new Lambertian(Vector3D(0,0,1))));
	
	ray_tracer->clRender("ray_tracer");

	delete ray_tracer;

	return 0;
}
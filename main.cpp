#include "ray_tracer.h"

int main() {

	RayTracer *ray_tracer = new RayTracer({ 600, 300 }, 100);
	
	ray_tracer->SetCamera(Vector3D(5, 0, 0));
	ray_tracer->AddItem(new Sphere(Vector3D(0, 0, 0), 0.5, new Metal(Vector3D(.2588, .5294, .9608), 1.4)));
	ray_tracer->Render("ray_tracer");

	delete ray_tracer;
	
	return 0;
}
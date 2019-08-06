#include "ray_tracer.h"

int main() {

	RayTracer *ray_tracer = new RayTracer({ 1000, 500 }, 100);
	
	ray_tracer->SetCamera(Vector3D(0, 0, 0));

	//ray_tracer->AddItem(new Sphere(Vector3D(0, -1000, 0), 1000, new Lambertian(Vector3D(0.5, 0.5, 0.5))));;
	/*
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = drand48();
			Vector3D center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
			if ((center - Vector3D(4, 0.2, 0)).Length() > 0.9) {
				if (choose_mat < 0.4) {  // diffuse
					ray_tracer->AddItem(new Box(center-.3, center+.3, new Lambertian(Vector3D(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
				}
				else if (choose_mat < 0.8) {  // diffuse
					ray_tracer->AddItem(new Sphere(center, 0.2, new Lambertian(Vector3D(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
				}
				else if (choose_mat < 0.95) { // metal
					ray_tracer->AddItem(new Sphere(center, 0.2, new Metal(Vector3D(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48())));
				}
				else {  // glass
					ray_tracer->AddItem(new Sphere(center, 0.2, new Dielectric(1.5)));
				}
			}
		}
	}

		//ray_tracer->AddItem(new Box(Vector3D(-66, -12, -20), Vector3D(-50, 4,-4), new Lambertian(Vector3D(0.4, 0.2, 0.1))));
		ray_tracer->AddItem(new Box(Vector3D(-2, -.2, -1.5), Vector3D(0, 1.8,.5), new Lambertian(Vector3D(0.4, 0.2, 0.1))));
		ray_tracer->AddItem(new Sphere(Vector3D(-4, 1, 0), 1.0, new Dielectric(1.5)));
		ray_tracer->AddItem(new Sphere(Vector3D(4, 1, 0), 1.0, new Metal(Vector3D(0.7, 0.6, 0.5), 0.0)));
*/


	//OpenCL
	ray_tracer->AddItem(new Sphere(Vector3D(0, 0, -1), 0.5, 0, Vector3D(.2588,.5294,.9608), 1.0, 1.4));
	//ray_tracer->AddItem(new Sphere(Vector3D(0, -100.5, 0), 100, 0, Vector3D(0, 0.8, 0)));
	//ray_tracer->AddItem(new Box(Vector3D(-1), Vector3D(1), 0, Vector3D(0,1,0), 0.0, 1.4));

	//CPU
	//ray_tracer->AddItem(new Sphere(Vector3D(0, 0, -1), 0.5, new Lambertian(Vector3D(.2588, .5294, .9608))));
	//ray_tracer->AddItem(new Sphere(Vector3D(0, -100.5, 0), 100, new Lambertian(Vector3D(0, 0.8, 0))));
	//ray_tracer->AddItem(new Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(0,0,1))));	
	
	ray_tracer->clRender("ray_tracer");

	delete ray_tracer;
	
	return 0;
}
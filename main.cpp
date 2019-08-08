#include "ray_tracer.h"

int main() {

	SDim test = { 1600, 800 };
	int raysperpixel = 50;
	int objectcount = 32;
	int type = 1;

	RayTracer *ray_tracer = new RayTracer(test, raysperpixel);
	
	ray_tracer->SetCamera(Vector3D(5, 0, 0));

	for (int i = 0; i < objectcount; i++) {
		ray_tracer->AddItem(new Sphere(Vector3D(0, 0, 0), 0.5, new Metal(Vector3D(.2588, .5294, .9608), 1.4)));
	}

	//ray_tracer->AddItem(new Sphere(Vector3D(0, 0, 0), 0.5, new Metal(Vector3D(.2588, .5294, .9608), 1.4)));
	//ray_tracer->AddItem(new Sphere(Vector3D(0, 0, 1.1), 0.5, new Dielectric(1.3)));
	//ray_tracer->AddItem(new Sphere(Vector3D(0, -100.5, 0), 100, new Lambertian(Vector3D(0, 0.8, 0))));
	//ray_tracer->AddItem(new Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(0, 0, 1))));
	//ray_tracer->AddItem(new Box(Vector3D(-1), Vector3D(1), new Metal(Vector3D(0,1,0), 1.6)));
	//ray_tracer->AddItem(new Box(Vector3D(-.5), Vector3D(.5), new Lambertian(Vector3D(0, 1, 0))));
	//ray_tracer->AddItem(new Sphere(Vector3D(0, 0, 2.5), 1, new Lambertian(Vector3D(1, 0, 0))));

	if (type == 0) {
		ray_tracer->Render("ray_tracer");
	}
	else {
		ray_tracer->clRender("ray_tracer");
	}
	delete ray_tracer;
	
	return 0;
}
#include "ray_tracer.h"

int main() {

	RayTracer *ray_tracer = new RayTracer({ 1600, 800 }, 90);
	
	ray_tracer->SetCamera(Vector3D(90, 10, 80));
	
	//ray_tracer->AddItem(new Sphere(Vector3D(-100, -100.5, 0), 100.5, new Lambertian(Vector3D(0.137, 0.301, 0.141)))); // Grass

	// Level 1
	ray_tracer->AddItem(new Box(Vector3D(-20, 0, -20), Vector3D(20, 1, -3), new Lambertian(Vector3D(.3)))); // Right Bottom Level
	ray_tracer->AddItem(new Box(Vector3D(-18.5, 1, -18.5), Vector3D(18.5, 1.1, -4.5), new Lambertian(Vector3D(0.498, 0.321, 0.062)))); // Right Bottom Inside Floor
	ray_tracer->AddItem(new Box(Vector3D(-20, 0, -3), Vector3D(-3, 1, 20), new Lambertian(Vector3D(.3)))); // Left Bottom Level
	ray_tracer->AddItem(new Box(Vector3D(-19.5, 1, -3), Vector3D(-18.5, 4 , 18), new Lambertian(Vector3D(.4)))); // Back Left Wall
	ray_tracer->AddItem(new Box(Vector3D(-19.5, 1, -18), Vector3D(-18.5, 4 , -3), new Lambertian(Vector3D(.6)))); // Back Left Wall
	ray_tracer->AddItem(new Box(Vector3D(-18, 1, -19.5), Vector3D(18, 4, -18.5), new Lambertian(Vector3D(.6)))); // Back Right Wall
	ray_tracer->AddItem(new Box(Vector3D(-20, 1, -20), Vector3D(-18, 10, -18), new Lambertian(Vector3D(.7)))); // Back Center Bottom Column
	ray_tracer->AddItem(new Box(Vector3D(-20, 1, 18), Vector3D(-18, 10, 20), new Lambertian(Vector3D(.7)))); // Back Left Bottom Column
	ray_tracer->AddItem(new Box(Vector3D(18, 1, -20), Vector3D(20, 10, -18), new Lambertian(Vector3D(.7)))); // Back Right Bottom Column
	ray_tracer->AddItem(new Box(Vector3D(18.5, 1, -18), Vector3D(19.5, 4, -5), new Lambertian(Vector3D(.7)))); // Wall
	ray_tracer->AddItem(new Box(Vector3D(18.5, 8, -18), Vector3D(19.5, 11, -5), new Lambertian(Vector3D(.7)))); // Wall
	ray_tracer->AddItem(new Box(Vector3D(-20, 1, -5), Vector3D(-4, 10, -3), new Lambertian(Vector3D(.7)))); // Middle Wall
	ray_tracer->AddItem(new Box(Vector3D(-4, 1, -5), Vector3D(8, 4, -3), new Lambertian(Vector3D(.7)))); // Inside Bottom Front Right Wall
	ray_tracer->AddItem(new Box(Vector3D(-4, 8, -5), Vector3D(8, 10, -3), new Lambertian(Vector3D(.7)))); // Inside Top Front Right Wall
	ray_tracer->AddItem(new Box(Vector3D(8, 1, -5), Vector3D(11, 11, -3), new Lambertian(Vector3D(.7)))); // Column Left of Door
	ray_tracer->AddItem(new Box(Vector3D(11, 1, -5), Vector3D(18, 10, -3), new Lambertian(Vector3D(0.498, 0.321, 0.062)))); // Door
	ray_tracer->AddItem(new Box(Vector3D(18, 1, -5), Vector3D(20, 10, -3), new Lambertian(Vector3D(.7)))); // Front Right Bottom Column
	ray_tracer->AddItem(new Box(Vector3D(-6, 1, 4), Vector3D(-4, 10, 6), new Lambertian(Vector3D(.7)))); // Front Center Left Column
	ray_tracer->AddItem(new Box(Vector3D(-6.5, 1, 6), Vector3D(-4.5, 4, 18), new Lambertian(Vector3D(.4)))); // Wall
	
	ray_tracer->AddItem(new Box(Vector3D(-18, 1, 18.5), Vector3D(-6, 4, 19.5), new Lambertian(Vector3D(.4)))); // Far Left Wall
	ray_tracer->AddItem(new Box(Vector3D(-6, 1, 18), Vector3D(-4, 10, 20), new Lambertian(Vector3D(.7)))); // Front Left Bottom Column

	ray_tracer->AddItem(new Box(Vector3D(20, 9, -4.5), Vector3D(21.5, 9.5, -4.0), new Lambertian(Vector3D(0)))); // Light Post 1
	ray_tracer->AddItem(new Sphere(Vector3D(21.4, 8.8, -19), .2, new Metal(Vector3D(1, 0.901, 0.121), 1.3))); // Light Ball 1
	ray_tracer->AddItem(new Sphere(Vector3D(21.4, 8.6, -19), .7, new Dielectric(1.3))); // Light Ball 1

	ray_tracer->AddItem(new Box(Vector3D(20, 9, -19.5), Vector3D(21.5, 9.5, -19), new Lambertian(Vector3D(0)))); // Light Post 2
	ray_tracer->AddItem(new Sphere(Vector3D(21.4, 8.8, -4), .2, new Metal(Vector3D(1, 0.901, 0.121), 1.3))); // Light Ball 2
	ray_tracer->AddItem(new Sphere(Vector3D(21.4, 8.6, -4), .7, new Dielectric(1.3))); // Light Ball 2

	ray_tracer->AddItem(new Box(Vector3D(19, 9, -3), Vector3D(19.5, 9.5, -1.5), new Lambertian(Vector3D(0)))); // Light Post 3
	ray_tracer->AddItem(new Sphere(Vector3D(19.4, 8.8, -1.6), .2, new Metal(Vector3D(1, 0.901, 0.121), 1.3))); // Light Ball 3
	ray_tracer->AddItem(new Sphere(Vector3D(19.4, 8.6, -1.6), .7, new Dielectric(1.3))); // Light Ball 3

	ray_tracer->AddItem(new Box(Vector3D(9.5, 9, -3), Vector3D(10, 9.5, -1.5), new Lambertian(Vector3D(0)))); // Light Post 4
	ray_tracer->AddItem(new Sphere(Vector3D(9.9, 8.8, -1.6), .2, new Metal(Vector3D(1, 0.901, 0.121), 1.3))); // Light Ball 4
	ray_tracer->AddItem(new Sphere(Vector3D(9.9, 8.6, -1.6), .7, new Dielectric(1.3))); // Light Ball 4

	ray_tracer->AddItem(new Box(Vector3D(-4, 9, 19), Vector3D(-2.5, 9.5, 19.5), new Lambertian(Vector3D(0)))); // Light Post 5
	ray_tracer->AddItem(new Sphere(Vector3D(-2.6, 8.8, 19.1), .2, new Metal(Vector3D(1, 0.901, 0.121), 1.3))); // Light Ball 5
	ray_tracer->AddItem(new Sphere(Vector3D(-2.6, 8.6, 19.1), .7, new Dielectric(1.3))); // Light Ball 5

	ray_tracer->AddItem(new Box(Vector3D(-4, 9, 5), Vector3D(-2.5, 9.5, 5.5), new Lambertian(Vector3D(0)))); // Light Post 6
	ray_tracer->AddItem(new Sphere(Vector3D(-2.6, 8.8, 5.3), .2, new Metal(Vector3D(1, 0.901, 0.121), 1.3))); // Light Ball 6
	ray_tracer->AddItem(new Sphere(Vector3D(-2.6, 8.6, 5.1), .7, new Dielectric(1.3))); // Light Ball 6

	// Level 2
	ray_tracer->AddItem(new Box(Vector3D(-20, 23, -20), Vector3D(12, 24, -3), new Lambertian(Vector3D(.3)))); // Top Level
	ray_tracer->AddItem(new Box(Vector3D(-20, 10, -20), Vector3D(20, 11, -3), new Lambertian(Vector3D(.3)))); // Right Middle Level
	ray_tracer->AddItem(new Box(Vector3D(-18.5, 11, -18.5), Vector3D(18.5, 11.1, -4.5), new Lambertian(Vector3D(0.498, 0.321, 0.062)))); // Right Middle Level
	ray_tracer->AddItem(new Box(Vector3D(-20, 10, -3), Vector3D(-4, 11, 20), new Lambertian(Vector3D(.3)))); // Left Middle Level
	ray_tracer->AddItem(new Box(Vector3D(-19.5, 11, -18), Vector3D(-18.5, 14, -3), new Lambertian(Vector3D(.6)))); // Inside Bottom Back Left Wall
	ray_tracer->AddItem(new Box(Vector3D(-19.5, 19, -18), Vector3D(-18.5, 23, -3), new Lambertian(Vector3D(.6)))); // Inside Top Back Left Wall
	ray_tracer->AddItem(new Box(Vector3D(-19.5, 11, -5), Vector3D(-18.5, 14, 19.5), new Lambertian(Vector3D(.4)))); // Outside Back Left Wall
	ray_tracer->AddItem(new Box(Vector3D(-20, 11, -19.5), Vector3D(10, 14, -18.5), new Lambertian(Vector3D(.6)))); // Inside Bottom Back Right Wall
	ray_tracer->AddItem(new Box(Vector3D(-20, 19, -19.5), Vector3D(10, 23, -18.5), new Lambertian(Vector3D(.6)))); // Inside Top Back Right Wall
	ray_tracer->AddItem(new Box(Vector3D(12, 11, -19.5), Vector3D(19.5, 14, -18.5), new Lambertian(Vector3D(.4)))); // Outside Back Right Wall
	ray_tracer->AddItem(new Box(Vector3D(-20, 11, -20), Vector3D(-18, 23, -18), new Lambertian(Vector3D(.7)))); // Back Center Middle Column
	ray_tracer->AddItem(new Box(Vector3D(-20, 11, -5), Vector3D(-18, 23, -3), new Lambertian(Vector3D(.7)))); // Back Left Middle Column
	ray_tracer->AddItem(new Box(Vector3D(10, 11, -20), Vector3D(12, 23, -18), new Lambertian(Vector3D(.7)))); // Back Right Middle Column
	ray_tracer->AddItem(new Box(Vector3D(-5.5, 11, -3), Vector3D(-4.5, 14, 19.5), new Lambertian(Vector3D(.4)))); // Outside Front Left Wall
	ray_tracer->AddItem(new Box(Vector3D(18.5, 11, -20), Vector3D(19.5, 14, -3.5), new Lambertian(Vector3D(.4)))); // Outside far right wall
	ray_tracer->AddItem(new Box(Vector3D(10.5, 19, -18), Vector3D(11.5, 23, -5), new Lambertian(Vector3D(.7)))); // Inside top far right wal
	ray_tracer->AddItem(new Box(Vector3D(-18, 19, -4.5), Vector3D(-5, 23, -3.5), new Lambertian(Vector3D(.7)))); // Inside Top Front Left walll
	ray_tracer->AddItem(new Box(Vector3D(-6, 11, -5), Vector3D(-4, 23, -3), new Lambertian(Vector3D(.7)))); // Front Center Middle Column
	ray_tracer->AddItem(new Box(Vector3D(-4, 11, -4.5), Vector3D(10, 14, -3.5), new Lambertian(Vector3D(.7)))); // Inside Bottom Front Right Wall
	ray_tracer->AddItem(new Box(Vector3D(-4, 19, -4.5), Vector3D(10, 23, -3.5), new Lambertian(Vector3D(.7)))); // Inside Top Front Right wall
	ray_tracer->AddItem(new Box(Vector3D(10, 11, -5), Vector3D(12, 23, -3), new Lambertian(Vector3D(.7)))); // Front Right Bottom Column
	ray_tracer->AddItem(new Box(Vector3D(12, 11, -4.5), Vector3D(19, 14, -3.5), new Lambertian(Vector3D(.4)))); // Outside Front Right Wall
	ray_tracer->AddItem(new Box(Vector3D(-19.5, 11, 18.5), Vector3D(-5.5, 14, 19.5), new Lambertian(Vector3D(.4)))); // Outide far left wall

	// Level 3

	ray_tracer->AddItem(new Box(Vector3D(-3, 0, -3), Vector3D(20, 1, 20), new Lambertian(Vector3D(0.615, 0.411, 0.015)))); // Deck
	ray_tracer->AddItem(new Box(Vector3D(2, 1, 2), Vector3D(17, 1.1, 17), new Lambertian(Vector3D(0.160, 0.250, 0.780)))); // Pool
	
	ray_tracer->AddItem(new Sphere(Vector3D(5, 1.4, 12), 1, new Lambertian(Vector3D(1, 0, 0))));
	ray_tracer->AddItem(new Sphere(Vector3D(12, 1.4, 7), 1.1, new Metal(Vector3D(1, .6, 0), 1.2)));
	ray_tracer->AddItem(new Sphere(Vector3D(15, 1.4, 15), 1.1, new Dielectric(1.5)));

	ray_tracer->Render("ray_tracer");
	ray_tracer->OpenImage();

	delete ray_tracer;
	
	return 0;
}
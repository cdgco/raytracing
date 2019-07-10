#ifndef RAYTRACERH
#define RAYTRACERH

#include "Sphere.h"
#include "ObjectList.h"
#include "camera.h"
#include "Material.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <cfloat>
#include <vector>
#define drand48() ((double)rand()/RAND_MAX)

std::vector<Vector3D> vArray;

// If ray hits sphere, change color based on position
Vector3D Color(const Ray& m_r, Object *world, int iDepth) {
	HitRecord rec;
	// If Ray hit Sphere
	if (world->Hit(m_r, 0.001, DBL_MAX, rec)) {
		Ray m_scattered;
		Vector3D m_attenuation;

		if (iDepth < 50 && rec.mat_ptr->Scatter(m_r, rec, m_attenuation, m_scattered)) {
			return m_attenuation * Color(m_scattered, world, iDepth + 1);
		}
		else {
			return Vector3D(0, 0, 0);
		}
	}
	else {
		Vector3D m_unit_direction = UnitVector(m_r.Direction());
		double dT = 0.5*(m_unit_direction.y() + 1.0);
		return (1.0 - dT)*Vector3D(1.0, 1.0, 1.0) + dT * Vector3D(0.5, 0.7, 1.0);
	}
}

class RayTracer {
public:
	RayTracer() {}
	// Initiate new Ray Tracer
	// iWidth: Output Image Width in Pixels
	// iHeight: Output Image Height in Pixels
	// iRaysPerPixels: How many color samples to take per pixel rendered
	RayTracer(const int iWidth, const int iHeight, const int iRaysPerPixel) {

		// Open Image File
		std::ofstream testFile("test.ppm");
		if (testFile.is_open()) {

			testFile << "P3\n" << iWidth << " " << iHeight << "\n255\n";

			// Create list of Spheres
			Object *list[5];
			// Fill list with two spheres
			list[0] = new Sphere(Vector3D(0, 0, -1), 0.5, new Lambertian(Vector3D(0.1, 0.2, 0.5)));
			list[1] = new Sphere(Vector3D(0, -100.5, -1), 100, new Lambertian(Vector3D(0.8, 0.8, 0.0)));
			list[2] = new Sphere(Vector3D(1, 0, -1), 0.5, new Metal(Vector3D(0.8, 0.6, 0.2), 1.0));
			list[3] = new Sphere(Vector3D(-1, 0, -1), 0.5, new Dielectric(1.5));
			list[4] = new Sphere(Vector3D(-1, 0, -1), -0.49, new Dielectric(1.5));
			// Initiate world from object list
			Object *world = new ObjectList(list, 5);
			// Create new environment
			Camera m_cam;
			
			// For each pixel on y axis
			for (int j = iHeight - 1; j >= 0; j--) {
				// For each pixel on x axis
				for (int i = 0; i < iWidth; i++) {
					// Initialize pixel
					Vector3D m_col(0, 0, 0);
					// for each sample within the pixel
					for (int s = 0; s < iRaysPerPixel; s++) {
						// For each sample, randomize the location
						double u = double(i + drand48()) / double(iWidth);
						double v = double(j + drand48()) / double(iHeight);
						Ray m_r = m_cam.GetRay(u, v);
						
						// sum all samples taken within pixel
						m_col += Color(m_r, world, 0);
					}
					// Average the color from all samples
					m_col /= double(iRaysPerPixel);
					m_col = Vector3D(sqrt(m_col[0]), sqrt(m_col[1]), sqrt(m_col[2]));
					// Convert RGB values from 0 - 1 scale to 0 - 255 scale
					int iRed = int(255.99*m_col[0]);
					int iGreen = int(255.99*m_col[1]);
					int iBlue = int(255.99*m_col[2]);

					// Write RGB values to ppm file
					testFile << iRed << " " << iGreen << " " << iBlue << "\n";
				}
			}
			// Close & Launch Image File
			testFile.close();
			system("start test.ppm");
		}
	}
};
#endif // RAYTRACERH
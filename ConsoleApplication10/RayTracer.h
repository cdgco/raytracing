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
	// dims: Struct with output image dimensions 
	// iRaysPerPixels: How many color samples to take per pixel rendered
	// m_cam: Camera object
	// world: object world
	RayTracer(Dims &dims, const int iRaysPerPixel, Camera m_cam, Object *world) {

		int iWidth = dims.X;
		int iHeight = dims.Y;

		double dR = cos(M_PI / 4);

		// Open Image File
		std::ofstream testFile("test.ppm");
		if (testFile.is_open()) {

			testFile << "P3\n" << iWidth << " " << iHeight << "\n255\n";

			// For each pixel on y axis
			for (int j = iHeight - 1; j >= 0; j--) {
				// For each pixel on x axis
				for (int i = 0; i < iWidth; i++) {
					// Initialize pixel
					Vector3D m_col(0, 0, 0);
					// For each sample within the pixel
					for (int s = 0; s < iRaysPerPixel; s++) {
						// For each sample, choose a random location within the pixel
						double u = double(i + drand48()) / double(iWidth);
						double v = double(j + drand48()) / double(iHeight);
						Ray m_r = m_cam.GetRay(u, v);
						
						// sum all samples taken within pixel
						m_col += Color(m_r, world, 0);
					}
					// Average the color from all samples
					m_col /= double(iRaysPerPixel);
					// Create vector from averaged color samples, take square root to gamma correct color
					m_col = Vector3D(sqrt(m_col[0]), sqrt(m_col[1]), sqrt(m_col[2]));
					// Convert RGB values from 0 - 1 vector scale to 0 - 255 decimal color scale
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
#ifndef RAYTRACERH
#define RAYTRACERH

#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "Box.h"
#include <fstream>
#include <windows.h>
#include <cfloat>
#include <string>

// Enable (1) / Disable (0) Console Progress Bar 
// Slight Performance Drop When Enabled
#define PROGRESSBAR 1

#if PROGRESSBAR == 1
#include "ProgressBar.hpp"
#endif

class RayTracer {
public:
	RayTracer() {}

	// Calculate if rays hit objects in array
	bool VectorHit(vList &vector, const Ray& m_r, double tmin, double tmax, HitRecord& rec) {
		HitRecord temp_rec;
		bool bHitAnything = false;
		double dClosestSoFar = tmax;
		for (size_t i = 0; i < vector.size(); i++) {
			if (vector[i]->Hit(m_r, tmin, dClosestSoFar, temp_rec)) {
				bHitAnything = true;
				dClosestSoFar = temp_rec.dT;
				rec = temp_rec;
			}
		}
		return bHitAnything;
	}

	// If ray hits sphere, change color based on position
	Vector3D Color(const Ray& m_r, vList &vector, int iDepth) {
		HitRecord rec;

		if (VectorHit(vector, m_r, 0.001, DBL_MAX, rec)) {
			Ray m_scattered;
			Vector3D m_attenuation;
			if (iDepth < 50 && rec.mat_ptr->Scatter(m_r, rec, m_attenuation, m_scattered)) {
				return m_attenuation * Color(m_scattered, vector, iDepth + 1);
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



	// Initiate new Ray Tracer
	// dims: Struct with output image dimensions 
	// iRaysPerPixels: How many color samples to take per pixel rendered
	// m_cam: Camera object
	// vector: Vector Array of Objects
	// filename: name of output ppm file
	RayTracer(SDim &dims, const int iRaysPerPixel, Camera m_cam, vList &vector, std::string filename) {

		int iWidth = dims.iX;
		int iHeight = dims.iY;
		#if PROGRESSBAR == 1
		ProgressBar progressBar(iHeight, 70);
		#endif

		double dR = cos(M_PI / 4);

		// Open Image File
		std::ofstream image(filename + (std::string)".ppm");
		if (image.is_open()) {

			image << "P3\n" << iWidth << " " << iHeight << "\n255\n";

			// For each pixel on y axis
			for (int j = iHeight - 1; j >= 0; j--) {
				// For each pixel on x axis
				for (int i = 0; i < iWidth; i++) {
					//if (j == 50 && i == 100) {
					//	std::cout << "stop";
					//}
					// Initialize pixel
					Vector3D m_col(0, 0, 0);
					// For each sample within the pixel
					for (int s = 0; s < iRaysPerPixel; s++) {
						// For each sample, choose a random location within the pixel
						double u = double(i + drand48()) / double(iWidth);
						double v = double(j + drand48()) / double(iHeight);
						Ray m_r = m_cam.GetRay(u, v);
						
						// sum all samples taken within pixel
						m_col += Color(m_r, vector, 0);
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
					image << iRed << " " << iGreen << " " << iBlue << "\n";
				}
				#if PROGRESSBAR == 1
				++progressBar;
				progressBar.display();
				#endif
			}
			// Close & Launch Image File
			image.close();
			vector.clear();
			#if PROGRESSBAR == 1
			progressBar.done();
			#endif
			system(("start " + filename + ".ppm").c_str());
		}
	}
};
#endif // RAYTRACERH
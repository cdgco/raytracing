#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <string>
#include <algorithm>
#include <fstream>
#include <windows.h>
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "sphere.h"
#include "omp.h" // OpenMP used for performance tracking only

// Optional console progress bar to track render progress. Enable (1); Disable (2). Causes slight performance decrease; Disable for testing.
#define PROGRESSBAR 0

#if PROGRESSBAR == 1
#include "progressbar.hpp"
#endif

/**

\mainpage C++ Ray Tracer

C++ Ray Tracer based on Peter Shirley's Ray Tracing in One Weekend

![VWI Banner](https://raw.githubusercontent.com/cdgco/raytracing/master/Demo.jpg)

<hr>

### Overview
This project is a C++ Ray Tracer based off of Peter Shirley's ray tracer in the book Ray Tracing in One Weekend. 

The project has been adapted from floats to doubles with new class names, a box object (based off of ScratchAPixel's Ray Tracer) and a single class wrapper which handles the ray tracer from a single function.

*/

double time0, time1, kilaPixels; // Initialize Performance Variables

/** Ray Tracing wrapper; intialization and calculation functions. */
class RayTracer {
public:
	RayTracer() {}
	/** Initiate a ray tracer instance

	Example:

		RayTracer(dimensions, 100, cam, vecList, "ray_tracer");
	*/
	RayTracer(const SDim &dims, const int iRaysPerPixel, Camera cam, vList &vector, std::string strFileName) {

		#if PROGRESSBAR == 1
		ProgressBar progressBar(dims.iY, 70);
		#endif

		std::ofstream ofImage(strFileName + (std::string)".ppm"); // Open Image File
		if (ofImage.is_open()) {
			ofImage << "P3\n" << dims.iX << " " << dims.iY << "\n255\n"; // PPM Header with dimensions and color index
			time0 = omp_get_wtime(); // Start tracking performance
			for (int j = dims.iY - 1; j >= 0; j--) { // For each row of pixels (height)
				for (int i = 0; i < dims.iX; i++) { // For each pixel in row (width)

					Vector3D m_col(0, 0, 0); // Initialize pixel color
					for (int s = 0; s < iRaysPerPixel; s++) { // For each anti-aliasing sample
						double u = double(i + drand48()) / double(dims.iX); // Randomize location of ray within pixel (x)
						double v = double(j + drand48()) / double(dims.iY); // Randomize location of ray within pixel (y)
						Ray m_r = cam.GetRay(u, v); // Create ray from randomized location

						m_col += Color(m_r, vector, 0); // Sum all anti-aliased rays
					}
					m_col /= double(iRaysPerPixel); // Get average color from all samples taken (anti-aliasing)
					m_col = Vector3D(sqrt(m_col[0]), sqrt(m_col[1]), sqrt(m_col[2])); // Correct gamma of pixel

					// Convert pixel color values to 8-bit color depth (0-255)
					int iRed = int(255.99*m_col[0]); 
					int iGreen = int(255.99*m_col[1]);
					int iBlue = int(255.99*m_col[2]);

					// Write pixel to image
					ofImage << iRed << " " << iGreen << " " << iBlue << "\n";
				}
				#if PROGRESSBAR == 1
				++progressBar;
				progressBar.display();
				#endif
			}
			time1 = omp_get_wtime(); // Stop tracking performance
			kilaPixels = (dims.iX * dims.iY) / (time1 - time0) / 1000; // Calculate Performance
			printf("Dimensions\tNum Objects\tRays Per Pixel\tPerformance (KP/Sec)\tExecution Time (Sec)\n"); // Output Performance
			printf("%d x %d\t%d\t\t%d\t\t%8.3lf\t\t%8.3lf\n", dims.iX, dims.iY, vector.size(), iRaysPerPixel, kilaPixels, (time1 - time0));

			vector.erase(vector.begin(), vector.end()); // Remove elements from vector and invalidate iterators

			ofImage.close(); // Close image file
			#if PROGRESSBAR == 1
			progressBar.done();
			#endif
			//system(("start " + strFileName + ".ppm").c_str()); // Open image automatically after rendering
		}
	}
	/** Return Color Vector3D if ray intersects object.

	Example:

		Color(ray, vectorList, 0);
	*/
	Vector3D Color(const Ray &r, vList &vector, int iDepth) {
		HitRecord temp_rec, rec;
		bool bHitAnything = false;
		double dClosestSoFar = DBL_MAX;
		for (size_t i = 0; i < vector.size(); i++) {
			if (vector[i]->Hit(r, temp_rec, 0.001, dClosestSoFar)) {
				bHitAnything = true;
				dClosestSoFar = temp_rec.dT;
				rec = temp_rec;
			}
		}
		if (bHitAnything) {
			Ray m_scattered;
			Vector3D m_attenuation;
			if (iDepth < 50 && rec.pmCurMat->Scatter(r, rec, m_attenuation, m_scattered)) {
				return m_attenuation * Color(m_scattered, vector, iDepth + 1);
			}
			else {
				return Vector3D(0);
			}
		}
		else {
			Vector3D m_unit_direction = UnitVector(r.Direction());
			double dT = 0.5*(m_unit_direction.y() + 1.0);
			return (1.0 - dT)*Vector3D(1.0) + dT * Vector3D(0.5, 0.7, 1.0);
		}
	}
};
#endif // RAY_TRACER_H
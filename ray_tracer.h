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
#include "box.h"
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

/** Ray Tracing wrapper; intialization and calculation functions. */
class RayTracer {
public:
	void AddItem(Object* object) {
		m_list.push_back(object);
	}
	void ClearItems() {
		m_list.erase(m_list.begin(), m_list.end()); // Remove elements from vector and invalidate iterators
	}
	void SetCamera(const SDim &dims, Vector3D lookFrom, Vector3D lookAt = Vector3D(0), Vector3D viewUp = Vector3D(0, 1, 0), double aperture = 0.1, double Fov = 40) {
		m_camera = Camera(dims, lookFrom, lookAt, viewUp, aperture, Fov);
	}

	/** Initiate a ray tracer instance

	Example:

		RayTracer();

		RayTracer(dimensions, 100);

		RayTracer(dimensions, 100, cam);
	*/
	RayTracer();
	RayTracer(const SDim &dims, const int iRaysPerPixel) : m_Dims(dims), m_iRaysPerPixel(iRaysPerPixel) {}
	RayTracer(const SDim &dims, const int iRaysPerPixel, Camera &cam) : m_Dims(dims), m_iRaysPerPixel(iRaysPerPixel), m_camera(cam) {}
	~RayTracer() { ClearItems(); };
	virtual void Render(const std::string &strFileName);

	static Vector3D Color(const Ray &r, vList &vector, int iDepth) {
		HitRecord temp_rec, rec;
		bool bHitAnything = false;
		double dClosestSoFar = DBL_MAX;
		for (size_t i = 0; i < vector.size(); i++) {
			if (vector[i]->Hit(r, temp_rec, 0.001, dClosestSoFar)) {
				bHitAnything = true;
				dClosestSoFar = temp_rec.m_dT;
				rec = temp_rec;
			}
		}
		if (bHitAnything) {
			Ray m_scattered;
			Vector3D m_attenuation;
			if (iDepth < 50 && rec.m_pmCurMat->Scatter(r, rec, m_attenuation, m_scattered)) {
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
	const SDim m_Dims;
	vList m_list;
	Camera m_camera;
	int m_iRaysPerPixel;
};

void RayTracer::Render(const std::string &strFileName) {

	#if PROGRESSBAR == 1
	ProgressBar progressBar(dims.iY, 70);
	#endif

	/** Return Color Vector3D if ray intersects object.

	Example:

		Color(ray, vectorList, 0);
	*/

	double time0, time1, kilaPixels; // Initialize Performance Variables

	std::ofstream ofImage(strFileName + (std::string)".ppm"); // Open Image File
	if (ofImage.is_open()) {
		ofImage << "P3\n" << m_Dims.iX << " " << m_Dims.iY << "\n255\n"; // PPM Header with dimensions and color index
		time0 = omp_get_wtime(); // Start tracking performance
		for (int j = m_Dims.iY - 1; j >= 0; j--) { // For each row of pixels (height)
			for (int i = 0; i < m_Dims.iX; i++) { // For each pixel in row (width)

				Vector3D m_col(0, 0, 0); // Initialize pixel color
				for (int s = 0; s < m_iRaysPerPixel; s++) { // For each anti-aliasing sample
					double u = double(i + drand48()) / double(m_Dims.iX); // Randomize location of ray within pixel (x)
					double v = double(j + drand48()) / double(m_Dims.iY); // Randomize location of ray within pixel (y)
					Ray m_r = m_camera.GetRay(u, v); // Create ray from randomized location

					m_col += Color(m_r, m_list, 0); // Sum all anti-aliased rays
				}
				m_col /= double(m_iRaysPerPixel); // Get average color from all samples taken (anti-aliasing)
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
		kilaPixels = (m_Dims.iX * m_Dims.iY) / (time1 - time0) / 1000; // Calculate Performance
		printf("Dimensions\tNum Objects\tRays Per Pixel\tPerformance (KP/Sec)\tExecution Time (Sec)\n"); // Output Performance
		printf("%d x %d\t%d\t\t%d\t\t%8.3lf\t\t%8.3lf\n", m_Dims.iX, m_Dims.iY, m_list.size(), m_iRaysPerPixel, kilaPixels, (time1 - time0));

		ofImage.close(); // Close image file
		#if PROGRESSBAR == 1
		progressBar.done();
		#endif
		system(("start " + strFileName + ".ppm").c_str()); // Open image automatically after rendering
	}
}
#endif // RAY_TRACER_H
#include "ray_tracer.h"

/*! Link renderable objects to ray tracer instance
*
*	Example:
*
*		ray_tracer->AddItem(&Sphere(Vector3D(0), 1, new Lambertian(Vector3D(1)));
*/
void RayTracer::AddItem(Object* object) {
	m_list.push_back(object);
}
/*! Remove all objects from ray tracer instance vector list
*
*	Example:
*
*		ray_tracer->ClearItems();
*	or
*
*		delete ray_tracer;
*
*/
void RayTracer::ClearItems() {
	m_list.erase(m_list.begin(), m_list.end()); // Remove elements from vector and invalidate iterators
}
/*! Specify the desired perspective of the output image for the instance.
*	Only necessary if camera object not passed through inline initialization.
*
*	Example:
*
*		ray_tracer->SetCamera(Vector3D(10, 0, 0));
*
*/
void RayTracer::SetCamera(Vector3D lookFrom, Vector3D lookAt, Vector3D viewUp, double aperture, double Fov) {
	m_camera = Camera(m_dims, lookFrom, lookAt, viewUp, aperture, Fov);
}

/*! Return Color Vector3D if ray intersects object.
*
*	Example:
*
*		Color(ray, vectorList, 0);
*/
Vector3D RayTracer::Color(const Ray &r, int iDepth) {
	HitRecord temp_rec, rec;
	bool bHitAnything = false;
	double dClosestSoFar = DBL_MAX;
	for (size_t i = 0; i < m_list.size(); i++) {
		if (m_list[i]->Hit(r, temp_rec, 0.001, dClosestSoFar)) {
			bHitAnything = true;
			dClosestSoFar = temp_rec.m_dT;
			rec = temp_rec;
		}
	}
	if (bHitAnything) {
		Ray rScattered;
		Vector3D vAttenuation;
		if (iDepth < 50 && rec.m_pmCurMat->Scatter(r, rec, vAttenuation, rScattered)) {
			return vAttenuation * Color(rScattered, iDepth + 1);
		}
		else {
			return Vector3D(0);
		}
	}
	else {
		Vector3D vUnitDirection = UnitVector(r.Direction());
		double dT = 0.5*(vUnitDirection.y() + 1.0);
		return (1.0 - dT)*Vector3D(1.0) + dT * Vector3D(0.5, 0.7, 1.0);
	}
}

/*! Calculations and image output function for ray tracer instance.
*
*	Example:
*
*		ray_tracer->Render("image");
*/
void RayTracer::Render(const std::string &strFileName) {

	#if PROGRESSBAR == 1
	ProgressBar progressBar(dims.iY, 70);
	#endif

	double dStartTime, dEndTime, dKilaPixels; // Initialize Performance Variables

	std::ofstream ofImage(strFileName + (std::string)".ppm"); // Open Image File
	if (ofImage.is_open()) {
		ofImage << "P3\n" << m_dims.m_iX << " " << m_dims.m_iY << "\n255\n"; // PPM Header with dimensions and color index
		dStartTime = omp_get_wtime(); // Start tracking performance
		for (int j = m_dims.m_iY - 1; j >= 0; j--) { // For each row of pixels (height)
			for (int i = 0; i < m_dims.m_iX; i++) { // For each pixel in row (width)

				Vector3D col(0, 0, 0); // Initialize pixel color
				for (int s = 0; s < m_iRaysPerPixel; s++) { // For each anti-aliasing sample
					double u = double(i + drand48()) / double(m_dims.m_iX); // Randomize location of ray within pixel (x)
					double v = double(j + drand48()) / double(m_dims.m_iY); // Randomize location of ray within pixel (y)
					Ray m_r = m_camera.GetRay(u, v); // Create ray from randomized location

					col += Color(m_r, 0); // Sum all anti-aliased rays
				}
				col /= double(m_iRaysPerPixel); // Get average color from all samples taken (anti-aliasing)
				col = Vector3D(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); // Correct gamma of pixel

				// Convert pixel color values to 8-bit color depth (0-255) and write to file
				ofImage << int(255.99*col[0]) << " " << int(255.99*col[1]) << " " << int(255.99*col[2]) << "\n";
			}
			#if PROGRESSBAR == 1
			++progressBar;
			progressBar.display();
			#endif
		}
		dEndTime = omp_get_wtime(); // Stop tracking performance
		dKilaPixels = (m_dims.m_iX * m_dims.m_iY) / (dEndTime - dStartTime) / 1000; // Calculate Performance
		printf("Dimensions\tNum Objects\tRays Per Pixel\tPerformance (KP/Sec)\tExecution Time (Sec)\n"); // Output Performance
		printf("%d x %d\t%d\t\t%d\t\t%8.3lf\t\t%8.3lf\n", m_dims.m_iX, m_dims.m_iY, m_list.size(), m_iRaysPerPixel, dKilaPixels, (dEndTime - dStartTime));

		ofImage.close(); // Close image file
		#if PROGRESSBAR == 1
		progressBar.done();
		#endif
		system(("start " + strFileName + ".ppm").c_str()); // Open image automatically after rendering
	}
}
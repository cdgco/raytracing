#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <string>
#include <algorithm>
#include <fstream>
#include <windows.h>
#include "camera.h"
#include "material.h"
#include "sphere.h"

#define PROGRESSBAR 1

#if PROGRESSBAR == 1
#include "progressbar.hpp"
#endif

/**

\mainpage C++ Ray Tracer

C++ Ray Tracer based on Peter Shirley's Ray Tracing in One Weekend

![VWI Banner](https://raw.githubusercontent.com/cdgco/raytracing/master/BoxDemo.jpg)

<hr>

### Overview
This project is a C++ Ray Tracer based off of Peter Shirley's ray tracer in the book Ray Tracing in One Weekend. 

The project has been adapted from floats to doubles with new class names, a box object (based off of ScratchAPixel's Ray Tracer) and a single class wrapper which handles the ray tracer from a single function.


### Usage
The ray tracer function is contained in the RayTracer.h file.

To initialize a ray tracer, create a new Vector Array by calling: 
```
vArray VectorName
```

Then use the following format for each object in the render: 
```
NewItem(VectorName, &OBJECT)
```

Next, set the dimensions of the output image (Width x Height): 
```
Dim a = { 200, 100 }
```

Finally, call the ray tracer: 
```
RayTracer(DIMENSIONS, QUALITY, CAMERA, VectorName, FILENAME)
```

Example:
```
#include "RayTracer.h"

int main() {

	std::vector<Object*> r1;
	NewItem(r1, &Sphere(Vector3D(0, 0, 0), 0.5, new Lambertian(Vector3D(0.8, 0.3, 0.3))));
	Dim a = { 200, 100 };
	RayTracer(a, 30, Camera(a, Vector3D(3, 1, 3)), r1, "RayTracer");

	return 0;
}
```
<hr>

### Variable Descriptions

- `OBJECT`:
  - `Sphere`:
	- Description: Sphere Object
	- Format: `Sphere(Vector3D(LOCATION), SIZE, MATERIAL)`
	- Example: `Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0)))`
  - `Box`
	- Description: Box Object
	- Format: `Box(Vector3D(BOUND1), Vector3D(BOUND2), MATERIAL)`
	- Example: `Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(1, 0, 0)))`
  - `Sphere.BBox`
	- Description: Sphere Object with surrounding bounding Box
	- Format: `Sphere(Vector3D(LOCATION),SIZE, MATERIAL).BBox(VectorName)`
	- Example: `Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))).BBox(VectorName)`
- `LOCATION`: (X,Y,Z) location of object (double).
- `SIZE`: Sphere radius (double).
- `MATERIAL`:
  - `Lambertian`
	- Example: `new Lambertian(Vector3D(COLOR))`.
	- `COLOR`: (R,G,B) color value, 0.0 - 1.0 (Vector3D).
  - `Metal`
	- Example: `new Metal(Vector3D(COLOR), FUZZ)`.
	- `COLOR`: (R,G,B) color value, 0.0 - 1.0 (Vector3D).
	- `FUZZ`: Fuzz / Blur Level, 0.1 - 1.0 (double).
  - `Dialectric`:
	- Example: `new Dielectric(REFIDX)`.
	- `REFIDX`: Refractive Index. Recommended 1.0 - 2.4 (double).
- `DIMENSIONS`: Dims struct name (SDim).
- `QUALITY`: Number of rays per pixel (double).
- `CAMERA`
  - Format: Camera(DIMS, LOOKFROM, LOOKAT, UP, APERTURE, FOV)
  - Example: `Camera(dims, Vector3D(3, 1, 3), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 0.1, 30)`
  - `DIMS`: Dim struct name (SDim).
  - `LOOKFROM`: (X,Y,Z) location of camera (Vector3D).
  - `LOOKAT` (Optional): (X,Y,Z) location where camera is pointed (Vector3D).
  - `UP` (Optional): (X,Y,Z) location of camera y-shift (Vector3D).
  - `APERTURE` (Optional): Camera lens aperture (double).
  - `FOV` (Optional): Camera lens field of view in degrees (double).
- `FILENAME`: Name for ouput ppm file. Do not include `.ppm` (string).

Note: Vector3D can be intialized with one or three paramaters. If one paramater is supplied, all three will use that value. i.e: Vector3D(1) == Vector3D(1,1,1).

*/



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

		std::ofstream ofImage(strFileName + (std::string)".ppm");
		if (ofImage.is_open()) {
			ofImage << "P3\n" << dims.iX << " " << dims.iY << "\n255\n";

			for (int j = dims.iY - 1; j >= 0; j--) {
				for (int i = 0; i < dims.iX; i++) {

					Vector3D m_col(0, 0, 0);
					for (int s = 0; s < iRaysPerPixel; s++) {
						double u = double(i + drand48()) / double(dims.iX);
						double v = double(j + drand48()) / double(dims.iY);
						Ray m_r = cam.GetRay(u, v);

						m_col += Color(m_r, vector, 0);
					}
					m_col /= double(iRaysPerPixel);
					m_col = Vector3D(sqrt(m_col[0]), sqrt(m_col[1]), sqrt(m_col[2]));

					int iRed = int(255.99*m_col[0]);
					int iGreen = int(255.99*m_col[1]);
					int iBlue = int(255.99*m_col[2]);

					ofImage << iRed << " " << iGreen << " " << iBlue << "\n";
				}
				#if PROGRESSBAR == 1
				++progressBar;
				progressBar.display();
				#endif
			}
			ofImage.close();
			vector.erase(vector.begin(), vector.end());
			vector.clear();
			#if PROGRESSBAR == 1
			progressBar.done();
			#endif
			system(("start " + strFileName + ".ppm").c_str());
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
# C++ Ray Tracer
C++ Ray Tracer based on Peter Shirley's Ray Tracing in One Weekend

![Ray Tracer Banner](https://raw.githubusercontent.com/cdgco/raytracing/master/Demo.jpg)
<hr>

### Usage
The ray tracer function is contained in the RayTracer.h file.

To initialize a ray tracer, create a new Vector Array by calling: `vArray VectorName`.

Then use the following format for each object in the render: `NewItem(VectorName, &OBJECT)`.

Next, set the dimensions of the output image (Width x Height): `Dim a = { 200, 100 }`.
 
Finally, call the ray tracer: `RayTracer(DIMENSIONS, QUALITY, CAMERA, VectorName, FILENAME)`.
 
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
* `OBJECT`:
  * `Sphere`:
    * Description: Sphere Object
    * Format: `Sphere(Vector3D(LOCATION), SIZE, MATERIAL)`
    * Example: `Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0)))`
  * `Box`
    * Description: Box Object
    * Format: `Box(Vector3D(BOUND1), Vector3D(BOUND2), MATERIAL)`
    * Example: `Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(1, 0, 0)))`
  * `Sphere.BBox`
    * Description: Sphere Object with surrounding bounding Box
    * Format: `Sphere(Vector3D(LOCATION),SIZE, MATERIAL).BBox(VectorName)`
    * Example: `Sphere(Vector3D(0), 1, new Lambertian(Vector3D(0.51, 0.7, 1.0))).BBox(VectorName)`
* `LOCATION`: (X,Y,Z) location of object (double).
* `SIZE`: Sphere radius (double).
* `MATERIAL`:
  * `Lambertian`
    * Example: `new Lambertian(Vector3D(COLOR))`.
    * `COLOR`: (R,G,B) color value, 0.0 - 1.0 (Vector3D).
  * `Metal`
    * Example: `new Metal(Vector3D(COLOR), FUZZ)`.
    * `COLOR`: (R,G,B) color value, 0.0 - 1.0 (Vector3D).
    * `FUZZ`: Fuzz / Blur Level, 0.1 - 1.0 (double).
  * `Dialectric`: 
    * Example: `new Dielectric(REFIDX)`.
    * `REFIDX`: Refractive Index. Recommended 1.0 - 2.4 (double).
* `DIMENSIONS`: Dims struct name (Dims).
* `QUALITY`: Number of rays per pixel (double).
* `CAMERA`
  * Format: Camera(DIMS, LOOKFROM, LOOKAT, UP, APERTURE, FOV) 
  * Example: `Camera(dims, Vector3D(3, 1, 3), Vector3D(0, 0, 0), Vector3D(0, 1, 0), 0.1, 30)`
  * `DIMS`: Dims struct name (Dims).
  * `LOOKFROM`: (X,Y,Z) location of camera (Vector3D).
  * `LOOKAT` (Optional): (X,Y,Z) location where camera is pointed (Vector3D).
  * `UP` (Optional): (X,Y,Z) location of camera y-shift (Vector3D).
  * `APERTURE` (Optional): Camera lens aperture (double).
  * `FOV` (Optional): Camera lens field of view in degrees (double).
* `FILENAME`: Name for ouput ppm file. Do not include `.ppm` (string).

Note: Vector3D can be intialized with one or three paramaters. If one paramater is supplied, all three will use that value. i.e: Vector3D(1) == Vector3D(1,1,1).

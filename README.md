# C++ Ray Tracer
C++ Ray Tracer based on Peter Shirley's Ray Tracing in One Weekend

### Usage
The ray tracer function is contained in the RayTracer.h file.

To initialize a ray tracer, create a new Vector Array by calling: `vector<Object*> VectorArray`.

Then use the following format for each object in the render: `NewItem(VectorArray, &Sphere(Vector3D(LOCATION), SIZE, new MATERIAL))`.

Next, set the dimensions of the output image (Width x Height): `Dim a = { 200, 100 }`.
 
Finally, call the ray tracer: `RayTracer(DIMENSINOS, QUALITY, CAMERA, VectorArray)`.
 
Example:
```
#include "RayTracer.h"

int main() {

	std::vector<Object*> r1;
	NewItem(r1, &Sphere(Vector3D(0, 0, 0), 0.5, new Lambertian(Vector3D(0.8, 0.3, 0.3))));
	Dim a = { 200, 100 };
	RayTracer(a, 30, Camera(a, Vector3D(3, 1, 3)), r1);

	return 0;
}
```

Variable Descriptions:
* `LOCATION`: (X,Y,Z) location of object (double).
* `SIZE`: Sphere radius (double).
* `MATERIAL`:
  * `Lambertian`
    * Example: `new Lambertian(Vector3D(COLOR))`
    * `COLOR`: (R,G,B) color value, 0.0 - 1.0 (double).
  * `Metal`
    * Example: `new Metal(Vector3D(COLOR), FUZZ)`
    * `COLOR`: (R,G,B) color value, 0.0 - 1.0 (double).
    * `FUZZ`: Fuzz / Blur Level, 0.1 - 1.0 (double).
  * `Dialectric`: 
    * Example: ` new Dielectric(REFIDX)`
    * `REFIDX`: Refractive Index. Recommended 1.0 - 2.4 (double).

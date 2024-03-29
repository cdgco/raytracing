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

/*! \mainpage C++ Ray Tracer
*
*	C++ / OpenCL Ray Tracer based on Peter Shirley's Ray Tracing in One Weekend
*
*	![VWI Banner](https://raw.githubusercontent.com/cdgco/raytracing/master/Demo.png)
*/

/*! Ray Tracing wrapper; intialization and calculation functions. */
class RayTracer {
public:

	/*! Initiate a ray tracer instance
	*
	*	Example:
	*
	*		RayTracer();
	*/
	RayTracer();
	/*! Initiate a ray tracer instance
	*
	*	Example:
	*
	*		RayTracer(dimensions, 100);
	*/
	RayTracer(const SDim &dims, const int iRaysPerPixel = 100) : m_dims(dims), m_iRaysPerPixel(iRaysPerPixel) {}
	/*! Initiate a ray tracer instance
	*
	*	Example:
	*
	*		RayTracer(dimensions, 100, cam);
	*/
	RayTracer(const SDim &dims, const int iRaysPerPixel, Camera &cam) : m_dims(dims), m_iRaysPerPixel(iRaysPerPixel), m_camera(cam) {}

	virtual void SetCamera(Vector3D lookFrom, Vector3D lookAt = Vector3D(0, 10, 0), Vector3D viewUp = Vector3D(0, 1, 0), double aperture = .1, double Fov = 40);
	virtual void AddItem(Object *object);
	virtual void RandomScene();
	virtual void ShowPerformance();
	virtual void OpenImage();
	virtual Vector3D Color(const Ray &r, int iDepth);
	virtual void Render(const std::string &strFileName);
	virtual int clRender(const std::string &strFileName);
	virtual void ClearItems();
	~RayTracer() { ClearItems(); };

	vList m_list; //!< Vector list of objects to be rendered
	const SDim m_dims; //!< Struct containing the dimensions of the desired output image {x, y} (in pixels)
	int m_iRaysPerPixel; //!< Number of anti-aliasing samples to take per pixel
	Camera m_camera; //!< Camera object specifying the desired perspective of the output image

};
#endif // RAY_TRACER_H
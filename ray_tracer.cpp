#include "ray_tracer.h"
#include <memory>
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
	vList::iterator it;
	for (it = m_list.begin(); it != m_list.end(); ) {
		delete * it;
		it = m_list.erase(it);
	}
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
/*! Return Color Vector3D if ray intersects object.
*
*	Example:
*
*		Color(ray, vectorList, 0);
*/
Vector3D RayTracer::clColor(const Ray &r, int iDepth) {
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
	ProgressBar progressBar(m_dims.m_iY, 70);
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
		dKilaPixels = ((double)m_dims.m_iX * (double)m_dims.m_iY) / (dEndTime - dStartTime) / 1000; // Calculate Performance
		printf("Dimensions\tNum Objects\tRays Per Pixel\tPerformance (KP/Sec)\tExecution Time (Sec)\n"); // Output Performance
		printf("%d x %d\t%zu\t\t%d\t\t%8.3lf\t\t%8.3lf\n", m_dims.m_iX, m_dims.m_iY, m_list.size(), m_iRaysPerPixel, dKilaPixels, (dEndTime - dStartTime));

		ofImage.close(); // Close image file
		#if PROGRESSBAR == 1
		progressBar.done();
		#endif
		system(("start " + strFileName + ".ppm").c_str()); // Open image automatically after rendering
	}
}
/*! Calculations and image output function for ray tracer instance.
*
*	Example:
*
*		ray_tracer->Render("image");
*/
int RayTracer::clRender(const std::string &strFileName) {

	#ifndef NUM_ELEMENTS
	#define	NUM_ELEMENTS m_dims.m_iX * m_dims.m_iY
	#endif

	double dStartTime, dEndTime, dKilaPixels; // Initialize Performance Variables

	#if PROGRESSBAR == 1
	ProgressBar progressBar(m_dims.m_iY, 70);
	#endif

	const char * CL_FILE_NAME = { "kernel.cl" };
	void Wait(cl_command_queue);

	FILE *fp;
	errno_t err = fopen_s(&fp, CL_FILE_NAME, "rb");
	if (err != 0) {
		return 1;
	}

	cl_platform_id platform;
	cl_device_id device;

	cl_int status = clGetPlatformIDs(1, &platform, NULL);
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

	typedef struct _cl_tag_Ray {
		cl_double3 a;
		cl_double3 b;
	} sRay;

	typedef struct _cl_tag_SDim {
		int a;
		int b;
	} sSDim;

	typedef struct _cl_tag_Camera {
		cl_double3 lookFrom;
		cl_double3 lookAt;
		cl_double3 viewUp;
		double aperture;
		double Fov;
	} sCamera;

	typedef struct _cl_tag_Object {
		int type;
		cl_double3 center;
		double radius;
		cl_double3 bound1;
		cl_double3 bound2;
		int *pmCurmat;
	} sObject;

	cl_double3 *hA = new cl_double3[NUM_ELEMENTS]; // Output Color
	int *hB = new int[NUM_ELEMENTS]; // Dims X
	int *hC = new int[NUM_ELEMENTS]; // Dims Y
	double *hD = new double[NUM_ELEMENTS]; // Sample Size
	sCamera *hE = new sCamera[NUM_ELEMENTS]; // Camera
	double *hF = new double[NUM_ELEMENTS * 2]; // Random Numbers
	sObject *hG = new sObject[NUM_ELEMENTS]; // Deconstructed Object List
	int *hH = new int[NUM_ELEMENTS]; // Object List Size
	
	for (int i = 0; i < NUM_ELEMENTS; i++) {
		hB[i] = m_dims.m_iX;
		hC[i] = m_dims.m_iY;
		hD[i] = (double)m_iRaysPerPixel;
		hE[i].lookFrom.x = m_camera.m_vOrigin.x();
		hE[i].lookFrom.y = m_camera.m_vOrigin.y();
		hE[i].lookFrom.z = m_camera.m_vOrigin.z();
		hE[i].lookAt.x = m_camera.m_vLookAt.x();
		hE[i].lookAt.y = m_camera.m_vLookAt.y();
		hE[i].lookAt.z = m_camera.m_vLookAt.z();
		hE[i].viewUp.x = m_camera.m_vViewUp.x();
		hE[i].viewUp.y = m_camera.m_vViewUp.y();
		hE[i].viewUp.z = m_camera.m_vViewUp.z();
		hE[i].aperture = m_camera.m_dAperture;
		hE[i].Fov = m_camera.m_dFov;
		hH[i] = (int)m_list.size();
	}

	for (size_t i = 0; i < m_list.size(); i++) {
		hG[i].type = m_list[i]->clType();
		hG[i].center.x = m_list[i]->clCenter().x();
		hG[i].center.y = m_list[i]->clCenter().y();
		hG[i].center.z = m_list[i]->clCenter().z();
		hG[i].radius = m_list[i]->clRadius();
		hG[i].bound1.x = m_list[i]->clBound1().x();
		hG[i].bound1.y = m_list[i]->clBound1().y();
		hG[i].bound1.z = m_list[i]->clBound1().z();
		hG[i].bound2.x = m_list[i]->clBound2().x();
		hG[i].bound2.y = m_list[i]->clBound2().y();
		hG[i].bound2.z = m_list[i]->clBound2().z();
		hG[i].pmCurmat = reinterpret_cast<int *>(m_list[i]->clMatPtr());
	}

	for (int i = 0; i < NUM_ELEMENTS * 2; i++) {
		hF[i] = drand48();
	}
	
	size_t raySize = NUM_ELEMENTS * sizeof(cl_double3);
	size_t intSize = NUM_ELEMENTS * sizeof(int);
	size_t cameraSize = NUM_ELEMENTS * sizeof(sCamera);
	size_t doubleSize = NUM_ELEMENTS * sizeof(double);
	size_t objectSize = NUM_ELEMENTS * sizeof(sObject);

	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);

	cl_command_queue cmdQueue = clCreateCommandQueue(context, device, 0, &status);

	cl_mem dA = clCreateBuffer(context, CL_MEM_WRITE_ONLY, raySize, NULL, &status);
	cl_mem dB = clCreateBuffer(context, CL_MEM_READ_ONLY, intSize, NULL, &status);
	cl_mem dC = clCreateBuffer(context, CL_MEM_READ_ONLY, intSize, NULL, &status);
	cl_mem dD = clCreateBuffer(context, CL_MEM_READ_ONLY, doubleSize, NULL, &status);
	cl_mem dE = clCreateBuffer(context, CL_MEM_READ_ONLY, cameraSize, NULL, &status);
	cl_mem dF = clCreateBuffer(context, CL_MEM_READ_ONLY, doubleSize, NULL, &status);
	cl_mem dG = clCreateBuffer(context, CL_MEM_READ_ONLY, objectSize, NULL, &status);
	cl_mem dH = clCreateBuffer(context, CL_MEM_READ_ONLY, intSize, NULL, &status);

	status = clEnqueueWriteBuffer(cmdQueue, dA, CL_FALSE, 0, raySize, hA, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dB, CL_FALSE, 0, intSize, hB, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dC, CL_FALSE, 0, intSize, hC, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dD, CL_FALSE, 0, doubleSize, hD, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dE, CL_FALSE, 0, cameraSize, hE, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dF, CL_FALSE, 0, doubleSize, hF, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dG, CL_FALSE, 0, intSize, hG, 0, NULL, NULL);
	Wait(cmdQueue);

	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *clProgramText = new char[fileSize + 1];
	size_t n = fread(clProgramText, 1, fileSize, fp);
	clProgramText[fileSize] = '\0';
	fclose(fp);
	if (n != fileSize) {
		fprintf(stderr, "Expected to read %d bytes read from '%s' -- actually read %d.\n", fileSize, CL_FILE_NAME, n);
	}

	char *strings[1];
	strings[0] = clProgramText;
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)strings, NULL, &status);
	delete[] clProgramText;

	char *options = {};
	status = clBuildProgram(program, 1, &device, options, NULL, NULL);
	if (status != CL_SUCCESS) {
		size_t size;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
		cl_char *log = new cl_char[size];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, log, NULL);
		fprintf(stderr, "clBuildProgram failed:\n%s\n", log);
		delete[] log;
	}

	cl_kernel kernel = clCreateKernel(program, "Render", &status);

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &dA);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &dB);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &dC);
	status = clSetKernelArg(kernel, 3, sizeof(cl_mem), &dD);
	status = clSetKernelArg(kernel, 4, sizeof(cl_mem), &dE);
	status = clSetKernelArg(kernel, 5, sizeof(cl_mem), &dF);
	status = clSetKernelArg(kernel, 6, sizeof(cl_mem), &dG);
	status = clSetKernelArg(kernel, 7, sizeof(cl_mem), &dH);

	size_t globalWorkSize[3] = { size_t(NUM_ELEMENTS), 1, 1 };
	size_t localWorkSize[3] = { size_t(1),   1, 1 };

	Wait(cmdQueue);

	std::ofstream ofImage(strFileName + (std::string)".ppm"); // Open Image File
	if (ofImage.is_open()) {
		ofImage << "P3\n" << m_dims.m_iX << " " << m_dims.m_iY << "\n255\n"; // PPM Header with dimensions and color index
		dStartTime = omp_get_wtime(); // Start tracking performance
		status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
		Wait(cmdQueue);

		status = clEnqueueReadBuffer(cmdQueue, dA, CL_TRUE, 0, raySize, hA, 0, NULL, NULL);
		int pixelcount = 0;
		for (int j = m_dims.m_iY - 1; j >= 0; j--) { // For each row of pixels (height)
			for (int i = 0; i < m_dims.m_iX; i++) { // For each pixel in row (width)

				ofImage << int(255.99*hA[pixelcount].x) << " " << int(255.99*hA[pixelcount].y) << " " << int(255.99*hA[pixelcount].z) << "\n";
				pixelcount++;
			}
			#if PROGRESSBAR == 1
			++progressBar;
			progressBar.display();
			#endif
		}
		dEndTime = omp_get_wtime(); // Stop tracking performance
		dKilaPixels = ((double)m_dims.m_iX * (double)m_dims.m_iY) / (dEndTime - dStartTime) / 1000; // Calculate Performance
		printf("Dimensions\tNum Objects\tRays Per Pixel\tPerformance (KP/Sec)\tExecution Time (Sec)\n"); // Output Performance
		printf("%d x %d\t%zu\t\t%d\t\t%8.3lf\t\t%8.3lf\n", m_dims.m_iX, m_dims.m_iY, m_list.size(), m_iRaysPerPixel, dKilaPixels, (dEndTime - dStartTime));

		ofImage.close(); // Close image file
		#if PROGRESSBAR == 1
		progressBar.done();
		#endif
		system(("start " + strFileName + ".ppm").c_str()); // Open image automatically after rendering

		clReleaseKernel(kernel);
		clReleaseProgram(program);
		clReleaseCommandQueue(cmdQueue);
		clReleaseMemObject(dA);
		clReleaseMemObject(dB);
		clReleaseMemObject(dC);
		clReleaseMemObject(dD);
		clReleaseMemObject(dE);
		clReleaseMemObject(dF);
		clReleaseMemObject(dG);
		clReleaseMemObject(dH);

		delete[] hA;
		delete[] hB;
		delete[] hC;
		delete[] hD;
		delete[] hE;
		delete[] hF;
		delete[] hG;
		delete[] hH;
	}
}

void Wait(cl_command_queue queue) {
	cl_event wait;
	cl_int status;

	status = clEnqueueMarker(queue, &wait);
	status = clWaitForEvents(1, &wait);
}
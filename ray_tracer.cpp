#include "ray_tracer.h"
#include <memory>
#include <random>
/*! Specify the desired perspective of the output image for the instance.
*	Only necessary if camera object not passed through inline initialization.
*
*	Example:
*
*		ray_tracer->SetCamera(Vector3D(10, 0, 0));
*
*/
cl_double3 double3(const Vector3D &v2) {
	return { v2.m_dE[0], v2.m_dE[1], v2.m_dE[2] };
}
void RayTracer::SetCamera(Vector3D lookFrom, Vector3D lookAt, Vector3D viewUp, double aperture, double Fov) {
	//if (lookFrom.y() == 0) { lookFrom.m_dE[1] = 0.00000001; }
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
		//double3 target = (double3)((double3)rec.m_vP + (double3)rec.m_vNormal + (double3)random);
		//return 0.5*(double3)(Color((Ray) {rec.m_vP, target - rec.m_vP}, x, ObjLen, random));
		return 0.5*Vector3D(rec.m_vNormal.x() + 1, rec.m_vNormal.y() + 1, rec.m_vNormal.z() + 1);
	}
	/*
	if (bHitAnything) {
		Ray rScattered;
		Vector3D vAttenuation;
		if (iDepth < 50 && rec.m_pmCurMat->Scatter(r, rec, vAttenuation, rScattered)) {
			return vAttenuation * Color(rScattered, iDepth + 1);
		}
		else {
			return Vector3D(0);
		}
	}*/
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

		ofImage.close(); // Close image file
		#if PROGRESSBAR == 1
		progressBar.done();
		#endif
		dKilaPixels = ((double)m_dims.m_iX * (double)m_dims.m_iY) / (dEndTime - dStartTime) / 1000; // Calculate Performance
		printf("Dimensions\tNum Objects\tRays Per Pixel\tPerformance (KP/Sec)\tExecution Time (Sec)\n"); // Output Performance
		printf("%d x %d\t%zu\t\t%d\t\t%8.3lf\t\t%8.3lf\n", m_dims.m_iX, m_dims.m_iY, m_list.size(), m_iRaysPerPixel, dKilaPixels, (dEndTime - dStartTime));
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

	#define	NUM_ELEMENTS m_dims.m_iX * m_dims.m_iY

	double dStartTime, dEndTime, dKilaPixels; // Initialize Performance Variables

	const char * CL_FILE_NAME = { "kernel.cl" };
	void Wait(cl_command_queue);

	FILE *fp;
	errno_t err = fopen_s(&fp, CL_FILE_NAME, "rb");
	if (err != 0) { return 1; }

	cl_platform_id platform;
	cl_device_id device;

	cl_int status = clGetPlatformIDs(1, &platform, NULL);
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

	typedef struct _cl_tag_Ray {
		cl_double3 a;
		cl_double3 b;
	} sRay;
	typedef struct _cl_tag_SDim {
		cl_int a;
		cl_int b;
	} sSDim;
	typedef struct _cl_tag_Camera {
		cl_double3 lookFrom;
		cl_double3 lookAt;
		cl_double3 viewUp;
		cl_double aperture;
		cl_double Fov;
	} sCamera;
	typedef struct _cl_tag_sMaterial {
		cl_double3 m_vColor;
		cl_double m_dFuzz;
		cl_double m_dRefIdx;
		cl_int m_MType;
	} sMaterial;
	typedef struct _cl_tag_sObject {
		cl_double3 m_vCenter;
		cl_double3 m_vBound1;
		cl_double3 m_vBound2;
		cl_double m_dRadius;
		cl_bool m_iType;
	} sObject;

	cl_double4 *hA = new cl_double4[NUM_ELEMENTS]; // Output Color
	cl_int2 *hB = new cl_int2[1]; // Dimensions
	sCamera *hC = new sCamera[NUM_ELEMENTS]; // Camera
	cl_double *hD = new cl_double[NUM_ELEMENTS]; // Random Numbers
	sObject *hE = new sObject[NUM_ELEMENTS]; // Deconstructed Object List
	cl_int *hF = new cl_int[1]; // Object List Size
	cl_double3 *hG = new cl_double3[NUM_ELEMENTS]; // Object List Size
	sMaterial *hH = new sMaterial[NUM_ELEMENTS]; // Object List Size
	cl_double3 *hI = new cl_double3[NUM_ELEMENTS]; // Object List Size
	
	hB[0] = { m_dims.m_iX, m_dims.m_iY };
	hC[0].lookFrom = double3(m_camera.m_vOrigin);
	hC[0].lookAt = double3(m_camera.m_vLookAt);
	hC[0].viewUp = double3(m_camera.m_vViewUp);
	hC[0].aperture = m_camera.m_dAperture;
	hC[0].Fov = m_camera.m_dFov;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	for (int i = 0; i < NUM_ELEMENTS; i++) {
		hD[i] = dist(mt);
		Vector3D tempRand = Camera::RandomInUnitDisk();
		hI[i] = double3(tempRand);
	}
	hF[0] = m_list.size();
	for (int i = 0; i < int(m_list.size()); i++) {
		hE[i].m_vCenter = double3(m_list[i]->clCenter());
		hE[i].m_vBound1 = double3(m_list[i]->clBound1());
		hE[i].m_vBound2 = double3(m_list[i]->clBound2());
		hE[i].m_dRadius = double(m_list[i]->clRadius());
		// If Box
		if (m_list[i]->clType() == 1) {
			hE[i].m_iType = true;
		}
		// If Sphere
		else {
			hE[i].m_iType = false;
		}
		hH[i].m_vColor = double3(m_list[i]->clColor());
		hH[i].m_dFuzz = m_list[i]->clFuzz();
		hH[i].m_dRefIdx = m_list[i]->clRefIdx();
		hH[i].m_MType = m_list[i]->clMType();
	}
	for (int i = 0; i < m_iRaysPerPixel * 4; i++) {
		Vector3D tempRand = Material::RandomInUnitSphere();
		hG[i] = double3(tempRand);
	}

	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
	cl_command_queue cmdQueue = clCreateCommandQueue(context, device, 0, &status);

	cl_mem dA = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size_t(NUM_ELEMENTS * sizeof(cl_double4)), NULL, &status);
	cl_mem dB = clCreateBuffer(context, CL_MEM_READ_ONLY, size_t(sizeof(cl_int2)), NULL, &status);
	cl_mem dC = clCreateBuffer(context, CL_MEM_READ_ONLY, size_t(NUM_ELEMENTS * sizeof(sCamera)), NULL, &status);
	cl_mem dD = clCreateBuffer(context, CL_MEM_READ_ONLY, size_t(m_iRaysPerPixel * 4 * sizeof(cl_double)), NULL, &status);
	cl_mem dE = clCreateBuffer(context, CL_MEM_READ_ONLY, size_t(NUM_ELEMENTS * sizeof(sObject)), NULL, &status);
	cl_mem dF = clCreateBuffer(context, CL_MEM_READ_ONLY, size_t(sizeof(cl_int)), NULL, &status);
	cl_mem dG = clCreateBuffer(context, CL_MEM_READ_ONLY, size_t(NUM_ELEMENTS * sizeof(cl_double3)), NULL, &status);
	cl_mem dH = clCreateBuffer(context, CL_MEM_READ_ONLY, size_t(NUM_ELEMENTS * sizeof(sMaterial)), NULL, &status);
	cl_mem dI = clCreateBuffer(context, CL_MEM_READ_ONLY, size_t(NUM_ELEMENTS * sizeof(cl_double3)), NULL, &status);

	status = clEnqueueWriteBuffer(cmdQueue, dA, CL_FALSE, 0, size_t(NUM_ELEMENTS * sizeof(cl_double4)), hA, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dB, CL_FALSE, 0, size_t(sizeof(cl_int2)), hB, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dC, CL_FALSE, 0, size_t(NUM_ELEMENTS * sizeof(sCamera)), hC, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dD, CL_FALSE, 0, size_t(NUM_ELEMENTS * sizeof(cl_double)), hD, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dE, CL_FALSE, 0, size_t(NUM_ELEMENTS * sizeof(sObject)), hE, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dF, CL_FALSE, 0, size_t(sizeof(cl_int)), hF, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dG, CL_FALSE, 0, size_t(NUM_ELEMENTS * sizeof(cl_double3)), hG, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dH, CL_FALSE, 0, size_t(NUM_ELEMENTS * sizeof(sMaterial)), hH, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, dI, CL_FALSE, 0, size_t(NUM_ELEMENTS * sizeof(cl_double3)), hI, 0, NULL, NULL);

	Wait(cmdQueue);

	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *clProgramText = new char[fileSize + 1];
	size_t n = fread(clProgramText, 1, fileSize, fp);
	clProgramText[fileSize] = '\0';
	fclose(fp);

	char *strings[1];
	strings[0] = clProgramText;
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)strings, NULL, &status);
	delete[] clProgramText;

	char *options = {};
	status = clBuildProgram(program, 1, &device, options, NULL, NULL);


	char *str;

	size_t sstr;

	status = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, NULL, NULL, &sstr);

	str = (char*)malloc(sstr);

	status |= clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sstr, str, NULL);

	printf("\nBuild Log:\t %s \n", str);

	free(str);


	cl_kernel kernel = clCreateKernel(program, "Render", &status);

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &dA);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &dB);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &dC);
	status = clSetKernelArg(kernel, 3, sizeof(cl_mem), &dD);
	status = clSetKernelArg(kernel, 4, sizeof(cl_mem), &dE);
	status = clSetKernelArg(kernel, 5, sizeof(cl_mem), &dF);
	status = clSetKernelArg(kernel, 6, sizeof(cl_mem), &dG);
	status = clSetKernelArg(kernel, 7, sizeof(cl_mem), &dH);
	status = clSetKernelArg(kernel, 8, sizeof(cl_mem), &dI);
	
	size_t globalWorkSize[1] = { size_t(NUM_ELEMENTS) };
	size_t localWorkSize[1] = { size_t(m_iRaysPerPixel) };

	Wait(cmdQueue);

	std::ofstream ofImage(strFileName + (std::string)".ppm"); // Open Image File
	if (ofImage.is_open()) {
		ofImage << "P3\n" << m_dims.m_iX << " " << m_dims.m_iY << "\n255\n"; // PPM Header with dimensions and color index
		dStartTime = omp_get_wtime(); // Start tracking performance
		status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
		Wait(cmdQueue);

		status = clEnqueueReadBuffer(cmdQueue, dA, CL_TRUE, 0, size_t(NUM_ELEMENTS * sizeof(cl_double4)), hA, 0, NULL, NULL);

		// PRINTS IMAGE VERTICALLY MIRRORED
		for (int i = NUM_ELEMENTS - 1; i >= 0; i--) {

			double ix = 1 + (i / m_dims.m_iX);
			int curY = int(floor(ix));
			int curX = m_dims.m_iX - (i - ((curY - 1) * m_dims.m_iX)) - 1;

			int curInt = ((curY - 1) * m_dims.m_iX) + curX;
			ofImage << int(255.99*hA[curInt].x) << " " << int(255.99*hA[curInt].y) << " " << int(255.99*hA[curInt].z) << "\n";

			if (i > 10200 && i < 10300) {
				printf("Pixel: %lf\t Vector3D(%.2lf, %.2lf, %.2lf)\tRGB(%d, %d, %d)\n", hA[i].w, hA[i].x, hA[i].y, hA[i].z, int(255.99*hA[i].x), int(255.99*hA[i].y), int(255.99*hA[i].z));
			}

		}

		dEndTime = omp_get_wtime(); // Stop tracking performance

		ofImage.close(); // Close image file
		/*
		for (int i = 0; i < 50; i++) {
			printf("Pixel: %.2lf\t Vector3D(%.2lf, %.2lf, %.2lf)\tRGB(%d, %d, %d)\n", hA[i].w, hA[i].x, hA[i].y, hA[i].z, int(255.99*hA[i].x), int(255.99*hA[i].y), int(255.99*hA[i].z));
		}
		*/
		dKilaPixels = ((double)m_dims.m_iX * (double)m_dims.m_iY) / (dEndTime - dStartTime) / 1000; // Calculate Performance
		printf("\nDimensions\tNum Objects\tRays Per Pixel\tPerformance (KP/Sec)\tExecution Time (Sec)\n"); // Output Performance
		printf("%d x %d\t%zu\t\t%d\t\t%8.3lf\t\t%8.3lf\n", m_dims.m_iX, m_dims.m_iY, m_list.size(), m_iRaysPerPixel, dKilaPixels, (dEndTime - dStartTime));
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
		clReleaseMemObject(dI);

		delete[] hA, hB, hC, hD, hE, hF, hG, hH, hI;
	}
	return 0;
}

void Wait(cl_command_queue queue) {
	cl_event wait;
	cl_int status;

	status = clEnqueueMarker(queue, &wait);
	status = clWaitForEvents(1, &wait);
}
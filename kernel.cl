#pragma OPENCL EXTENSION cl_khr_fp64 : enable

kernel void Render(global double3 *dA, global const int *dB, global const int *dC, global const int *dD, global const struct Camera *dE, global const double *dF) {

	int curImgRay = get_global_id(0); // Current ray in image
	int raysPerPixel = get_local_size(0); // Number of rays in pixel
	int curPixRay = get_local_id(0); // Current Ray in pixel
	int dimsX = get_group_id(0); // Current X Pixel
	int dimsY = get_group_id(1); // Current Y Pixel

	for (int s = 0; s < dD; s++) { // For each anti-aliasing sample
		double u = (i + dF[curImgRay]) / dimsX; // Randomize location of ray within pixel (x)
		double v = (j + dF[1 + (2 * curImgRay)]) / dimsY; // Randomize location of ray within pixel (y)
		struct Ray m_r = dE.GetRay(u, v); // Create ray from randomized location

		dA += Color(m_r, 0); // Sum all anti-aliased rays
	}
	dA /= (dA.x / dD, dA.y / dD, dA.z / dD); // Get average color from all samples taken (anti-aliasing)
	dA = double3(sqrt(dA[0]), sqrt(dA[1]), sqrt(dA[2])); // Correct gamma of pixel
}


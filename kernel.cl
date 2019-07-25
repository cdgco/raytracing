#pragma OPENCL EXTENSION cl_khr_fp64 : enable

typedef struct tag_Ray {
	double3 a;
	double3 b;
} Ray;

typedef struct tag_Camera {
	double3 lookFrom;
	double3 lookAt;
	double3 viewUp;
	double aperture;
	double Fov;
} Camera;

typedef struct tag_HitRecord {
	double m_dT; //!< Position along ray where intersection takes place.
	double3 m_vP; //!< Vector3D location of intersection.
	double3 m_vNormal; //!< Vector3D normal to point of intersection.
	int *m_pmCurMat; //!< Pointer to Material that the specified pixel should render.
} HitRecord;

double Length(double3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

double3 UnitVector(double3 v) {
	return v / Length(v);
}

kernel void Render(global Ray *dA, global int *dB, global int *dC, global double *dD, global const Camera *dE, global const double *dF) {

	int curImgRay = get_global_id(0); // Current ray in image
	int raysPerPixel = get_local_size(0); // Number of rays in pixel
	int curPixRay = get_local_id(0); // Current Ray in pixel
	int dimsX = get_group_id(0); // Current X Pixel
	int dimsY = get_group_id(1); // Current Y Pixel

	double3 vP;
	while ((vP.x * vP.x) + (vP.y * vP.y) + (vP.z * vP.z) >= 1.0) {
		vP.x = 2.0*dF[curImgRay] - 1;
		vP.x = 2.0*dF[curImgRay * 2] - 1;
		vP.z = 0;
	}
	double dHalfHeight = tan(dE[curImgRay].Fov*3.14159265359 / 360);
	double dHalfWidth = (dB[curImgRay] / dC[curImgRay]) * dHalfHeight;
	double dFocusDist = Length((dE[curImgRay].lookFrom - dE[curImgRay].lookAt));
	double3 m_vW = UnitVector(dE[curImgRay].lookFrom - dE[curImgRay].lookAt);
	double3 m_viewUp = dE[curImgRay].viewUp;
	double3 tempcross = { (m_viewUp.y * m_vW.z - m_viewUp.z * m_vW.y), (-(m_viewUp.x * m_vW.z - m_viewUp.z * m_vW.x)), (m_viewUp.x * m_vW.y - m_viewUp.y * m_vW.x) };
	double3 m_vU = UnitVector(tempcross);
	double3 m_vV = { (m_vW.y * m_vU.z - m_vW.z * m_vU.y), (-(m_vW.x * m_vU.z - m_vW.z * m_vU.x)), (m_vW.x * m_vU.y - m_vW.y * m_vU.x) };
	double3 m_vLowerLeftCorner = dE[curImgRay].lookFrom - dHalfWidth * dFocusDist * m_vU - dHalfHeight * dFocusDist * m_vV - dFocusDist * m_vW;
	double3 m_vHorizontal = 2 * dHalfWidth*dFocusDist*m_vU;
	double3 m_vVertical = 2 * dHalfHeight*dFocusDist*m_vV;
	double3 vRD = (dE[curImgRay].aperture / 2) * vP;
	double3 vOffset = m_vU * vRD.x + m_vV * vRD.y;
	double3 vOrigin = dE[curImgRay].lookFrom;

	double u = (get_group_id(1) + dF[curImgRay]) / (double)dB[curImgRay]; // Randomize location of ray within pixel (x)
	double v = (get_group_id(0) + dF[curImgRay * 2]) / (double)dC[curImgRay]; // Randomize location of ray within pixel (y)

	Ray dX = { vOrigin + vOffset, m_vLowerLeftCorner + u * m_vHorizontal + v * m_vVertical - vOrigin - vOffset };
	dA[curImgRay] = dX;
}

double3 Color(const Ray r, vector m_list, int iDepth) {
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
	double3 vx;
	if (bHitAnything) {
		Ray rScattered;
		double3 vAttenuation;
		if (iDepth < 50 && rec.m_pmCurMat->Scatter(r, rec, vAttenuation, rScattered)) {
			return vAttenuation * Color(rScattered, iDepth + 1);
		}
		else {
			vx.xyz = 0;
		}
	}
	else {
		double3 vUnitDirection = UnitVector(r.a);
		double dT = 0.5*(vUnitDirection.y + 1.0);
		vx.x = (1.0 - dT)*1.0 + dT * 0.5;
		vx.y = (1.0 - dT)*1.0 + dT * 0.7;
		vx.z = (1.0 - dT)*1.0 + dT * 1.0;
	}
}


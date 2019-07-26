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
	double m_dT;
	double3 m_vP;
	double3 m_vNormal;
	int *m_pmCurMat;
} HitRecord;

typedef struct tag_Sphere {
	double3 m_vCenter;
	double m_dRadius;
	int *m_pmCurMat;
} Sphere;

typedef struct tag_Box {
	double3 m_vBounds[2];
	double m_vCenter;
	int *m_pmCurMat;
} Box;

double Length(double3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

double3 UnitVector(double3 v) {
	return v / Length(v);
}
double3 PointAtParameter(Ray r, double t) { return r.a + t * r.b; }

double Dot(const double3 v1, const double3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
double3 InvDir(const Ray r) {
	return 1 / r.b;
}

kernel void Render(global double3 *dA, global int *dB, global int *dC, global double *dD, global const Camera *dE, global const double *dF) {

	int gid = get_global_id(0); // Current ray in image
	int raysPerPixel = get_local_size(0); // Number of rays in pixel
	int curPixRay = get_local_id(0); // Current Ray in pixel
	int dimsX = get_group_id(0); // Current X Pixel
	int dimsY = get_group_id(1); // Current Y Pixel

	double3 vP;
	while ((vP.x * vP.x) + (vP.y * vP.y) + (vP.z * vP.z) >= 1.0) {
		vP.x = 2.0*dF[gid] - 1;
		vP.x = 2.0*dF[gid * 2] - 1;
		vP.z = 0;
	}
	double dHalfHeight = tan(dE[gid].Fov*3.14159265359 / 360);
	double dHalfWidth = (dB[gid] / dC[gid]) * dHalfHeight;
	double dFocusDist = Length((dE[gid].lookFrom - dE[gid].lookAt));
	double3 m_vW = UnitVector(dE[gid].lookFrom - dE[gid].lookAt);
	double3 m_viewUp = dE[gid].viewUp;
	double3 tempcross = { (m_viewUp.y * m_vW.z - m_viewUp.z * m_vW.y), (-(m_viewUp.x * m_vW.z - m_viewUp.z * m_vW.x)), (m_viewUp.x * m_vW.y - m_viewUp.y * m_vW.x) };
	double3 m_vU = UnitVector(tempcross);
	double3 m_vV = { (m_vW.y * m_vU.z - m_vW.z * m_vU.y), (-(m_vW.x * m_vU.z - m_vW.z * m_vU.x)), (m_vW.x * m_vU.y - m_vW.y * m_vU.x) };
	double3 m_vLowerLeftCorner = dE[gid].lookFrom - dHalfWidth * dFocusDist * m_vU - dHalfHeight * dFocusDist * m_vV - dFocusDist * m_vW;
	double3 m_vHorizontal = 2 * dHalfWidth*dFocusDist*m_vU;
	double3 m_vVertical = 2 * dHalfHeight*dFocusDist*m_vV;
	double3 vRD = (dE[gid].aperture / 2) * vP;
	double3 vOffset = m_vU * vRD.x + m_vV * vRD.y;
	double3 vOrigin = dE[gid].lookFrom;

	for (int j = dC[gid] - 1; j >= 0; j--) { // For each row of pixels (height)
		for (int i = 0; i < dB[gid]; i++) { // For each pixel in row (width)
			double3 col;
			col.xyz = 1;

			double u = i / dB[gid]; // Randomize location of ray within pixel (x)
			double v = j / dC[gid]; // Randomize location of ray within pixel (y)

			Ray dX = { vOrigin + vOffset, m_vLowerLeftCorner + u * m_vHorizontal + v * m_vVertical - vOrigin - vOffset };
			double3 p = PointAtParameter(dX, 2.0);
			col.x = sqrt(col.x); // Correct gamma of pixel
			col.y = sqrt(col.y); // Correct gamma of pixel
			col.z = sqrt(col.z); // Correct gamma of pixel
			dA[gid] = col;
		}
	}
}


/** double3 Color(const Ray r, vector m_list, int iDepth) {
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
*/

bool clSphereHit(const Sphere s, const Ray r, HitRecord rec, double tMin, double tMax) {

	double3 vOC = r.a - s.m_vCenter;
	double dA = Dot(r.a, r.a);
	double dB = Dot(vOC, r.a);
	double dC = Dot(vOC, vOC) - s.m_dRadius * s.m_dRadius;
	double dDiscriminant = dB * dB - dA * dC;

	if (dDiscriminant > 0) {
		double dT = (-dB - sqrt(dDiscriminant)) / dA;
		if (dT < tMax && dT > tMin) {
			rec.m_dT = dT;
			rec.m_vP = PointAtParameter(r, dT);
			rec.m_vNormal = (PointAtParameter(r, dT) - s.m_vCenter) / s.m_dRadius;
			rec.m_pmCurMat = s.m_pmCurMat;
			return true;
		}
		dT = (-dB + sqrt(dDiscriminant)) / dA;
		if (dT < tMax && dT > tMin) {
			rec.m_dT = dT;
			rec.m_vP = PointAtParameter(r, dT);
			rec.m_vNormal = (PointAtParameter(r, dT) - s.m_vCenter) / s.m_dRadius;
			rec.m_pmCurMat = s.m_pmCurMat;
			return true;
		}
	}
	return false;
}

bool clBoxHit(const Box b, const Ray r, HitRecord rec, double tMin, double tMax) {

	int m_iSign[3];
	m_iSign[0] = (InvDir(r).x < 0);
	m_iSign[1] = (InvDir(r).y < 0);
	m_iSign[2] = (InvDir(r).z < 0);

	double tmin = (b.m_vBounds[m_iSign[0]].x - r.a.x) * InvDir(r).x;
	double tmax = (b.m_vBounds[1 - m_iSign[0]].x - r.a.x) * InvDir(r).x;
	double tymin = (b.m_vBounds[m_iSign[1]].y - r.a.y) * InvDir(r).y;
	double tymax = (b.m_vBounds[1 - m_iSign[1]].y - r.a.y) * InvDir(r).y;
	double tzmin = (b.m_vBounds[m_iSign[2]].z - r.a.z) * InvDir(r).z;
	double tzmax = (b.m_vBounds[1 - m_iSign[2]].z - r.a.z) * InvDir(r).z;

	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	double dT = tmin;
	if (dT < 0) { dT = tmax; if (dT < 0) return false; }

	//rec = { dT, r.PointAtParameter(dT), (r.PointAtParameter(dT) - m_vCenter), m_pmCurMat }; // Renders darkened color if camera off axis, if on axis, renders black
	rec = { dT, r.PointAtParameter(rec.m_dT), BoxNormal(r.PointAtParameter(rec.m_dT)), m_pmCurMat }; // Renders color if camera off axis and no objects behind, if on axis, renders black
	return true;
}
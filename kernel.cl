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

typedef struct tag_Object {
	int type;
	double3 m_vCenter;
	double m_dRadius;
	double3 m_vBound1;
	double3 m_vBound2;
	int *m_pmCurMat;
} Object;

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

bool clHit(const Object x, const Ray r, HitRecord rec, double tMin, double tMax) {
	if (x.type == 0) {

		double3 vOC = r.a - x.m_vCenter;
		double dA = Dot(r.a, r.a);
		double dB = Dot(vOC, r.a);
		double dC = Dot(vOC, vOC) - x.m_dRadius * x.m_dRadius;
		double dDiscriminant = dB * dB - dA * dC;

		if (dDiscriminant > 0) {
			double dT = (-dB - sqrt(dDiscriminant)) / dA;
			if (dT < tMax && dT > tMin) {
				rec.m_dT = dT;
				rec.m_vP = PointAtParameter(r, dT);
				rec.m_vNormal = (PointAtParameter(r, dT) - x.m_vCenter) / x.m_dRadius;
				rec.m_pmCurMat = x.m_pmCurMat;
				return true;
			}
			dT = (-dB + sqrt(dDiscriminant)) / dA;
			if (dT < tMax && dT > tMin) {
				rec.m_dT = dT;
				rec.m_vP = PointAtParameter(r, dT);
				rec.m_vNormal = (PointAtParameter(r, dT) - x.m_vCenter) / x.m_dRadius;
				rec.m_pmCurMat = x.m_pmCurMat;
				return true;
			}
		}
		return false;
	}
	else if (x.type == 1) {

		double3 bounds[2];
		bounds[0] = x.m_vBound1;
		bounds[1] = x.m_vBound2;

		int m_iSign[3];
		m_iSign[0] = (InvDir(r).x < 0);
		m_iSign[1] = (InvDir(r).y < 0);
		m_iSign[2] = (InvDir(r).z < 0);

		double tmin = (bounds[m_iSign[0]].x - r.a.x) * InvDir(r).x;
		double tmax = (bounds[1 - m_iSign[0]].x - r.a.x) * InvDir(r).x;
		double tymin = (bounds[m_iSign[1]].y - r.a.y) * InvDir(r).y;
		double tymax = (bounds[1 - m_iSign[1]].y - r.a.y) * InvDir(r).y;
		double tzmin = (bounds[m_iSign[2]].z - r.a.z) * InvDir(r).z;
		double tzmax = (bounds[1 - m_iSign[2]].z - r.a.z) * InvDir(r).z;

		if ((tmin > tymax) || (tymin > tmax)) return false;
		if (tymin > tmin) tmin = tymin;
		if (tymax < tmax) tmax = tymax;

		if ((tmin > tzmax) || (tzmin > tmax)) return false;
		if (tzmin > tmin) tmin = tzmin;
		if (tzmax < tmax) tmax = tzmax;

		double dT = tmin;
		if (dT < 0) { dT = tmax; if (dT < 0) return false; }

		rec.m_dT = dT;
		rec.m_vP = PointAtParameter(r, dT);
		rec.m_vNormal = PointAtParameter(r, dT) - x.m_vCenter;
		rec.m_pmCurMat = x.m_pmCurMat;
		return true;
	}
}

kernel void Render(global double3 *dA, global int *dB, global int *dC, global double *dD, global const Camera *dE, global const double *dF, global Object *dG, global const int *dH) {

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


	HitRecord temp_rec, rec;
	bool bHitAnything = false;
	double dClosestSoFar = DBL_MAX;

	for (int j = dC[gid] - 1; j >= 0; j--) { // For each row of pixels (height)
		for (int i = 0; i < dB[gid]; i++) { // For each pixel in row (width)
			double3 col;
			col.xyz = .6;

			double u = i / dB[gid]; // Randomize location of ray within pixel (x)
			double v = j / dC[gid]; // Randomize location of ray within pixel (y)

			Ray dX = { vOrigin + vOffset, m_vLowerLeftCorner + u * m_vHorizontal + v * m_vVertical - vOrigin - vOffset };
			double3 p = PointAtParameter(dX, 2.0);

			for (int i = 0; i < dH[gid]; i++) {
				if (clHit(dG[gid], dX, temp_rec, 0.001, dClosestSoFar)) {
					bHitAnything = true;
					dClosestSoFar = temp_rec.m_dT;
					rec = temp_rec;
				}
			}

			col.x = sqrt(col.x); // Correct gamma of pixel
			col.y = sqrt(col.y); // Correct gamma of pixel
			col.z = sqrt(col.z); // Correct gamma of pixel
			dA[gid] = col;
		}
	}
}
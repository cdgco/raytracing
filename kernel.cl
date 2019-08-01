#pragma OPENCL EXTENSION cl_khr_fp64 : enable

// Define C++ Classes as OpenCL structs
typedef struct _cl_tag_Ray {
	double3 a;
	double3 b;
} Ray;
typedef struct _cl_tag_Camera {
	double3 lookFrom;
	double3 lookAt;
	double3 viewUp;
	double aperture;
	double Fov;
} Camera;
typedef struct _cl_tag_HitRecord {
	double m_dT;
	double3 m_vP;
	double3 m_vNormal;
	int *m_pmCurMat;
} HitRecord;
typedef struct _cl_tag_sMaterial {
	double3 m_vColor;
	double m_dFuzz;
	double m_dRefIdx;
	int m_MType;
} sMaterial;
typedef struct _cl_tag_Object {
	double3 m_vCenter;
	double3 m_vBound1;
	double3 m_vBound2;
	double m_dRadius;
	bool m_type;
} Object;

// Define math functions
double3 UnitVector(double3 v) {
	return v / length(v);
}
double3 PointAtParameter(Ray r, double t) { 
	return r.a + t * r.b; 
}
double3 InvDir(const Ray r) {
	return 1 / r.b;
} 
double3 NormalCalc(const double3 vP, const Object ob) {
	double3 m_vBounds[2];
	m_vBounds[0] = ob.m_vBound1;
	m_vBounds[1] = ob.m_vBound2;
	double3 m_vCenter = ob.m_vCenter;
	double3 result;
	// Intersects within front face
	if (vP.x > m_vCenter.x && (vP.x == m_vBounds[0].x || vP.x == m_vBounds[1].x)) {
		// Falls on top or bottom edge
		if ((vP.y == m_vBounds[0].y || vP.y == m_vBounds[1].y) && (vP.z != m_vBounds[0].z && vP.z != m_vBounds[1].z)) {
			// Falls on top edge
			if (vP.y > m_vCenter.y) {
				return (double3)(1, 1, 0) * vP;
			}
			// Falls on bottom edge
			else if (vP.y < m_vCenter.y) {
				return (double3)(1, 1, 0) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.z == m_vBounds[0].z || vP.z == m_vBounds[1].z) && (vP.y != m_vBounds[0].y && vP.y != m_vBounds[1].y)) {
			// Falls on right edge
			if (vP.z < m_vCenter.x) {
				return (double3)(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.z > m_vCenter.z) {
				return (double3)(1, 0, 1) * vP;
			}
		}
		// Top left corner
		else if (vP.z == m_vBounds[1].z && vP.y == m_vBounds[1].y) {
			return (double3)(1, 1, 1) * vP;
		}
		// Bottom left corner
		else if (vP.z == m_vBounds[0].z && vP.y == m_vBounds[1].y) {
			return (double3)(1, 1, 1) * vP;
		}
		// Top right corner
		else if (vP.z == m_vBounds[1].z && vP.y == m_vBounds[0].y) {
			return (double3)(1, 1, 1) * vP;
		}
		// Bottom right corner
		else if (vP.z == m_vBounds[0].z && vP.y == m_vBounds[0].y) {
			return (double3)(1, 1, 1) * vP;
		}
		// Falls within center of face
		else { return (double3)(1, 0, 0) * vP; }
	}
	// Intersects within back face
	else if (vP.x < m_vCenter.x && (vP.x == m_vBounds[0].x || vP.x == m_vBounds[1].x)) {
		// Falls on top or bottom edge
		if ((vP.y == m_vBounds[0].y || vP.y == m_vBounds[1].y) && (vP.z != m_vBounds[0].z && vP.z != m_vBounds[1].z)) {
			// Falls on top edge
			if (vP.y > m_vCenter.y) {
				return (double3)(1, 0, 1) * vP;
			}
			// Falls on bottom edge
			else if (vP.y < m_vCenter.y) {
				return (double3)(1, 0, 1) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.z == m_vBounds[0].z || vP.z == m_vBounds[1].z) && (vP.y != m_vBounds[0].y && vP.y != m_vBounds[1].y)) {
			// Falls on right edge
			if (vP.z < m_vCenter.z) {
				return (double3)(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.z > m_vCenter.z) {
				return (double3)(1, 0, 1) * vP;
			}
		}
		// Top left corner
		else if (vP.x == m_vBounds[1].x && vP.y == m_vBounds[1].y) {
			return (double3)(1, 1, 1) * vP;
		}
		// Bottom left corner
		else if (vP.x == m_vBounds[0].x && vP.y == m_vBounds[1].y) {
			return (double3)(1, 1, 1) * vP;
		}
		// Top right corner
		else if (vP.x == m_vBounds[1].x && vP.y == m_vBounds[0].y) {
			return (double3)(1, 1, 1) * vP;
		}
		// Bottom right corner
		else if (vP.x == m_vBounds[0].x && vP.y == m_vBounds[0].y) {
			return (double3)(1, 1, 1) * vP;
		}
		// Falls within center of face
		else { return (double3)(-1, 0, 0) * vP; }
	}
	// Intersects within left face
	else if (vP.z > m_vCenter.z && (vP.z == m_vBounds[0].z || vP.z == m_vBounds[1].z)) {
		// Falls on top or bottom edge
		if ((vP.y == m_vBounds[0].y || vP.y == m_vBounds[1].y) && (vP.x != m_vBounds[0].x && vP.x != m_vBounds[1].x)) {
			// Falls on top edge
			if (vP.y > m_vCenter.y) {
				return (double3)(0, 1, 1) * vP;
			}
			// Falls on bottom edge
			else if (vP.y < m_vCenter.y) {
				return (double3)(0, 1, 1) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.x == m_vBounds[0].x || vP.x == m_vBounds[1].x) && (vP.y != m_vBounds[0].y && vP.y != m_vBounds[1].y)) {
			// Falls on right edge
			if (vP.x > m_vCenter.x) {
				return (double3)(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.x < m_vCenter.x) {
				return (double3)(1, 0, 1) * vP;
			}
		}
		// Falls within center of face
		else { return (double3)(0, 0, 1) * vP; }
	}
	// Intersects within right face
	else if (vP.z < m_vCenter.z && (vP.z == m_vBounds[0].z || vP.z == m_vBounds[1].z)) {
		// Falls on top or bottom edge
		if ((vP.y == m_vBounds[0].y || vP.y == m_vBounds[1].y) && (vP.x != m_vBounds[0].x && vP.x != m_vBounds[1].x)) {
			// Falls on top edge
			if (vP.y > m_vCenter.y) {
				return (double3)(1, 1, 0) * vP;
			}
			// Falls on bottom edge
			else if (vP.y < m_vCenter.y) {
				return (double3)(1, 1, 0) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.x == m_vBounds[0].x || vP.x == m_vBounds[1].x) && (vP.y != m_vBounds[0].y && vP.y != m_vBounds[1].y)) {
			// Falls on right edge
			if (vP.x < m_vCenter.x) {
				return (double3)(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.x > m_vCenter.x) {
				return (double3)(1, 0, 1) * vP;
			}
		}
		// Falls within center of face
		else { return (double3)(0, 0, -1) * vP; }
	}
	// Intersects within top face
	else if (vP.y > m_vCenter.y && (vP.y == m_vBounds[0].y || vP.y == m_vBounds[1].y)) {
		// Falls on top or bottom edge
		if ((vP.x == m_vBounds[0].x || vP.x == m_vBounds[1].x) && (vP.z != m_vBounds[0].z && vP.z != m_vBounds[1].z)) {
			// Falls on top edge
			if (vP.x < m_vCenter.x) {
				return (double3)(1, 1, 0) * vP;
			}
			// Falls on bottom edge
			else if (vP.x > m_vCenter.x) {
				return (double3)(1, 1, 0) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.z == m_vBounds[0].z || vP.z == m_vBounds[1].z) && (vP.x != m_vBounds[0].x && vP.x != m_vBounds[1].x)) {
			// Falls on right edge
			if (vP.z < m_vCenter.z) {
				return (double3)(0, 1, 1) * vP;
			}
			// Falls on left edge
			else if (vP.z > m_vCenter.z) {
				return (double3)(0, 1, 1) * vP;
			}
		}
		// Falls within center of face
		else { return (double3)(0, 1, 0) * vP; }
	}
	// Intersects within bottom face
	else if (vP.y < m_vCenter.y && (vP.y == m_vBounds[0].y || vP.y == m_vBounds[1].y)) {
		// Falls on top or bottom edge
		if ((vP.x == m_vBounds[0].x || vP.x == m_vBounds[1].x) && (vP.z != m_vBounds[0].z && vP.z != m_vBounds[1].z)) {
			// Falls on top edge
			if (vP.x > m_vCenter.x) {
				return (double3)(1, 1, 0) * vP;
			}
			// Falls on bottom edge
			else if (vP.x < m_vCenter.x) {
				return (double3)(1, 1, 0) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.z == m_vBounds[0].z || vP.z == m_vBounds[1].z) && (vP.x != m_vBounds[0].x && vP.x != m_vBounds[1].x)) {
			// Falls on right edge
			if (vP.z < m_vCenter.z) {
				return (double3)(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.z > m_vCenter.z) {
				return (double3)(1, 0, 1) * vP;
			}
			// Falls within center of face
			else { return (double3)(0, -1, 0) * vP; }
		}

	}
	else {
		// Ray outside of bounds
	}
}
double drand48(ulong randoms, double gid) {
	ulong seed = randoms + gid;
	seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
	uint uimax = ~0;
	ulong result = (seed >> 16);
	return (double)result;
}

// Define external functions
bool Material(const int type) {
	// Lambertian
	if (type == 0) {

	}
	// Metal
	else if (type == 1) {

	}
	// Dialectric
	else if (type == 2) {

	}
}
bool hit(const Object x, const Ray r, double tMin, double tMax, HitRecord *rec) {
	if (!x.m_type) {

	double3 vOC = r.a - x.m_vCenter;
		double dA = dot(r.b, r.b);
		double dB = dot(vOC, r.b);
		double dC = dot(vOC, vOC) - (x.m_dRadius * x.m_dRadius);
		double dDiscriminant = (dB * dB) - (dA * dC);

		if (dDiscriminant > 0) {
			double dT = (-dB - sqrt(dDiscriminant)) / dA;
			if (dT < tMax && dT > tMin) {
				rec->m_dT = dT;
				rec->m_vP = PointAtParameter(r, dT);
				rec->m_vNormal = (PointAtParameter(r, dT) - x.m_vCenter) / x.m_dRadius;
				//rec->m_pmCurMat = x.m_pmCurMat;
				return true;
			}
			dT = (-dB + sqrt(dDiscriminant)) / dA;
			if (dT < tMax && dT > tMin) {
				rec->m_dT = dT;
				rec->m_vP = PointAtParameter(r, dT);
				rec->m_vNormal = (PointAtParameter(r, dT) - x.m_vCenter) / x.m_dRadius;
				//rec->m_pmCurMat = x.m_pmCurMat;
				return true;
			}
		}
		return false;
	}
	else if (x.m_type) {
			/*
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

		rec->m_dT = dT;
		rec->m_vP = PointAtParameter(r, dT);
		rec->m_vNormal = NormalCalc(PointAtParameter(r, dT), x);
		//rec->m_pmCurMat = x.m_pmCurMat;
		return true;
		*/
	}
}
bool worldHit(const Object *x, int ObjLen, const Ray r, HitRecord *rec) {
	HitRecord temp_rec;
	bool hitAnything = false;
	double closestSoFar = DBL_MAX;
	for (int i = 0; i < ObjLen; i++) {
		if (hit(x[i], r, 0.001, closestSoFar, &temp_rec)) {
			hitAnything = true;
			closestSoFar = temp_rec.m_dT;
			*rec = temp_rec;
		}
	}
	return hitAnything;
}
double3 Color(const Ray r, Object *x, const int ObjLen, const double3 random, sMaterial *mats) {
	HitRecord rec;
	if (worldHit(x, ObjLen, r, &rec)) {
		//double3 target = (double3)((double3)rec.m_vP + (double3)rec.m_vNormal + (double3)random);
		//return 0.5*(double3)(Color((Ray) {rec.m_vP, target - rec.m_vP}, x, ObjLen, random));
		return 0.5*(double3)(rec.m_vNormal.x + 1, rec.m_vNormal.y + 1, rec.m_vNormal.z + 1);
	}
	else {
		double3 unitDirection = UnitVector(r.b);
		double t1 = 0.5*(unitDirection.y + 1.0);
		return (1.0 - t1) + (t1 * (double3)(0.5, 0.7, 1.0));
	}
}
Ray getRay(double u, double v, double dimsX, double dimsY, double3 vP, Camera *cam) {
	double dHalfHeight = tan(cam->Fov*3.14159265359 / 360);
	double dHalfWidth = (dimsX / dimsY) * dHalfHeight;
	double dFocusDist = length((cam->lookFrom - cam->lookAt));
	double3 m_vW = UnitVector(cam->lookFrom - cam->lookAt);
	double3 m_viewUp = cam->viewUp;
	double3 m_vU = UnitVector(cross(m_viewUp, m_vW));
	double3 m_vV = cross(m_vW, m_vU);
	double3 m_vLowerLeftCorner = cam->lookFrom - dHalfWidth * dFocusDist * m_vU - dHalfHeight * dFocusDist * m_vV - dFocusDist * m_vW;
	double3 m_vHorizontal = 2 * dHalfWidth*dFocusDist*m_vU;
	double3 m_vVertical = 2 * dHalfHeight*dFocusDist*m_vV;
	double3 vRD = (cam->aperture / 2) * vP;
	double3 vOffset = m_vU * vRD.x + m_vV * vRD.y;
	double3 vOrigin = cam->lookFrom;

	return (Ray){ vOrigin, ((double3)(-2.0,-1.0,-1.0) + (u * (double3)(4.0,0.0,0.0)) + (v * (double3)(0.0,2.0,0.0))) };

}

kernel void Render(global double4 *pixel, global int2 *dims, global const Camera *cam, global const double *drand, global const Object *list, global const int *listLen, global const double3 *randomVector, global const sMaterial *materials) {

	int gid = get_global_id(0); // Current ray in image
	int lid = get_local_id(0); // Current Ray in pixel
	int raysPerPixel = get_local_size(0); // Number of rays in pixel
	int2 dim = dims[0]; // Image Dimensions
	int ObjLen = listLen[0]; // # objects in list
	
	double3 vP;
	while ((vP.x * vP.x) + (vP.y * vP.y) + (vP.z * vP.z) >= 1.0) {
		vP.x = 2.0*drand[gid] - 1;
		vP.x = 2.0*drand[gid * 2] - 1;
		vP.z = 0;
	}
	
	int j = floor((double)(1 + (gid / dim.x))); // Current Y
	int i = gid - ((j - 1) * dim.x); // Current X

	// Object list initialized from kernel struct, max objects in image defined by array size
	Object world[1024];
	sMaterial mats[1024];
	for (int i = 0; i < ObjLen; i++) {
		world[i] = list[i];
		mats[i] = materials[i];
	}

	double3 col = (double3)(0);
	for (int s = 0; s < raysPerPixel; s++) {
		double u = (double)(i + drand[lid + s]) / (double)dim.x;
		double v = (double)(j + drand[lid + (s*2)]) / (double)dim.y;
		Ray r = getRay(u, v, dim.x, dim.y, vP, &cam);
		col += Color(r, &world, ObjLen, randomVector[lid], &mats);
	}
	col = sqrt(col / raysPerPixel);

	pixel[gid] = (double4)(col, gid);
}
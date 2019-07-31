#pragma OPENCL EXTENSION cl_khr_fp64 : enable

// Define C++ Classes as OpenCL structs
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

// Define external functions
bool clHit(const Object x, const Ray r, HitRecord rec, double tMin, double tMax) {
	if (x.type == 0) {

		double3 vOC = r.a - x.m_vCenter;
		double dA = dot(r.a, r.a);
		double dB = dot(vOC, r.a);
		double dC = dot(vOC, vOC) - x.m_dRadius * x.m_dRadius;
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
		rec.m_vNormal = NormalCalc(PointAtParameter(r, dT), x);
		rec.m_pmCurMat = x.m_pmCurMat;
		return true;
	}
}
double hit_sphere(const double3 center, double radius, const Ray r) {
	double3 vOC = r.a - center;
	double dA = dot(r.b, r.b);
	double dB = 2.0 * dot(vOC, r.b);
	double dC = dot(vOC, vOC) - (radius * radius);
	double dDiscriminant = (dB * dB) - (4 * dA * dC);
	if (dDiscriminant < 0) {
		return -1.0;
	}
	else {
		return (-dB - sqrt(dDiscriminant)) / (2.0*dA);
	}
}
double3 Color(const Ray r) {
	double t = hit_sphere((double3)(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		double3 N = UnitVector(PointAtParameter(r, t) - (double3)(0, 0, -1));
		return  0.5*(N + 1.0);
	}
	double3 unitDirection = UnitVector(r.b);
	double t1 = 0.5*(unitDirection.y + 1.0);
	return (1.0 - t1) + (t1 * (double3)(0.5, 0.7, 1.0));
}

kernel void Render(global double4 *dA, global int *dB, global int *dC, global double *dD, global const Camera *dE, global const double *dF, global Object *dG, global const int *dH) {

	int gid = get_global_id(0); // Current ray in image
	int raysPerPixel = get_local_size(0); // Number of rays in pixel
	int curPixRay = get_local_id(0); // Current Ray in pixel
	int dimsX = dB[gid]; // Current X Pixel
	int dimsY = dC[gid]; // Current Y Pixel
	
	double3 vP;
	while ((vP.x * vP.x) + (vP.y * vP.y) + (vP.z * vP.z) >= 1.0) {
		vP.x = 2.0*dF[gid] - 1;
		vP.x = 2.0*dF[gid * 2] - 1;
		vP.z = 0;
	}
	
	double dHalfHeight = tan(dE[gid].Fov*3.14159265359 / 360);
	double dHalfWidth = (dimsX / dimsY) * dHalfHeight;
	double dFocusDist = length((dE[gid].lookFrom - dE[gid].lookAt));
	double3 m_vW = UnitVector(dE[gid].lookFrom - dE[gid].lookAt);
	double3 m_viewUp = dE[gid].viewUp;
	double3 m_vU = UnitVector(cross(m_viewUp, m_vW));
	double3 m_vV = cross(m_vW, m_vU);
	double3 m_vLowerLeftCorner = dE[gid].lookFrom - dHalfWidth * dFocusDist * m_vU - dHalfHeight * dFocusDist * m_vV - dFocusDist * m_vW;
	double3 m_vHorizontal = 2 * dHalfWidth*dFocusDist*m_vU;
	double3 m_vVertical = 2 * dHalfHeight*dFocusDist*m_vV;
	double3 vRD = (dE[gid].aperture / 2) * vP;
	double3 vOffset = m_vU * vRD.x + m_vV * vRD.y;
	double3 vOrigin = dE[gid].lookFrom;
	
	HitRecord temp_rec, rec;
	bool bHitAnything = false;
	double dClosestSoFar = DBL_MAX;
	
	double ix = 1 + (gid / dimsX);
	int j = floor(ix); // Current x
	int i = gid - ((j - 1) * dimsX); // Current Y
	
	double u = (double)i / (double)dimsX;
	double v = (double)j / (double)dimsY;

	Ray r = { (double3)(0), ((double3)(-2.0,-1.0,-1.0) + (u * (double3)(4.0,0.0,0.0)) + (v * (double3)(0.0,2.0,0.0))) };
	
	double3 col = Color(r);

	dA[gid] = (double4)(col.x, col.y, col.z, gid);
	
}
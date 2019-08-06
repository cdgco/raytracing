#pragma OPENCL EXTENSION cl_khr_fp64 : enable

// Define C++ Classes as OpenCL structs
typedef struct _cl_tag_sObject {
	double3 m_vCenter;
	double3 m_vBounds1;
	double3 m_vBounds2;
	double m_dRadius;
	bool m_type;
} sObject;
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
typedef struct _cl_tag_Material {
	double3 m_vColor;
	double m_dFuzz;
	double m_dRefIdx;
	int m_MType;
} Material;
typedef struct _cl_tag_HitRecord {
	double m_dT;
	double3 m_vP;
	double3 m_vNormal;
	Material m_curmat;
} HitRecord;


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
double3 NormalCalc(const double3 vP, const sObject ob) {

	double3 m_vBounds[2] = { ob.m_vBounds1, ob.m_vBounds2 };
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
double SquaredLength(double3 m_dE) {
	return m_dE.x * m_dE.x + m_dE.y * m_dE.y + m_dE.z * m_dE.z;
}
// Define external functions
double drand(uint2 random) {
	int gid = get_global_id(0);
	uint seed = random.x + gid;
	uint t = seed ^ (seed << 11);
	uint uimax = UINT_MAX;
	double result = (double)(random.y ^ (random.y >> 19) ^ (t ^ (t >> 8)));
	do {
		result = result / 10;
	} while (result > 1.0);
	return result;
}
double3 RandomInUnitSphere(uint2 random) {
	double3 m_vP;
	do {
		m_vP = (double3)(2.0*(double3)(drand(random), drand(random), drand(random)) - (double3)(1));
	} while (SquaredLength(m_vP) >= 1.0);
	return m_vP;
}
double3 RandomInUnitDisk(uint2 random) {
	double3 vP;
	do {
		vP = (double3)(2.0*(double3)(drand(random), drand(random), 0) - (double3)(1, 1, 0));
	} while (dot(vP, vP) >= 1.0);
	return vP;
}
double3 Reflect(const double3 v, const double3 n) {
	return (double3)(v - 2 * dot(v, n)*n);
}
bool Refract(const double3 v, const double3 n, double NiOverNt, double3 refracted) {
	double3 vUV = UnitVector(v);
	double dDT = dot(vUV, n);
	double dDiscriminant = 1.0 - (NiOverNt * NiOverNt * (1 - dDT * dDT));
	if (dDiscriminant > 0) {
		refracted = NiOverNt * (vUV - (n * dDT)) - (n * sqrt(dDiscriminant));
		return true;
	}
	else return false;
}
double schlick(double cosine, double ref_idx) {
	double d0 = (1 - ref_idx) / (1 + ref_idx);
	d0 = d0 * d0;
	return d0 + (1 - d0)*pow((1 - cosine), 5);
}
bool scatter(Material *mat, const Ray r_in, const HitRecord *rec, double3 *attenuation, Ray *scattered, const double3 rus) {
	int lid = get_local_id(0);
	int gid = get_global_id(0);
	// Lambertian
	if (mat->m_MType == 0) {
		double3 vTarget = rec->m_vP + rec->m_vNormal + rus;
		*scattered = (Ray) { rec->m_vP, vTarget - rec->m_vP };
		*attenuation = (double3)mat->m_vColor;
		return true;
	}
	// Metal
	else if (mat->m_MType == 1) {
		Ray scattered1;
		double fuzz;
		if (mat->m_dFuzz < 1) { fuzz = mat->m_dFuzz; }
		else { fuzz = 1; }
		double3 vReflected = Reflect(UnitVector(r_in.b), rec->m_vNormal);
		scattered1 = (Ray){ rec->m_vP, vReflected + fuzz * rus};
		*scattered = scattered1;
		*attenuation = mat->m_vColor;
		bool bScatter = (dot(scattered1.b, rec->m_vNormal) > 0);
		return bScatter;
	}
	// Dialectric
	else if (mat->m_MType == 2) {
		double3 vOutwardNormal, m_refracted;
		double dNiOverNt, dReflectProb, dCosine;
		double3 vReflected = Reflect(r_in.b, rec->m_vNormal);
		*attenuation = (double3)(1.0);
		if (dot(r_in.b, rec->m_vNormal) > 0) {
			vOutwardNormal = -rec->m_vNormal;
			dNiOverNt = mat->m_dRefIdx;
			dCosine = mat->m_dRefIdx * dot(r_in.b, rec->m_vNormal) / length(r_in.b);
		}
		else {
			vOutwardNormal = rec->m_vNormal;
			dNiOverNt = 1.0 / mat->m_dRefIdx;
			dCosine = -(dot(r_in.b, rec->m_vNormal)) / length(r_in.b);
		}
		if (Refract(r_in.b, vOutwardNormal, dNiOverNt, m_refracted)) {
			dReflectProb = schlick(dCosine, mat->m_dRefIdx);
		}
		else {
			dReflectProb = 1.0;
		}
		if (drand(gid) < dReflectProb) {
			*scattered = (Ray) { rec->m_vP, vReflected };
		}
		else {
			*scattered = (Ray) { rec->m_vP, m_refracted };
		}
		return true;
	}
}
bool hit(const sObject x, const Material m, const Ray r, double tMin, double tMax, HitRecord *rec) {
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
				rec->m_curmat = m;
				return true;
			}
			dT = (-dB + sqrt(dDiscriminant)) / dA;
			if (dT < tMax && dT > tMin) {
				rec->m_dT = dT;
				rec->m_vP = PointAtParameter(r, dT);
				rec->m_vNormal = (PointAtParameter(r, dT) - x.m_vCenter) / x.m_dRadius;
				rec->m_curmat = m;
				return true;
			}
		}
		return false;
	}
	else if (x.m_type) {

		double3 bounds[2] = { x.m_vBounds1, x.m_vBounds2 };

		int m_iSign[3];
		m_iSign[0] = (InvDir(r).x < 0);
		m_iSign[1] = (InvDir(r).y < 0);
		m_iSign[2] = (InvDir(r).z < 0);

		double tmin = ((bounds[m_iSign[0]].x - r.a.x) * InvDir(r).x);
		double tmax = ((bounds[1 - m_iSign[0]].x - r.a.x) * InvDir(r).x);
		double tymin = ((bounds[m_iSign[1]].y - r.a.y) * InvDir(r).y);
		double tymax = ((bounds[1 - m_iSign[1]].y - r.a.y) * InvDir(r).y);
		double tzmin = ((bounds[m_iSign[2]].z - r.a.z) * InvDir(r).z);
		double tzmax = ((bounds[1 - m_iSign[2]].z - r.a.z) * InvDir(r).z);

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
		rec->m_curmat = m;
		return true;

	}
}
bool worldHit(const sObject *x, const Material *m, int ObjLen, const Ray r, HitRecord *rec) {
	HitRecord temp_rec;
	bool hitAnything = false;
	double closestSoFar = DBL_MAX;
	for (int i = 0; i < ObjLen; i++) {
		if (hit(x[i], m[i], r, 0.001, closestSoFar, &temp_rec)) {
			hitAnything = true;
			closestSoFar = temp_rec.m_dT;
			*rec = temp_rec;
		}
	}
	return hitAnything;
}
double3 Color(const Ray *r, sObject *x, Material *m, const int ObjLen, int depth, const double3 rus) {
	int lid = get_local_id(0);
	int gid = get_global_id(0);
	HitRecord rec;
	if (worldHit(x, m, ObjLen, *r, &rec)) {
		Ray scattered;
		double3 attenuation;
		// OpenCL does not support recursive functions, so the ray tracer is limited to 1 ray and 1 shadow ray :(
		if (depth < 1 && scatter(&rec.m_curmat, *r, &rec, &attenuation, &scattered, rus)) {
			return (double3)attenuation*Color(&scattered, x, m, ObjLen, depth + 2, rus);
		}
		else {
			return (double3)(0);
		}
		
	}
	else {
		double3 unitDirection = UnitVector(r->b);
		double t1 = 0.5*(unitDirection.y + 1.0);
		return (1.0 - t1) + (t1 * (double3)(0.5, 0.7, 1.0));
	}
}
Ray getRay(double s, double t, int2 dims, Camera cam, double3 rud) {
	
	bool v = 0;

	double aspect = dims.x / dims.y;
	double theta = cam.Fov*M_PI / 180;
	double half_height = tan(theta / 2);
	double half_width = aspect * half_height;
	
	if (!v) {
		double3 lower_left_corner = (double3)(-half_width, -half_height, -1.0);
		double3 horizontal = (double3)(2 * half_width, 0.0, 0.0);
		double3 vertical = (double3)(0.0, 2 * half_height, 0.0);
		double3 vOrigin = (double3)(0);

		return (Ray) { (double3)vOrigin, (double3)((double3)lower_left_corner + (double3)(s * horizontal) + (double3)(t * vertical) - (double3)vOrigin) };
	}
	if (v) {
		double lens_radius = cam.aperture / 2;
		double3 vOrigin = cam.lookFrom;
		double dFocusDist = length(cam.lookFrom - cam.lookAt);
		double3 m_vW = UnitVector(cam.lookFrom - cam.lookAt);
		double3 m_vU = UnitVector(cross(cam.viewUp, m_vW));
		double3 m_vV = cross(m_vW, m_vU);
		double3 m_vLowerLeftCorner = vOrigin - (half_width * dFocusDist * m_vU) - (half_height * dFocusDist * m_vV) - (dFocusDist * m_vW);
		double3 m_vHorizontal = 2 * half_width*dFocusDist*m_vU;
		double3 m_vVertical = 2 * half_height*dFocusDist*m_vV;
		double3 rd = lens_radius * rud;
		double3 offset = ((m_vU + rd.x) + (m_vV + rd.y));

		return (Ray) { (double3)(vOrigin - offset), (double3)(m_vLowerLeftCorner + (s * m_vHorizontal) + (t * m_vVertical) - vOrigin - offset) };

	}
}

kernel void Render(global double4 *pixel, global int2 *dims, global const double16 *cam, global const double *drand48, global const double16 *list, global const int *listLen, global const double3 *randomInUnitSphere, global const double8 *materials, global const double3 *randomInUnitDisk, global const uint2 *randomseed) {

	int gid = get_global_id(0); // Current ray in image
	int lid = get_local_id(0); // Current Ray in pixel
	int raysPerPixel = get_local_size(0); // Number of rays in pixel
	int2 dim = dims[0]; // Image Dimensions
	int ObjLen = listLen[0]; // # objects in list
	int wgNum = get_group_id(0);
	
	int j = floor((double)(1 + (gid / dim.x))); // Current Y
	int i = gid - ((j - 1) * dim.x); // Current X

	// Object list initialized from kernel struct, max objects in image defined by array size
	sObject world[1024];
	Material mats[1024];
	Camera camx[0];
	for (int i = 0; i < ObjLen; i++) {
		world[i].m_vCenter = (double3){ list[i].s0, list[i].s1, list[i].s2 };
		world[i].m_vBounds1 = (double3){ list[i].s3, list[i].s4, list[i].s5 };
		world[i].m_vBounds2 = (double3){ list[i].s6, list[i].s7, list[i].s8 };
		world[i].m_dRadius = list[i].s9;
		world[i].m_type = (int)(list[i].sa);

		mats[i].m_vColor = (double3){ materials[i].s0, materials[i].s1, materials[i].s2 };
		mats[i].m_dFuzz = materials[i].s3;
		mats[i].m_dRefIdx = materials[i].s4;
		mats[i].m_MType = (int)(materials[i].s5);
	}
	camx[0].lookFrom = (double3) { cam[0].s0, cam[0].s1, cam[0].s2 };
	camx[0].lookAt = (double3) { cam[0].s3, cam[0].s4, cam[0].s5 };
	camx[0].viewUp = (double3) { cam[0].s6, cam[0].s7, cam[0].s8 };
	camx[0].aperture = cam[0].s9;
	camx[0].Fov = cam[0].sa;
	
	double3 col = (double3)(0);

	int randomid = (int)(floor((double)(gid / lid)));

	for (int s = 0; s < raysPerPixel; s++) {
		double u = (double)(i + (drand(randomseed[0] + s) * drand48[lid + s])) / (double)dim.x;
		double v = (double)(j + (drand(randomseed[0] + (s * 2)) * drand48[lid + (s *2)])) / (double)dim.y;
		Ray r = getRay(u, v, dim, camx[0], randomInUnitDisk[(int)(floor((double)(gid / lid)))]);
		col += Color(&r, &world, &mats, ObjLen, 0, randomInUnitSphere[(int)(floor((double)(gid / lid)))]);
	}
	col = sqrt(col / raysPerPixel);

	pixel[gid] = (double4)(randomInUnitSphere[gid], gid);
}
#include "box.h"

/*! Return boolean value if box is hit by specified ray.
*
*	Example:
*
*		Box::Hit(ray, hitrec, 0.001, DBL_MAX);
*/
bool Box::Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const {
	double tmin = (m_vBounds[r.m_iSign[0]].x() - r.Origin().x()) * r.m_vInvDir.x();
	double tmax = (m_vBounds[1 - r.m_iSign[0]].x() - r.Origin().x()) * r.m_vInvDir.x();
	double tymin = (m_vBounds[r.m_iSign[1]].y() - r.Origin().y()) * r.m_vInvDir.y();
	double tymax = (m_vBounds[1 - r.m_iSign[1]].y() - r.Origin().y()) * r.m_vInvDir.y();
	double tzmin = (m_vBounds[r.m_iSign[2]].z() - r.Origin().z()) * r.m_vInvDir.z();
	double tzmax = (m_vBounds[1 - r.m_iSign[2]].z() - r.Origin().z()) * r.m_vInvDir.z();

	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	double dT = tmin;
	if (dT < 0) { dT = tmax; if (dT < 0) return false; }

	rec = { dT, r.PointAtParameter(dT), NormalCalc(r.PointAtParameter(rec.m_dT)), m_pmCurMat };
	return true;
}
Vector3D Box::NormalCalc(const Vector3D vP) const {
	// Intersects within front face
	if (vP.x() > m_vCenter.x() && (vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x())) {
		// Falls on top or bottom edge
		if ((vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y()) && (vP.z() != m_vBounds[0].z() && vP.z() != m_vBounds[1].z())) {
			// Falls on top edge
			if (vP.y() > m_vCenter.y()) {
				return Vector3D(1, 1, 0) * vP;
			}
			// Falls on bottom edge
			else if (vP.y() < m_vCenter.y()) {
				return Vector3D(1, 1, 0) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z()) && (vP.y() != m_vBounds[0].y() && vP.y() != m_vBounds[1].y())) {
			// Falls on right edge
			if (vP.z() < m_vCenter.x()) {
				return Vector3D(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.z() > m_vCenter.z()) {
				return Vector3D(1, 0, 1) * vP;
			}
		}
		// Top left corner
		else if (vP.z() == m_vBounds[1].z() && vP.y() == m_vBounds[1].y()) {
			return Vector3D(1, 1, 1) * vP;
		}
		// Bottom left corner
		else if (vP.z() == m_vBounds[0].z() && vP.y() == m_vBounds[1].y()) {
			return Vector3D(1, 1, 1) * vP;
		}
		// Top right corner
		else if (vP.z() == m_vBounds[1].z() && vP.y() == m_vBounds[0].y()) {
			return Vector3D(1, 1, 1) * vP;
		}
		// Bottom right corner
		else if (vP.z() == m_vBounds[0].z() && vP.y() == m_vBounds[0].y()) {
			return Vector3D(1, 1, 1) * vP;
		}
		// Falls within center of face
		else { return Vector3D(1, 0, 0) * vP; }
	}
	// Intersects within back face
	else if (vP.x() < m_vCenter.x() && (vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x())) {
		// Falls on top or bottom edge
		if ((vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y()) && (vP.z() != m_vBounds[0].z() && vP.z() != m_vBounds[1].z())) {
			// Falls on top edge
			if (vP.y() > m_vCenter.y()) {
				return Vector3D(1, 0, 1) * vP;
			}
			// Falls on bottom edge
			else if (vP.y() < m_vCenter.y()) {
				return Vector3D(1, 0, 1) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z()) && (vP.y() != m_vBounds[0].y() && vP.y() != m_vBounds[1].y())) {
			// Falls on right edge
			if (vP.z() < m_vCenter.z()) {
				return Vector3D(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.z() > m_vCenter.z()) {
				return Vector3D(1, 0, 1) * vP;
			}
		}
		// Top left corner
		else if (vP.x() == m_vBounds[1].x() && vP.y() == m_vBounds[1].y()) {
			return Vector3D(1, 1, 1) * vP;
		}
		// Bottom left corner
		else if (vP.x() == m_vBounds[0].x() && vP.y() == m_vBounds[1].y()) {
			return Vector3D(1, 1, 1) * vP;
		}
		// Top right corner
		else if (vP.x() == m_vBounds[1].x() && vP.y() == m_vBounds[0].y()) {
			return Vector3D(1, 1, 1) * vP;
		}
		// Bottom right corner
		else if (vP.x() == m_vBounds[0].x() && vP.y() == m_vBounds[0].y()) {
			return Vector3D(1, 1, 1) * vP;
		}
		// Falls within center of face
		else { return Vector3D(-1, 0, 0) * vP; }
	}
	// Intersects within left face
	else if (vP.z() > m_vCenter.z() && (vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z())) {
		// Falls on top or bottom edge
		if ((vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y()) && (vP.x() != m_vBounds[0].x() && vP.x() != m_vBounds[1].x())) {
			// Falls on top edge
			if (vP.y() > m_vCenter.y()) {
				return Vector3D(0, 1, 1) * vP;
			}
			// Falls on bottom edge
			else if (vP.y() < m_vCenter.y()) {
				return Vector3D(0, 1, 1) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x()) && (vP.y() != m_vBounds[0].y() && vP.y() != m_vBounds[1].y())) {
			// Falls on right edge
			if (vP.x() > m_vCenter.x()) {
				return Vector3D(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.x() < m_vCenter.x()) {
				return Vector3D(1, 0, 1) * vP;
			}
		}
		// Falls within center of face
		else { return Vector3D(0, 0, 1) * vP; }
	}
	// Intersects within right face
	else if (vP.z() < m_vCenter.z() && (vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z())) {
		// Falls on top or bottom edge
		if ((vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y()) && (vP.x() != m_vBounds[0].x() && vP.x() != m_vBounds[1].x())) {
			// Falls on top edge
			if (vP.y() > m_vCenter.y()) {
				return Vector3D(1, 1, 0) * vP;
			}
			// Falls on bottom edge
			else if (vP.y() < m_vCenter.y()) {
				return Vector3D(1, 1, 0) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x()) && (vP.y() != m_vBounds[0].y() && vP.y() != m_vBounds[1].y())) {
			// Falls on right edge
			if (vP.x() < m_vCenter.x()) {
				return Vector3D(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.x() > m_vCenter.x()) {
				return Vector3D(1, 0, 1) * vP;
			}
		}
		// Falls within center of face
		else { return Vector3D(0, 0, -1) * vP; }
	}
	// Intersects within top face
	else if (vP.y() > m_vCenter.y() && (vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y())) {
		// Falls on top or bottom edge
		if ((vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x()) && (vP.z() != m_vBounds[0].z() && vP.z() != m_vBounds[1].z())) {
			// Falls on top edge
			if (vP.x() < m_vCenter.x()) {
				return Vector3D(1, 1, 0) * vP;
			}
			// Falls on bottom edge
			else if (vP.x() > m_vCenter.x()) {
				return Vector3D(1, 1, 0) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z()) && (vP.x() != m_vBounds[0].x() && vP.x() != m_vBounds[1].x())) {
			// Falls on right edge
			if (vP.z() < m_vCenter.z()) {
				return Vector3D(0, 1, 1) * vP;
			}
			// Falls on left edge
			else if (vP.z() > m_vCenter.z()) {
				return Vector3D(0, 1, 1) * vP;
			}
		}
		// Falls within center of face
		else { return Vector3D(0, 1, 0) * vP; }
	}
	// Intersects within bottom face
	else if (vP.y() < m_vCenter.y() && (vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y())) {
		// Falls on top or bottom edge
		if ((vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x()) && (vP.z() != m_vBounds[0].z() && vP.z() != m_vBounds[1].z())) {
			// Falls on top edge
			if (vP.x() > m_vCenter.x()) {
				return Vector3D(1, 1, 0) * vP;
			}
			// Falls on bottom edge
			else if (vP.x() < m_vCenter.x()) {
				return Vector3D(1, 1, 0) * vP;
			}
		}
		// Falls on left or right edge
		else if ((vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z()) && (vP.x() != m_vBounds[0].x() && vP.x() != m_vBounds[1].x())) {
			// Falls on right edge
			if (vP.z() < m_vCenter.z()) {
				return Vector3D(1, 0, 1) * vP;
			}
			// Falls on left edge
			else if (vP.z() > m_vCenter.z()) {
				return Vector3D(1, 0, 1) * vP;
			}
			// Falls within center of face
			else { return Vector3D(0, -1, 0) * vP; }
		}

	}
	else {
		// Ray outside of bounds
	}
}
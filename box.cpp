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

	rec = { dT, r.PointAtParameter(dT), (r.PointAtParameter(dT) - m_vCenter), m_pmCurMat }; // Renders darkened color if camera off axis, if on axis, renders black
	//rec = { dT, r.PointAtParameter(rec.m_dT), BoxNormal(r.PointAtParameter(rec.m_dT)), m_pmCurMat }; // Renders color if camera off axis and no objects behind, if on axis, renders black
	return true;
}
int Box::clType() const {
	return 1;
}
Vector3D Box::clCenter() const {
	return m_vCenter;
}
double Box::clRadius() const {
	return 0;
}
Vector3D Box::clBound1() const {
	return m_vBounds[0];
}
Vector3D Box::clBound2() const {
	return m_vBounds[1];
}
Material* Box::clMatPtr() const {
	return m_pmCurMat;
}
Vector3D Box::BoxNormal(Vector3D inter) const {
	static const Vector3D normals[] = {
		Vector3D(1,0,0),
		Vector3D(0,1,0),
		Vector3D(0,0,1)
	};
	const Vector3D interRelative = inter - m_vCenter;
	const double xyCoef = interRelative.y() / interRelative.x();
	const double zyCoef = interRelative.y() / interRelative.z();
	int sign = 1;
	const int coef = ((xyCoef <= 1 && xyCoef >= -1) ? 1 : ((zyCoef < 1 && zyCoef > -1) ? 2 : 0));
	if (interRelative.x() < 0 || interRelative.y() < 0 || interRelative.z() < 0) {
		sign = -1;
	}
	return normals[coef] * sign * interRelative;
}
Vector3D Box::NormalCalc(const Vector3D vP) const {
	// Intersects within front face
	if (vP.x() > m_vCenter.x() && (vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x())) {
		// Falls on top or bottom edge
		if ((vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y()) && (vP.z() != m_vBounds[0].z() && vP.z() != m_vBounds[1].z())) {
			// Falls on top edge
			if (vP.y() > m_vCenter.y()) {
				
			}
			// Falls on bottom edge
			else if (vP.y() < m_vCenter.y()) {
				
			}
		}
		// Falls on left or right edge
		else if ((vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z()) && (vP.y() != m_vBounds[0].y() && vP.y() != m_vBounds[1].y())) {
			// Falls on right edge
			if (vP.z() < m_vCenter.x()) {
				
			}
			// Falls on left edge
			else if (vP.z() > m_vCenter.z()) {
				
			}
		}
		// Top left corner
		else if (vP.z() == m_vBounds[1].z() && vP.y() == m_vBounds[1].y()) {
			
		}
		// Bottom left corner
		else if (vP.z() == m_vBounds[0].z() && vP.y() == m_vBounds[1].y()) {
			
		}
		// Top right corner
		else if (vP.z() == m_vBounds[1].z() && vP.y() == m_vBounds[0].y()) {
			
		}
		// Bottom right corner
		else if (vP.z() == m_vBounds[0].z() && vP.y() == m_vBounds[0].y()) {
			
		}
		// Falls within center of face
		else { return Vector3D(vP.x(), 0, 0); }
	}
	// Intersects within back face
	else if (vP.x() < m_vCenter.x() && (vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x())) {
		// Falls on top or bottom edge
		if ((vP.y() == m_vBounds[0 ].y() || vP.y() == m_vBounds[1].y()) && (vP.z() != m_vBounds[0].z() && vP.z() != m_vBounds[1].z())) {
			// Falls on top edge
			if (vP.y() > m_vCenter.y()) {
				
			}
			// Falls on bottom edge
			else if (vP.y() < m_vCenter.y()) {
				
			}
		}
		// Falls on left or right edge
		else if ((vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z()) && (vP.y() != m_vBounds[0].y() && vP.y() != m_vBounds[1].y())) {
			// Falls on right edge
			if (vP.z() < m_vCenter.z()) {
				
			}
			// Falls on left edge
			else if (vP.z() > m_vCenter.z()) {
				
			}
		}
		// Top left corner
		else if (vP.x() == m_vBounds[1].x() && vP.y() == m_vBounds[1].y()) {
			
		}
		// Bottom left corner
		else if (vP.x() == m_vBounds[0].x() && vP.y() == m_vBounds[1].y()) {
			
		}
		// Top right corner
		else if (vP.x() == m_vBounds[1].x() && vP.y() == m_vBounds[0].y()) {
			
		}
		// Bottom right corner
		else if (vP.x() == m_vBounds[0].x() && vP.y() == m_vBounds[0].y()) {
			
		}
		// Falls within center of face
		else { return Vector3D(vP.x(), 0, 0); }
	}
	// Intersects within left face
	else if (vP.z() > m_vCenter.z() && (vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z())) {
		// Falls on top or bottom edge
		if ((vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y()) && (vP.x() != m_vBounds[0].x() && vP.x() != m_vBounds[1].x())) {
			// Falls on top edge
			if (vP.y() > m_vCenter.y()) {
				
			}
			// Falls on bottom edge
			else if (vP.y() < m_vCenter.y()) {
				
			}
		}
		// Falls on left or right edge
		else if ((vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x()) && (vP.y() != m_vBounds[0].y() && vP.y() != m_vBounds[1].y())) {
			// Falls on right edge
			if (vP.x() > m_vCenter.x()) {
				
			}
			// Falls on left edge
			else if (vP.x() < m_vCenter.x()) {
				
			}
		}
		// Falls within center of face
		else { return Vector3D(0, 0, vP.z()); }
	}
	// Intersects within right face
	else if (vP.z() < m_vCenter.z() && (vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z())) {
	// Falls on top or bottom edge
		if ((vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y()) && (vP.x() != m_vBounds[0].x() && vP.x() != m_vBounds[1].x())) {
			// Falls on top edge
			if (vP.y() > m_vCenter.y()) {
				
			}
			// Falls on bottom edge
			else if (vP.y() < m_vCenter.y()) {
				
			}
		}
		// Falls on left or right edge
		else if ((vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x()) && (vP.y() != m_vBounds[0].y() && vP.y() != m_vBounds[1].y())) {
			// Falls on right edge
			if (vP.x() < m_vCenter.x()) {
				
			}
			// Falls on left edge
			else if (vP.x() > m_vCenter.x()) {
				
			}
		}
		// Falls within center of face
		else { return Vector3D(0, 0, vP.z()); }
	}
	// Intersects within top face
	else if (vP.y() > m_vCenter.y() && (vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y())) {
	// Falls on top or bottom edge
		if ((vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x()) && (vP.z() != m_vBounds[0].z() && vP.z() != m_vBounds[1].z())) {
			// Falls on top edge
			if (vP.x() < m_vCenter.x()) {
				
			}
			// Falls on bottom edge
			else if (vP.x() > m_vCenter.x()) {
				
			}
		}
		// Falls on left or right edge
		else if ((vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z()) && (vP.x() != m_vBounds[0].x() && vP.x() != m_vBounds[1].x())) {
			// Falls on right edge
			if (vP.z() < m_vCenter.z()) {
				
			}
			// Falls on left edge
			else if (vP.z() > m_vCenter.z()) {
				
			}
		}
		// Falls within center of face
		else { return Vector3D(0, vP.y(), 0); }
	}
	// Intersects within bottom face
	else if (vP.y() < m_vCenter.y() && (vP.y() == m_vBounds[0].y() || vP.y() == m_vBounds[1].y())) {
	// Falls on top or bottom edge
		if ((vP.x() == m_vBounds[0].x() || vP.x() == m_vBounds[1].x()) && (vP.z() != m_vBounds[0].z() && vP.z() != m_vBounds[1].z())) {
			// Falls on top edge
			if (vP.x() > m_vCenter.x()) {
				
			}
			// Falls on bottom edge
			else if (vP.x() < m_vCenter.x()) {
				
			}
		}
		// Falls on left or right edge
		else if ((vP.z() == m_vBounds[0].z() || vP.z() == m_vBounds[1].z()) && (vP.x() != m_vBounds[0].x() && vP.x() != m_vBounds[1].x())) {
			// Falls on right edge
			if (vP.z() < m_vCenter.z()) {

			}
			// Falls on left edge
			else if (vP.z() > m_vCenter.z()) {

		}
		// Falls within center of face
		else { return Vector3D(0, vP.y(), 0); }
	}

}
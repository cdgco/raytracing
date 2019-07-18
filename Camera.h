#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

struct SDim {
	int iX;
	int iY;
};

class Camera {
public:
	Camera() {}
	Camera(const SDim &dims, Vector3D lookFrom, Vector3D lookAt = Vector3D(0, 0, 0), Vector3D vertUp = Vector3D(0, 1, 0), double aperture = 0.1, double Fov = 40) : m_vOrigin(lookFrom) {

		dLensRadius = aperture / 2;
		double dHalfHeight = tan(Fov*M_PI / 360);
		double dHalfWidth = (dims.iX / dims.iY) * dHalfHeight;
		double dFocusDist = (lookFrom - lookAt).Length();
		m_vW = UnitVector(lookFrom - lookAt);
		m_vU = UnitVector(vertUp.Cross(m_vW));
		m_vV = m_vW.Cross(m_vU);
		m_vLowerLeftCorner = m_vOrigin - dHalfWidth * dFocusDist * m_vU - dHalfHeight * dFocusDist * m_vV - dFocusDist * m_vW;
		m_vHorizontal = 2 * dHalfWidth*dFocusDist*m_vU;
		m_vVertical = 2 * dHalfHeight*dFocusDist*m_vV;
	}
	Ray GetRay(double s, double t) {
		Vector3D m_vRD = dLensRadius * RandomInUnitDisk();
		Vector3D m_vOffset = m_vU * m_vRD.x() + m_vV * m_vRD.y();
		return Ray(m_vOrigin + m_vOffset, m_vLowerLeftCorner + s * m_vHorizontal + t * m_vVertical - m_vOrigin - m_vOffset); 
	}

	Vector3D RandomInUnitDisk() {
		Vector3D m_vP;
		do {
			m_vP = 2.0*Vector3D(drand48(), drand48(), 0) - Vector3D(1, 1, 0);
		} while (m_vP.Dot(m_vP) >= 1.0);
		return m_vP;
	}

	Vector3D m_vU, m_vV, m_vW, m_vLowerLeftCorner, m_vHorizontal, m_vVertical, m_vOrigin;
	double dLensRadius;
};
#endif // CAMERA_H
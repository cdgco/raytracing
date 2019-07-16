#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

#define drand48() ((double)rand()/RAND_MAX)

// Dimensions of output file 
// X: Width in pixels
// Y: Height in pixels
struct Dim {
	int X;
	int Y;
};

Vector3D RandomInUnitDisk() {
	Vector3D m_p;
	do {
		m_p = 2.0*Vector3D(drand48(), drand48(), 0) - Vector3D(1, 1, 0);
	} while (m_p.Dot(m_p) >= 1.0);
	return m_p;
}

// Perspective of output
//		Dims dims: dimensions of output image
//		Vector3D m_look_from: (X,Y,Z) coordinate camera position
//		Vector3D m_look_at: (X,Y,Z) coordinate where camera is looking
//		Vector3D m_v_up: (X,Y,Z) coordinate y axis
//		double dAperture: aperture
//		double dFocusDist: focus distance
//		double dVFOV: Field of View
class Camera {
public:
	Camera(Dim &dims, Vector3D m_look_from, Vector3D m_look_at = Vector3D(0, 0, -1), Vector3D m_v_up = Vector3D(0, 1, 0), double dAperture = 0.1, double dVFOV = 30) {

		dLensRadius = dAperture / 2;
		double dAspect = dims.X / dims.Y;
		double dTheta = dVFOV*M_PI/180;
		double dHalfHeight = tan(dTheta/2);
		double dHalfWidth = dAspect * dHalfHeight;
		double dFocusDist = (m_look_from - m_look_at).Length();
		m_origin = m_look_from;
		m_w = UnitVector(m_look_from - m_look_at);
		m_u = UnitVector(m_v_up.Cross(m_w));
		m_v = m_w.Cross(m_u);
		m_lower_left_corner = m_origin - dHalfWidth * dFocusDist * m_u - dHalfHeight * dFocusDist * m_v - dFocusDist * m_w;
		m_horizontal = 2 * dHalfWidth*dFocusDist*m_u;
		m_vertical = 2 * dHalfHeight*dFocusDist*m_v;
	}
	Ray GetRay(double s, double t) {
		Vector3D m_rd = dLensRadius * RandomInUnitDisk();
		Vector3D m_offset = m_u * m_rd.x() + m_v * m_rd.y();
		return Ray(m_origin + m_offset, m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - m_offset); 
	}
	Vector3D m_u, m_v, m_w, m_lower_left_corner, m_horizontal, m_vertical, m_origin;
	double dLensRadius;
};

#endif // CAMERAH
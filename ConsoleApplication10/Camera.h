#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

class Camera {
public:
	// Initialize Environment
	Camera() {
		m_lower_left_corner = Vector3D(-2.0, -1.0, -1.0);
		m_horizontal = Vector3D(4.0, 0.0, 0.0);
		m_vertical = Vector3D(0.0, 2.0, 0.0);
		m_origin = Vector3D(0.0, 0.0, 0.0);
	}
	Ray GetRay(double u, double v) {
		return Ray(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin); 
	}
	Vector3D m_lower_left_corner;
	Vector3D m_horizontal;
	Vector3D m_vertical;
	Vector3D m_origin;
};

#endif // CAMERAH
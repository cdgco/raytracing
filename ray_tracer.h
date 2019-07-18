#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cfloat>
#include <string>
#include <algorithm>
#include <fstream>
#include <windows.h>
#include "camera.h"
#include "material.h"
#include "sphere.h"

#define PROGRESSBAR 1

#if PROGRESSBAR == 1
#include "progressbar.hpp"
#endif

class RayTracer {
public:
	RayTracer() {}
	RayTracer(const SDim &dims, const int iRaysPerPixel, Camera cam, vList &vector, std::string strFileName) {

#if PROGRESSBAR == 1
		ProgressBar progressBar(dims.iY, 70);
#endif

		std::ofstream ofImage(strFileName + (std::string)".ppm");
		if (ofImage.is_open()) {
			ofImage << "P3\n" << dims.iX << " " << dims.iY << "\n255\n";

			for (int j = dims.iY - 1; j >= 0; j--) {
				for (int i = 0; i < dims.iX; i++) {

					Vector3D m_col(0, 0, 0);
					for (int s = 0; s < iRaysPerPixel; s++) {
						double u = double(i + drand48()) / double(dims.iX);
						double v = double(j + drand48()) / double(dims.iY);
						Ray m_r = cam.GetRay(u, v);

						m_col += Color(m_r, vector, 0);
					}
					m_col /= double(iRaysPerPixel);
					m_col = Vector3D(sqrt(m_col[0]), sqrt(m_col[1]), sqrt(m_col[2]));

					int iRed = int(255.99*m_col[0]);
					int iGreen = int(255.99*m_col[1]);
					int iBlue = int(255.99*m_col[2]);

					ofImage << iRed << " " << iGreen << " " << iBlue << "\n";
				}
#if PROGRESSBAR == 1
				++progressBar;
				progressBar.display();
#endif
			}
			ofImage.close();
			vector.erase(vector.begin(), vector.end());
			vector.clear();
#if PROGRESSBAR == 1
			progressBar.done();
#endif
			system(("start " + strFileName + ".ppm").c_str());
		}
	}

	Vector3D Color(const Ray& r, vList &vector, int iDepth) {
		HitRecord m_Rec;
		for (size_t i = 0; i < vector.size(); i++) {
			if (vector[i]->Hit(r, 0.001, DBL_MAX, m_Rec)) {
				Ray m_rScattered;
				Vector3D m_vAttenuation;
				if (iDepth < 50 && m_Rec.pmCurMat->Scatter(r, m_Rec, m_vAttenuation, m_rScattered)) {
					return m_vAttenuation * Color(m_rScattered, vector, iDepth + 1);
				}
				else return Vector3D(0);
			}
			else {
				double dT = 0.5*(r.UnitDirection.y() + 1.0);
				return (1.0 - dT)*Vector3D(1.0) + dT * Vector3D(0.5, 0.7, 1.0);
			}
		}
	}
};
#endif // RAYTRACER_H
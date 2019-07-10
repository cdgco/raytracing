#ifndef OBJECTLISTH
#define OBJECTLISTH

#include "Object.h"

class ObjectList : public Object {
public:
	ObjectList() {}
	ObjectList(Object **l, int n) { list = l; iListSize = n; }
	virtual bool Hit(const Ray& m_r, double tmin, double tmax, HitRecord& rec) const;
	Object **list;
	int iListSize;
};

bool ObjectList::Hit(const Ray& m_r, double tmin, double tmax, HitRecord& rec) const {
	HitRecord temp_rec;
	bool bHitAnything = false;
	double dClosestSoFar = tmax;
	for (int i = 0; i < iListSize; i++) {
		if (list[i]->Hit(m_r, tmin, dClosestSoFar, temp_rec)) {
			bHitAnything = true;
			dClosestSoFar = temp_rec.dT;
			rec = temp_rec;
		}
	}
	return bHitAnything;
}


#endif // OBJECTLISTH
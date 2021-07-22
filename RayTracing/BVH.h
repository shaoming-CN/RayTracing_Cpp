#pragma once
#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"

#include "Intersection.h"
#include "Hittable_list.h"

#include <algorithm>

inline bool box_compare(const shared_ptr<hittable>a, const shared_ptr<hittable>b, int axis) {
	aabb box_a, box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)) {
		std::cerr << "No bounding box in bvh_node constructor.\n";
	}

	return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_compare_x(const shared_ptr<hittable>a, const shared_ptr<hittable>b) {
	return box_compare(a, b, 0);
}

bool box_compare_y(const shared_ptr<hittable>a, const shared_ptr<hittable>b) {
	return box_compare(a, b, 1);
}

bool box_compare_z(const shared_ptr<hittable>a, const shared_ptr<hittable>b) {
	return box_compare(a, b, 2);
}

class bvh_node :public hittable {
public:
	shared_ptr<hittable>left;
	shared_ptr<hittable>right;
	aabb box;

public:
	bvh_node() {};
	bvh_node(hittable_list& list, double time0, double time1)
		:bvh_node(list.objects, 0, list.objects.size(), time0, time1) {};
	bvh_node(std::vector<shared_ptr<hittable>>& src_objects,
		size_t start, size_t end, double time0, double time1);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& output_box)const;
};

bvh_node::bvh_node(std::vector<shared_ptr<hittable>>& src_objects,
	size_t start, size_t end, double time0, double time1) {
	auto objects = src_objects;
	//1.选取一个轴分配
	int axis = random_int(0, 2);

	auto comparator = (axis == 0) ? box_compare_x :
					  (axis == 1) ? box_compare_y :
									box_compare_z;

	size_t object_span = end - start;

	if (object_span == 1) {
		left = right = objects[start];
	}
	else if (object_span == 2) {
		if (comparator(objects[start], objects[start + 1])) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else {
		//2.objects.size()>=3 sort排序图元
		std::sort(objects.begin(), objects.end(),
			comparator);

		//3.对半分，每个子树分到一半的物体--recursive
		auto mid = start + object_span / 2;
		left = make_shared<bvh_node>(objects, start, mid, time0, time1);
		right = make_shared<bvh_node>(objects, mid, end, time0, time1);
	}

	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) ||
		!right->bounding_box(time0, time1, box_right))
		std::cerr << "No bounding box in bvh_node constructor.\n";
	box = surrounding_box(box_left, box_right);
}

bool bvh_node::bounding_box(double t0, double t1, aabb& output_box)const {
	output_box = box;
	return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	//先判断光线是否和bvh节点中的aabb盒相交
	if (!box.hit(r, t_min, t_max))
		return false;

	bool hit_left = left->hit(r, t_min, t_max, rec);
	bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

	return hit_left || hit_right;
}

#endif // !BVH_H


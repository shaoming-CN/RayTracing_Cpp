#pragma once
#ifndef MOVINGSPHERE_H
#define MOVINGSPHERE_H

#include "rtweekend.h"
#include "Intersection.h"

class moving_sphere :public hittable {
public:
	point3 centre0, centre1;
	double radius;
	double time0, time1;
	
	shared_ptr<material>mat_ptr;
public:
	moving_sphere() {};
	moving_sphere(double t0, double t1, 
		          point3 cen0, point3 cen1, 
		          double r, 
		          shared_ptr<material>m)
		:time0(t0), time1(t1), centre0(cen0), centre1(cen1), radius(r), mat_ptr(m) {};

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, aabb& output_box)const override;

	point3 center(double time) const;

};

point3 moving_sphere::center(double time)const {
	return centre0 + ((time - time0) / (time1 - time0)) * (centre1 - centre0);
}

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center(r.time());
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	//record
	rec.t = root;
	rec.p = r.at(rec.t);

	auto outward_normal = (rec.p - center(r.time())) / radius;
	rec.set_face_normal(r, outward_normal);

	rec.mat_ptr = mat_ptr;

	return true;
}

bool moving_sphere::bounding_box(double t0, double t1, aabb& output_box)const {
	aabb box0(
		center(t0) - vec3(radius, radius, radius),
		center(t0) - vec3(radius, radius, radius)
	);

	aabb box1(
		center(t1) - vec3(radius, radius, radius),
		center(t1) + vec3(radius, radius, radius)
	);

	output_box = surrounding_box(box0, box1);
	return true;
}

#endif // !MOVINGSPHERE_H


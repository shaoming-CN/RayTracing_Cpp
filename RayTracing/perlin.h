#pragma once
#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

class perlin {
public:
	perlin() {
		//ranfloat = new double[point_cnt];
		ranvec = new vec3[point_cnt];
		for (int i = 0; i < point_cnt; i++) {
			//0-1
			//ranfloat[i] = random_double();
			ranvec[i] = unit_vector(vec3::random(-1, 1));
		}

		//�����С255��ÿ��Ԫ�ط�Χ0-255
		perm_x = perlin_generate_perm();
		perm_y = perlin_generate_perm();
		perm_z = perlin_generate_perm();
	}

	~perlin() {
		delete[]ranvec;
		delete[]perm_x;
		delete[]perm_y;
		delete[]perm_z;
	}

	double noise(const point3& p)const {
		//gradient
	    //auto u = p.x() - floor(p.x());
	    //auto v = p.y() - floor(p.y());
	    //auto w = p.z() - floor(p.z());

		//Hermit
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());

		//Hermit
		//u = u * u * (3 - 2 * u);
		//v = v * v * (3 - 2 * v);
		//w = w * w * (3 - 2 * w);

		auto i = static_cast<int>(floor(p.x()));
		auto j = static_cast<int>(floor(p.y()));
		auto k = static_cast<int>(floor(p.z()));
		vec3 c[2][2][2];

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = ranvec[
							perm_x[(i + di) & 255] ^
							perm_y[(j + dj) & 255] ^
							perm_z[(k + dk) & 255]
					];

		//c--�ݶ������Ͷ��㵽�����ľ����������
		//c--����ݶ�����
		return trilinear_interp(c, u, v, w);

		//return 0-1
		//return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
		//return trilinear_interp(c, u, v, w);
	}

	double turb(const point3& p, int depth = 7)const {
		auto accum = 0.0;
		auto temp_p = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; i++) {
			accum += weight * noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}
		return fabs(accum);
	}

private:
	static const int point_cnt = 256;
	//double* ranfloat;
	vec3* ranvec;
	int* perm_x;
	int* perm_y;
	int* perm_z;

	static int* perlin_generate_perm() {
		auto p = new int[point_cnt];

		for (int i = 0; i < perlin::point_cnt; i++) {
			p[i] = i;
		}
		permute(p, point_cnt);
		return p;
	}

	static void permute(int* p, int n) {
		for (int i = n - 1; 0 <= i; i--) {
			auto target = random_int(0, i);
			auto temp = p[i];
			p[i] = p[target];
			p[target] = temp;
		}
	}

	static double trilinear_interp(vec3 c[2][2][2], double u, double v, double w) {
		auto accum = 0.0;
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);

		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++) {
					vec3 weight_v(u - i, v - j, w - k);
					accum += (i * uu + (1 - i) * (1 - uu))
							  * (j * vv + (1 - j) * (1 - vv))
							  * (k * ww + (1 - k) * (1 - ww))
							  * dot(c[i][j][k], weight_v);
				}
		return accum;
	}
};

#endif // !PERLIN_H


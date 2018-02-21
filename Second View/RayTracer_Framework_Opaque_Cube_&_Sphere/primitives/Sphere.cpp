#include "sphere.h"
#include "../Imageio/Imageio.h"
#include <math.h>

Intersect_Cond	Sphere::intersection_check(const M3DVector3f start, const M3DVector3f dir, float & distance, M3DVector3f intersection_p)
{
	M3DVector3f ray_to_center;
	m3dSubtractVectors3(ray_to_center, _pos, start);
	float ray_center_length = m3dDotProduct(ray_to_center, ray_to_center);

	float closest_point = m3dDotProduct(ray_to_center, dir);
	if (closest_point < 0)
	{
		return _k_miss;
	}

	float halfCord2 = (_rad * _rad) - (ray_center_length - (closest_point * closest_point));
	if (halfCord2 < 0)
	{
		return _k_miss;
	}

	Intersect_Cond type;
	M3DVector3f tmp;
	m3dSubtractVectors3(tmp, start, _pos);
	float length = m3dDotProduct(tmp, tmp);
	if (length < _rad2)
	{
		type = _k_inside;
		distance = closest_point + sqrt(halfCord2);
	}
	else
	{
		type = _k_hit;
		distance = closest_point - sqrt(halfCord2);
	}

	M3DVector3f tmp_v;
	m3dCopyVector3(tmp_v, dir);
	m3dScaleVector3(tmp_v, distance);
	m3dAddVectors3(intersection_p, start, tmp_v);

	return type;
}

void	Sphere::shade(M3DVector3f view, M3DVector3f intersect_p, const Light & sp_light, M3DVector3f am_light, M3DVector3f color, bool shadow)
{
	float shininess_constant = 180;
	M3DVector3f n_vector, l_vector, h_vector, r_vector, light_Position, light_Color;
	m3dSubtractVectors3(n_vector, intersect_p, _pos);
	m3dNormalizeVector(n_vector);
	sp_light.get_light(light_Position, light_Color);

	m3dSubtractVectors3(l_vector, light_Position, intersect_p);
	m3dNormalizeVector(l_vector);

	float diffuse_coefficient = m3dDotProduct(l_vector, n_vector);
	diffuse_coefficient = (diffuse_coefficient < 0) ? 0 : diffuse_coefficient;

	m3dAddVectors3(h_vector, l_vector, view);
	m3dNormalizeVector(h_vector);
	float specular_coefficient = m3dDotProduct(n_vector, h_vector);
	specular_coefficient = (specular_coefficient < 0) ? 0 : pow(specular_coefficient, shininess_constant);
	for (int i = 0; i < 3; i++)
	{
		color[i] = (_kd * _color[i] * diffuse_coefficient) + (_ks * _color[i] * specular_coefficient) + (_ka * _color[i] * am_light[i]);
	}
	shadow = true;
}

void	Sphere::get_reflect_direct(const M3DVector3f direct, const M3DVector3f intersect_p, M3DVector3f reflect_direct)
{
	M3DVector3f normal;
	M3DVector3f n_direct;
	m3dCopyVector3(n_direct, direct);
	m3dScaleVector3(n_direct, -1);

	m3dSubtractVectors3(normal, intersect_p, _pos);
	m3dNormalizeVector(normal);
	m3dScaleVector3(normal, 2 * m3dDotProduct(n_direct, normal));
	m3dSubtractVectors3(reflect_direct, normal, n_direct);
	m3dNormalizeVector(reflect_direct);
}

bool	Sphere::get_refract_direct(const M3DVector3f direct, const M3DVector3f intersect_p, M3DVector3f refract_direct, float delta, bool is_in)
{
	M3DVector3f n_direct;
	m3dCopyVector3(n_direct, direct);
	m3dScaleVector3(n_direct, -1);

	float  Sc = (is_in) ? 1.0 / _delta : _delta / 1.0;

	M3DVector3f normal;
	m3dSubtractVectors3(normal, intersect_p, _pos);
	m3dNormalizeVector(normal);

	if (is_in)
	{
		m3dScaleVector3(normal, -1);
	}

	float prod_n_d = m3dDotProduct(normal, n_direct);
	float R = 1 - Sc*Sc*(1 - prod_n_d*prod_n_d);
	if (R >= 0)
	{
		m3dScaleVector3(normal, Sc * prod_n_d - sqrtf(R));

		M3DVector3f light;
		m3dCopyVector3(light, n_direct);
		m3dScaleVector3(light, Sc);

		m3dSubtractVectors3(refract_direct, normal, light);
		m3dNormalizeVector(refract_direct);
	}
	else
	{
		get_reflect_direct(direct, intersect_p, refract_direct);
		return false;
	}
	return true;
}

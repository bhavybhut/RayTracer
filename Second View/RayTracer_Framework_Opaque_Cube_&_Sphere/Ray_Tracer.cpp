#include "Ray_Tracer.h"

Ray_Tracer::Ray_Tracer(void)
{
	float	dim = 512;
	_dim[0] = dim;
	_dim[1] = dim;
	_dim[2] = dim;
	_scene.set_dim(_dim);
	_scene.assemble();
	_view_plane.set_origin(0, 0, _dim[2]);
	_view_plane.set_u(1.0,0.0,0.0);
	_view_plane.set_v(0.0,1.0,0.0);
	_view_plane.set_eye(_dim[0]*1/2.0,_dim[1]/2.0, _dim[2] + 2000);

	_min_weight	=	0.01;
	_max_depth	=	20;
}

Ray_Tracer::~Ray_Tracer(void)
{	}


void	Ray_Tracer::run(Image & image)
{
	image.ncolorChannels = 3;
	image.nx = _dim[0];
	image.ny = _dim[1];

	image.n = image.nx * image.ny * image.ncolorChannels;
	image.data = new unsigned char[image.n];
	image.fdata = new float[image.n];

	M3DVector3f	ray;
	M3DVector3f	color;
	M3DVector3f	pij;
	float	weight = 1.0;
	float dx = 1;
	float dy = 1;
	int percent = 0;
	printf("Start Ray Tracing...........\n");
	printf("Progress:  %2d%%", percent);
	for (int j = 0; j < image.ny; j ++)
	{
		for (int i = 0; i < image.nx; i++)
		{
			_view_plane.get_pij(pij, i * dx, j *dy);
			_view_plane.get_per_ray(ray,pij);
			
			ray_tracing(pij, ray, weight, color, 0, NULL);
			unsigned int index = j*image.nx + i;
			for (int k = 0; k < 3; k++)
			{
				image.fdata[index*3 + k] = color[k];
			}
		}
		
		percent = (int) ((j+1)*100.0/image.ny);
		printf("\b\b\b\b%3d%%", percent);
	}

	percent = 100;
	printf("\b\b\b\b%3d%%", percent);
	printf("\nRay Tracing Finished! \n");

	//Normalizing Image
	double max_v = 0;
	double min_v = 1e+6;
	for(int m = 0; m < image.n; m++)
	{
		if(image.fdata[m] > max_v)
		{
			max_v = image.fdata[m]; 
		}
		if(image.fdata[m] < min_v)
		{
			min_v = image.fdata[m]; 
		}
	}

	max_v -= min_v;
	if(max_v > 1e-5)
	{
		for(int m = 0; m < image.n; m++)
		{
			image.data[m] = (unsigned char)((image.fdata[m]) * 255.0/max_v);
			if (image.data[m] > 255)
			{
				image.data[m] = 255;
			}
		}
	}else
	{
		printf("Error:happens in Normalizing the image into (0,255)....\n");
	}
//End of Normalizing Imaging
}


void Ray_Tracer::ray_tracing(M3DVector3f start, M3DVector3f direct, float weight, M3DVector3f color, unsigned int depth, Basic_Primitive * prim_in)
{
	if (weight < _min_weight || depth > _max_depth)
	{
		color[0] = color[1] = color[2] = 0.0;
		return;
	}
	else
	{
		m3dNormalizeVector(direct);
		Basic_Primitive * prim = NULL;
		M3DVector3f intersect_p;
		Intersect_Cond type = _scene.intersection_check(start, direct, &prim, intersect_p);
		if (type == _k_miss) {
			color[0] = color[1] = color[2] = 0.0;
		}
		else
		{
			M3DVector3f	am_light;
			M3DVector3f	reflect_direct, refract_direct;
			M3DVector3f local_color, reflect_color, refract_color;
			m3dLoadVector3(local_color, 0, 0, 0);
			m3dLoadVector3(reflect_color, 0, 0, 0);
			m3dLoadVector3(refract_color, 0, 0, 0);
			float	ks, kt, ws, wt;
			_scene.get_amb_light(am_light);
			prim->shade(direct, intersect_p, _scene.get_sp_light(), am_light, local_color, true);
			prim->get_properties(ks, kt, ws, wt);

			// Reflection
			if (ks > 1.0 && ws >0.55)
			{
				prim->get_reflect_direct(direct, intersect_p, reflect_direct);
				M3DVector3f counter;
				m3dCopyVector3(counter, reflect_direct);
				m3dScaleVector3(counter, 1);
				m3dAddVectors3(intersect_p, intersect_p, counter);
				ray_tracing(intersect_p, reflect_direct, weight * ks, reflect_color, depth + 1, prim);
			}

			//Refraction
			if (kt >0.50 && wt > 0.55)
			{
				bool is_in = type == _k_inside ? true : false;
				float	delta = 1.0;
				if (prim_in != NULL && is_in)
				{
					delta = prim_in->get_delta();
				}
				bool result = prim->get_refract_direct(direct, intersect_p, refract_direct, delta, is_in);
				M3DVector3f counter;
				m3dCopyVector3(counter, refract_direct);
				m3dScaleVector3(counter, 0.05);
				m3dAddVectors3(intersect_p, intersect_p, counter);
				ray_tracing(intersect_p, refract_direct, weight * kt, refract_color, depth + 1, prim);
			}
			for (int k = 0; k < 3; k++)
			{
				float Il = (1 - ws - wt)*local_color[k];
				color[k] = Il + ws * reflect_color[k] + wt * refract_color[k];
			}
			m3dScaleVector3(color, weight);
		}
	}
}

bool Ray_Tracer::check_shadow(M3DVector3f intersect_p)
{
	const Light	& sp_light = _scene.get_sp_light();
	M3DVector3f	intersect_to_light,sp_light_pos;
	sp_light.get_light_pos(sp_light_pos);
	m3dSubtractVectors3(intersect_to_light,intersect_p, sp_light_pos);
	float distance = m3dDotProduct(intersect_to_light,intersect_to_light);
	m3dNormalizeVector(intersect_to_light);
	
	Basic_Primitive * prim = NULL;
	M3DVector3f intersect_p_n;
	if (_scene.intersection_check(sp_light_pos,intersect_to_light,&prim,intersect_p_n) == _k_hit)
	{	
		M3DVector3f incr;
		m3dCopyVector3(incr,intersect_to_light);
		m3dScaleVector3(incr,0.05);
		m3dAddVectors3(intersect_p_n,intersect_p_n,incr);

		M3DVector3f intersect;
		m3dSubtractVectors3(intersect,sp_light_pos,intersect_p_n);
		float new_distance = m3dDotProduct(intersect,intersect);
		if (new_distance < distance)
		{
			return true;
		}
	}
	return false;
}


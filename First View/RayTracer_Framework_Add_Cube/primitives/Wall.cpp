#include "Wall.h"
#include <math.h>
#include <GL\glut.h>
#include "../Imageio/Imageio.h"

Intersect_Cond Wall::intersection_check(const M3DVector3f start, const M3DVector3f dir, float &distance, M3DVector3f intersection_p)
{
	if (_tr1.intersection_check(start,dir,distance,intersection_p) == _k_hit)
	{
		return _k_hit;
	}else
	{
		return _tr2.intersection_check(start,dir,distance,intersection_p);
	}
}


void	Wall::shade(M3DVector3f view, M3DVector3f intersect_p,const Light & sp_light, M3DVector3f am_light, M3DVector3f color,bool shadow)
{
	//Decrease shininess to 15 to 20 for plastic paint colors
	//Used 100 just to show shading effects in walls
	float shininess_constant = 15;
	M3DVector3f n_vector, l_vector, h_vector, r_vector, light_Position, light_Color, _pos;
	m3dLoadVector3(_pos, 1, 1, 1);
	m3dSubtractVectors3(n_vector, intersect_p, _pos);
	m3dNormalizeVector(n_vector);
	sp_light.get_light(light_Position, light_Color);

	m3dSubtractVectors3(l_vector, light_Position, intersect_p);
	m3dNormalizeVector(l_vector);

	float diffuse_coefficient = m3dDotProduct(l_vector, n_vector);
	diffuse_coefficient = (diffuse_coefficient < 0) ? 0.05 : diffuse_coefficient;

	m3dAddVectors3(h_vector, l_vector, view);
	m3dNormalizeVector(h_vector);
	float specular_coefficient = m3dDotProduct(n_vector, h_vector);
	specular_coefficient = (specular_coefficient < 0) ? 0.05 : pow(specular_coefficient, shininess_constant);
	for (int i = 0; i < 3; i++)
	{
		color[i] = (_kd * _color[i] * diffuse_coefficient) + (_ks * _color[i] * specular_coefficient) + (_ka * _color[i] * am_light[i]);
	}
	shadow = true;
}


void	Wall::get_reflect_direct(const M3DVector3f direct,const M3DVector3f intersect_p, M3DVector3f reflect_direct)
{
	M3DVector3f normal;
	M3DVector3f n_direct;
	m3dCopyVector3(n_direct, direct);
	m3dScaleVector3(n_direct, -1);
	_tr1.normal(normal);
	m3dScaleVector3(normal, 2 * m3dDotProduct(n_direct, normal));
	m3dSubtractVectors3(reflect_direct, normal, n_direct);
	m3dNormalizeVector(reflect_direct);
}


void Wall::load_texture(std::string file_name)
{
	Image img_texture;
	img_texture.nx = 256;
	img_texture.ny = 256;
	img_texture.ncolorChannels = 3;
	img_texture.data = (unsigned char*)malloc(img_texture.ncolorChannels * img_texture.nx *  img_texture.ny * sizeof(unsigned char));
	ReadImage(file_name.c_str(), img_texture.nx, img_texture.ny, img_texture.data, img_texture.ncolorChannels);
	_texture = (Image *)new Image(img_texture);

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _texture->nx,
		_texture->ny, 0, GL_RGB, GL_UNSIGNED_BYTE,
		_texture->data);
}

void Wall::texture_color(M3DVector3f pos, M3DVector3f color)
{	
	
}

void	Wall::get_texel(float x, float y, M3DVector3f color)
{
	return;
}
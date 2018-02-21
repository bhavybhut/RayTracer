#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene(void)
{	
	Basic_Primitive * prim = NULL;
	for (Prim_List::iterator it = _prim_list.begin(); it != _prim_list.end();)
	{
		prim = * it;
		_prim_list.erase(it++);
		delete prim;
	}
}

void	Scene::assemble()
{
	M3DVector3f	x0y0z0 = {0, 0, 0};
	M3DVector3f	x1y0z0 = {_dim[0], 0, 0};
	M3DVector3f	x0y1z0 = {0,_dim[1], 0};
	M3DVector3f	x1y1z0 = {_dim[0], _dim[1], 0};
	M3DVector3f	x0y0z1 = {0,0,_dim[2]};
	M3DVector3f	x1y0z1 = {_dim[0],0,_dim[2]};
	M3DVector3f	x0y1z1 = { 0,_dim[1],_dim[2]};
	M3DVector3f	x1y1z1 = { _dim[0],_dim[1],_dim[2]};

	//Walls
	//M3DVector3f wall_color_left = { 0.61,0.14,0.25 };
	M3DVector3f wall_color_left = { 0.36,0.67,0.92 };
	Wall * left_wall = new Wall(x0y1z0, x0y1z1, x0y0z1, x0y0z0, wall_color_left);
	left_wall->load_texture("../images/rock_wall.ppm");
	_prim_list.push_back(left_wall); // Left wall

	//M3DVector3f wall_color_right = { 0.36,0.67,0.92 };
	M3DVector3f wall_color_right = { 0.61,0.14,0.25 };
	Wall * right_wall = new Wall(x1y1z1, x1y1z0, x1y0z0, x1y0z1, wall_color_right);
	right_wall->load_texture("../images/rock_wall.ppm");
	_prim_list.push_back(right_wall);	// Right wall

	//M3DVector3f wall_color_top = { 1.0, 1.0, 0.0 };
	M3DVector3f wall_color_top = { 0.35, 0.45, 0.36 };
	Wall * top_wall = new Wall(x1y1z1, x0y1z1, x0y1z0, x1y1z0, wall_color_top);
	top_wall->load_texture("../images/rock_wall.ppm");
	_prim_list.push_back(top_wall);	// Top wall

	M3DVector3f wall_color_bottom = { 0.79,0.85,0.86 };
	Wall * bottom_wall = new Wall(x1y0z1, x1y0z0, x0y0z0, x0y0z1, wall_color_bottom);
	//bottom_wall->set_properties(0.9, 0, 0.9, 0);
	bottom_wall->load_texture("../images/rock_wall.ppm");
	_prim_list.push_back(bottom_wall);	// Bottom wall

	//M3DVector3f back_wall_color = { 0.74,0.90,0.29 };
	M3DVector3f back_wall_color = { 1.0, 1.0, 0.0 };
	Wall * back_wall = new Wall(x1y1z0, x0y1z0, x0y0z0, x1y0z0, back_wall_color);
	back_wall->load_texture("../images/nature.ppm");
	_prim_list.push_back(back_wall);	// Back wall
//End of walls

//Cube
	M3DVector3f	c_x0y0z0 = { _dim[0] * 0.12, _dim[1] * 0.1, _dim[2] * 0.2 };
	M3DVector3f	c_x1y0z0 = { _dim[0] * 0.32, _dim[1] * 0.1, _dim[2] * 0.2 };
	M3DVector3f	c_x0y1z0 = { _dim[0] * 0.12, _dim[1] * 0.3, _dim[2] * 0.2 };
	M3DVector3f	c_x1y1z0 = { _dim[0] * 0.32, _dim[1] * 0.3, _dim[2] * 0.2 };

	M3DVector3f	c_x0y0z1 = { _dim[0] * 0.1, _dim[1] * 0.135, _dim[2] * 0.4 };
	M3DVector3f	c_x1y0z1 = { _dim[0] * 0.3, _dim[1] * 0.135, _dim[2] * 0.4 };
	M3DVector3f	c_x0y1z1 = { _dim[0] * 0.1, _dim[1] * 0.335, _dim[2] * 0.4 };
	M3DVector3f	c_x1y1z1 = { _dim[0] * 0.3, _dim[1] * 0.335, _dim[2] * 0.4 };

	float k = 0.0;
	float w = 0.6;
	M3DVector3f cube_color_left = { 0.36,0.67,0.92 };
	Cube * c_left = new Cube(c_x0y1z0, c_x0y1z1, c_x0y0z1, c_x0y0z0, cube_color_left);
	c_left->set_properties(k, w, k, w);
	_prim_list.push_back(c_left);

	M3DVector3f cube_color_right = { 0.61,0.14,0.25 };
	Cube * c_right = new Cube(c_x1y1z1, c_x1y1z0, c_x1y0z0, c_x1y0z1, cube_color_right);
	c_right->set_properties(k, w, k, w);
	_prim_list.push_back(c_right);

	M3DVector3f cube_color_top = { 0.5,1.0,0.0 };
	Cube * c_top = new Cube(c_x1y1z1, c_x0y1z1, c_x0y1z0, c_x1y1z0, cube_color_top);
	c_top->set_properties(k, w, k, w);
	_prim_list.push_back(c_top);

	M3DVector3f cube_color_bottom = { 0.79,0.85,0.86 };
	Cube * c_bottom = new Cube(c_x1y0z1, c_x1y0z0, c_x0y0z0, c_x0y0z1, cube_color_bottom);
	c_bottom->set_properties(k, w, k, w);
	_prim_list.push_back(c_bottom);

	M3DVector3f cube_back_color = { 1.0, 1.0, 0.0 };
	Cube * c_back = new Cube(c_x1y1z0, c_x0y1z0, c_x0y0z0, c_x1y0z0, cube_back_color);
	c_back->set_properties(k, w, k, w);
	_prim_list.push_back(c_back);

	M3DVector3f cube_front_color = { 0.35, 0.45, 0.36 };
	Cube * c_front = new Cube(c_x0y1z1, c_x1y1z1, c_x1y0z1, c_x0y0z1, cube_front_color);
	c_front->set_properties(k, w, k, w);
	_prim_list.push_back(c_front);
//End Cube
	
//Spheres
	float	rad = _dim[2] / 4.0;
	M3DVector3f sphere_color1 = {0.5, 0.1, 1.0};
	M3DVector3f sphere_center1 = {_dim[0] - rad - 20, rad, _dim[2]*2.0/3.0  - rad};
	Sphere * sp1 = new Sphere(sphere_center1,rad,sphere_color1); 
	sp1->set_properties(0.0, 0.9, 0.0, 0.9);
	_prim_list.push_back(sp1);
//End of Spheres

//Lights code
	M3DVector3f light_Position = { _dim[0], _dim[1], _dim[2] };
	M3DVector3f light_Color = { 1.0, 1.0, 1.0 };
	_sp_light.set_light(light_Position, light_Color);

	for (int i = 0; i < 3; i++) {
		_amb_light[i] = 0.5;
	}
}

Intersect_Cond	Scene::intersection_check(const M3DVector3f start, const M3DVector3f dir, Basic_Primitive * * prim_intersect, M3DVector3f closest_point)
{
	Basic_Primitive * prim = NULL;
	float		distance;
	M3DVector3f point;
	
	float	min_distance = 0x0ffffff;
	*prim_intersect = NULL;

	Intersect_Cond ret = _k_miss;
	Intersect_Cond tmp;
	for (Prim_List::iterator it = _prim_list.begin(); it != _prim_list.end(); it++)
	{
		prim = * it;
		tmp = prim->intersection_check(start,dir,distance,point);
		if (tmp != _k_miss)
		{
			if (distance < min_distance)
			{
				min_distance = distance;
				*prim_intersect = prim;
				m3dCopyVector3(closest_point,point);
				ret = tmp;
			}
		}
	}

	return ret;
}
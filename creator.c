/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 13:46:02 by ekiriche          #+#    #+#             */
/*   Updated: 2018/03/20 13:46:28 by ekiriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	set_coefs(t_coefs *coefs, t_map *scroll, float *x, float *y)
{
	*x = 50;
	*y = 600;
	coefs->coef1 = 500 / scroll->column + 0.5;
	coefs->coef2 = 300 / scroll->row + 1.5;
	coefs->coef3 = 100 / scroll->max_z + 2.5;
	coefs->i = -1;
}

void	ulti_norm(t_map *scroll, t_point2 *xy, t_coefs *coefs, t_color **wow)
{
	scroll->int_map = coords_z(scroll, wow);
	scroll->max_z = look_for_max_z(scroll);
	set_coefs(coefs, scroll, &xy->x, &xy->y);
}

void	ulti_norm2(t_color **wow, t_coefs *coefs, t_point2 *xy)
{
	destroy_color(wow);
	ft_memdel((void**)&coefs);
	ft_memdel((void**)&xy);
}

void	ulti_norm3(t_point2 *xy,
		t_coefs *coefs, t_point **coords, t_map *scroll)
{
	coords[coefs->i][coefs->j].x = xy->x + coefs->coef1 * coefs->j;
	coords[coefs->i][coefs->j].y = xy->y - coefs->coef2 * coefs->j * 0.5;
	coords[coefs->i][coefs->j].z = scroll->int_map[coefs->i][coefs->j] *
		coefs->coef3;
}

t_point	**ultimate_creator(t_map *scroll)
{
	t_point2	*xy;
	t_point		**coords;
	t_coefs		*coefs;
	t_color		**wow;

	coefs = (t_coefs*)malloc(sizeof(t_coefs));
	wow = (t_color**)malloc(sizeof(t_color*) * (scroll->row + 128000));
	xy = (t_point2*)malloc(sizeof(t_point2));
	ulti_norm(scroll, xy, coefs, wow);
	coords = (t_point**)malloc(sizeof(t_point*) * (scroll->row + 128000));
	while (++coefs->i < scroll->row)
	{
		coords[coefs->i] = (t_point*)malloc(sizeof(t_point) *
				(scroll->column + 64000));
		coefs->j = -1;
		while (++coefs->j < scroll->column)
		{
			ulti_norm3(xy, coefs, coords, scroll);
			coords[coefs->i][coefs->j].color = wow[coefs->i][coefs->j].color;
		}
		xy->x += coefs->coef1;
		xy->y += coefs->coef2 * 0.5;
	}
	ulti_norm2(wow, coefs, xy);
	return (coords);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_drawing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 13:48:34 by ekiriche          #+#    #+#             */
/*   Updated: 2018/03/20 13:50:07 by ekiriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

long int	ft_strtol(char *line)
{
	long int	rslt;
	long int	*hexa;
	int			cmp;
	int			power;

	cmp = 1;
	power = 0;
	rslt = 0;
	hexa = (long int*)malloc(sizeof(long int) * ft_strlen(line) + 1024000);
	while (line[++cmp])
	{
		if (line[cmp] >= 'A' && line[cmp] <= 'F')
			hexa[cmp] = line[cmp] + 10 - 'A';
		else if (line[cmp] >= 'a' && line[cmp] <= 'f')
			hexa[cmp] = line[cmp] + 10 - 'a';
		else if (line[cmp] >= '0' && line[cmp] <= '9')
			hexa[cmp] = line[cmp] - '0';
	}
	while (--cmp >= 0)
	{
		rslt += hexa[cmp] * ft_power(16, power);
		power++;
	}
	ft_memdel((void**)&hexa);
	return (rslt);
}

void	destroy_color(t_color **wow)
{
	int	i;

	i = 0;
	while (wow[i])
	{
		ft_memdel((void**)&wow[i]);
		i++;
	}
	ft_memdel((void**)&wow);
}

void		some_drawing2(t_hold *holdup, t_coefs *ig, t_point **lul, t_map *scroll)
{
	if (ig->j + 1 < scroll->column)
	{
		holdup->x1 = lul[ig->i][ig->j].x;
		holdup->y1 = lul[ig->i][ig->j].y - lul[ig->i][ig->j].z;
		holdup->x2 = lul[ig->i][ig->j + 1].x;
		holdup->y2 = lul[ig->i][ig->j + 1].y - lul[ig->i][ig->j + 1].z;
		holdup->color = lul[ig->i][ig->j].color;
		draw_line(holdup);
	}
	if (ig->j - 1 > 0)
	{
		holdup->x1 = lul[ig->i][ig->j].x;
		holdup->y1 = lul[ig->i][ig->j].y - lul[ig->i][ig->j].z;
		holdup->x2 = lul[ig->i][ig->j - 1].x;
		holdup->y2 = lul[ig->i][ig->j - 1].y - lul[ig->i][ig->j - 1].z;
		holdup->color = lul[ig->i][ig->j].color;
		draw_line(holdup);
	}
}

void	some_drawing1(t_hold *holdup, t_coefs *ig, t_point **lul, t_map *scroll)
{
	if (ig->i - 1 > 0)
	{
		holdup->x1 = lul[ig->i][ig->j].x;
		holdup->y1 = lul[ig->i][ig->j].y - lul[ig->i][ig->j].z;
		holdup->x2 = lul[ig->i - 1][ig->j].x;
		holdup->y2 = lul[ig->i - 1][ig->j].y - lul[ig->i - 1][ig->j].z;
		holdup->color = lul[ig->i][ig->j].color;
		draw_line(holdup);
	}
	if (ig->i + 1 < scroll->row)
	{
		holdup->x1 = lul[ig->i][ig->j].x;
		holdup->y1 = lul[ig->i][ig->j].y - lul[ig->i][ig->j].z;
		holdup->x2 = lul[ig->i + 1][ig->j].x;
		holdup->y2 = lul[ig->i + 1][ig->j].y - lul[ig->i + 1][ig->j].z;
		holdup->color = lul[ig->i][ig->j].color;
		draw_line(holdup);
	}
	some_drawing2(holdup, ig, lul, scroll);
}

void	print_smth(void *mlx_ptr, void *mlx_win, t_point **lul, t_map *scroll)
{
	t_hold	*holdup;
	t_coefs *ig;

	holdup = (t_hold*)malloc(sizeof(t_hold));
	ig = (t_coefs*)malloc(sizeof(t_coefs));
	holdup->mlx_ptrz = mlx_ptr;
	holdup->mlx_winz = mlx_win;

	ig->i = 0;
	while (ig->i < scroll->row)
	{
		ig->j = 0;
		while (ig->j < scroll->column)
		{
			some_drawing1(holdup, ig, lul, scroll);
			ig->j++;
		}
		ig->i++;
	}
	ft_memdel((void**)&holdup);
	ft_memdel((void**)&ig);
}

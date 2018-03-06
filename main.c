/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 14:35:57 by ekiriche          #+#    #+#             */
/*   Updated: 2018/03/06 18:17:28 by ekiriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <math.h>
#include <fcntl.h>
#include <stdlib.h>
#include "ft_printf.h"
#include "libft.h"

typedef struct	s_point
{
	float	x;
	float	y;
	float	z;
}				t_point;

typedef struct	s_map
{
	char	**map;
	int		**int_map;
	int		column;
	int		row;
}				t_map;

typedef struct	s_coefs
{
	int		i;
	int		j;
	float	coef1;
	float	coef2;
	float	coef3;
}				t_coefs;

int		escape_key(int key, void *param)
{
	if (key == 53)
		exit(0);
	return (0);
}

char	**coords_from_file(int fd)
{
	char	**map;
	char	*str;
	char	*str2;

	while (get_next_line(fd, &str))
	{
		if (str2 == NULL)
			str2 = ft_strdup(str);
		else
			str2 = ft_strjoin(str2, str);
		str2 = ft_strjoin(str2, "\n");
	}
	map = ft_strsplit(str2, '\n');
	return (map);
}

int 	**coords_z(char **map, int rows, int columns)
{
	int **ans;
	int i;
	int j;
	int k;

	i = -1;
	ans = (int**)malloc(sizeof(int*) * (rows + 1));
	while (++i < rows)
	{
		j = -1;
		k = 0;
		ans[i] = (int*)malloc(sizeof(int) * (columns + 1));
		while (map[i][++j])
			if (map[i][j] != ' ')
			{
				ans[i][k] = ft_atoi(map[i] + j);
				k++;
				while (map[i][j] != ' ' && map[i][j])
					j++;
			}
		ans[i][k] = '\0';
	}
	ans[i] = NULL;
	return (ans);
}

void	draw_line(int x1, int y1, int x2, int y2, void *mlx_ptr, void *mlx_win)
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	
	mlx_pixel_put(mlx_ptr, mlx_win, x2, y2, 40000);
	while (x1 != x2 || y1 != y2)
	{
		mlx_pixel_put(mlx_ptr, mlx_win, x1, y1, 40000);
		const int error2 = error * 2;
		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

int		count_column(char **str)
{
	int	ans;
	int	i;

	ans = 0;
	i = 0;
	while (str[0][i])
	{
		if (str[0][i] != ' ')
			ans++;
		while (str[0][i] != ' ' && str[0][i])
			i++;
		if (str[0][i] == '\0')
			break ;
		i++;
	}
	return (ans);
}

int		count_row(char **str)
{
	int	ans;

	ans = 0;
	while (str[ans])
		ans++;
	return (ans);
}

void	set_coefs(t_coefs *coefs, int columns, float *x, float *y)
{
	*x = 50;
	*y = 600;
	if (columns > 30)
	{
		coefs->coef1 = 4;
		coefs->coef2 = 4;
		coefs->coef3 = 6;
	}
	else if (columns > 100)
	{
		coefs->coef1 = 0.1;
		coefs->coef2 = 0.1;
		coefs->coef3 = 0.1;
	}
	else
	{
		coefs->coef1 = 15;
		coefs->coef2 = 15;
		coefs->coef3 = 15;
	}
	coefs->i = -1;
}

t_point	**ultimate_creator(int rows, int columns, int **map)
{
	float		x;
	float		y;
	t_point	**coords;
	t_coefs	*coefs;

	coefs = (t_coefs*)malloc(sizeof(t_coefs));
	set_coefs(coefs, columns, &x, &y);
	coords = (t_point**)malloc(sizeof(t_point*) * (rows + 1));
	while (++coefs->i < rows)
	{
		coords[coefs->i] = malloc(sizeof(t_point) * (columns + 1));
		coefs->j = -1;
		while (++coefs->j < columns)
		{
			coords[coefs->i][coefs->j].x = (float)(x + coefs->coef1 * (float)coefs->j);
			coords[coefs->i][coefs->j].y = (float)(y - coefs->coef2 * (float)coefs->j * 0.5);
			coords[coefs->i][coefs->j].z = (float)((float)map[coefs->i][coefs->j] * coefs->coef3);
		}
		x += coefs->coef1;
		y += coefs->coef2 * 0.5;
	}
	return (coords);
}

void	print_smth(void *mlx_ptr, void *mlx_win, t_point **lul, t_map *scroll)
{
	int	i;
	int	j;

	i = 0;
	while (i < scroll->row)
	{
		j = 0;
		while (j < scroll->column)
		{
			if (i - 1 > 0)
				draw_line(lul[i][j].x, lul[i][j].y - lul[i][j].z, lul[i - 1][j].x, lul[i - 1][j].y - lul[i - 1][j].z, mlx_ptr, mlx_win);
			if (i + 1 < scroll->row)
				draw_line(lul[i][j].x, lul[i][j].y - lul[i][j].z, lul[i + 1][j].x, lul[i + 1][j].y - lul[i + 1][j].z, mlx_ptr, mlx_win);
			if (j + 1 < scroll->column)
				draw_line(lul[i][j].x, lul[i][j].y - lul[i][j].z, lul[i][j + 1].x, lul[i][j + 1].y - lul[i][j + 1].z, mlx_ptr, mlx_win);
			if (j - 1 > 0)
				draw_line(lul[i][j].x, lul[i][j].y - lul[i][j].z, lul[i][j - 1].x, lul[i][j - 1].y - lul[i][j - 1].z, mlx_ptr, mlx_win);
			j++;
		}
		i++;
	}
}

void	destroy_point(t_point **lul, int rows)
{
	int i;

	i = rows - 1;
	while (i > 0)
	{
		ft_memdel((void**)&lul[i]);
		i--;
	}
	ft_memdel((void**)&lul);
}

int		main(int argc, char **argv)
{
	void	*mlx_ptr;
	void	*mlx_win;
	int		fd;
	t_map	*scroll;
	t_point	**lul;

	mlx_ptr = mlx_init();
	fd = open(argv[1], O_RDONLY);
	scroll = (t_map*)malloc(sizeof(t_map));
	scroll->map = coords_from_file(fd);
	scroll->column = count_column(scroll->map);
	scroll->row = count_row(scroll->map);
	mlx_win = mlx_new_window(mlx_ptr, 1500, 1000, "yay");
	scroll->int_map = coords_z(scroll->map, scroll->row, scroll->column);
	lul = ultimate_creator(scroll->row, scroll->column, scroll->int_map);
	print_smth(mlx_ptr, mlx_win, lul, scroll);
	destroy_point(lul, scroll->row);
//	ft_memdel((void**)&scroll->map);
//	ft_memdel((void**)&scroll->int_map);
//	ft_memdel((void**)&scroll);
	//	ft_printf("%d %d\n", row, column);
	mlx_key_hook(mlx_win, escape_key, (void *)0);
	mlx_loop(mlx_ptr);
}

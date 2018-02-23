/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 14:35:57 by ekiriche          #+#    #+#             */
/*   Updated: 2018/02/23 18:50:46 by ekiriche         ###   ########.fr       */
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
	int	x;
	int	y;
	int	z;
}				t_point;

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
		ft_memdel((void**)&str);
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

	i = 0;
	ans = (int**)malloc(sizeof(int*) * (rows + 1));
	while (i < rows)
	{
		j = 0;
		k = 0;
		ans[i] = (int*)malloc(sizeof(int) * (columns + 1));
		while (map[i][j])
		{
			if (map[i][j] != ' ')
			{
				ans[i][k] = ft_atoi(map[i] + j);
				k++;
				while (map[i][j] != ' ' && map[i][j])
					j++;
			}
			j++;
		}
		ans[i][k] = '\0';
		i++;
	}
	ans[i] = NULL;
	return (ans);
}

void line (int x0, int x1, int y0, int y1, int color, void *mlx_ptr, void *mlx_win)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = (dy << 1) - dx;
	int d1 = dy << 1;
	int d2 = (dy - dx) << 1;

	mlx_pixel_put(mlx_ptr, mlx_win, x0, y0, color);
	for(int x = x0 + 1, y = y0; x <= x1; x++)
	{
		if ( d >0)
		{
			d += d2;
			y += 1;
		}
		else
			d += d1;
		mlx_pixel_put(mlx_ptr, mlx_win, x, y, color);
	}
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

void segment(int x0, int y0, int x1, int y1, int color, void *mlx_ptr, void *mlx_win)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = x1 >= x0 ? 1 : -1;
	int sy = y1 >= y0 ? 1 : -1;

	if (dy <= dx)
	{
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;
		mlx_pixel_put(mlx_ptr, mlx_win, x0, y0, color);
		for(int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx)
		{
			if ( d >0)
			{
				d += d2;
				y += sy;
			}
			else
				d += d1;
			mlx_pixel_put(mlx_ptr, mlx_win, x, y, color);
		}
	}
	else
	{
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		mlx_pixel_put(mlx_ptr, mlx_win, x0, y0, color);
		for(int y = y0 + sy, x = x0, i = 1; i <= dy; i++, y += sy)
		{
			if ( d >0)
			{
				d += d2;
				x += sx;
			}
			else
				d += d1;
			mlx_pixel_put(mlx_ptr, mlx_win, x, y, color);
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

t_point	**ultimate_creator(int rows, int columns, int **map)
{
	int		x;
	int		y;
	int		i;
	int		j;
	int		coef1;
	int		coef2;
	int 	coef3 = 10;
	t_point	**coords;

	x = 300;
	y = 500;
	coef1 = 20;
	coef2 = 20;
	i = -1;
	coords = (t_point**)malloc(sizeof(t_point*) * (rows + 1));
	while (++i < rows)
	{
		coords[i] = malloc(sizeof(t_point) * (columns + 1));
		j = -1;
		while (++j < columns)
		{
			coords[i][j].x = x + coef1 * j;
			coords[i][j].y = y - coef2 * j * 0.5;
			coords[i][j].z = map[i][j] * 15;
	//		ft_printf("%d %d %d\n", coords[i][j].x, coords[i][j].y, coords[i][j].z);
		}
		x += coef1;
		y += coef2 * 0.5;
		//		ft_printf("%d\n", i);
	}
	return (coords);
}

void	print_smth(void *mlx_ptr, void *mlx_win, t_point **lul, int rows, int columns)
{
	int	i;
	int	j;

	i = 0;
//	while (j + 1 < columns)
//	{
//		draw_line(lul[0][j].x, lul[0][j].y, lul[0][j + 1].x, lul[0][j + 1].y, mlx_ptr, mlx_win);
//		j++;
//	}
	while (i < rows)
	{
		j = 0;
		while (j < columns)
		{
			if (i - 1 > 0)
				draw_line(lul[i][j].x, lul[i][j].y - lul[i][j].z, lul[i - 1][j].x, lul[i - 1][j].y - lul[i - 1][j].z, mlx_ptr, mlx_win);
			if (i + 1 < rows)
				draw_line(lul[i][j].x, lul[i][j].y - lul[i][j].z, lul[i + 1][j].x, lul[i + 1][j].y - lul[i + 1][j].z, mlx_ptr, mlx_win);
			if (j + 1 < columns)
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
	char	**map;
	int 	**int_map;
	int		column = 0;
	int		row = 0;
	t_point	**lul;
	mlx_ptr = mlx_init();
	mlx_win = mlx_new_window(mlx_ptr, 1200, 1000, "yay");
	fd = open(argv[1], O_RDONLY);
	map = coords_from_file(fd);
	column = count_column(map);
	row = count_row(map);
	int_map = coords_z(map, row, column);
	lul = ultimate_creator(row, column, int_map);
	print_smth(mlx_ptr, mlx_win, lul, row, column);
	destroy_point(lul, row);
	ft_memdel((void**)&map);
	ft_memdel((void**)&int_map);
	//	ft_printf("%d %d\n", row, column);
	mlx_key_hook(mlx_win, escape_key, (void *)0);
	mlx_loop(mlx_ptr);
}

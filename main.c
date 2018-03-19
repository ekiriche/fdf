/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 14:35:57 by ekiriche          #+#    #+#             */
/*   Updated: 2018/03/19 18:19:35 by ekiriche         ###   ########.fr       */
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
	float		x;
	float		y;
	float		z;
	long int	color;
}				t_point;

typedef struct	s_color
{
	int	color;
}				t_color;

typedef struct	s_map
{
	char		**map;
	int			**int_map;
	int			max_z;
	int			column;
	int			row;
	char		*color;
	long int	color_int;
}				t_map;

typedef struct	s_coefs
{
	int		i;
	int		j;
	float	coef1;
	float	coef2;
	float	coef3;
}				t_coefs;

typedef struct	s_hold
{
	int			x1;
	int			x2;
	int			y1;
	int			y2;
	long int	color;
	void		*mlx_ptrz;
	void		*mlx_winz;
}				t_hold;

int		escape_key(int key, void *param)
{
	if (key == 53)
		exit(0);
	return (0);
}

long int	ft_strtol(char *line)
{
	long int	rslt;
	long int	*hexa;
	int			cmp;
	int			power;

	cmp = 0;
	power = 0;
	rslt = 0;
	hexa = (long int*)malloc(sizeof(long int) * ft_strlen(line) + 64000);
	while (line[cmp])
	{
		if (line[cmp] >= 'A' && line[cmp] <= 'F')
			hexa[cmp] = line[cmp] + 10 - 'A';
		else if (line[cmp] >= 'a' && line[cmp] <= 'f')
			hexa[cmp] = line[cmp] + 10 - 'a';
		else if (line[cmp] >= '0' && line[cmp] <= '9')
			hexa[cmp] = line[cmp] - '0';
		cmp++;
	}
	while (--cmp >= 0)
	{
		rslt += hexa[cmp] * ft_power(16, power);
		power++;
	}
	ft_memdel((void**)&hexa);
	return (rslt);
}

int		look_for_max_z(t_map *map)
{
	int	max;
	int	i;
	int	j;

	max = 1;
	i = 0;
	while (i < map->row)
	{
		j = 0;
		while (j < map->column)
		{
			if (map->int_map[i][j] > max)
				max = map->int_map[i][j];
			j++;
		}
		i++;
	}
	return (max);
}

char	*my_strjoin1(char *s1, char *s2)
{
	char	*res;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	if (!(res = (char*)malloc(sizeof(char) *
					(ft_strlen(s1) + ft_strlen(s2)) + 1)))
		return (NULL);
	while (s1[++i])
		res[i] = s1[i];
	while (s2[j])
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	res[i] = '\0';
	ft_memdel((void**)&s1);
	return (res);
}

char	**coords_from_file(int fd)
{
	char	**map;
	char	*str;
	char	*str2;
	int		step;

	step = 0;
	while (get_next_line(fd, &str))
	{
		if (step == 0)
			str2 = ft_strdup(str);
		else
			str2 = my_strjoin1(str2, str);
		str2 = my_strjoin1(str2, "\n");
		ft_memdel((void**)&str);
		step = 1;
	}
	map = ft_strsplit(str2, '\n');
	if (step == 1)
		ft_memdel((void**)&str2);
	return (map);
}

int 	**coords_z(t_map *scroll, t_color **wow)
{
	int **ans;
	int i;
	int j;
	int k;
	int	count;

	i = -1;
	count = 0;
	ans = (int**)malloc(sizeof(int*) * (scroll->row + 64000));
	while (++i < scroll->row)
	{
		j = -1;
		k = 0;
		ans[i] = (int*)malloc(sizeof(int) * (scroll->column + 64000));
		wow[i] = (t_color*)malloc(sizeof(t_color) * (scroll->column + 64000));
		while (scroll->map[i][++j])
			if (scroll->map[i][j] != ' ')
			{
				ans[i][k] = ft_atoi(scroll->map[i] + j);
				k++;
				while (scroll->map[i][j] != ' ' && scroll->map[i][j] && scroll->map[i][j] != ',')
					j++;
				if (scroll->map[i][j] == ',')
				{
					while (scroll->map[i][j + count + 1] != ' ' && scroll->map[i][j + count + 1])
						count++;
					scroll->color = ft_strsub(scroll->map[i] + j + 1, 0, count);
					wow[i][k - 1].color = ft_strtol(scroll->color);
					ft_memdel((void**)&scroll->color);
					j += count + 1;
				}
				else
					wow[i][k - 1].color = 40000;
				count = 0;
			}
		ans[i][k] = '\0';
	}
	ans[i] = NULL;
	return (ans);
}

void	draw_line(t_hold *h)
{
	const int deltaX = abs(h->x2 - h->x1);
	const int deltaY = abs(h->y2 - h->y1);
	const int signX = h->x1 < h->x2 ? 1 : -1;
	const int signY = h->y1 < h->y2 ? 1 : -1;
	int error = deltaX - deltaY;

	mlx_pixel_put(h->mlx_ptrz, h->mlx_winz, h->x2, h->y2, h->color);
	while (h->x1 != h->x2 || h->y1 != h->y2)
	{
		mlx_pixel_put(h->mlx_ptrz, h->mlx_winz, h->x1, h->y1, h->color);
		const int error2 = error * 2;
		if (error2 > -deltaY)
		{
			error -= deltaY;
			h->x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			h->y1 += signY;
		}
	}
}

int		count_column(char **str)
{
	int	ans;
	int	i;
	int	j;
	int	check;

	ans = 0;
	j = 0;
	check = 0;
	while (str[j])
	{
		i = 0;
		while (str[j][i])
		{
			if (str[j][i] != ' ')
				ans++;
			while (str[j][i] != ' ' && str[j][i])
				i++;
			if (str[j][i] == '\0')
				break ;
			i++;
		}
		if (check != 0 && check != ans)
			return (-1);
		check = ans;
		ans = 0;
		j++;
	}
	ft_printf("%d\n", check);
	return (check);
}

int		count_row(char **str)
{
	int	ans;

	ans = 0;
	while (str[ans])
		ans++;
	ft_printf("%i\n", ans);
	return (ans);
}

void	set_coefs(t_coefs *coefs, t_map *scroll, float *x, float *y)
{
	*x = 50;
	*y = 600;
	coefs->coef1 = 500 / scroll->column + 0.5;
	coefs->coef2 = 300 / scroll->row + 1.5;
	coefs->coef3 = 100 / scroll->max_z + 2.5;
	coefs->i = -1;
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

t_point	**ultimate_creator(t_map *scroll)
{
	float		x;
	float		y;
	t_point	**coords;
	t_coefs	*coefs;
	t_color	**wow;

	coefs = (t_coefs*)malloc(sizeof(t_coefs));
	wow = (t_color**)malloc(sizeof(t_color*) * (scroll->row + 64000));
	scroll->int_map = coords_z(scroll, wow);
	scroll->max_z = look_for_max_z(scroll);
	set_coefs(coefs, scroll, &x, &y);
	coords = (t_point**)malloc(sizeof(t_point*) * (scroll->row + 64000));
	while (++coefs->i < scroll->row)
	{
		coords[coefs->i] = (t_point*)malloc(sizeof(t_point) * (scroll->column + 64000));
		coefs->j = -1;
		while (++coefs->j < scroll->column)
		{
			coords[coefs->i][coefs->j].x = (float)(x + coefs->coef1 * (float)coefs->j);
			coords[coefs->i][coefs->j].y = (float)(y - coefs->coef2 * (float)coefs->j * 0.5);
			coords[coefs->i][coefs->j].z = (float)((float)scroll->int_map[coefs->i][coefs->j] * coefs->coef3);
			coords[coefs->i][coefs->j].color = wow[coefs->i][coefs->j].color;
		}
		x += coefs->coef1;
		y += coefs->coef2 * 0.5;
	}
	destroy_color(wow);
	ft_memdel((void**)&coefs);
	return (coords);
}

void	set_hold_mlx(t_hold *holdup, void *mlx_ptr, void *mlx_win)
{
	holdup->mlx_ptrz = mlx_ptr;
	holdup->mlx_winz = mlx_win;
}

void	some_drawing2(t_hold *holdup, t_coefs *ig, t_point **lul, t_map *scroll)
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
	set_hold_mlx(holdup, mlx_ptr, mlx_win);
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

void	destroy_point(t_point **lul, int rows)
{
	int i;

	i = rows - 1;
	while (i >= 0)
	{
		ft_memdel((void**)&lul[i]);
		i--;
	}
	ft_memdel((void**)&lul);
}

void	destroy_scroll(t_map *scroll, int rows)
{
	int i;

	i = rows - 1;
	while (i >= 0)
	{
		ft_memdel((void**)&scroll->map[i]);
		ft_memdel((void**)&scroll->int_map[i]);
		i--;
	}
	ft_memdel((void**)&scroll->map);
	ft_memdel((void**)&scroll->int_map);
	ft_memdel((void**)&scroll);
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
	if (fd == -1)
		return (0);
	scroll = (t_map*)malloc(sizeof(t_map));
	scroll->map = coords_from_file(fd);
	scroll->row = count_row(scroll->map);
	scroll->column = count_column(scroll->map);
	if (scroll->column == -1)
		return (0);
	mlx_win = mlx_new_window(mlx_ptr, 1500, 1000, "yay");
	lul = ultimate_creator(scroll);
	print_smth(mlx_ptr, mlx_win, lul, scroll);
	destroy_point(lul, scroll->row);
	destroy_scroll(scroll, scroll->row);
	mlx_key_hook(mlx_win, escape_key, (void *)0);
	mlx_loop(mlx_ptr);
	return (0);
}

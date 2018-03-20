/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 13:02:39 by ekiriche          #+#    #+#             */
/*   Updated: 2018/03/20 13:37:00 by ekiriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft/libft.h"
# include <math.h>

typedef struct	s_point2
{
	float		x;
	float		y;
}				t_point2;

typedef struct	s_point
{
	float		x;
	float		y;
	float		z;
	long int	color;
}				t_point;

typedef struct	s_color
{
	long int	color;
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
	int			i;
	int			j;
	float		coef1;
	float		coef2;
	float		coef3;
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

typedef struct	s_joke
{
	int			i;
	int			j;
	int			k;
	int			count;
}				t_joke;

typedef struct	s_draw
{
	int			deltax;
	int			deltay;
	int			signx;
	int			signy;
	int			error;
	int			error2;
}				t_draw;

int				escape_key(int key, void *param);
long int		ft_strtol(char *line);
int				look_for_max_z(t_map *map);
char			*my_strjoin1(char *s1, char *s2);
char			**coords_from_file(int fd);
void			coords_z_norm(int **ans, t_joke *p, t_map *scroll,
				t_color **wow);
void			set_draws(t_draw *p, t_hold *h);
void			draw_line(t_hold *h);
void			count_column_norm(int *j, int *i, char **str, int *ans);
int				count_column(char **str);
int				count_row(char **str);
void			set_coefs(t_coefs *coefs, t_map *scroll, float *x, float *y);
void			destroy_color(t_color **wow);
void			ulti_norm(t_map *scroll, t_point2 *xy, t_coefs *coefs,
				t_color **wow);
void			ulti_norm2(t_color **wow, t_coefs *coefs, t_point2 *xy);
void			ulti_norm3(t_point2 *xy, t_coefs *coefs, t_point **coords,
				t_map *scroll);
t_point			**ultimate_creator(t_map *scroll);
void			some_drowing2(t_hold *holdup, t_coefs *ig, t_point **lul,
				t_map *scroll);
void			some_drawing1(t_hold *holdup, t_coefs *ig, t_point **lul,
				t_map *scroll);
void			print_smth(void *mlx_ptr, void *mlx_win,
				t_point **lul, t_map *scroll);
void			destroy_point(t_point **lul, int rows);
void			destroy_scroll(t_map *scroll, int rows);
int				main(int argc, char **argv);

#endif

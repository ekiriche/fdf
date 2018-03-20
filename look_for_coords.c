/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   look_for_coords.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 13:47:15 by ekiriche          #+#    #+#             */
/*   Updated: 2018/03/20 13:47:53 by ekiriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

void	coords_z_norm(int **ans, t_joke *p, t_map *scroll, t_color **wow)
{
	if (scroll->map[p->i][p->j] != ' ')
	{
		ans[p->i][p->k] = ft_atoi(scroll->map[p->i] + p->j);
		p->count = 0;
		while (scroll->map[p->i][p->j] != ' ' && scroll->map[p->i][p->j]
				&& scroll->map[p->i][p->j] != ',')
			p->j++;
		if (scroll->map[p->i][p->j] == ',')
		{
			while (scroll->map[p->i][p->j + p->count + 1] != ' ' &&
					scroll->map[p->i][p->j + p->count + 1])
				p->count++;
			scroll->color = ft_strsub(scroll->map[p->i] + p->j + 1, 0,
					p->count);
			wow[p->i][p->k].color = ft_strtol(scroll->color);
			ft_memdel((void**)&scroll->color);
			p->j += p->count + 1;
		}
		else
			wow[p->i][p->k].color = 40000;
		p->k++;
	}
}

int		**coords_z(t_map *scroll, t_color **wow)
{
	int		**ans;
	t_joke	*p;

	p = (t_joke*)malloc(sizeof(t_joke));
	ans = (int**)malloc(sizeof(int*) * (scroll->row + 128000));
	p->i = -1;
	while (++p->i < scroll->row)
	{
		p->j = -1;
		p->k = 0;
		ans[p->i] = (int*)malloc(sizeof(int) * (scroll->column + 128000));
		wow[p->i] = (t_color*)malloc(sizeof(t_color) *
				(scroll->column + 128000));
		while (scroll->map[p->i][++p->j])
			coords_z_norm(ans, p, scroll, wow);
		ans[p->i][p->k] = '\0';
	}
	ans[p->i] = NULL;
	ft_memdel((void**)&p);
	return (ans);
}

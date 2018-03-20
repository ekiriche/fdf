/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seek_and_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 13:42:42 by ekiriche          #+#    #+#             */
/*   Updated: 2018/03/20 13:44:43 by ekiriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

void	count_column_norm(int *j, int *i, char **str, int *ans)
{
	while (str[*j][*i])
	{
		if (str[*j][*i] != ' ')
			*ans += 1;
		while (str[*j][*i] != ' ' && str[*j][*i])
			*i += 1;
		if (str[*j][*i] == '\0')
			break ;
		*i += 1;
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
		count_column_norm(&j, &i, str, &ans);
		if (check != 0 && check != ans)
			return (-1);
		check = ans;
		ans = 0;
		j++;
	}
	return (check);
}

int		count_row(char **str)
{
	int	ans;

	ans = 0;
	while (str[ans])
		ans++;
	return (ans);
}

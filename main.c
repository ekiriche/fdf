/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 14:35:57 by ekiriche          #+#    #+#             */
/*   Updated: 2018/02/21 18:25:09 by ekiriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <math.h>
#include <fcntl.h>
#include <stdlib.h>
#include "ft_printf.h"
#include "libft.h"

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

int		main(int argc, char **argv)
{
	void	*mlx_ptr;
	void	*mlx_win;
	int		fd;
	char	**map;

//	mlx_ptr = mlx_init();
//	mlx_win = mlx_new_window(mlx_ptr, 1200, 1000, "yay");
	fd = open("42", O_RDONLY);
	map = coords_from_file(fd);
//	mlx_key_hook(mlx_win, escape_key, (void *)0);
//	mlx_loop(mlx_ptr);
}

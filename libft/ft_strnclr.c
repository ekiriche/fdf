/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnclr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekiriche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/02 17:39:25 by ekiriche          #+#    #+#             */
/*   Updated: 2018/02/07 16:36:28 by ekiriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strnclr(char *str, int n)
{
	while (str[n])
	{
		str[n] = '\0';
		n++;
	}
}
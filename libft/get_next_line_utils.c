/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nadgalle <nadgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:13:46 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/08 17:24:10 by nadgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_chardup(char *res)
{
	res = malloc(sizeof(char));
	if (!res)
		return (NULL);
	res[0] = '\0';
	return (res);
}

char	*ft_fetch_next_line(char *buffer, size_t l_len)
{
	char	*next_line;

	next_line = malloc(l_len + 1);
	if (!next_line)
		return (NULL);
	ft_memmove(next_line, buffer, l_len);
	next_line[l_len] = '\0';
	return (next_line);
}

char	*ft_init_lines(char *lines)
{
	if (!lines)
	{
		lines = ft_chardup("");
		if (!lines)
			return (NULL);
	}
	return (lines);
}

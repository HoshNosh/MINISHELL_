/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:50:15 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/30 12:32:56 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

char	*ft_strdup(const char *s1)
{
	char	*s1p;
	size_t	len;
	size_t	i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	s1p = malloc(sizeof(char) * (len + 1));
	if (!s1p)
		return (NULL);
	i = 0;
	while (i < len)
	{
		s1p[i] = s1[i];
		i++;
	}
	s1p[len] = '\0';
	return (s1p);
}

/*
char	*ft_strdup(const char *s1)
{
	char	*s1p;
	size_t	len;

	len = ft_strlen(s1);
	s1p = (char *)malloc(sizeof(char) * (len + 1));
	if (!s1p)
		return (NULL);
	ft_memcpy(s1p, s1, len);
	s1p[len] = '\0';
	return (s1p);
}
*/

/*char	*ft_strdup(const char *src)
{
	size_t	i;
	char	*copy;

	copy = malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (src[i])
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

#include <stdio.h>
int main() {
    char str[] = "Hello, world!";
    char *dup = ft_strdup(str);
    printf("Duplicated string: %s\n", dup);
    free(dup);
    return 0;
}*/

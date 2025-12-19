/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:24:38 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/08 14:43:02 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dstp;
	unsigned const char	*srcp;

	if (n == 0)
		return (dst);
	dstp = (unsigned char *)dst;
	srcp = (unsigned const char *)src;
	while (n--)
	{
		*dstp++ = *srcp++;
	}
	return (dst);
}

/* void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dstp;
	unsigned const char	*srcp;

	if (dst == NULL && src == NULL)
		return (NULL);
	dstp = (unsigned char *)dst;
	srcp = (unsigned char *)src;
	while (n--)
	{
		*dstp++ = *srcp++;
	}
	return (dst);
} */

/*void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

#include <stdio.h>
#include "libft.h"

int main() {
    char src[] = "Hello";
    char dest[10];
    ft_memcpy(dest, src, 6);
    printf("Copied string: %s\n", dest);
    return 0;
}*/

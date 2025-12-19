/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:26:10 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/08 14:53:40 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	copy_len;
	char	*pdst;

	if (dstsize == 0)
		return (ft_strlen(src));
	src_len = ft_strlen(src);
	pdst = dst;
	while (*pdst != '\0' && (size_t)(pdst - dst) < dstsize)
		pdst++;
	dst_len = pdst - dst;
	if (dst_len >= dstsize)
		return (dstsize + src_len);
	copy_len = dstsize - dst_len - 1;
	if (copy_len > src_len)
		copy_len = src_len;
	ft_memcpy(pdst, src, copy_len);
	pdst[copy_len] = '\0';
	return (dst_len + src_len);
}

/*static void	ft_crash_func(void)
{
	int	*ptr;

	ptr = NULL;
	*ptr = 42;
}

 size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	copy_len;
	char	*pdst;

	if (!dst || !src)
		ft_crash_func();
	src_len = ft_strlen(src);
	pdst = dst;
	while (*pdst && (size_t)(pdst - dst) < dstsize)
		pdst++;
	dst_len = pdst - dst;
	if (dst_len >= dstsize)
		return (dstsize + src_len);
	copy_len = dstsize - dst_len - 1;
	if (copy_len > src_len)
		copy_len = src_len;
	ft_memcpy(pdst, src, copy_len);
	pdst[copy_len] = '\0';
	return (dst_len + src_len);
} */

/*size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;

	src_len = ft_strlen(src);
	dest_len = ft_strlen(dest);
	if (size == 0)
		return (src_len);
	if (size <= dest_len)
		return (size + src_len);
	i = 0;
	while (src[i] && (dest_len + i + 1) < size)
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest_len + src_len);
}

int main(void)
{
	char dest [13] = "Hello";
	char src [6] = " world";
ft_strlcat(dest, src, 12);
	printf("%s", dest);
}*/

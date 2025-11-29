/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 14:14:42 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/28 21:59:57 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_parse_flags(char **tokens, size_t *i, int *print_newline)
{
	size_t	b;

	*i = 1;
	*print_newline = 1;
	while (tokens[*i] && tokens[*i][0] == '-' && tokens[*i][1] == 'n')
	{
		b = 1;
		while (tokens[*i][b] == 'n')
			b++;
		if (tokens[*i][b] != '\0')
			break ;
		*print_newline = 0;
		(*i)++;
	}
}

void	ft_print_tok(char **tokens, size_t i, int output_fd)
{
	while (tokens[i])
	{
		if (output_fd != 0)
		{
			ft_putstr_fd(tokens[i], output_fd);
			if (tokens[i + 1])
				ft_putstr_fd(" ", output_fd);
		}
		else
		{
			printf("%s", tokens[i]);
			if (tokens[i + 1])
				printf(" ");
		}
		i++;
	}
}

void	ft_echo(char **tokens, int output_fd)
{
	size_t	i;
	int		print_newline;

	ft_parse_flags(tokens, &i, &print_newline);
	ft_print_tok(tokens, i, output_fd);
	if (print_newline)
	{
		if (output_fd != 0)
			ft_putstr_fd("\n", output_fd);
		else
			printf("\n");
	}
}

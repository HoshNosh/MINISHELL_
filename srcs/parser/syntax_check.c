/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:17:19 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 11:16:30 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
** Check que les pipes ne sont pas en 1ère/dernière pos ou consécutifs.
** Check qu'un pipe n'est pas le 1er token de la cmd.
** Return 1 si la syntaxe est valide, sinon 0.
*/
int	check_pipes(char **tokens)
{
	int	i;

	if (!tokens || !tokens[0])
		return (1);
	if (ft_strcmp(tokens[0], "|") == 0)
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			if (!tokens[i + 1] || ft_strcmp(tokens[i + 1], "|") == 0)
				return (0);
		}
		i++;
	}
	return (1);
}

/*
** Check syntaxe générale du tab de tokens. Fait ttes les vérif syntaxiques
** nécessaires. Return 1 si la syntaxe est correcte, sinon 0.
*/
int	syntax_check(char **tokens)
{
	if (!tokens)
		return (0);
	if (!check_pipes(tokens))
		return (0);
	return (1);
}

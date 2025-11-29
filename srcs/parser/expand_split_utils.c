/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:51:23 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 21:13:57 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer.h"

/*
** Split un token sur les caractères IFS et ajoute les mots au tableau.
** Return le nouvel index ou -1 en cas d'erreur.
*/
int	split_and_add_tokens(char **exp, int j, char *temp, int max)
{
	char	**split_result;

	split_result = split_on_ifs(temp);
	if (split_result)
	{
		j = add_split_words(split_result, exp, j, max);
		free_tokens(split_result);
		if (j == -1)
		{
			free(temp);
			return (-1);
		}
	}
	free(temp);
	return (j);
}

/*
** Compte le nombre de tokens après split sur IFS.
** Return le nombre total de mots.
*/
int	count_single_expanded(char *temp)
{
	char	**split;
	int		k;
	int		count;

	split = split_on_ifs(temp);
	if (!split)
		return (0);
	k = 0;
	count = 0;
	while (split[k])
	{
		if (split[k][0])
			count++;
		k++;
	}
	free_tokens(split);
	return (count);
}

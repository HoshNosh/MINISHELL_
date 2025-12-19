/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_markers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:14:11 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/06 16:21:10 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer.h"

/*
** Nettoie le marker final \x01 d'une chaîne.
** Décale tous les caractères d'une position.
*/
char	*clean_final_marker(char *str)
{
	char	*clean;
	int		i;
	int		j;

	if (!str)
		return (str);
	if (str[0] != '\x01')
		return (str);
	clean = malloc(ft_strlen(str));
	if (!clean)
		return (str);
	i = 1;
	j = 0;
	while (str[i])
		clean[j++] = str[i++];
	clean[j] = '\0';
	free(str);
	return (clean);
}

/*
** Vérifie si un résultat nettoyé est un opérateur à protéger.
** Return 1 pour |, <, >, <<, >>, 0 sinon.
*/
static int	is_operator_escaped(char *result)
{
	if (ft_strcmp(result, "|") == 0)
		return (1);
	if (ft_strcmp(result, "<") == 0)
		return (1);
	if (ft_strcmp(result, ">") == 0)
		return (1);
	if (ft_strcmp(result, "<<") == 0)
		return (1);
	if (ft_strcmp(result, ">>") == 0)
		return (1);
	return (0);
}

/*
** Supprime tous les markers d'échappement d'une chaîne.
** Return 1 si des markers ont été trouvés, 0 sinon.
*/
static int	remove_markers(char *result)
{
	int	i;
	int	j;
	int	has_marker;

	i = 0;
	j = 0;
	has_marker = 0;
	while (result[i])
	{
		if (result[i] == '\x02' || result[i] == '\x03'
			|| result[i] == '\x04')
		{
			has_marker = 1;
			i++;
		}
		else
			result[j++] = result[i++];
	}
	result[j] = '\0';
	return (has_marker);
}

/*
**  Nettoie les markers et protège les opérateurs si nécessaire.
** Ajoute \x01 devant les opérateurs échappés.
*/
char	*clean_escape_markers(char *result)
{
	int		has_marker;
	char	*protected;
	int		len;

	has_marker = remove_markers(result);
	if (has_marker && is_operator_escaped(result))
	{
		len = ft_strlen(result);
		protected = malloc(len + 2);
		if (!protected)
			return (result);
		protected[0] = '\x01';
		ft_strlcpy(protected + 1, result, len + 1);
		free(result);
		return (protected);
	}
	return (result);
}

/*
**  Compte les tokens générés par un token avec markers.
** Return toujours 1 (placeholder).
*/
int	count_tokens_with_markers(char *temp)
{
	(void)temp;
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
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
** Vérifie si une chaîne contient des caractères IFS.
** IFS = espace, tab, newline.
*/
int	has_ifs_chars(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

/*
** Compte le nombre de mots séparés par IFS.
** Return le nombre de mots trouvés.
*/
static int	count_ifs_words(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
			i++;
		if (str[i])
		{
			count++;
			while (str[i] && str[i] != ' ' && str[i] != '\t'
				&& str[i] != '\n')
				i++;
		}
	}
	return (count);
}

/*
** Remplit le tableau result avec les mots extraits.
** Chaque mot est alloué avec ft_substr.
*/
static void	fill_split_result(char **result, char *str)
{
	int	i;
	int	count;
	int	start;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
			i++;
		if (str[i])
		{
			start = i;
			while (str[i] && str[i] != ' ' && str[i] != '\t'
				&& str[i] != '\n')
				i++;
			result[count] = ft_substr(str, start, i - start);
			count++;
		}
	}
	result[count] = NULL;
}

/*
** Divise une chaîne sur les caractères IFS.
** Return un tableau de mots ou NULL.
*/
char	**split_on_ifs(char *str)
{
	char	**result;
	int		count;

	if (!str)
		return (NULL);
	count = count_ifs_words(str);
	if (count == 0)
		return (NULL);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	fill_split_result(result, str);
	return (result);
}

/*
** Ajoute chaque mot splitté au tableau expandé.
** Nettoie les markers d'échappement de chaque mot.
*/
int	add_split_words(char **split_result, char **exp, int j, int max)
{
	int	k;

	k = 0;
	while (split_result[k])
	{
		if (split_result[k][0])
		{
			if (j >= max)
				return (-1);
			exp[j] = ft_strdup(split_result[k]);
			exp[j] = clean_escape_markers(exp[j]);
			j++;
		}
		k++;
	}
	return (j);
}

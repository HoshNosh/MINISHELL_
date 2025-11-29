/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_varname.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:51:23 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 20:51:21 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

/*
** Calcule la longueur d'un nom de variable avec markers.
** Skip les markers \x04 en comptant les vrais caractères.
*/
int	calculate_var_len_with_markers(char *str, int start, int var_name_len)
{
	int	len;
	int	count;

	len = 0;
	count = 0;
	while (str[start + len] && count < var_name_len)
	{
		if (str[start + len] == '\x04')
			len++;
		else
		{
			len++;
			count++;
		}
	}
	return (len);
}

/*
** Extrait le nom d'une variable à partir d'une position.
** Gère les cas spéciaux et les markers.
*/
char	*get_var_name(char *str, int start)
{
	char	*special_case;
	int		len;
	int		dollar_in_quotes;

	if (!str || !str[start])
		return (NULL);
	dollar_in_quotes = (start >= 2 && str[start - 2] == '\x04');
	if (dollar_in_quotes && str[start] != '\x04')
		return (NULL);
	if (!dollar_in_quotes && str[start] == '\x04')
		return (ft_strdup(""));
	special_case = handle_special_var_cases(str, start);
	if (special_case)
		return (special_case);
	while (str[start] == '\x04')
		start++;
	if (!ft_isalpha(str[start]) && str[start] != '_')
		return (NULL);
	len = count_varname_len(str, start);
	return (extract_varname(str, start, len));
}

/*
** Vérifie si on doit continuer à compter le nom de variable.
** Return 1 pour continuer, 0 pour arrêter.
*/
static int	should_continue_varname(char *str, int pos, int in_quotes,
		int prev_was_marker)
{
	if (str[pos] == '\x03')
		return (0);
	if (str[pos] == '\x04')
		return (1);
	if (ft_isalnum(str[pos]) || str[pos] == '_')
	{
		if (in_quotes && !prev_was_marker)
			return (0);
		return (1);
	}
	return (0);
}

/*
** Compte la longueur d'un nom de variable avec ses markers.
** Gère les quotes et les markers d'échappement.
*/
int	count_varname_len(char *str, int start)
{
	int	len;
	int	in_quotes;
	int	prev_was_marker;

	len = 0;
	in_quotes = (start > 0 && str[start - 1] == '\x04');
	prev_was_marker = in_quotes;
	while (str[start + len])
	{
		if (!should_continue_varname(str, start + len, in_quotes,
				prev_was_marker))
			break ;
		if (str[start + len] == '\x04')
			prev_was_marker = 1;
		else
			prev_was_marker = 0;
		len++;
	}
	return (len);
}

/*
** Vérifie si un token est une assignation (contient '=').
** Return 1 si assignation, 0 sinon.
*/
int	is_assignment(char *temp)
{
	int	i;

	i = 0;
	while (temp[i] && temp[i] != '\x02' && temp[i] != '\x04')
	{
		if (temp[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

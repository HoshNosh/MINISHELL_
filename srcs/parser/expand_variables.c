/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:50:31 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/06 16:18:48 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer.h"

/*
** Joint trois chaînes en une seule.
** Return la chaîne résultante ou NULL si erreur.
*/
char	*join_three_strings(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	if (!s1 || !s2 || !s3)
		return (NULL);
	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	return (result);
}

/*
** Remplace une variable dans une chaîne par sa valeur.
** Découpe avant/après et reconstruit la chaîne.
*/
char	*replace_variable(char *str, int start, int len, char *value)
{
	char	*before;
	char	*after;
	char	*result;

	if (!str || !value)
		return (str);
	before = ft_substr(str, 0, start);
	after = ft_strdup(str + start + len);
	if (!before || !after)
	{
		free(before);
		free(after);
		return (str);
	}
	result = join_three_strings(before, value, after);
	free(before);
	free(after);
	if (!result)
		return (str);
	return (result);
}

/*
** Récupère la valeur d'une variable d'environnement.
** Gère les cas spéciaux: $?, $UID, $EUID.
*/
char	*get_variable_value(char *var_name, t_expand_ctx *ctx)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(ctx->last_exit_code));
	if (ft_strcmp(var_name, "$") == 0 || ft_strcmp(var_name, "$$") == 0)
		return (ft_itoa(getpid()));
	if (ft_strcmp(var_name, "UID") == 0)
		return (ft_itoa(getuid()));
	if (ft_strcmp(var_name, "EUID") == 0)
		return (ft_itoa(geteuid()));
	value = get_env_value(var_name, ctx->env);
	if (!value && (ft_strcmp(var_name, "USER") == 0
			|| ft_strcmp(var_name, "HOME") == 0
			|| ft_strcmp(var_name, "PWD") == 0))
		return (ft_strdup(""));
	return (value);
}

/*
** Traite une seule variable dans un token.
** Extrait le nom, récupère la valeur et remplace.
*/
int	process_single_var(char **result, int i, t_expand_ctx *ctx)
{
	char	*var_name;
	char	*var_value;
	char	*new_result;
	int		var_len;

	var_name = get_var_name(*result, i + 1);
	if (!var_name)
		return (i + 1);
	var_value = get_variable_value(var_name, ctx);
	var_len = calculate_var_len_with_markers(*result, i + 1,
			ft_strlen(var_name)) + 1;
	new_result = replace_variable(*result, i, var_len, var_value);
	if (new_result != *result)
	{
		free(*result);
		*result = new_result;
		i += ft_strlen(var_value);
	}
	else
		i++;
	free(var_name);
	free(var_value);
	return (i);
}

/*
**Traite toutes les variables d'un token.
** Remplace chaque $ trouvé hors single quotes.
*/
char	*process_token_variables(char *result, t_expand_ctx *ctx)
{
	int	i;

	i = 0;
	while (result[i])
	{
		if (result[i] == '$' && result[i + 1]
			&& !(i > 0 && result[i - 1] == '\x02'))
		{
			i = process_single_var(&result, i, ctx);
		}
		else
			i++;
	}
	return (result);
}

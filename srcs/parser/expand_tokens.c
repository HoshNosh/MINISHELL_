/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:50:31 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 19:33:36 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer.h"

/*
** Expanse le tilde (~) en répertoire HOME.
** Gère ~ seul ou suivi de /.
*/
static char	*expand_tilde(char *token, t_expand_ctx *ctx)
{
	char	*home;
	char	*result;

	if (!token || token[0] != '~')
		return (ft_strdup(token));
	if (token[0] == '~' && (token[1] == '\0' || token[1] == '/'))
	{
		home = get_env_value("HOME", ctx->env);
		if (!home)
			return (ft_strdup(token));
		if (token[1] == '\0')
			result = ft_strdup(home);
		else
			result = ft_strjoin(home, token + 1);
		free(home);
		return (result);
	}
	return (ft_strdup(token));
}

/*
** Expanse un token unique en traitant les variables.
** Gère les tokens spéciaux (commençant par \x01).
*/
char	*expand_token(char *token, t_expand_ctx *ctx)
{
	char	*result;

	if (token && token[0] == '\x01')
		return (ft_strdup(token + 1));
	if (!token)
		return (NULL);
	result = expand_tilde(token, ctx);
	if (!result)
		return (NULL);
	return (process_token_variables(result, ctx));
}

/*
** Vérifie si un token contient des markers d'échappement.
** Return 1 si markers trouvés, 0 sinon.
*/
static int	has_markers(char *temp)
{
	int	i;

	i = 0;
	while (temp[i])
	{
		if (temp[i] == '\x02' || temp[i] == '\x04')
			return (1);
		i++;
	}
	return (0);
}

/*
** Traite un token simple (avec word splitting si besoin).
*/
static int	handle_simple(t_expand_args *args)
{
	if (!args->has_q && args->temp[0] && has_ifs_chars(args->temp)
		&& !is_assignment(args->temp))
		return (split_and_add_tokens(args->exp, args->j, args->temp,
				args->max));
	if (args->temp[0] || args->has_q)
	{
		if (args->j >= args->max)
			return (free(args->temp), -1);
		args->exp[args->j++] = clean_escape_markers(args->temp);
		return (args->j);
	}
	free(args->temp);
	return (args->j);
}

/*
** Traite un token après expansion et l'ajoute au tableau.
** Gère le word splitting et les markers d'échappement.
*/
int	process_expanded_token(t_expand_args *args)
{
	int	result;

	if (has_markers(args->temp) && !is_assignment(args->temp))
	{
		result = process_token_by_segments(args->exp, args->j, args->temp,
				args->max);
		free(args->temp);
		return (result);
	}
	return (handle_simple(args));
}

/*
** Traite un token simple sans markers.
** Return le nouvel index ou -1 si erreur.
*
static int	process_simple_token(char **exp, int j, char *temp,
		int has_q, int max)
{
	if (!has_q && temp[0] && has_ifs_chars(temp) && !is_assignment(temp))
		return (split_and_add_tokens(exp, j, temp, max));
	else if (temp[0] || has_q)
	{
		if (j >= max)
		{
			free(temp);
			return (-1);
		}
		exp[j] = clean_escape_markers(temp);
		j++;
	}
	else
		free(temp);
	return (j);
}*/

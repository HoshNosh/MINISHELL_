/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:14:11 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 19:52:06 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer.h"

/*
** Compte le nombre de tokens dans un tableau.
** Parcourt jusqu'au premier pointeur NULL.
*/
int	count_tokens_for_expansion(char **tokens)
{
	int	token_count;

	token_count = 0;
	while (tokens[token_count])
		token_count++;
	return (token_count);
}

/*
** Vérifie si un token contient des quotes ou markers.
** Return 1 si trouvé, 0 sinon.
*/
static int	check_has_quotes(char *token)
{
	if (ft_strchr(token, '\'') || ft_strchr(token, '"')
		|| ft_strchr(token, '\x02') || ft_strchr(token, '\x03')
		|| ft_strchr(token, '\x04'))
		return (1);
	return (0);
}

/*
** Compte le nombre de tokens après expansion.
** Gère les markers et le splitting sur IFS.
*/
static int	count_expanded_tokens(char **tokens, t_expand_ctx *ctx)
{
	char	*temp;
	int		i;
	int		count;
	int		has_marker;

	i = -1;
	count = 0;
	while (tokens[++i])
	{
		temp = expand_token(tokens[i], ctx);
		if (!temp)
			return (-1);
		has_marker = (ft_strchr(temp, '\x02') || ft_strchr(temp, '\x04'));
		if (has_marker)
			count += count_tokens_with_markers(temp);
		else if (temp[0] && has_ifs_chars(temp))
			count += count_single_expanded(temp);
		else if (temp[0])
			count++;
		free(temp);
	}
	return (count);
}

/*
** Boucle principale de traitement des tokens.
** Expanse et traite chaque token individuellement.
*/
static int	process_token_loop(char **tokens, char **expanded,
		t_expand_ctx *ctx, int max_tokens)
{
	char			*temp;
	int				i;
	int				j;
	int				has_q;
	t_expand_args	args;

	i = -1;
	j = 0;
	while (tokens[++i])
	{
		has_q = check_has_quotes(tokens[i]);
		temp = expand_token(tokens[i], ctx);
		if (!temp)
			return (-1);
		args.exp = expanded;
		args.j = j;
		args.temp = temp;
		args.has_q = has_q;
		args.max = max_tokens;
		j = process_expanded_token(&args);
		if (j == -1 || j >= max_tokens)
			return (-1);
	}
	return (j);
}

/*
** Fonction principale d'expansion de tokens.
** Alloue et remplit le tableau de tokens expansés.
*/
char	**expand_tokens(char **tokens, t_expand_ctx *ctx)
{
	char	**expanded;
	int		j;
	int		max_tokens;

	if (!tokens || !ctx)
		return (NULL);
	max_tokens = count_expanded_tokens(tokens, ctx);
	if (max_tokens == -1)
		return (NULL);
	max_tokens += 5;
	expanded = ft_calloc(sizeof(char *), max_tokens + 1);
	if (!expanded)
		return (NULL);
	j = process_token_loop(tokens, expanded, ctx, max_tokens);
	if (j == -1)
		return (free_tokens(expanded), NULL);
	expanded[j] = NULL;
	return (expanded);
}

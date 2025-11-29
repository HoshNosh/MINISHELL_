/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 03:59:22 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 19:05:37 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"

/*
** Compte le nombre de tokens utiles (non liés aux redirections).
** Saute les opérateurs et leurs cibles pour compter seulement les args.
*/
int	count_argv_tokens(char **tokens, int start, int end)
{
	int	i;
	int	count;
	int	redir_type;

	i = start;
	count = 0;
	while (i < end)
	{
		redir_type = is_redirection(tokens[i]);
		if (redir_type)
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

/*
** Libère les éléments déjà alloués de argv en cas d'erreur.
** Parcourt argv de count-1 à 0 et free chaque élément.
*/
static void	free_partial_argv(char **argv, int count)
{
	while (count > 0)
	{
		count--;
		free(argv[count]);
	}
}

/*
** Copie dans argv uniquement les tokens non-redirections.
** Les opérateurs et leurs cibles sont ignorés.
*/
int	fill_argv(char **tokens, int start, int end, char **argv)
{
	int	i;
	int	argv_index;
	int	redir_type;

	i = start;
	argv_index = 0;
	while (i < end)
	{
		redir_type = is_redirection(tokens[i]);
		if (redir_type)
			i += 2;
		else
		{
			argv[argv_index] = ft_strdup(tokens[i]);
			if (!argv[argv_index])
				return (free_partial_argv(argv, argv_index), -1);
			argv_index++;
			i++;
		}
	}
	argv[argv_index] = NULL;
	return (0);
}

/*
** Traite une seule redirection et l'ajoute à la liste.
** Valide la syntaxe et crée le nœud de redirection.
*/
static int	process_single_redirection(char **tokens, int *i, int end,
		t_redirect **redir)
{
	t_redirect	*new_redir;
	int			redir_type;

	redir_type = is_redirection(tokens[*i]);
	if (!redir_type)
	{
		(*i)++;
		return (0);
	}
	if (*i + 1 >= end || !tokens[*i + 1])
		return (-1);
	new_redir = new_redirection(redir_type, tokens[*i + 1], tokens[*i]);
	if (!new_redir)
	{
		free_redirections(*redir);
		*redir = NULL;
		return (-1);
	}
	add_redirect(redir, new_redir);
	*i += 2;
	return (0);
}

/*
** Parcourt les tokens et construit la liste chaînée des redirections.
** Chaque opérateur est associé à son fichier cible (ex: ">" out.txt).
*/
int	extract_redir(char **tokens, int start, int end, t_redirect **redir)
{
	int	i;

	i = start;
	*redir = NULL;
	while (i < end)
	{
		if (process_single_redirection(tokens, &i, end, redir) == -1)
			return (-1);
	}
	return (0);
}

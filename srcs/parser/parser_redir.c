/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 03:59:22 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 18:56:48 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"

/*
** Extrait le numéro de fd du token de redirection (ex: "2>" retourne 2).
** Return 0 pour stdin, 1 pour stdout par défaut.
*/
static int	get_redir_fd(char *token, int type)
{
	int	fd;
	int	i;

	if (!token || !ft_isdigit(token[0]))
	{
		if (type == REDIR_IN || type == REDIR_HEREDOC)
			return (0);
		return (1);
	}
	fd = 0;
	i = 0;
	while (ft_isdigit(token[i]))
	{
		fd = fd * 10 + (token[i] - '0');
		i++;
	}
	return (fd);
}

/*
** Crée un nouveau nœud de redirection avec type et fichier cible.
** Alloue la struct et duplique le nom de fichier.
*/
t_redirect	*new_redirection(int type, char *filename, char *redir_token)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->fd = get_redir_fd(redir_token, type);
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

/*
** Ajoute une redirection à la fin de la liste chaînée.
** Maintient l'ordre des redirections.
*/
void	add_redirect(t_redirect **head, t_redirect *new_redir)
{
	t_redirect	*current;

	if (!head || !new_redir)
		return ;
	if (!*head)
	{
		*head = new_redir;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

/*
** Free récursivement toute la liste des redirections.
** Libère le filename et la struct de chaque nœud.
*/
void	free_redirections(t_redirect *redir)
{
	t_redirect	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
}

/*
** Parse les redirections et sépare les arguments du tableau argv.
** Compte et copie les tokens non-redirections dans argv.
*/
int	parse_redir(char **tokens, int start, int end, t_command *cmd)
{
	int	argv_count;

	argv_count = count_argv_tokens(tokens, start, end);
	cmd->argv = malloc(sizeof(char *) * (argv_count + 1));
	if (!cmd->argv)
		return (-1);
	if (fill_argv(tokens, start, end, cmd->argv) == -1)
	{
		free(cmd->argv);
		return (-1);
	}
	if (extract_redir(tokens, start, end, &cmd->redir) == -1)
	{
		free_tokens(cmd->argv);
		free_redirections(cmd->redir);
		return (-1);
	}
	return (argv_count);
}

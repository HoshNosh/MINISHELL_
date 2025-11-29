/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 03:59:34 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 10:49:54 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"

/*
** Check si token se termine par opérateur de redirection avec fd.
** Utilise les helpers pour vérifier input et output.
*/
int	check_fd_redir(char *token)
{
	int	len;
	int	result;

	len = ft_strlen(token);
	result = check_fd_redir_output(token, len);
	if (result)
		return (result);
	return (check_fd_redir_input(token, len));
}

/*
** Vérifie si un token contient des markers d'échappement.
** Return 1 si markers trouvés, 0 sinon.
*/
int	has_escape_markers_parser(char *token)
{
	int	i;

	if (!token)
		return (0);
	if (token[0] == '\x01')
		return (1);
	i = 0;
	while (token[i])
	{
		if (token[i] == '\x02' || token[i] == '\x03' || token[i] == '\x04')
			return (1);
		i++;
	}
	return (0);
}

/*
** Check si token est un opérateur de redirection.
** Compare avec tous les opérateurs de redir supportés.
*/
int	is_redirection(char *token)
{
	int	fd_redir;

	if (!token || has_escape_markers_parser(token))
		return (0);
	if (ft_strcmp(token, "<<") == 0)
		return (REDIR_HEREDOC);
	if (ft_strcmp(token, ">>") == 0)
		return (REDIR_APPEND);
	if (ft_strcmp(token, "<") == 0)
		return (REDIR_IN);
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">|") == 0)
		return (REDIR_OUT);
	fd_redir = check_fd_redir(token);
	if (fd_redir)
		return (fd_redir);
	return (0);
}

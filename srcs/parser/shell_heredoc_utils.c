/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:26:20 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 11:31:35 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "builtins.h"
#include "expand.h"
#include "lexer.h"
#include "parser.h"

/*
** Gestionnaire SIGINT spécifique pour les heredocs
** Ctrl-C doit interrompre la lecture du heredoc
*/
static void	heredoc_sig_handler(int sig)
{
	g_sigint_received = sig;
	write(1, "\n", 1);
}

/*
** Configure sigaction SANS SA_RESTART pour interrompre read().
*/
static void	setup_heredoc_signals(struct sigaction *old_action)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = heredoc_sig_handler;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old_action);
}

/*
** Trouve le dernier heredoc dans une liste de redirections.
*/
static t_redirect	*find_last_heredoc(t_redirect *redir)
{
	t_redirect	*last_heredoc;
	t_redirect	*r;

	last_heredoc = NULL;
	r = redir;
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
			last_heredoc = r;
		r = r->next;
	}
	return (last_heredoc);
}

/*
** Traite tous les heredocs d'une commande.
*/
int	process_command_heredocs(t_command *cmd, t_expand_ctx *ctx)
{
	t_redirect	*r;
	t_redirect	*last_heredoc;
	int			is_last;

	last_heredoc = find_last_heredoc(cmd->redir);
	r = cmd->redir;
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			if (r == last_heredoc)
				is_last = 1;
			else
				is_last = 0;
			if (process_heredoc_redir(r, &is_last, ctx) == -1)
				return (-1);
		}
		r = r->next;
	}
	return (0);
}

/*
** Parcourt l'AST et traite tous les heredocs avec sigaction
sans SA_RESTART.
*/
int	read_heredocs_before_exec(t_node *node, t_mother_shell *shell)
{
	struct sigaction	old_action;
	t_expand_ctx		ctx;
	int					result;

	ctx.env = shell->env;
	ctx.last_exit_code = shell->last_status;
	setup_heredoc_signals(&old_action);
	init_heredoc_counter(1);
	result = 0;
	if (node && node->type == NODE_COMMAND && node->command
		&& node->command->redir)
		result = process_command_heredocs(node->command, &ctx);
	else if (node && node->type == NODE_PIPE)
	{
		result = read_heredocs_before_exec(node->left, shell);
		if (result == 0)
			result = read_heredocs_before_exec(node->right, shell);
	}
	sigaction(SIGINT, &old_action, NULL);
	return (result);
}

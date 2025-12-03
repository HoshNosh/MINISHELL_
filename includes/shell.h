/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 01:04:04 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 21:34:54 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

#include "minishell.h"
#include "expand.h"

/* ========== shell_loop.c ========== */
char	*read_input(t_mother_shell *shell);
void	handle_signal_status(t_mother_shell *shell);
void	shell_loop(t_mother_shell *shell);

/* ========== shell_prepare.c ========== */
int		prepare_tokens(char *line, char ***expanded_tokens,
			t_mother_shell *shell);
int		process_line(char *line, t_mother_shell *shell);
int		execute_and_cleanup_ast(t_mother_shell *shell);

/* ========== shell_heredoc.c ========== */
int		init_heredoc_counter(int reset);
int		process_heredoc_redir(t_redirect *r, int *is_last, t_expand_ctx *ctx);
//int		process_heredoc_redir(t_redirect *r, int *is_last);

/* ========== shell_heredoc_utils.c ========== */
int		read_heredocs_before_exec(t_node *node, t_mother_shell *shell);
//int		read_heredocs_before_exec(t_node *node);
int		process_command_heredocs(t_command *cmd, t_expand_ctx *ctx);
//int		process_command_heredocs(t_command *cmd);
//void	heredoc_sig_handler(int sig);


#endif

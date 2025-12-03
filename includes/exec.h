/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 21:56:01 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/01 11:50:43 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "minishell.h"
#include "expand.h"


/* ========== exec_commande_utils.c ========== */


/* ========== exec_commande.c ========== */


/* ========== exec_heredoc_read.c ========== */


/* ========== exec_heredoc.c ========== */


/* ========== exec_path.c ========== */


/* ========== exec_pipe_utils.c ========== */


/* ========== exec_pipe.c ========== */


/* ========== exec_redir_handle.c ========== */


/* ========== exec_redir_inout.c ========== */


/* ========== exec_redir_validate.c ========== */


/* ========== exec_utils.c ========== */



/* ========== exec_utils2.c ========== */




/* ========== exec_variables.c ========== */

int	check_and_handle_assignment(t_node *node, t_mother_shell *shell);



/* EXEC HEREDOC AND READ */
int	read_heredoc_content(char *limiter_n, int tmpfile_fd, t_expand_ctx *ctx);
//int	read_heredoc_content(char *limiter_n, int tmpfile_fd);
int	get_heredoc(t_command *command, t_expand_ctx *ctx);
//int	get_heredoc(t_command *command);
char	*ft_get_heredoc_filename(int index);
//void	prepare_heredocs(t_node *node);
//void read_heredoc_content(char *limiter_n, int tmpfile_fd);
void	heredoc_sigint_handler(int sig);
//void	read_heredocs_before_exec(t_node *node);
void	print_eof_warning(char *limiter_n);

/* EXEX PATH */
void	ft_check_path(char *path, t_command *command);
char	**get_path_tab(char **env);
char	*ft_join_path(char *cmd, char *path);
char	*get_path(char *cmd, char **env);

/* EXEC REDIR HANDLE AND VALIDATE AND INOUT */
int		handle_redirections(t_redirect *redir);
void	close_validation_fds(int *fds, int count);
int	validate_redir_phase(t_redirect *tmp, t_redirect *last_heredoc);

void	infile_redirection(t_command *command);
void	outfile_truncate_redirection(t_command *command);
void	outfile_append_redirection(t_command *command);

//int	validate_and_store(t_redirect *redir, int *fds, int *count);
int	validate_all_redirections(t_redirect *redir, int **fds_out, int *count_out);

/* EXEC UTILS */
void	ft_exit_error(char *path, char *error, int code, t_command *cmd);
void	ft_free_command(t_command *command);
void	ft_exit_free(char *msg, int status, t_command *cmd);
void	close_inherited_fds(void);
void	setup_signal_handling(void);

/* EXEC COMMANDE AND PIPE */
int	execute_simple_command(t_node *node, t_mother_shell *shell);
int	execute_pipe(t_node *node, t_mother_shell *shell);
int	execute_ast(t_node *node, t_mother_shell *shell);


int	check_empty_or_special(t_node *node, t_mother_shell *shell);
int	handle_no_argv(t_node *node, t_mother_shell *shell);
void	handle_wait_status(t_mother_shell *shell, int status);
int	execute_builtin_with_redir(t_node *node, t_mother_shell *shell);

//void read_heredocs_before_exec(t_node *node);
int	check_redirections_validity(t_redirect *redir);

void	free_shell(t_mother_shell *shell);


/* ========== exec_variable.c ========== */
int		is_variable_assignment(char *token);
int		handle_variable_assignment(char *assignment, t_mother_shell *shell);
int	check_and_handle_assignment(t_node *node, t_mother_shell *shell);

#endif

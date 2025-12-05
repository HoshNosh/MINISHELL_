/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 21:56:01 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 12:37:22 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include "expand.h"

/* ========== exec_commande_utils.c ========== */
int		check_empty_or_special(t_node *node, t_mother_shell *shell);
int		handle_no_argv(t_node *node, t_mother_shell *shell);
void	handle_wait_status(t_mother_shell *shell, int status);
int		execute_builtin_with_redir(t_node *node, t_mother_shell *shell);

/* ========== exec_commande.c ========== */
void	child_exit_with_cleanup(t_node *node, t_mother_shell *shell,
			int status);
void	execute_builtin_child(t_node *node, t_mother_shell *shell);
void	execute_external_child(t_node *node, t_mother_shell *shell);
int		execute_forked_command(t_node *node, t_mother_shell *shell);
int		execute_simple_command(t_node *node, t_mother_shell *shell);

/* ========== exec_heredoc_read.c ========== */
int		read_heredoc_content(char *limiter_n, int tmpfile_fd,
			t_expand_ctx *ctx);

/* ========== exec_heredoc.c ========== */
char	*ft_get_heredoc_filename(int index);
int		get_heredoc(t_command *command, t_expand_ctx *ctx);
void	print_eof_warning(char *limiter_n);

/* ========== exec_path.c ========== */
void	ft_check_path(char *path, t_command *command);
char	**get_path_tab(char **env);
char	*ft_join_path(char *cmd, char *path);
char	*get_path(char *cmd, char **env);

/* ========== exec_pipe_utils.c ========== */
int		check_redirections_validity(t_redirect *redir);
int		execute_ast(t_node *node, t_mother_shell *shell);

/* ========== exec_pipe.c ========== */
int		execute_pipe(t_node *node, t_mother_shell *shell);

/* ========== exec_redir_handle.c ========== */
void	close_validation_fds(int *fds, int count);
int		handle_redirections(t_redirect *redir);

/* ========== exec_redir_inout.c ========== */
void	infile_redirection(t_command *command);
void	outfile_truncate_redirection(t_command *command);
void	outfile_append_redirection(t_command *command);

/* ========== exec_redir_validate.c ========== */
int		validate_all_redirections(t_redirect *redir, int **fds_out,
			int *count_out);
int		validate_redir_phase(t_redirect *tmp, t_redirect *last_heredoc);

/* ========== exec_utils.c ========== */
void	ft_exit_error(char *path, char *error, int code, t_command *cmd);
void	ft_exit_free(char *msg, int status, t_command *cmd);
void	ft_free_command(t_command *command);
void	close_inherited_fds(void);

/* ========== exec_utils2.c ========== */
void	ft_puterror(char *cmd, char *cmd2, char *error);
char	**ft_add_to_array(char **arr, char *str);
void	ft_free_tab(char **tab);

/* ========== exec_variables.c ========== */
int		is_variable_assignment(char *token);
int		handle_variable_assignment(char *assignment, t_mother_shell *shell);
int		check_and_handle_assignment(t_node *node, t_mother_shell *shell);

#endif

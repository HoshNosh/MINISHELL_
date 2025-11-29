/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 21:54:15 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 21:59:23 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "minishell.h"


int	ft_tabchr_home(char **env);
int	tab_size(char **tab);
void	ft_cd(char **tokens, int *exit_status, char ***envp);
void	ft_update_oldpwd_var(char ***envp, char *oldpwd);
void	ft_update_pwd_var(char ***envp);
void	ft_cd_invalid_opt(char **tokens, int *exit_code, char ***envp);
void	ft_parse_flags(char **tokens, size_t *i, int *print_newline);
void	ft_print_tok(char **tokens, size_t i, int output_fd);
void	ft_echo(char **tokens, int output_fd);
char		*ft_strdup2(char *src, int n);
int		ft_env(char **envp, int output_fd, char **argv);
char	**ft_minimal_env(char **envp);
void		ft_exit(char **tokens, int *exit_status, t_mother_shell *shell);
void	ft_print_export(char **env, int output_fd);
void	ft_swap(char **s1, char **s2);
char		**ft_sort_env(char **env, int output_fd);
int	ft_extract_key_value(char *token, char **key, char **value);
int	ft_replace_env_var(char **envp, char *key, char *value);
int	ft_add_new_env_var(char ***envp, char *key, char *value);
int	ft_check_identifier_single(char *token, int *exit_status);
char	**ft_export(char **envp, char **tokens, int *exit_status, int fd);
void	ft_pwd(int output_fd, char **tokens, int *exit_code);
char	**ft_unset(char **envp, char **tokens, int *exit_status);
int	ft_check_option(int *exit_status, char **tokens);
char	**ft_duplicate_env(char **envp);
int			ft_is_builtin(char *str);
void	ft_exec_builtin(char **tokens, t_builtin_ctx *ctx);
//void		ft_exec_builtin(char **tokens, char ***envp, int *exit, int fd, t_mother_shell *shell);
int	ft_error_builtin(char *path, char *error, int error_code, int *exit_code);
int	ft_check_path_builtin(char *path, int *exit_code);
int	ft_check_redirection(t_command *command, int *exit_code);


char	*ft_getenv(char **env, char *var);

void	ft_puterror(char *cmd, char *cmd2, char *error);
char **ft_add_to_array(char **arr, char *str);
void ft_free_tab(char **tab);

int	ft_create_env_entry(char ***envp, size_t size, char *key, char *value);
int	ft_process_export_token(char ***envp, char *token, int *exit_status);


#endif

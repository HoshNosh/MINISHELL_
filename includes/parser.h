/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:39:38 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 11:14:06 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H


/* ========== parser_fd.c ============== */
int			check_fd_redir(char *token);
int			has_escape_markers_parser(char *token);
int			is_redirection(char *token);


/* ========== parser_pipes.c ========== */
t_node		*parse_pipe_sequence(char **tokens, int start);



/* ========== parser_redir_utils.c ========== */
int			count_argv_tokens(char **tokens, int start, int end);
int			fill_argv(char **tokens, int start, int end, char **argv);
int			extract_redir(char **tokens, int start, int end,
				t_redirect **redir);



/* ========== parser_redir.c ========== */
t_redirect	*new_redirection(int type, char *filename, char *redir_token);
void		add_redirect(t_redirect **head, t_redirect *new_redir);
void		free_redirections(t_redirect *redir);
int			parse_redir(char **tokens, int start, int end, t_command *cmd);


/* ========== parser_utils.c ========== */
int			count_tokens_from(char **tokens, int start);
int			find_next_pipe(char **tokens, int start);
int			check_fd_redir_output(char *token, int len);
int			check_fd_redir_input(char *token, int len);


/* ========== parser.c ========== */
t_command	*new_command(char **tokens, int start, int end);
t_node		*add_command_to_tree(t_node *root, char **tokens, int start,
				int end);
t_node		*parse_tokens(char **tokens);


#endif

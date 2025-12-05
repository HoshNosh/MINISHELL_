/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:53:47 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 11:47:10 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
# include "expand.h"

typedef struct s_copy_ctx
{
	char	*line;
	char	*token;
	int		*i;
	int		*j;
	int		len;
}	t_copy_ctx;

/* ========== lexer_escape.c ========== */
int		handle_escape_in_quotes(char *line, int len, char quote);
int		count_quote_content(char *line, char quote);
void	process_quotes_in_token(char *line, char *token, int *i, int *j);

/* ========== lexer_token_copy.c ========== */
char	*copy_token(char *line, int len);

/* ========== lexer_token_utils.c ========== */
int		token_len(char *line);

/* ========== lexer_utils.c ========== */
int		handle_quotes_and_ops(char *line);
int		get_operator_len(char *line);
void	free_tokens(char **tokens);
char	**lexer_split(char *line);

/* ========== syntax_check.c ========== */
int		check_pipes(char **tokens);
int		syntax_check(char **tokens);

/* ========== syntax_validation.c ========== */
int		is_pipe(char *token);
int		validate_redirections(char **tokens);
int		validate_pipes(char **tokens);
int		validate_syntax(char **tokens);

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

/* ========== shell_heredoc_utils.c ========== */
int		read_heredocs_before_exec(t_node *node, t_mother_shell *shell);
int		process_command_heredocs(t_command *cmd, t_expand_ctx *ctx);

#endif

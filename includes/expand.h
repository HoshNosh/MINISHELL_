/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:52:35 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/28 20:47:25 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

typedef struct s_expand_ctx
{
	char	**env;
	int		last_exit_code;
}	t_expand_ctx;

typedef struct s_segment_ctx
{
	char	**exp;
	int		*j;
	int		max;
	char	**current;
}	t_segment_ctx;

typedef struct s_expand_args
{
	char	**exp;
	int		j;
	char	*temp;
	int		has_q;
	int		max;
}	t_expand_args;

/* ========== expand_core.c ========== */
int			count_tokens_for_expansion(char **tokens);
char		**expand_tokens(char **tokens, t_expand_ctx *ctx);

/* ========== expand_markers.c ========== */
char		*clean_final_marker(char *str);
char		*clean_escape_markers(char *result);
int			count_tokens_with_markers(char *temp);

/* ========== expand_segments_utils.c ========== */
int			process_token_by_segments(char **exp, int j, char *temp, int max);

/* ========== expand_segments.c ========== */
char		*append_piece(char *current, char *piece);
int			is_marker_char(char c);
int			is_quoted_char(char *temp, int index);
int			extract_next_segment(char *temp, int i, int *is_quoted);
int			process_quoted_segment(char *segment, char **current);

/* ========== expand_split_utils.c ========== */
int			split_and_add_tokens(char **exp, int j, char *temp, int max);
int			count_single_expanded(char *temp);

/* ========== expand_split.c ========== */
int			has_ifs_chars(char *str);
char		**split_on_ifs(char *str);
int			add_split_words(char **split_result, char **exp, int j, int max);

/* ========== expand_tokens.c ========== */
char		*expand_token(char *token, t_expand_ctx *ctx);
int			process_expanded_token(t_expand_args *args);

/* ========== expand_utils.c ========== */
int			has_variable(char *str);
char		*get_env_value(char *var_name, char **env);
int			is_in_single_quotes(char *str, int pos);
char		*handle_special_var_cases(char *str, int start);
char		*extract_varname(char *str, int start, int len);

/* ========== expand_variables.c ========== */
char		*join_three_strings(char *s1, char *s2, char *s3);
char		*replace_variable(char *str, int start, int len, char *value);
char		*get_variable_value(char *var_name, t_expand_ctx *ctx);
int			process_single_var(char **result, int i, t_expand_ctx *ctx);
char		*process_token_variables(char *result, t_expand_ctx *ctx);

/* ========== expand_varname.c ========== */
int			calculate_var_len_with_markers(char *str, int start, int vlen);
char		*get_var_name(char *str, int start);
int			count_varname_len(char *str, int start);
int			is_assignment(char *temp);

#endif

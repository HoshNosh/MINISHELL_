/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:53:47 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 19:21:31 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

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



#endif

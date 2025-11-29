/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_copy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 07:18:35 by sdossa            #+#    #+#             */
/*   Updated: 2025/11/29 19:52:28 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
** Traite un segment $'...' en ajoutant \x02 avant chaque caractère.
** Modifie i et j pour pointer après le segment traité.
*/
static void	handle_dollar_single(t_copy_ctx *ctx)
{
	*(ctx->i) += 2;
	while (*(ctx->i) < ctx->len && ctx->line[*(ctx->i)] != '\'')
	{
		ctx->token[(*ctx->j)++] = '\x02';
		ctx->token[(*ctx->j)++] = ctx->line[(*ctx->i)++];
	}
	if (ctx->line[*(ctx->i)] == '\'')
		(*ctx->i)++;
}

/*
** Traite $" ou quote normale et ajoute marker de fin si nécessaire.
** Modifie i et j pour pointer après le segment traité.
*/
static void	handle_quote_marker(t_copy_ctx *ctx)
{
	int	start_j;

	if (ctx->line[*(ctx->i)] == '$')
		(*ctx->i)++;
	start_j = *(ctx->j);
	process_quotes_in_token(ctx->line, ctx->token, ctx->i, ctx->j);
	if (*(ctx->j) == start_j)
		ctx->token[(*ctx->j)++] = '\x03';
	else if (*(ctx->i) < ctx->len && (ctx->line[*(ctx->i)] == '\''
			|| ctx->line[*(ctx->i)] == '"'))
		ctx->token[(*ctx->j)++] = '\x03';
}

/*
** Traite un backslash suivi d'un caractère.
** Échappe le $ si nécessaire.
*/
static void	handle_backslash(t_copy_ctx *ctx)
{
	(*ctx->i)++;
	if (ctx->line[*(ctx->i)] == '$')
		ctx->token[(*ctx->j)++] = '\x02';
	ctx->token[(*ctx->j)++] = ctx->line[(*ctx->i)++];
}

/*
** Boucle principale de copie d'un token.
** Gère tous les cas spéciaux (quotes, escape, etc.).
*/
static void	copy_loop(t_copy_ctx *ctx)
{
	while (*(ctx->i) < ctx->len)
	{
		if (ctx->line[*(ctx->i)] == '$' && ctx->line[*(ctx->i) + 1] == '\'')
			handle_dollar_single(ctx);
		else if (ctx->line[*(ctx->i)] == '$'
			&& ctx->line[*(ctx->i) + 1] == '"')
			handle_quote_marker(ctx);
		else if (ctx->line[*(ctx->i)] == '\'' || ctx->line[*(ctx->i)] == '"')
			handle_quote_marker(ctx);
		else if (ctx->line[*(ctx->i)] == '\\' && ctx->line[*(ctx->i) + 1])
			handle_backslash(ctx);
		else
			ctx->token[(*ctx->j)++] = ctx->line[(*ctx->i)++];
	}
	ctx->token[*(ctx->j)] = '\0';
}

/*
** Copie un token depuis la ligne de commande.
** Alloue et remplit le buffer avec gestion des quotes et escape.
*/
char	*copy_token(char *line, int len)
{
	t_copy_ctx	ctx;
	char		*token;
	int			i;
	int			j;

	token = malloc((len * 2) + 1);
	if (!token)
		return (NULL);
	i = 0;
	j = 0;
	ctx.line = line;
	ctx.token = token;
	ctx.i = &i;
	ctx.j = &j;
	ctx.len = len;
	copy_loop(&ctx);
	return (token);
}

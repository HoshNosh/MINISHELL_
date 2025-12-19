/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_segments_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:51:23 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/06 16:16:34 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer.h"

/*
** Ajoute un mot au tableau en gérant le contexte actuel.
** Return 0 si succès, -1 si erreur.
*/
static int	add_word(t_segment_ctx *ctx, char *word, int is_last, int ends_ifs)
{
	char	*piece;
	char	*old_current;

	piece = ft_strdup(word);
	if (!piece)
		return (-1);
	old_current = *ctx->current;
	if (*ctx->current)
		piece = append_piece(*ctx->current, piece);
	if (!piece && old_current)
		return (free(old_current), -1);
	if (!is_last || ends_ifs)
	{
		if (!piece || *ctx->j >= ctx->max)
			return (free(piece), -1);
		ctx->exp[(*ctx->j)++] = piece;
		*ctx->current = NULL;
	}
	else
		*ctx->current = piece;
	return (0);
}

/*
** Traite les mots d'un segment non-quoté avec word spliting.
** Return 0 si succès, -1 si erreur.
*/
static int	process_unquoted(char *segment, t_segment_ctx *ctx)
{
	char	**words;
	int		k;
	int		ends_with_ifs;
	int		len;

	if (!segment || !segment[0])
		return (0);
	ends_with_ifs = 0;
	len = ft_strlen(segment);
	if (len > 0 && (segment[len - 1] == ' ' || segment[len - 1] == '\t'
			|| segment[len - 1] == '\n'))
		ends_with_ifs = 1;
	words = ft_split(segment, ' ');
	if (!words)
		return (0);
	k = 0;
	while (words[k] && *ctx->j < ctx->max)
	{
		if (add_word(ctx, words[k], !words[k + 1], ends_with_ifs) == -1)
			return (free_tokens(words), -1);
		k++;
	}
	free_tokens(words);
	return (0);
}

/*
** Traite un seul segment (quoté ou non-quoté).
** Return 0 si succès, -1 si erreur.
*/
static int	process_segment(char *temp, int *i, t_segment_ctx *ctx)
{
	int		seg_len;
	int		is_quoted;
	char	*segment;

	seg_len = extract_next_segment(temp, *i, &is_quoted);
	if (seg_len <= 0)
		return (0);
	segment = ft_substr(temp, *i, seg_len);
	if (!segment)
		return (-1);
	if (is_quoted)
	{
		if (process_quoted_segment(segment, ctx->current) == -1)
			return (free(segment), -1);
	}
	else
	{
		if (process_unquoted(segment, ctx) == -1)
			return (free(segment), -1);
	}
	free(segment);
	*i += seg_len;
	return (0);
}

/*
** Finalise le traitement avec le current restant.
*/
static int	finalize_current(char *current, char **exp, int j, int max)
{
	if (current && j < max)
	{
		exp[j++] = current;
		return (j);
	}
	if (current)
		return (free(current), -1);
	return (j);
}

/*
** Traite un token segment par segment (quoté/non-quoté).
** Applique le word splitting uniquement sur les segments non-quotés.
*/
int	process_token_by_segments(char **exp, int j, char *temp, int max)
{
	t_segment_ctx	ctx;
	int				i;
	char			*current;

	ctx.exp = exp;
	ctx.j = &j;
	ctx.max = max;
	ctx.current = &current;
	i = 0;
	current = NULL;
	while (temp[i] && j < max)
	{
		if (temp[i] == '\x03')
		{
			i++;
			continue ;
		}
		if (process_segment(temp, &i, &ctx) == -1)
			return (-1);
	}
	return (finalize_current(current, exp, j, max));
}

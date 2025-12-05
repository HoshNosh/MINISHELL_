/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:26:20 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/05 11:32:46 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "builtins.h"
#include "expand.h"
#include "lexer.h"
#include "parser.h"

/*
** Compteur statique pour générer des noms de fichiers temporaires uniques.
** Reset si reset=1, sinon incrémente et retourne la valeur.
*/
int	init_heredoc_counter(int reset)
{
	static int	counter = 0;

	if (reset)
		counter = 0;
	return (counter++);
}

/*
** Crée et ouvre un fichier temporaire pour le heredoc.
** Return le fd ou -1 si erreur.
*/
static int	open_heredoc_file(char **tmp_filename)
{
	int	fd;

	*tmp_filename = ft_get_heredoc_filename(init_heredoc_counter(0));
	fd = open(*tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc");
		free(*tmp_filename);
		*tmp_filename = NULL;
	}
	return (fd);
}

/*
** Lit le contenu du heredoc et l'écrit dans le fichier.
** Gère le limiteur avec newline. Return 0 succès, -1 si CTRL-C.
*/
static int	write_heredoc_content(t_redirect *r, int fd, t_expand_ctx *ctx)
{
	char	*original_filename;
	char	*limiter;
	int		result;

	original_filename = ft_strdup(r->filename);
	if (!original_filename)
		return (-1);
	limiter = ft_strjoin(original_filename, "\n");
	free(original_filename);
	if (!limiter)
		return (-1);
	result = read_heredoc_content(limiter, fd, ctx);
	free(limiter);
	return (result);
}

/*
** Finalise le heredoc: assigne le fichier ou le supprime.
** Si dernier heredoc, remplace filename, sinon supprime le fichier.
*/
static void	finalize_heredoc(t_redirect *r, char *tmp, int is_last)
{
	if (is_last)
	{
		free(r->filename);
		r->filename = tmp;
	}
	else
	{
		unlink(tmp);
		free(tmp);
	}
}

/*
** Traite un heredoc: crée fichier temporaire, lit contenu, finalise.
** Comportement bash: tous les heredocs sont lus, seul le dernier est utilisé.
** Return 0 succès, -1 si CTRL-C.
*/
int	process_heredoc_redir(t_redirect *r, int *is_last, t_expand_ctx *ctx)
{
	char	*tmp;
	int		fd;
	int		result;

	fd = open_heredoc_file(&tmp);
	if (fd == -1)
		return (-1);
	result = write_heredoc_content(r, fd, ctx);
	close(fd);
	if (result == -1)
	{
		unlink(tmp);
		free(tmp);
		return (-1);
	}
	finalize_heredoc(r, tmp, *is_last);
	return (0);
}

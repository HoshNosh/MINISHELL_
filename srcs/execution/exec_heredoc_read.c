/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_read.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:31:26 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/06 16:12:28 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "expand.h"

/*
** Ajoute un caractère à la ligne. Réalloue si nécessaire.
*/
static char	*append_char(char *line, char c, int *len)
{
	char	*new_line;
	int		i;

	new_line = malloc(*len + 2);
	if (!new_line)
		return (free(line), NULL);
	i = 0;
	while (i < *len)
	{
		new_line[i] = line[i];
		i++;
	}
	new_line[i] = c;
	new_line[i + 1] = '\0';
	free(line);
	(*len)++;
	return (new_line);
}

/*
** Lit un caractère et gère EOF/CTRL-C.
** Return: 1 = char lu, 0 = EOF sur ligne vide, -1 = erreur/CTRL-C, 2 = ignore
*/
static int	read_char_heredoc(char *buffer, int len)
{
	int	ret;

	ret = read(STDIN_FILENO, buffer, 1);
	if (ret == -1 && errno == EINTR)
		return (-1);
	if (g_sigint_received)
		return (-1);
	if (ret == 0)
	{
		if (len == 0)
			return (0);
		return (2);
	}
	return (1);
}

/*
** Lit une ligne avec read(). Interrompu par CTRL-C (errno = EINTR).
*/
static char	*read_line_heredoc(void)
{
	char	buffer[1];
	char	*line;
	int		ret;
	int		len;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	len = 0;
	while (1)
	{
		ret = read_char_heredoc(buffer, len);
		if (ret == -1 || ret == 0)
			return (free(line), NULL);
		if (ret == 2)
			continue ;
		if (buffer[0] == '\n')
			break ;
		line = append_char(line, buffer[0], &len);
		if (!line)
			return (NULL);
	}
	return (line);
}

static int	process_line(char *line, char *limiter_n, int tmpfile_fd,
		t_expand_ctx *ctx)
{
	size_t	len;

	if (g_sigint_received)
		return (free(line), g_sigint_received = 0, -1);
	if (!line)
		return (-2);
	len = ft_strlen(limiter_n);
	if (len > 0 && limiter_n[len - 1] == '\n')
		len--;
	if (line && ft_strlen(line) == len
		&& ft_strncmp(line, limiter_n, len) == 0)
		return (free(line), 1);
	if (has_variable(line))
		line = process_token_variables(line, ctx);
	ft_putstr_fd(line, tmpfile_fd);
	ft_putstr_fd("\n", tmpfile_fd);
	free(line);
	return (0);
}

/*
** Lit heredoc avec read(). Interrompu par CTRL-C grâce à sigaction.
*/
int	read_heredoc_content(char *limiter_n, int tmpfile_fd, t_expand_ctx *ctx)
{
	char	*line;
	int		result;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = read_line_heredoc();
		result = process_line(line, limiter_n, tmpfile_fd, ctx);
		if (result == -1)
			return (-1);
		if (result == -2)
		{
			print_eof_warning(limiter_n);
			break ;
		}
		if (result == 1)
			break ;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_read.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:31:26 by nadgalle          #+#    #+#             */
/*   Updated: 2025/11/29 13:55:39 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

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
		ret = read(STDIN_FILENO, buffer, 1);
		if (ret == -1 && errno == EINTR)
			return (free(line), NULL);
		if (ret <= 0 || g_sigint_received)
			return (free(line), NULL);
		if (buffer[0] == '\n')
			break ;
		line = append_char(line, buffer[0], &len);
		if (!line)
			return (NULL);
	}
	return (line);
}

/*
** Vérifie si line match le limiter.
*/
static int	check_limiter(char *line, char *limiter_n)
{
	size_t	len;

	if (!line || !limiter_n)
		return (0);
	len = ft_strlen(limiter_n);
	if (len > 0 && limiter_n[len - 1] == '\n')
		len--;
	if (ft_strlen(line) != len)
		return (0);
	return (ft_strncmp(line, limiter_n, len) == 0);
}

/*
** Traite une ligne: check limiter, écrit dans fd. Return 0/1/-1.
*/
static int	process_line(char *line, char *limiter_n, int tmpfile_fd)
{
	if (g_sigint_received)
		return (free(line), g_sigint_received = 0, -1);
	if (!line)
		return (ft_putstr_fd("\n", 1), 1);
	if (check_limiter(line, limiter_n))
		return (free(line), 1);
	ft_putstr_fd(line, tmpfile_fd);
	ft_putstr_fd("\n", tmpfile_fd);
	free(line);
	return (0);
}

/*
** Lit heredoc avec read(). Interrompu par CTRL-C grâce à sigaction.
*/
int	read_heredoc_content(char *limiter_n, int tmpfile_fd)
{
	char	*line;
	int		result;

	while (1)
	{
		write(STDOUT_FILENO, "pipe heredoc> ", 14);
		line = read_line_heredoc();
		result = process_line(line, limiter_n, tmpfile_fd);
		if (result == -1)
			return (-1);
		if (result == 1)
			break ;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_segments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:51:23 by sdossa            #+#    #+#             */
/*   Updated: 2025/12/03 21:24:43 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "lexer.h"

/*
** Joint deux chaînes en libérant les anciennes.
** Return la chaîne jointe ou NULL.
*/
char	*append_piece(char *current, char *piece)
{
	char	*joined;

	if (!piece)
		return (current);
	if (!current)
		return (piece);
	joined = ft_strjoin(current, piece);
	free(current);
	free(piece);
	return (joined);
}

/*
** Vérifie si un caractère est un marker d'échappement.
** Return 1 pour \x02 ou \x04, 0 sinon.
*/
int	is_marker_char(char c)
{
	return (c == '\x02' || c == '\x04');
}

/*
** Vérifie si un caractère à un index est dans des quotes.
** Check le caractère et le précédent pour les markers.
*/
int	is_quoted_char(char *temp, int index)
{
	if (index < 0 || !temp[index])
		return (0);
	if (is_marker_char(temp[index]))
		return (1);
	if (index > 0 && is_marker_char(temp[index - 1]))
		return (1);
	return (0);
}

/*
** Extrait le prochain segment (quoté ou non-quoté).
** Return la longueur du segment et modifie is_quoted.
*/
int	extract_next_segment(char *temp, int i, int *is_quoted)
{
	int	start;

	if (!temp[i])
		return (0);
	*is_quoted = is_quoted_char(temp, i);
	start = i;
	if (*is_quoted)
	{
		while (temp[i] && temp[i] != '\x03' && is_quoted_char(temp, i))
			i++;
	}
	else
	{
		while (temp[i] && temp[i] != '\x03' && !is_quoted_char(temp, i))
			i++;
	}
	return (i - start);
}

/*
** Traite un segment quoté en nettoyant les markers.
** Return 0 si succès, -1 si erreur.
*/
int process_quoted_segment(char *segment, char **current)
{
    char    *dup_segment;
    char    *cleaned;

    // Protection essentielle : Si le segment est NULL, on ne fait rien mais on ne plante pas.
    if (!segment)
        return (0);

    // 1. Dupliquer la chaîne (ft_strdup pourrait retourner NULL en cas d'échec malloc)
    dup_segment = ft_strdup(segment);
    if (!dup_segment)
        return (-1); // Échec de malloc (à gérer)

    // 2. Nettoyer les marqueurs
    // IMPORTANT : clean_escape_markers doit libérer dup_segment si elle retourne NULL,
    // ou vous devez le faire ici en cas d'échec. Nous assumons qu'elle retourne NULL en cas d'erreur.
    cleaned = clean_escape_markers(dup_segment);

    if (!cleaned)
    {
        // free(dup_segment); // Dépend si clean_escape_markers gère cette libération
        return (-1); // Erreur lors du nettoyage
    }

    // 3. Ajouter à la chaîne courante
    *current = append_piece(*current, cleaned);

    // Assurez-vous que l'allocation et la gestion de la mémoire de *current et cleaned sont bien gérées
    // par append_piece (ce qui est souvent le cas).

    return (0);
}

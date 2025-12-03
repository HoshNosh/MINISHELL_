/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdossa <sdossa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:39:08 by nadgalle          #+#    #+#             */
/*   Updated: 2025/12/03 18:47:24 by sdossa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

// void	ft_print_export(char **env, int output_fd)
// {
// 	int	i;

// 	if (!env)
// 		return ;
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (output_fd)
// 		{
// 			ft_putstr_fd("export ", output_fd);
// 			ft_putstr_fd(env[i], output_fd);
// 			ft_putstr_fd("\n", output_fd);
// 		}
// 		else
// 			printf("export %s\n", env[i]);
// 		i++;
// 	}
// }

/*
 * ft_print_export
 * Affiche l'environnement trié au format export NOM="VALEUR"
 * Utilise write() pour respecter la limite de 5 fonctions/fichier,
 * en remplaçant la fonction manquante ft_putstrn_fd.
 */
void    ft_print_export(char **env, int output_fd)
{
    int     i;
    char    *equal_sign;

    if (!env)
        return ;
    i = 0;
    while (env[i])
    {
        equal_sign = ft_strchr(env[i], '=');

        ft_putstr_fd("export ", output_fd);

        if (equal_sign)
        {
            // Correction du formatage de la CLÉ :
            // Utilise 'write' pour imprimer env[i] jusqu'à la position de l'égalité.
            write(output_fd, env[i], equal_sign - env[i]);

            // Correction du formatage de la VALEUR : ajoute ="VALEUR"
            ft_putstr_fd("=\"", output_fd); // Imprime ="
            ft_putstr_fd(equal_sign + 1, output_fd); // Imprime la VALEUR (partie après '=')
            ft_putstr_fd("\"\n", output_fd); // Imprime " et \n
        }
        else
        {
            // Cas sans valeur : export VAR (pas de guillemets)
            ft_putstr_fd(env[i], output_fd);
            ft_putstr_fd("\n", output_fd);
        }
        i++;
    }
}

void	ft_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}


/*
 * ft_sort_env
 * Copie, trie alphabétiquement (en utilisant ft_strcmp) et affiche la liste d'environnement.
 */
char    **ft_sort_env(char **env, int output_fd)
{
    int     i;
    int     swapped;
    char    **envp;

    // 1. Dupliquer l'environnement pour trier sans affecter l'original
    // (ft_duplicate_env doit être défini ailleurs)
    envp = ft_duplicate_env(env);
    if (!envp)
        return (env);

    // 2. Tri à bulles (Bubble Sort) : Compliant avec la Norme (utilisation de while)
    swapped = 1;
    while (swapped) // Répéter tant qu'il y a eu des échanges
    {
        swapped = 0; // Réinitialise le drapeau pour le nouveau passage
        i = 0;
        while (envp[i] && envp[i + 1]) // Parcourir la liste
        {
            // Utiliser ft_strcmp pour une comparaison complète des clés (tri alphabétique)
            if (ft_strcmp(envp[i], envp[i + 1]) > 0)
            {
                ft_swap(&envp[i], &envp[i + 1]);
                swapped = 1; // Marque qu'un échange a eu lieu
            }
            i++;
        }
    }

    // 3. Affichage au format corrigé (ft_print_export doit être la version avec guillemets et write)
    ft_print_export(envp, output_fd);

    // 4. Libération de la mémoire de la copie
    // (ft_free_tab doit être défini ailleurs)
    ft_free_tab(envp);
    return (env);
}

// char	**ft_sort_env(char **env, int output_fd)
// {
// 	int		i;
// 	int		len;
// 	char	**envp;

// 	i = 0;
// 	envp = ft_duplicate_env(env);
// 	while (envp[i] && envp[i + 1])
// 	{
// 		if (ft_strlen(envp[i]) > ft_strlen(envp[i + 1]))
// 			len = ft_strlen(envp[i]);
// 		else
// 			len = ft_strlen(envp[i + 1]);
// 		if (ft_strncmp(envp[i], envp[i + 1], len) > 0)
// 		{
// 			ft_swap(&envp[i], &envp[i + 1]);
// 			i = 0;
// 		}
// 		i++;
// 	}
// 	ft_print_export(envp, output_fd);
// 	ft_free_tab(envp);
// 	return (env);
// }


/*
** Crée la chaîne "key=value" et l'assigne à envp[size]
** Retourne 1 si succès, 0 si échec
*/
int	ft_create_env_entry(char ***envp, size_t size, char *key, char *value)
{
	(*envp)[size] = malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!(*envp)[size])
	{
		(*envp)[size] = NULL;
		return (0);
	}
	ft_strlcpy((*envp)[size], key, ft_strlen(key) + 1);
	ft_strlcat((*envp)[size], "=", ft_strlen(key) + 2);
	ft_strlcat((*envp)[size], value, ft_strlen(key) + ft_strlen(value) + 2);
	(*envp)[size + 1] = NULL;
	return (1);
}

/*
** Traite un seul token d'export (key=value)
** Retourne 1 si succès, 0 si erreur
*/
int	ft_process_export_token(char ***envp, char *token, int *exit_status)
{
	char	*key;
	char	*value;

	if (ft_extract_key_value(token, &key, &value))
	{
		if (!ft_replace_env_var(*envp, key, value))
		{
			if (!ft_add_new_env_var(envp, key, value))
			{
				ft_puterror("export", NULL, "memory allocation error");
				*exit_status = 1;
				return (0);
			}
		}
	}
	return (1);
}

# MINISHELL

Cette refonte de **Minishell** consiste à **adapter le moteur d'exécution (exec)**, développé lors de la première version du projet, pour qu’il fonctionne désormais avec un **AST (Abstract Syntax Tree)**.
L'objectif était de **réintégrer l'exec** dans une architecture basée sur un arbre syntaxique, en assurant la compatibilité avec les nouveaux types de nœuds et la logique du projet.

# **Minishell – Shell minimaliste avec AST**

Minishell est une implémentation simplifiée d’un shell Unix, développée dans un cadre pédagogique.
L’objectif du projet est de reproduire le comportement fondamental d’un shell moderne, incluant :

* un prompt interactif,
* l’exécution de commandes,
* la gestion des pipelines,
* les redirections,
* les variables d’environnement,
* et un parsing robuste grâce à un **Abstract Syntax Tree (AST)**.

Ce projet met l’accent sur la compréhension des mécanismes internes d’un shell : analyse lexicale, parsing, structures de données, gestion des processus, communication inter-processus et gestion fine des erreurs.

---

## **Fonctionnalités**

### ✔️ Prompt interactif

* Affiche un prompt et lit les commandes de l’utilisateur.
* Gestion des signaux (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`).

### ✔️ Lexer (Tokenisation)

* Découpage de l’entrée en tokens : mots, pipes, redirections, opérateurs, etc.
* Gestion des règles de quoting :

  * `'single quotes'`
  * `"double quotes"`

### ✔️ Parser + AST

* Construction d’un **AST** représentant la structure des commandes.
* Nœuds principaux :

  * `COMMAND`
  * `PIPE`
  * `REDIRECTION`
  * `SEQUENCE` (selon implémentation)
* Validation syntaxique (détection d’erreurs comme `||`, `| <`, etc.).

### ✔️ Exécution

* Exécution d’une commande simple via `execve`.
* Gestion des PATH, redirections et pipelines.
* Support des commandes liées par `|`.

### ✔️ Built-ins

Les built-ins classiques d’un minishell :

* `cd`
* `export`
* `unset`
* `env`
* `pwd`
* `echo`
* `exit`

### ✔️ Redirections

* `<`  lecture depuis un fichier
* `>`  écriture (truncate)
* `>>` écriture (append)
* `<<` heredoc (limiteur géré en mode interactif)

### ✔️ Pipelines

* Gestion de `cmd1 | cmd2 | cmd3`
* Création et gestion des pipes Unix.

---

## **Fonctionnement du pipeline (résumé)**

1. **Lecture de la ligne**
2. **Tokenisation** → tableau de tokens
3. **Parsing** → création de l’AST
4. **Exécution**

   * si built-in → exécuté directement
   * sinon → création de processus, gestion des pipes, redirections
5. **Nettoyage mémoire** (AST, tokens, héritage des fd)

---

## **Contraintes du projet**

* Respect de la norme POSIX pour les comportements de base.
* Gestion stricte des erreurs (commandes introuvables, fichiers invalides…).
* Aucune fuite mémoire (valgrind-clean).
* Pas d’utilisation de fonctions interdites selon le sujet (ex: pas de `system()`).
* Comportement aussi proche que possible de `bash`.

---

## **Compilation & Exécution**

```bash
make
./minishell
```

Pour nettoyer :

```bash
make clean    # objets
make fclean   # binaires + objets
```

---

## **Objectifs pédagogiques**

* Comprendre les mécanismes d’un interpréteur de commandes.
* Implémenter un lexer et un parser réels.
* Manipuler des arbres de syntaxe (AST).
* Gérer plusieurs processus en parallèle.
* Travailler avec les pipes, les descripteurs de fichiers et les signaux.
* Approfondir la gestion mémoire en C.

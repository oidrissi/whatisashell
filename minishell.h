/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:51:07 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/23 15:49:44 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>

//linked list to store env variables
typedef struct		s_env
{
    char			*key;
    char			*value;
    struct s_env	*next;
}					t_env;

typedef struct    s_red
{
    char            *name;
    int             type;
    struct s_red    *next;
}               t_red;

typedef struct  s_cmd
{
    char    **args;
    t_red     red;
    struct s_cmd *next;
    struct s_cmd *prev;
}               t_cmd;

typedef struct  s_expr
{
    t_cmd     *cmd;
    struct s_expr *next;
}   t_expr;

t_expr         *g_sh;

int     main(int ac, char **av, char **env);
int	    ft_strlen(char *s);
char	**ft_strsplit(char *s, char c);
int	    is_builtin(char *s);


#endif
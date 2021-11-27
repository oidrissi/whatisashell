/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:51:07 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/26 23:03:17 by oidrissi         ###   ########.fr       */
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
    t_red     *red;
    struct s_cmd *next;
    t_env     *env;
}               t_cmd;

int     main(int ac, char **av, char **env);
int	    ft_strlen(char *s);
char	**ft_strsplit(char *s, char c);
int	    is_builtin(char *s);
int     in_quotes(char *s, int pos);
char	*ft_strtrim(char *s);
char	*ft_strdup(char *s);
t_cmd	*ft_lstnew(char **args, t_red *red);
int		ft_strcmp(char *s1, char *s2);
char	*ft_substr(char *s, int start, int len);


#endif
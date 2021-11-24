/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:52:22 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/24 01:17:30 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_error;

t_expr	*init_sh()
{
	g_sh = (t_expr *)malloc(sizeof(t_expr));
	g_sh->cmd = (t_cmd *)malloc(sizeof(t_cmd));
	g_sh->cmd->args = NULL;
	g_sh->cmd->red.name = NULL;
	g_sh->cmd->red.type = 0;
	g_sh->cmd->red.next = NULL;
	g_sh->cmd->next = NULL;
	g_sh->cmd->prev = NULL;
	return (g_sh);
}

int 	tab_len(char **s) {

	int i = 0;
	if (s)
		while (s[i] != NULL)
			i++;			
	return i;
}

char	**realloc_str(char **s, char *t) {

	char **ret = NULL;
	int len = tab_len(s);

	int i = 0;

	ret = (char **)malloc(sizeof(char *) * (len + 2));
	if (s == NULL)
	{
		ret[0] = strdup(t);
		ret[1] = NULL;
		return ret;

	}
	while (s[i] != NULL) {
		ret[i] = s[i];
		i++;
	}
	ret[i] = t;
	ret[i + 1] = NULL;
	return ret;
}


int get_wordlen(char *s, int *pos, char del) {

	int i = *pos;
	int len = ft_strlen(s);
	int dbl = 0;
	int	sgl = 0;
	int c = 0;
	int j = 0;
	while (i < len)
	{
		if (s[i] == '\'')
		{
			sgl++;
			j = i + 1;
			if (s[j] == '\0')
				return (0);
			while (s[j])
			{
				if (s[j] == '\'')
					sgl++;
				i++;
				c++;
				j++;
			}
		}
		if (s[i] == '\"')
		{
			dbl++;
			j = i + 1;
			while (s[j])
			{
				if (s[j] == '\"')
					dbl++;
				i++;
				c++;
				j++;
			}
		}
		i++;
		c++;
	}
	if (s[i] == del && (sgl % 2 == 0) && (dbl % 2 == 0))
			return c;
	return c;
}

int in_quotes(char *s, int pos)
{
	int j = pos - 1;
	
	int indb = 0;
	int	insgl = 0;
	while (j >= 0)
	{
		while ( j >= 0 && (s[j] == '\'' || s[j] == '\"'))
		{
			if (s[j] == '\'')
				insgl++;
			else if (s[j] == '\"')
				indb++;
			j--;	
		}
		j--;
	}
	if (indb % 2 == 0 && insgl % 2 == 0)
		return 1;
	return 0;
}
char	*get_token(char *s, int *pos, char del) {

	int i = *pos;
	int len = ft_strlen(s);
	int word_len  = get_wordlen(s, pos, del);
	char *ret = (char *)malloc(sizeof(char) * (word_len + 1));
	int c  = 0;
	int j = 0;
	while (i < len && c < word_len)
	{
		j = 0;
		if ((i == len || s[i] == del) && in_quotes(s, i))
			break ;
		ret[c] = s[i];
		i++;
		c++;
	}
	*pos = i;
	ret[c] = '\0';
	return ret;
}

char	**new_split(char *s,  char d) {

	char **ret = NULL;
	int i = 0;
	int len;

	len = ft_strlen(s);
	while (i < len)
	{
		ret = realloc_str(ret, get_token(s, &i, d));
		i++;
	}
	return ret;
}
// check if string contains pipe, if not return 0

t_expr	*fill_sh(char *line)
{
	g_sh->cmd->args = new_split(line, '|');
	return (g_sh);
}

//substr
char	*ft_substr(char *s, int start, int len)
{
	char *ret;
	int i;
	
	ret = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		ret[i] = s[start + i];
		i++;
	}
	ret[i] = '\0';
	return ret;
}

// if quotes are closed properly, return 1
int		proper_quotes(char *s)
{
	int i;
	int sgl;
	int dbl;
	int len;
	
	len = ft_strlen(s);
	sgl = 0;
	dbl = 0;
	i = 0;
	int j;
	while (s[i])
	{
		if (s[i] == '\'')
		{
			sgl++;
			j = i + 1;
			if (s[j] == '\0')
				return (0);
			while (s[j])
			{
				if (s[j] == '\'')
					sgl++;
				i++;
				j++;
			}
		}
		if (s[i] == '\"')
		{
			dbl++;
			j = i + 1;
			while (s[j])
			{
				if (s[j] == '\"')
					dbl++;
				i++;
				j++;
			}
		}
		i++;
	}
	if ( dbl % 2 == 0 && sgl % 2 == 0)
		return (1);
	return (0);
}

// check if the string contains whitespaces at the end of the string and trim them
char	*trim_whitespaces(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	if (!s || !*s)
		return (NULL);
	while (s[len - 1] == ' ')
		len--;
	return (ft_substr(s, i, len - i));
}

int		parse(char *s)
{
	if (!s || !*s)
		return (0);
	if (s[0] == '|' || s[ft_strlen(s) - 1] == '|')
		return (0);
	if (!proper_quotes(s))
		return (0);
	return (1);
}

char	*ft_strtrim(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (len + 1));
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	int j = 0;
	while (s[i] != '\0')
	{
		ret[j] = s[i];
		i++;
		j++;
	}
	ret[j] = '\0';
	return ret;
}

// count number of env variables in string
int		count_env(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	int c = 0;
	while (i < len)
	{
		if (s[i] == '$')
			c++;
		i++;
	}
	return c;
}

// get_env
char	*get_env(char *s)
{
	char *ret = NULL;
	int i = 0;
	int len = ft_strlen(s);
	while (i < len)
	{
		if (s[i] == '$')
		{
			i++;
			ret = get_token(s, &i, ' ');
			break ;
		}
		i++;
	}
	return ret;
}

// while there are still env variables in string, replace them with the value of the env variable
char	*replace_env(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (len + 1));
	int j = 0;
	while (i < len)
	{
		if (s[i] == '$')
		{
			i++;
			while (s[i] != ' ' && s[i] != '\t' && s[i] != '\0')
			{
				ret[j] = s[i];
				i++;
				j++;
			}
			ret[j] = '\0';
			char *tmp = get_env(ret);
			if (tmp)
			{
				int k = 0;
				while (tmp[k] != '\0')
				{
					ret[j] = tmp[k];
					j++;
					k++;
				}
				ret[j] = '\0';
			}
			i++;
			j = 0;
		}
		else
		{
			ret[j] = s[i];
			i++;
			j++;
		}
	}
	ret[j] = '\0';
	return ret;
}

//check if token contains a redirection symbol and set the redirection type
int		check_redirection(char *token)
{
	int i = 0;
	int len = ft_strlen(token);
	while (i < len)
	{
		if (token[i] == '>')
		{
			if (token[i + 1] == '>')
			{
				g_sh->cmd->red.type = 2;
				return (1);
			}
			g_sh->cmd->red.type = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

// if string contains env variables, replace them with the value of the env variable
// char	*replace_env(char *s)
// {
// 	int i = 0;
// 	int len = ft_strlen(s);
// 	char *ret = (char *)malloc(sizeof(char) * (len * 10 + 1));
// 	while (s[i] != '$')
// 	{
// 		ret[i] = s[i];
// 		i++;
// 	}
// 	i++;
// 	char **env = get_env(s);
// 	char *val = getenv(env);
// 	if (val)
// 	{
// 		int j = 0;
// 		while (val[j])
// 		{
// 			ret[i + j] = val[j];
// 			j++;
// 		}
// 		ret[i + j] = '\0';
// 	}
// 	else
// 	{
// 		ret[i] = '\0';
// 	}
// 	return ret;
// }

// check if token is 

int main(int ac, char **av, char **env)
{
    char *line;
	int		i;
	int		nb_env;
	
	i = 0;
	(void)av;
	g_sh = init_sh();
	g_error = 0;
	if (ac != 1)
		return (0);
	i = 0;
	while (1)
	{
		line = readline("$> ");
		if (*line)
			add_history(line);
		nb_env = count_env(line);
		line = trim_whitespaces(line);
		if (nb_env > 0)
			line = replace_env(line);
		if (!parse(line))
		{
			if (!line)
				continue ;
			write(1, "syntax error\n", 13);
			continue;
		}
		g_sh = fill_sh(line);
		while (g_sh->cmd->args[i])
		{
			char **sub = new_split(ft_strtrim(g_sh->cmd->args[i]), ' ');
			int len = tab_len(sub);
			int j = 0;
			printf("Command %d + args ==> %s\n", i, g_sh->cmd->args[i]);
			while (j < len)
			{
				printf("\t%d\t\t**%s**\n", j, sub[j]);
				j++;
			}
			pid_t pid = fork();
			if (pid == 0)
			{
				if (execve(sub[0], sub, env) == -1 && !is_builtin(sub[0]))
				{
					write(1, "command not found\n", 19);
					return (0);
				}
			}
			else
			{
				wait(NULL);
			}	
			i++;
		}
		i = 0;
	}
	free(line);
	return (0);
}
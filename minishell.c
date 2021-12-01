/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 19:52:22 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/27 02:52:03 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_red	*init_red()
{
	g_sh->red = (t_red *)malloc(sizeof(t_red));
	g_sh->red->name = NULL;
	g_sh->red->type = 0;
	g_sh->red->next = NULL;
	return (g_sh->red);
}

t_red	*fill_red(t_red *new, char *name, int type)
{	
	new->name = name;
	new->type = type;
	return (new);
}

t_cmd	*init_sh()
{
	g_sh = (t_cmd *)malloc(sizeof(t_cmd));
	g_sh->args = NULL;
	init_red();
	g_sh->next = NULL;
	g_sh->prev = NULL;
	return (g_sh);
}

// if space, take the string until the space
char	*get_arg(char *str, int *i)
{
	int		compt;
	int		compt2;
	char	*arg;

	compt = 0;
	while (str[compt] == ' ')
		compt++;	
	compt2 = compt;
	while (str[compt] != ' ' && str[compt] != '\0')
		compt++;
	if (!(arg = (char *)malloc(sizeof(char) * (compt - compt2 + 1))))
		return (NULL);
	compt = compt2;
	compt2 = 0;
	while (str[compt] && str[compt] != ' ')
		arg[compt2++] = str[compt++];
	arg[compt2] = '\0';
	*i += compt + 1;
	return (arg);
}

t_red	*redirections(char *str, int exit_status, char **env)
{
	t_red *red;
	int i;
	char *s;
	char *vv;

	s = NULL;
	i = 0;
	red = (t_red *)malloc(sizeof(t_red));
	while (red && str[i])
	{
		if (str[i] == '>' || str[i] == '<')
		{
			if (str[i] == '>')
			{
				if (str[i + 1] == '>')
				{
					red->type = 2;
					vv = get_arg(str + i + 2, &i);
					file_name(vv, exit_status, &s, env);
					if (!s)
					{
						char a[2];
						a[0]= (char)130;
						a[1] = '\0';
						s = ft_strjoin(a,vv);
					}
					red = fill_red(red, s, red->type);
					// printf("%s\n", s);
				}
				else
				{
					red->type = 1;
					vv = get_arg(str + i + 1, &i);
					// printf("%s\n", vv);
					file_name(vv, exit_status, &s, env);
					if (!s)
					{
						char a[2];
						a[0]= (char)130;
						a[1] = '\0';
						s = ft_strjoin(a,vv);
					}
					red = fill_red(red, s, red->type);
				}
			}
			else if (str[i] == '<')
			{
				if (str[i + 1] == '<')
				{
					red->type = 4;
					vv = get_arg(str + i + 2, &i);
					file_name(vv, exit_status, &s, env);
					if (!s)
					{
						char a[2];
						a[0]= (char)130;
						a[1] = '\0';
						s = ft_strjoin(a,vv);
					}
				}
				else
				{
					red->type = 3;
					vv = get_arg(str + i + 1, &i);
					file_name(vv, exit_status, &s, env);
					if (!s)
					{
						char a[2];
						a[0]= (char)130;
						a[1] = '\0';
						s = ft_strjoin(a,vv);
					}
				}
			}
			if (str[i])
			{
				printf("redir_type: %d && filename %s\n", red->type, s);
				red->next = (t_red *)malloc(sizeof(t_red));
				red = red->next;
			}
			else
			{
				printf("redir_type: %d && filename %s\n", red->type, s);
				red->next = NULL;
				return (red);
			}
		}
		else
			i++;
	}
	return (red);
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
	if ((s[i] == del && s[i + 1] != del) && (sgl % 2 == 0) && (dbl % 2 == 0))
			return c;
	return c;
}

//is_alnum
int		is_alnum(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

char	*get_token(char *s, int *pos, char del)
{
	int i;
	int len;
	int word_len;
	char *ret;
	int c;

	c = 0;
	i = *pos;
	len = ft_strlen(s);
	word_len  = get_wordlen(s, pos, del);
	ret = (char *)malloc(sizeof(char) * (word_len + 1));
	while (i < len && c < word_len)
	{
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
		char* b= get_token(s, &i, d);
		if (b != '\0')
			ret = realloc_str(ret, b);
		i++;
	}
	return ret;
}

// if the string contains > or <, delete everything after it until the next space
void	delete_after_redir(char *s)
{
	int i = 0;
	int len = ft_strlen(s);
	while (i < len && s[i] != ' ')
	{
		if (s[i] == '>' || s[i] == '<')
		{
			s[i] = '\0';
			return ;
		}
		i++;
	}
}

// check if string contains pipe, if not return 0
t_cmd	*fill_sh(char *line, int exit_status, char **env)
{
	init_sh();
	char ** args;
	int i;

	i = 0;
	args = new_split(line, '|');
	while (args[i])
	{
		printf("CMD %d contains: \n", i);
		g_sh = ft_lstnew(new_split(ft_strtrim(args[i]), ' '), redirections(args[i], exit_status, env));
		int j = 0;
		while (g_sh->args[j])
		{
			if (g_sh->args[j][0] == '>' || g_sh->args[j][0] == '<')
				break ;
			char *expanded = expand(g_sh->args[j], env);
			printf("%s\n", expanded);
			j++;
		}
		g_sh = g_sh->next;
		i++;
	}
	return (g_sh);
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
				{
					sgl++;
					i += 1;
					break ;
				}
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
				{
					dbl++;
					i += 1;
					break ;
				}
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
	int i;
	int len;

	i = 0;
	len = ft_strlen(s);
	if (!s || !*s)
		return (NULL);
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	if (i == len)
		return (ft_strdup(""));
	while (s[len - 1] == ' ')
		len--;
	return (ft_substr(s, i, len - i));
}

// if more than two '>' or '<' are found, return 0
int		toomuch(char *s)
{
	int i;
	int c;

	c = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '>' || s[i] == '<')
			c++;
		i++;
	}
	if (c > 2)
		return (1);
	return (0);
}

int		parse(char *s)
{
	int i;

	i = 0;
	if (!s || !*s)
		return (2);
	while (s[i])
	{
		if (s[i] == '|' && s[i + 1] == '|')
			return (0);
		i++;
	}
	if (s[0] == '|' || s[ft_strlen(s) - 1] == '|' || s[ft_strlen(s) - 1] == '>' || s[ft_strlen(s) - 1] == '<')
		return (0);
	if (!proper_quotes(s))
		return (0);
	if (toomuch(s))
		return (0);
	return (1);
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

char	*append(char *s, char c) {

	int i = 0;
	int len = ft_strlen(s);
	char *ret = (char *)malloc(sizeof(char) * (len + 2));
	
	if (s ) {
		while (i < len)
		{
			ret[i] = s[i];
			i++;
		}
	}
	ret[i] = c;
	ret[i + 1] = '\0';
	return ret;
}

char	*parse_token(char *token ) {

	char *ret = ft_strdup("");

	int i = 0;
	while (i < ft_strlen(token) && token[i] != '\0')
	{
		if ((token[i] == '>' || token[i] == '<'))
		{
			i++;
			if (token[i] == ' ')
				i++;
		}
		ret = append(ret, token[i]);
		i++;
	}
	ret = append(ret, '\0');
	return ret;
}

//getenv
char	*get_env(char *s, char **env)
{
	int i;
	char *tmp;

	i = 0;
	while (env[i])
	{
		tmp = ft_substr(env[i], 0, ft_strchr(env[i], '='));
		if (ft_strcmp(tmp, s) == 0)
			return (ft_substr(env[i], ft_strchr(env[i], '=') + 1, ft_strlen(env[i])));
		i++;
	}
	return (NULL);
}

// check if character is a number
int		is_num(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

//everytime we find a $, we take the characters after it and expand it
char	*expand(char *s, char **env)
{
	int i;
	int b;
	int len;
	char *ret;
	char *penv;
	int sgl;

	b = 0;
	i = 0;
	sgl = 0;
	len = ft_strlen(s);
	ret = (char *)malloc(sizeof(char) * (100));
	while (i < len)
	{
		if (s[i] == '\'' && i < len)
			sgl++;
		else if (s[i] == '$' && s[i + 1] == '$')
		{
			ret[b++] = '$';
			i += 2;
		}
		else if (s[i] == '$' && s[i + 1] == '\'')
			ret[b++] = s[i++];
		else if ((s[i] != '$' || sgl % 2 != 0) && i < len)
			ret[b++] = s[i++];
		else if (s[i] == '$' && is_num(s[i + 1]))
		{
			i++;
			if (is_num(s[i]) && i < len)
				i++;
			ret[b] = s[i];
		}
		else if (s[i] == '$' && s[i + 1])
		{
			i++;
			int j = i;
			while (s[j] && is_alnum(s[j]))
				j++;
			penv = get_env(ft_substr(s, i, j - i), env);
			i = j;
			if (penv == NULL)
			{
				while (s[j] && is_alnum(s[j]))
				{
					ret[b] = s[j];
					b++;
					j++;
				}
				break ;
			}
			else
			{
				int k = 0;
				while (penv[k])
				{
					ret[b] = penv[k];
					b++;
					k++;
				}
			}
		}
	}
	ret[b] = '\0';
	return ret;
}

void	handlesig(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ignctl(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	boucle(char *line, char **env, int exit_status)
{
	while (1)
	{
		line = readline("Sh> ");
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			exit(1);
		}
		if (*line)
			add_history(line);
		line = trim_whitespaces(line);
		if (!parse(line))
		{
			write(1, "syntax error\n", 13);
			continue;
		}
		else if (parse(line) == 2)
			continue ;
		if (line == NULL || !*line)
				continue ;
		g_sh = fill_sh(line, exit_status, env);
	}
}

int main(int ac, char **av, char **env)
{
    char	*line;
	int 	exit_status;

	exit_status = 0;
	(void)av;
	if (ac != 1)
		return (0);
	ignctl();
	signal(SIGINT, handlesig);
	signal(SIGQUIT, SIG_IGN);
	boucle(line, env, exit_status);
	free(line);
	return (0);
}
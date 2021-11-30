#include "minishell.h"

char	*copy_string(char *string)
{
	int		n;
	char	*new_string;
	int		i;

	i = -1;
	n = ft_strlen(string);
	new_string = malloc((n + 1) * sizeof(char));
	while (string[++i])
		new_string[i] = string[i];
	new_string[i] = '\0';
	return (new_string);
}

int	compaiiiire(char *argument, char *env)
{
	int	j;
	int	k;

	j = -1;
	k = -1;
	while (env[++j] != '=' && env[j] && argument[++k] != '=' && argument[k])
		if (env[j] != argument[k])
			break ;
	if ((!env[j] || env[j] == '=') && (!argument[j] || argument[j] == '='))
	{
		if (argument[j] == '=')
			return (2);
		return (1);
	}
	return (0);
}

char	*get_string(int i)
{
	int		n;
	int		save;
	char	*s;

	n = 1;
	save = i;
	while ((i / 10) != 0)
	{
		n++;
		i = i / 10;
	}
	s = malloc((n + 1) * sizeof(char));
	s[n] = '\0';
	while ((save / 10) != 0)
	{
		s[--n] = (save % 10) + 48;
		save = save / 10;
	}
	s[--n] = save + 48;
	return (s);
}

int	should_add(char a, char b)
{
	if ((a == '$' && (b < 48 || b > 57) && (b < 65 || b > 90) && (b < 97 || b > 122) && b != '_' && b != '?') || a != '$')
		return (1);
	return (0);
}

void	add(char **s, char *a, char c)
{
	char *tmp;

	if (!*s)
		*s = "";
	tmp = *s;
	*s = ft_strjoin(*s, a);
	if (*tmp != '\0')
		free(tmp);
	if (c == 'f')
		free(a);
}

void	fullfil(char **tmp, char *a, int j, int k)
{
	int i;

	i = -1;
	while (++i < k)
		tmp[0][i] = a[j++];
}
char *grab_value(char *a)
{
	int i;

	i = -1;
	while (a[++i] != '=' && a[i])
		continue;
	if (!a[i] || (a[i] == '=' && a[i + 1] == '\0'))
		return (NULL);
	return (copy_string(a + i + 1));
	
}
char	*searchin_env(char *tmp, char **env)
{
	int i = -1;

	while (env[++i])
		if (compaiiiire(tmp, env[i]))
			return (grab_value(env[i]));
	return (NULL);
}

int	eeexpand(char *a, int i, char **env, char **value)
{
	int j;
	char *tmp;

	j = i + 1;
	while(a[++i] == '_' || (a[i]>=48 && a[i]<=57) || (a[i]>=65 && a[i]<=90) || (a[i]>=97 && a[i]<=122))
		continue ;
	tmp = malloc ((i - j + 1) * sizeof(char));
	tmp[i - j] = '\0';
	fullfil(&tmp, a, j, i - j);
	*value = searchin_env(tmp, env);
	free(tmp);
	return (i);
}

int		add_exit_status(int exit_status, char **s, int i)
{
	char *b;

	b = get_string(exit_status);
	add(s, b, 'f');
	return (i + 2);
}

void file_name(char *vv, int exit_status, char **s, char **env)
{
	char tab[2];
	char i;
	char *value;

	i = 0;
	tab[1] = '\0';
	while (vv[i])
	{
		if (should_add(vv[i], vv[i + 1]))
		{
			tab[0] = vv[i++];
			add(s, tab, 'n');
		}
		else if (vv[i + 1] >= 48 && vv[i + 1] <= 57)
			i = i + 2;
		else if (vv[i + 1] == '?')
			i = add_exit_status(exit_status, s, i);
		else
		{
			i = eeexpand(vv, i, env, &value);
			if (value)
				add(s, value, 'f');
		}
	}
}

// int	main(int argc, char **argv, char **env)
// {
// 	int exit_status = 10;
// 	char *file = "$5$4$UsSER"; // add special char if s null and s is null only if nothing expanded
// 	char *s = NULL;
	
// 	file_name(file, exit_status, &s, env);
// 	printf("%s\n", s);
// 	if (!s)
// 	{
// 		char a[2];
// 		a[0]= (char)130;
// 		a[1] = '\0';
// 		s = ft_strjoin(a,file);
// 	}
// }

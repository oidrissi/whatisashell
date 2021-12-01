/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 12:04:54 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/26 12:05:05 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int in_quotes(char *s, int pos)
{
	int j;
	int indb;
	int	insgl;

	j = pos - 1;
	indb = 0;
	insgl = 0;
	while (j >= 0)
	{
		while (j >= 0 && (s[j] == '\'' || s[j] == '\"'))
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

//ft_strjoin
char	*ft_strjoin(char *s1, char *s2)
{
	char *ret;
	int i;
	int j;
	
	ret = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	i = 0;
	if (!ret)
		return NULL;
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		ret[i + j] = s2[j];
		j++;
	}
	ret[i + j] = '\0';
	return ret;
}

//ft_itoa
char	*ft_itoa(int n)
{
	char	*ret;
	int		i;
	int		neg;
	
	neg = 0;
	if (n == -2147483648)
		return ft_strdup("-2147483648");
	if (n < 0)
	{
		neg = 1;
		n = -n;
	}
	i = 1;
	while (n / i >= 10)
		i *= 10;
	ret = (char *)malloc(sizeof(char) * (i + 1));
	if (!ret)
		return NULL;
	ret[i] = '\0';
	while (i > 0)
	{
		ret[i - 1] = n % 10 + '0';
		n /= 10;
		i /= 10;
	}
	if (neg)
		ret[0] = '-';
	return ret;
}

//strchr
int		ft_strchr(char *s, int c)
{
	int i;
	int len;

	i = 0;
	len = ft_strlen(s);
	while (i < len)
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (i);
}

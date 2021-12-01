/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:50:11 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/26 23:04:57 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//create a new linked list
t_cmd	*ft_lstnew(char **args, t_red *red)
{
    t_cmd *new;

    if (!(new = (t_cmd *)malloc(sizeof(t_cmd))))
        return (NULL);
    new->args = args;
    if (!red)
        new->red = NULL;
    new->red = red;
    // new->env = env;
    return (new);
}

//ft_itoa
char	*ft_itoa(int n)
{
    char	*str;
    int		i;
    int		sign;

    i = 1;
    sign = (n < 0) ? -1 : 1;
    n = n * sign;
    while (n /= 10)
        i++;
    if (!(str = (char *)malloc(sizeof(char) * (i + 1))))
        return (NULL);
    str[i] = '\0';
    while (i--)
    {
        str[i] = (n % 10) * sign + '0';
        n = n / 10;
    }
    if (sign == -1)
        str[0] = '-';
    return (str);
}

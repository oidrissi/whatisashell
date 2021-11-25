/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oidrissi <oidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:50:11 by oidrissi          #+#    #+#             */
/*   Updated: 2021/11/25 14:38:31 by oidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//create a new linked list
t_cmd	*ft_lstnew(char **args, t_red red)
{
    t_cmd *new;

    if (!(new = (t_cmd *)malloc(sizeof(t_cmd))))
        return (NULL);
    new->args = args;
    new->red = red;
    //new->red.type = 0;
    new->next = NULL;
    return (new);
}

// add a new element to the list
void	ft_lstadd(t_cmd **lst, t_cmd *new)
{
    t_cmd *tmp;

    if (!*lst)
        *lst = new;
    else
    {
        tmp = *lst;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

// insert a new element at the beginning of the list
void	ft_lstinsert(t_cmd **lst, t_cmd *new)
{
    new->next = *lst;
    *lst = new;
}

// ft_lstdelone
void    ft_lstdelone(t_cmd **lst)
{
    t_cmd *tmp;

    tmp = *lst;
    *lst = tmp->next;
    free(tmp);
}

// free the list
void	ft_lstdel(t_cmd **lst)
{
    t_cmd *tmp;

    while (*lst)
    {
        tmp = *lst;
        *lst = (*lst)->next;
        ft_lstdelone(&tmp);
    }
}
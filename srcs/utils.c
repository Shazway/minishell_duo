/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 18:55:18 by mdkhissi          #+#    #+#             */
/*   Updated: 2022/07/13 16:04:29 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	str_arr_free(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	str_arr_size(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

char	**str_arr_add(char **sarr, char **entry, int len_entry)
{
	int		len_sarr;
	char	**new;
	int		i;
	int		j;

	len_sarr = str_arr_size(sarr);
	new = malloc(sizeof(char *) * (len_sarr + len_entry + 1));
	if (!new)
		return (NULL);
	new[len_sarr + len_entry] = NULL;
	i = 0;
	while (i < len_sarr)
	{
		new[i] = ft_strdup(sarr[i]);
		i++;
	}
	j = 0;
	while (entry && j < len_entry)
	{
		new[i] = ft_strdup(entry[j]);
		j++;
		i++;
	}
	return (new);
}

int	ft_malloc(void **p, int length)
{
	*p = malloc(length);
	if (*p)
		return (1);
	return (0);
}

int	change_path(char *str, char *path)
{
	if (!str)
	{
		if (!getenv("HOME"))
		{
			free(path);
			return (printf("bash: cd: HOME not set\n"));
		}
	}
	if (is_double_dash(str) == -1)
	{
		free(path);
		return (printf("bash: --: invalid option\n"));
	}
	if (chdir(str) == -1)
	{
		printf("cd: %s: No such file or directory\n", str);
		free(path);
		return (1);
	}
	else
		free(path);
	return (1);
}

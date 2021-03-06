/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 20:27:56 by tmoragli          #+#    #+#             */
/*   Updated: 2022/07/31 14:46:41 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_data *data, int ac, char **av)
{
	int		i;
	char	**to_del;
	int		j;

	j = 0;
	to_del = malloc(ac * sizeof(char *));
	if (!to_del)
		msh_exit(data, 1);
	i = 1;
	while (av && av[i])
	{
		if (is_validid(av[i], ft_strlen(av[i])))
		{
			to_del[j] = ft_strjoin(av[i], "=");
			if (!to_del[j])
				return (!str_arr_free(to_del) * -1);
			j++;
		}
		i++;
	}
	to_del[j] = NULL;
	data->env_str = str_arr_del(data->env_str, to_del, j);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 19:24:29 by mdkhissi          #+#    #+#             */
/*   Updated: 2022/07/31 14:46:22 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*concat_path(char *goal, char *folder_name)
{
	char	*temp;

	if (!goal)
		return (NULL);
	if (ft_strncmp(goal, "/", ft_strlen(goal)))
	{
		temp = goal;
		goal = ft_strjoin(goal, "/");
		free(temp);
	}
	if (!goal)
		return (NULL);
	temp = goal;
	goal = ft_strjoin(goal, folder_name);
	free(temp);
	if (!goal)
		return (NULL);
	return (goal);
}

int	change_path_worker(char *goal, char *foldername, t_data *data)
{
	int	ret;

	free(data->old_path);
	data->old_path = ft_strdup(data->relative_path);
	if (!data->old_path)
	{
		ft_free_strs(&goal, &foldername, NULL, NULL);
		msh_exit(data, 1);
	}
	ret = new_pwd(ft_strdup(foldername), &(data->relative_path));
	ft_free_strs(&goal, &foldername, NULL, NULL);
	return (ret);
}

int	change_path(char *goal, char *foldername, t_data *data)
{
	if (!foldername)
	{
		free(goal);
		msh_exit(data, 1);
	}
	if (!goal)
	{
		free(foldername);
		msh_exit(data, 1);
	}
	if (chdir(goal) == -1)
	{
		perror("minishell: cd");
		ft_free_strs(&goal, &foldername, NULL, NULL);
		return (1);
	}
	if (!change_path_worker(goal, foldername, data))
		msh_exit(data, 1);
	return (0);
}

int	cd(t_data *data, int ac, char **str)
{
	char	*path;
	char	*arg;

	arg = NULL;
	if (ac > 2)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2) > 0);
	if (ac == 1 || (ac == 2 && !ft_strncmp(str[1], "~", ft_strlen(str[1]))))
		return (cd_home(get_var("HOME", data), "HOME", data));
	arg = ft_strdup(str[1]);
	if (!arg)
		msh_exit(data, 1);
	if (is_dash(arg) != 0)
		return (cd_dash(arg, data));
	if (arg[0] == '/')
		return (change_path(arg, ft_strdup(arg), data));
	path = ft_strdup(data->relative_path);
	path = concat_path(path, arg);
	if (!path)
		return (-1);
	if (change_path(path, arg, data))
		return (1);
	return (0);
}

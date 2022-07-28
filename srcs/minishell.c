/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdkhissi <mdkhissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 19:02:08 by tmoragli          #+#    #+#             */
/*   Updated: 2022/07/28 19:58:28 by mdkhissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(t_data *data)
{
	free(data->prompt_path);
	data->prompt_path = get_var("PWD", data);
	//if (!data->prompt_path)
		//msh_exit(data);
}

void	set_prompt_string(t_data *data)
{
	char	*tmp;
	char	*cr_dir;
	char	*crf_dolar;

	cr_dir = ft_strrchr(data->prompt_path, '/');
	if ((cr_dir && !cr_dir[1]) || !cr_dir)
		cr_dir = data->prompt_path;
	else
		cr_dir = cr_dir + 1;
	crf_dolar = ft_strjoin(cr_dir,
			"\001 ▶\002 \001\033[1;34m\002""\001\033[0m\002");
	if (!crf_dolar)
		msh_exit(data);
	tmp = data->prompt;
	data->prompt = ft_strjoin("\001\033[1;32m\002""╔\002"
			"\001\033[1;32m\002"" minishell" " " "\001╝\002" " " "\001\033[1;31m\002""",
			crf_dolar);
	if (!data->prompt)
		msh_exit(data);
	free(crf_dolar);
	free(tmp);
}

void	ministart(t_data *data)
{
	add_history(data->input);
	if (!is_opened_quotes(data) || !check_input(data))
	{
		data->ret = 2;
		ft_printf("%s", data->error_msh);
		return ;
	}
	if (ft_strchr(data->input, '$'))
		data->input = expand_variables(data, data->input);
	if (parsing(data))
	{
		delete_quotes(data);
		open_redirections(data);
		search_cmds(data);
		execute(data);
		free_pipes(data);
		ft_lstclear(&data->cmd, &free_cmd);
	}
}

void	minishell_sh(t_data *data)
{
	while (1)
	{
		if (signal_intercept())
			exit(1);
		update_pwd(data);
		set_prompt_string(data);
		data->input = readline(data->prompt);
		if (!data->input)
			break ;
		else
			ministart(data);
		data->input = ft_free(data->input);
	}
}

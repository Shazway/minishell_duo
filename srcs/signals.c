/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 23:24:49 by mdkhissi          #+#    #+#             */
/*   Updated: 2022/07/23 16:45:44 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct sigaction	g_signals;

int	signal_intercept(void)
{
	if (sigaction(SIGCHLD, &(g_signals), NULL) == -1)
		return (1);
	if (sigaction(SIGINT, &(g_signals), NULL) == -1)
		return (1);
	if (sigaction(SIGQUIT, &(g_signals), NULL) == -1)
		return (1);
	return (0);
}

void	secondary_handler(int signal, siginfo_t *s, void *trash)
{
	(void)s;
	(void)trash;
	if (signal == SIGINT)
	{
		printf("\n");
		return ;
	}
	if (signal == SIGQUIT)
		return ;
	if (signal == SIGCHLD)
		printf("A child has died secondary handler\n");
}

void	sig_info_main(int signal, siginfo_t *s, void *trash)
{
	(void)s;
	(void)trash;
	if (signal == SIGINT)
	{
		printf("^C\n");
		printf("SIGINT main\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signal == SIGQUIT)
		return ;
	if (signal == SIGCHLD)
		printf("A child has died main handler\n");
}

int	termios_setup(t_data *data)
{
	int	rc;

	rc = tcgetattr(0, &data->termios);
	if (rc)
		return (1);
	data->termios.c_lflag &= ~ECHOCTL;
	rc = tcsetattr(0, 0, &data->termios);
	if (rc)
		return (1);
	return (0);
}

int	msh_init(t_data *data)
{
	g_signals.sa_sigaction = sig_info_main;
	data->input = NULL;
	data->env_str = NULL;
	data->cmd = NULL;
	data->n_cmd = 0;
	data->pips = NULL;
	data->ret = -1;
	if (set_env(data))
		return (1);
	data->relative_path = get_var("PWD", data);
	data->old_path = ft_strdup(data->relative_path);
	data->prompt = NULL;
	return (0);
}

t_cmd	*init_cmd(int i)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->i = i;
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->ac = 0;
	cmd->fullpath = NULL;
	cmd->fin = -1;
	cmd->fout = -1;
	cmd->no_fork = 0;
	cmd->builtin = 0;
	return (cmd);
}

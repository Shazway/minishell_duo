/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdkhissi <mdkhissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 23:00:30 by mdkhissi          #+#    #+#             */
/*   Updated: 2022/07/22 00:21:39 by mdkhissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    execute(t_data *data)
{
    t_cmd   *cmd;
    t_list  *c_idx;
	int		pid;

	alloc_pipes(data);
    c_idx = data->cmd;
    while (c_idx != NULL)
    {
		cmd = c_idx->content;
		if (c_idx->next != NULL)
			init_pipe(data, cmd->i);
		if (cmd->no_fork)
			run_cmd(data, cmd, cmd->i, data->n_cmd);
		else
		{
			pid = fork();
			if (pid == -1)
				return ;
			else if (pid == 0)
				run_cmd(data, cmd, cmd->i, data->n_cmd);
		}
		c_idx = c_idx->next;
	}
	close_pipes(data->pips, data->n_cmd - 1);
	wait_cmds(data);
}

void	wait_cmds(t_data *data)
{
	t_list	*i;
	t_cmd	*cmd;

	i = data->cmd;
	cmd = i->content;
	while (i)
	{
		close_cmd_files(cmd);
		if (!cmd->no_fork)
			wait(&data->ret);
		i = i->next;
	}
}

void	run_cmd(t_data *data, t_cmd *cmd, int i, int n)
{
	int		r;
	int		w;
	int		fdtest;

	r = i - 1 * (i > 0);
	w = i - 1 * (i == n - 1);
	if (cmd->fin == -1)
	{
		if (i != 0 && n > 1)
			dup2(data->pips[r].fd[0], STDIN_FILENO);
	}
	else
	{
		dup2(cmd->fin, STDIN_FILENO);
		close(cmd->fin);
	}
	if (cmd->fout == -1)
	{
		if (i != n - 1 && n > 1)
			dup2(data->pips[w].fd[1], STDOUT_FILENO);
	}
	else
	{
		dup2(cmd->fout, STDOUT_FILENO);
		close(cmd->fout);
	}
	close_unused_pipes(data->pips, r, w, n);
	if (cmd->builtin)
		exec_builtin(data, cmd);
	else
	{
		if (execve(cmd->fullpath, cmd->args, data->env_str) == -1)
		{
			fdtest = -1;
			if (!cmd->fullpath)
				cmd_notfound(cmd->name);
			else if (open(cmd->fullpath, O_WRONLY | O_APPEND) == -1)
				perror("minishell: ");
			else if (access(cmd->fullpath, X_OK) == -1)
				perror("minishell: ");
			if (fdtest != -1)
				close(fdtest);
			exit(EXIT_FAILURE);
		}
	}
}

void	close_unused_pipes(t_pipex *pips, int r, int w, int n)
{
	int	i;

	i = 0;
	while (i <= r && n > 1)
		close(pips[i++].fd[0]);
	i = 0;
	while (i <= w && n > 1)
		close(pips[i++].fd[1]);
}

void	exec_builtin(t_data *data, t_cmd *cmd)
{
	int	ret;

	ret = cmd->func(data, cmd->ac, cmd->args);
	if (!cmd->no_fork)
		exit(ret);
	else
		data->ret = ret;
}

void	cmd_notfound(char *cmd_name)
{
	printf("%s: command not found\n", cmd_name);
}

 int	here_doc(char *lim, int expand, char **envr)
{
	int		fd;
	char	*buf;
	char	*p;
	int		len_lim;

	fd = open("/tmp/msh_here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!fd)
		perror("open");
	len_lim = ft_strlen(lim);
	while (fd)
	{
		buf = readline("> ");
		if (!buf)
		{
			printf("signal\n");
			break ;
		}
		p = ft_strnstr(buf, lim, len_lim);
		if (p && p == buf && p[len_lim] == '\0')
		{
			free(buf);
			break ;
		}
		heredoc_writer(fd, buf, expand, envr);
		free(buf);
	}
	close(fd);
	fd = open("/tmp/msh_here_doc", O_RDONLY);
	return (fd);
}

void	heredoc_writer(int fd, char *buf, int expand, char **envr)
{
	char	*p;
	char	*var;
	int		from;
	char	*value;

	p = NULL;
	from = 0;
	if (expand)
	{
		p = ft_strchr(buf, '$');
		while (p)
		{
			write(fd, buf + from, p - (buf + from));
			var = extract_var(p + 1);
			from += 1 + ft_strlen(var);
			value = find_var(envr, var);
			ft_putstr_fd(value, fd);
			free(value);
			free(var);
			p = ft_strchr(buf + from, '$');
		}
	}
	if (!expand || !p)
		ft_putendl_fd(buf + from, fd);
}

char	*extract_var(char *pvar)
{
	int		to;
	char	*var;

	to = is_validid(pvar, -1);
	if (to > 0)
	{
		var = ft_substr(pvar, 0, to);
		return (var);
	}
	return (NULL);
}

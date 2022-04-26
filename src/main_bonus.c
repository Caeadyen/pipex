/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:59:29 by hrings            #+#    #+#             */
/*   Updated: 2022/04/05 11:32:07 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex_bonus.h>

static void	child(t_pipex *pipex, int num);
static void	runshellcmd(t_pipex *pipex, int num);
void		split_process(t_pipex *pipex);
static int	check_para(int argc, char **argv);

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;
	int		offset;

	offset = check_para(argc, argv);
	pipex = init_pipex(argc, offset);
	initfiles(pipex, argc, argv);
	creat_pipes(pipex);
	initpath(pipex, envp);
	initcommands(pipex, pipex->num_com);
	if (offset)
		handle_here_doc(pipex);
	split_process(pipex);
	error_handler(pipex, pipex->error);
	return (0);
}

void	split_process(t_pipex *pipex)
{
	int		status;
	pid_t	pid;
	int		index;

	index = 0;
	pid = 0;
	while (index < pipex->num_com)
	{
		pid = fork();
		if (pid == 0)
			child(pipex, index);
		index++;
	}
	close_pipes(pipex);
	waitpid(pid, &status, WUNTRACED);
	pipex->error = WEXITSTATUS(status);
}

static void	child(t_pipex *pipex, int num)
{
	if (num == 0 && !pipex->here_doc)
	{
		dup2(pipex->pipes[1], 1);
		dup2(pipex->fdin, 0);
		close_pipes(pipex);
	}
	else if (num == 0 && pipex->here_doc)
	{
		dup2(pipex->pipes[1], 1);
		dup2(pipex->doc_pipes[0], 0);
		close_pipes(pipex);
	}
	else if (num == (pipex->num_com - 1))
	{
		dup2(pipex->pipes[num * 2 - 2], 0);
		dup2(pipex->fdout, 1);
		close_pipes(pipex);
	}
	else
	{
		dup2(pipex->pipes[num * 2 - 2], 0);
		dup2(pipex->pipes[2 * num + 1], 1);
		close_pipes(pipex);
	}
	runshellcmd(pipex, num);
}

static void	runshellcmd(t_pipex *pipex, int num)
{
	execve(pipex->com[num]->path, pipex->com[num]->arg, pipex->envp);
	error_handler(pipex, 127);
}

static int	check_para(int argc, char **argv)
{
	if (argc < 5)
	{
		ft_putstr_fd("invalid number of parameter\n", 2);
		exit(1);
	}
	if (!ft_strncmp("here_doc", argv[1], 9))
	{	
		if (argc < 6)
		{
			ft_putstr_fd("invalid number of parameter\n", 2);
			exit(1);
		}
		return (1);
	}
	else
		return (0);
}

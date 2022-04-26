/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 15:59:29 by hrings            #+#    #+#             */
/*   Updated: 2022/04/05 11:57:24 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

static void	child(t_pipex *pipex, int num);
static void	runshellcmd(t_pipex *pipex, int num);
void		split_process(t_pipex *pipex);

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;

	if (argc != 5)
	{
		ft_putstr_fd("invalid number of parameter\n", 2);
		return (1);
	}
	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
		exit(1);
	pipex->error = 0;
	pipex->num_com = argc - 3;
	initfiles(pipex, argc, argv);
	if (pipe(pipex->pipes) == -1)
		error_handler_pip(pipex);
	initpath(pipex, envp);
	initcommands(pipex, 2);
	split_process(pipex);
	error_handler(pipex, pipex->error);
	return (0);
}

void	split_process(t_pipex *pipex)
{
	int	status;

	if (pipex->error != 1)
	{
		pipex->pids1 = fork();
		if (pipex->pids1 == 0)
			child(pipex, 0);
	}
	pipex->pids2 = fork();
	if (pipex->pids2 == 0)
		child(pipex, 1);
	close(pipex->pipes[0]);
	close(pipex->pipes[1]);
	waitpid(pipex->pids2, &status, WUNTRACED);
	pipex->error = WEXITSTATUS(status);
}

static void	child(t_pipex *pipex, int num)
{
	if (num == 0)
	{
		dup2(pipex->pipes[1], STDOUT_FILENO);
		dup2(pipex->fdin, STDIN_FILENO);
		close(pipex->pipes[0]);
		runshellcmd(pipex, 0);
	}
	else
	{
		dup2(pipex->pipes[0], STDIN_FILENO);
		dup2(pipex->fdout, STDOUT_FILENO);
		close(pipex->pipes[1]);
		runshellcmd(pipex, 1);
	}
}

static void	runshellcmd(t_pipex *pipex, int num)
{
	execve(pipex->com[num]->path, pipex->com[num]->arg, pipex->envp);
	error_handler(pipex, 127);
}

void	freecommand(t_pipex *pipex)
{
	int	index;
	int	index2;

	index = 0;
	while (index < pipex->num_com)
	{
		index2 = 0;
		while (pipex->com[index]->arg[index2])
		{
			free(pipex->com[index]->arg[index2]);
			index2++;
		}
		free(pipex->com[index]->arg);
		free(pipex->com[index]->path);
		free(pipex->com[index]);
		index++;
	}
	free(pipex->com);
}

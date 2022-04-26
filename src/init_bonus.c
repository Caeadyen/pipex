/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 16:27:03 by hrings            #+#    #+#             */
/*   Updated: 2022/04/05 12:16:57 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex_bonus.h>
#include <fcntl.h>
#include <sys/stat.h>

static void	openinput(t_pipex *pipex);
static void	openoutput(t_pipex *pipex);

void	initfiles(t_pipex *pipex, int argc, char **argv)
{
	pipex->argv = argv;
	pipex->error = 0;
	pipex->inputfile = argv[1];
	pipex->outputfile = argv[argc - 1];
	if (!pipex->here_doc)
		openinput(pipex);
	openoutput(pipex);
	if (pipex->error)
		error_handler_file(pipex);
}

static void	openinput(t_pipex *pipex)
{
	if (access(pipex->inputfile, F_OK) == -1)
	{
		pipex->error = 1;
		pipex->fdin = open("/dev/null", O_RDONLY);
	}
	else if (access(pipex->inputfile, R_OK) == -1)
		pipex->error = 2;
	else
		pipex->fdin = open(pipex->inputfile, O_RDONLY);
}

static void	openoutput(t_pipex *pipex)
{
	mode_t	mode;
	int		oflag;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	if (pipex->here_doc)
		oflag = O_WRONLY | O_CREAT | O_APPEND;
	else
		oflag = O_WRONLY | O_CREAT | O_TRUNC;
	pipex->fdout = open(pipex->outputfile, oflag, mode);
	if (pipex->fdout < 0)
		pipex->error = 3;
}

void	initcommands(t_pipex *pipex, int num)
{
	int	index;
	int	off;

	off = 2 + pipex->here_doc;
	pipex->com = (t_parameter **)malloc(sizeof(t_parameter *) * num);
	if (!pipex->com)
		error_handler_com(pipex, -1);
	index = 0;
	while (index < num)
	{
		pipex->com[index] = (t_parameter *)malloc(sizeof(t_parameter));
		if (!pipex->com[index])
			error_handler_com(pipex, index);
		pipex->com[index]->arg = ft_split_pipex(pipex->argv[index + off], ' ');
		pipex->com[index]->path = getpath(pipex, pipex->com[index]->arg[0]);
		index++;
	}
}

void	initpath(t_pipex *pipex, char **envp)
{
	int		index;
	char	*result;

	index = 0;
	if (envp)
	{
		pipex->envp = envp;
		while (envp[index] && ft_strncmp(envp[index], "PATH=", 5))
			index++;
		result = envp[index] + 5;
		pipex->path = ft_split(result, ':');
	}
	else
	{
		pipex->path = NULL;
		pipex->envp = NULL;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrings <hrings@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 16:00:24 by hrings            #+#    #+#             */
/*   Updated: 2022/04/05 11:21:04 by hrings           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <libft.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>

typedef struct s_parameter
{
	char	*path;
	char	**arg;
}			t_parameter;

typedef struct s_pipex
{
	int			fdin;
	int			fdout;
	int			pipes[2];
	pid_t		pids1;
	pid_t		pids2;
	char		**path;
	char		**argv;
	char		**pathname;
	char		*inputfile;
	char		*outputfile;
	int			num_com;
	t_parameter	**com;
	int			error;
	char		**envp;
}				t_pipex;

void		error_handler(t_pipex *pipex, int num);
char		*joinpath(char *directory, char *command);
char		*getpath(t_pipex *pipex, char *command);
int			ft_strfind(char *str, char c);
void		initfiles(t_pipex *pipex, int argc, char **argv);
void		error_handler_file(t_pipex *pipex);
char		**ft_split_pipex(char const *s, char c);
void		free_pipex(t_pipex *pipex);
void		initcommands(t_pipex *pipex, int num);
void		initpath(t_pipex *pipex, char **envp);
void		freecommand(t_pipex *pipex);
void		error_handler_com(t_pipex *pipex, int index);
void		error_handler_pip(t_pipex *pipex);

#endif
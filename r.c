/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:33:44 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/05/14 13:06:05 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *check_path(t_env **list)
{
    t_env *tmp;

    if (!list || !*list)
        return (NULL);
        
    tmp = *list;
    while (tmp)
    {
        if (!ft_strcmp(tmp->key, "PATH"))
            return (tmp->value);
        tmp = tmp->next;
    }
    return (NULL);
}

void    ft_free_split(char **str)
{
    int i = 0;

    while(str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

void wait_for_children(t_cmd *cmd, pid_t child_pid)
{
    int status;

    status = 0;
    waitpid(child_pid, &status, 0);
    cmd->data.exit_status = WEXITSTATUS(status);
}
void handle_absolute_path(t_cmd *cmd, char **help)
{
    pid_t child_pid;
    
    child_pid = fork();
    if (child_pid == 0)
    {
        if(!(access(cmd->args[0], X_OK) == -1))
        {
            execve(cmd->args[0], cmd->args, help);
            cmd->data.exit_status = 0;
        }
        write(2, "minishell: ", 11);
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": No such file or directory\n", 28);
        cmd->data.exit_status = 127;
    }
    else if (child_pid > 0)
    {
        wait_for_children(cmd, child_pid);
    }
}
void ft_execute_path_command(t_cmd *cmd, char **env_array, char **split_path)
{
    char *tmp;
    char *helper;
    int i = 0;

    while (split_path[i])
    {
        tmp = ft_strjoin(split_path[i], "/");
        helper = ft_strjoin(tmp, cmd->args[0]);
        free(tmp);
        if (!helper)
        {
            i++;
            continue;
        }
        if(!(access(helper, X_OK) == -1))
            execve(helper, cmd->args, env_array);
        free(helper);
        i++;
    }
    write(2, "minishell: ", 11);
    write(2, cmd->args[0], ft_strlen(cmd->args[0]));
    write(2, ": command not found\n", 20);
    ft_free_split(split_path);
    exit(127);
}

void ft_excute_commands(t_cmd *cmd, t_env **env_list)
{
    char **env_doble;
    char *path;
    char **split_path;
    pid_t child_pid;

    env_doble = convert_env_list(env_list);
    if(!cmd->args || !cmd->args[0])
        return ;
    if(ft_strchr(cmd->args[0], '/'))
        handle_absolute_path(cmd, env_doble);
    path = check_path(env_list);
    // if(!path)
    // {
    //         puts("No Path");
    //     if( access(cmd->args[0] , X_OK | F_OK) == 0 )
    //     {
    //         puts("OK");
    //         execve(cmd->args[0], cmd->args, env_list);
    //         perror("exc-> ");
    //         return ;
    //     }
    //     write(2, "PATH not found\n", 15);
    //     return;
    // }
    split_path = ft_split(path, ':');
    if (!split_path)
        return ;
    child_pid = fork();
    if(child_pid == 0)
        ft_execute_path_command(cmd, env_doble, split_path);
    else if(child_pid > 0)
    {
        wait_for_children(cmd, child_pid);
        ft_free_split(split_path);
    }
    else
    {
        perror("fork failed");
        ft_free_split(split_path);
    }
}

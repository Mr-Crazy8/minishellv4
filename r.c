/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:33:44 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/05/19 15:44:42 by anel-men         ###   ########.fr       */
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
            cmd->data.exit_status = get_or_set(SET, 0);

        }
        write(2, "minishell: ", 11);
        write(2, cmd->args[0], ft_strlen(cmd->args[0]));
        write(2, ": No such file or directory\n", 28);
        cmd->data.exit_status = get_or_set(SET, 127);
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
        {
            execve(helper, cmd->args, env_array);
            cmd->data.exit_status = get_or_set(SET, 0);
        }
        free(helper);
        i++;
    }
    write(2, "minishell: ", 11);
    write(2, cmd->args[0], ft_strlen(cmd->args[0]));
    write(2, ": command not found\n", 20);
    ft_free_split(split_path);
    cmd->data.exit_status = get_or_set(SET, 127);
    exit(127);
}
// int check_directory(char *cmd)
// {
    
// }
void ft_excute_commands(t_cmd *cmd, t_env **env_list)
{
    char **env_doble;
    char *path;
    char **split_path ;
    pid_t child_pid;
    // t_data *data;

    env_doble = convert_env_list(env_list);
    if(!cmd->args || !cmd->args[0])
        return ;
    if(ft_strchr(cmd->args[0], '/'))
    {
            handle_absolute_path(cmd, env_doble);
            return ;
    }
    child_pid = fork();
    if(child_pid == 0)
    {
        path = check_path(env_list);
        if(!path)
        {
            if(access(cmd->args[0], X_OK) != -1)
            {
                puts("kkkk");
                execve(cmd->args[0], cmd->args, env_doble);
                cmd->data.exit_status = get_or_set(SET, 0);
                perror("excve");
            }
            write(2, "minishell: ", 11);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": No such file or directory\n", 28);
            cmd->data.exit_status = get_or_set(SET, 127);
            exit(127);

        }
        else
        {
            split_path = ft_split(path, ':');
            if (!split_path)
                return ;
            ft_execute_path_command(cmd, env_doble, split_path);
            ft_free_split(split_path);
        }
    }
    else if(child_pid > 0)
    {
        //puts("True");
        wait_for_children(cmd, child_pid);
    }
    else
    {
        perror("fork failed");
        ft_free_split(split_path);
    }
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lll.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:22:58 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/05/26 18:31:08 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft(char **args, t_env **list) // pipe moust be check and access;;;;*
{
    char **split_path;
    char **helper;
    char *path;
    int i= 0;
    char *tmp;

    helper = convert_env_list(list);
    path = check_path(list);
    if(!args || !args[0])
        return ;
    if(ft_strchr(args[0], '/'))
    {
        if(!(access(args[0], X_OK) == -1))
        {
            execve(args[0], args, helper);
            exit (1); //ila failat execve
        }
        write(1, "command not found\n", 18);
    }
    else
    {
        split_path = ft_split(path, ':');
        if(!split_path)
            return ;
        while(split_path[i])
        {
            tmp = ft_strjoin(split_path[i], "/");
            char *str = ft_strjoin(tmp, args[0]);
            free(tmp);
            if (!helper)
            {
                i++;
                continue;
            }
            if(!(access(str, X_OK) == -1))
            {
                execve(str, args, helper);
                exit (1);
            }
            free(str);
            i++;
        }
        i = 0;
        write(2, "command not found\n", 18);
        ft_free_split(split_path);
        exit(127);
    }        
}

void    pipe_all(t_cmd *head)
{
    t_cmd *cmd = head;

    while (cmd)
    {
        
        if (cmd->pipe_out)
        {
            pipe(cmd->fd_pipe); // creates cmd->fd_pipe[0] (read) and [1] (write)
        }
        cmd = cmd->next;
    }
}
char **convert_env_list(t_env **list)
{
    t_env * tmp;
    char **helper = NULL;
    char *str;
    char *temp;
    int  i = 0;
    tmp = *list;
    int len = ft_list_size(*list);

    helper = malloc(sizeof(char *) * (len + 1));
    while(tmp)
    {
        str = ft_strdup(tmp->key);
        temp = ft_strjoin(str, "=");
        free(str);
        str = ft_strjoin(temp, tmp->value);
        free(temp);
        helper[i] = ft_strdup(str);
        free(str);
        tmp = tmp->next;
        i++;
    }
    helper[i] = NULL;
    return (helper);
}
void close_all_pipes(t_cmd *cmd)
{
    t_cmd *tmp = cmd;
    
    while (tmp)
    {
        if (tmp->pipe_out)
        {
            close(tmp->fd_pipe[0]);
            close(tmp->fd_pipe[1]);
        }
        tmp = tmp->next;
    }
}

void check_close_red(t_cmd *cmd, t_cmd *prev,  t_env *env)
{
        if (prev && prev->pipe_out)
        {
            close(prev->fd_pipe[0]);
            close(prev->fd_pipe[1]);
        }
        if (cmd->pipe_out)
        {
            close(cmd->fd_pipe[0]);
            close(cmd->fd_pipe[1]);
        }
        ft_redircte(cmd->redirs);
}
// void ft_excute_mult_pipe(t_cmd *cmd, t_env *list_env, char *env[])
// {
//     pid_t pid;
//     t_cmd *prev = NULL;
//     t_cmd *head = NULL;

//     head = cmd;
//     pipe_all(cmd);
    

//     while (cmd)
//     {
//         pid = fork();
//         if (pid < 0)
//         {
//             perror("fork");
//             close_all_pipes(head);
//             return;
//         }
//         else if (pid == 0)
//         {
//             // ft_redircte(cmd->redirs);

//             if (prev && prev->pipe_out)
//                 dup2(prev->fd_pipe[0], 0);
//             if (cmd->pipe_out)
//                 dup2(cmd->fd_pipe[1], 1);
//             if(cmd->redirs != NULL)
//                 check_close_red(cmd, prev);

//             close_all_pipes(head);
//             // if(cmd->redirs->fd != -1)
//             execute_single_command(cmd, list_env, env);
//             exit(0); 
//         }

//         // Parent closes previous pipe ends
//         if (prev && prev->pipe_out)
//         {
//             close(prev->fd_pipe[0]);
//             close(prev->fd_pipe[1]);
//         }

//         prev = cmd;
//         cmd = cmd->next;
//     }

//     // close_all_pipes(head); // close remaining pipes
//     while (wait(NULL) > 0)
//         ;
// }

void ft_excute_mult_pipe(t_cmd *cmd, t_env *list_env, char *env[])
{
    pid_t pid;
    t_cmd *prev = NULL;
    t_cmd *head = NULL;
    int status;
    int last_status = 0;  // To store the exit status of the last command

    head = cmd;
    pipe_all(cmd);
    
    while (cmd)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            close_all_pipes(head);
            return;
        }
        else if (pid == 0)
        {
            if (prev && prev->pipe_out)
                dup2(prev->fd_pipe[0], 0);
            if (cmd->pipe_out)
                dup2(cmd->fd_pipe[1], 1);
            if(cmd->redirs != NULL)
                check_close_red(cmd, prev, list_env);

            close_all_pipes(head);
            execute_single_command(cmd, list_env, env);
            exit(cmd->data.exit_status);
        }

        // Parent closes previous pipe ends
        if (prev && prev->pipe_out)
        {
            close(prev->fd_pipe[0]);
            close(prev->fd_pipe[1]);
        }

        prev = cmd;
        cmd = cmd->next;
    }

    // Wait for all child processes and get the status of the last one
    pid_t wpid;
    while ((wpid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
        {
            last_status = WEXITSTATUS(status);
        }
    }
    
    // Set the exit status to the last command's status
    get_or_set(SET, last_status);
}


// // Close all pipes to prevent leaks



// void ft_excute_mult_pipe(t_cmd *cmd, t_env *list_env, char *env[])
// {
//     pid_t pid;
//     t_cmd * prev = NULL;
//     t_cmd *head;
//     t_cmd *tmp;
    
//     head = cmd;
//     // pipe_all(cmd->fd_pipe);
//     while(cmd)
//     {
//         if(cmd->pipe_out)
//             pipe(cmd->fd_pipe);
//         pid = fork();
//         if(pid < 0)
//         {
//             perror("fork");
//             close_all_pipes(head);
//         }
//         else if(pid == 0)
//         {
//             // puts("hdhdh");
//             if(cmd->redirs != NULL)
//                 check_close_red(cmd , prev);
//             else
//             {
//                 if(prev != NULL && prev->pipe_out)
//                 {
//                     dup2(prev->fd_pipe[0], 0);
//                     close(cmd->fd_pipe[0]);
//                     close(cmd->fd_pipe[1]);
//                 }
//                 if(cmd->pipe_out)
//                 {
//                     dup2(cmd->fd_pipe[1], 1);
//                     close(cmd->fd_pipe[0]);
//                     close(cmd->fd_pipe[1]);
//                 }
//             }
//             execute_single_command(cmd, list_env, env);
//         }
//         close_all_pipes(head);
//         // if (prev && prev->pipe_out)
//         // {
//         //     close(prev->fd_pipe[0]);
//         //     close(prev->fd_pipe[1]);
//         // }
//         // if(prev && prev->pipe_out)
//         // {
//         //     close(prev->fd_pipe[0]);
//         //     close(prev->fd_pipe[1]);
//         // }
//         while (wait(NULL) > 0)
//             ;
//         prev = cmd;
//         cmd = cmd->next;
//     }

// }



















// int exec_builtin(char **args)
// {
//     if (strcmp(args[0], "cd") == 0)
//         return (0);
//     else if (strcmp(args[0], "echo") == 0)
//         return (0);
//     else if (strcmp(args[0], "unset") == 0)
//         return (0);
//     else if (strcmp(args[0], "export") == 0)
//         return (0);
//         else if (strcmp(args[0], "pwd") == 0)
//             return (0);
//         else if (strcmp(args[0], "env") == 0)
//             return (0);
//         else if (strcmp(args[0], "exit") == 0)
//             return (0);
//         else
//             return (1);
// }
// void minishell_cmd(char *env[])
// {
//     pid_t pid = fork();
// if (pid == 0)
// {
//     char *args[] = {"./minishell", NULL};
//     execve("./minishell", args, env);
// }
// else
// {
//     waitpid(pid, NULL, 0);
// }

// }
// void f()
// {
//     system("leaks minishell");
// }
// void check_line(char *line, t_cmd **command, t_env *env, char *en[])
// {
//     char **splited;
//     int        i;
    
//     atexit(f);
//     i = 0;
//     splited = ft_split(line, ' ');
//     t_cmd *cmd;
//     if(!exec_builtin(splited))
//     {
//         while (splited[i])
//         {
//             if (strncmp("export", splited[i], 6) == 0 && strlen(splited[i]) == 6)
//                 ft_export(splited, &env);
//             else if (strncmp("env", splited[i], 3) == 0 && strlen(splited[i]) == 3)
//                 ft_env(env, en);
//             else if (strncmp("exit", splited[i], 4) == 0 && strlen(splited[i]) == 4)
//                 ft_exit(splited);
//             else if (strncmp("./minishell", splited[i], 11) == 0 && strlen(splited[i]) ==  11)
//             {
//                 ft_free_split(splited);
//                 minishell_cmd(en);
//             }
//             else if (strncmp("unset", splited[i], 5) == 0 && strlen(splited[i]) == 5)
//                 ft_unset(&env, splited + 1);
//             else if (strncmp("echo", splited[i], 4) == 0 && strlen(splited[i]) == 4)
//                 echo(splited);
//             else if (strncmp("pwd", splited[i], 3) == 0 && strlen(splited[i]) == 3)
//                 pwd();
//             else if (strncmp("cd", splited[i], 2) == 0 && strlen(splited[i]) == 2)
//                 ft_cd(splited, &env);
//             i++;
//         }
//         ft_free_split(splited);
//     }
//     else
//         ft_excute_commands(splited, &env);
// }

// int main(int argc, char *argv[], char *env[])
// {
//     t_env    *env_new = NULL;
//     char    *line;


//     (void)argv;
// 	(void)argc;
//     // int i = 0;
//     // while(env[i])
//     // {
//     //     printf("%s\n", env[i]);
//     //     i++;
//     // }
//     // if (!env || !*env)
//     //     return (1);
//     // if (argc < 0)
//     //     return (0);
// 	env_new = env_maker(env, &env_new);
//     // env_new = creat_env(env);
//     // print_strcut(env_new);
//     // env_cmd(env_new);
//     // export(&env_new, argv[1]);
//     // env_cmd(env_new);
// 	// t_env *tmp = 
//     // t_cmd **cmd;
//     // t_cmd *tmp;

//     // tmp->

//     // intialize_struct
//     while (1)
//     {
//       line = readline("minishell-> ");
//       if (!line || !*line)
//       {
//         write (2, "exit\n", 6);
//         break;
//       }
//       add_history(line);
//       check_line(line, env_new, env);
//     }
    
// }




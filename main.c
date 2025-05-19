/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:07:21 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/05/19 10:42:02 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char **args)
{
	if(!args || !*args)
		return (1);
	if (strcmp(args[0], "cd") == 0)
		return (0);
	else if (strcmp(args[0], "echo") == 0)
		return (0);
	else if (strcmp(args[0], "unset") == 0)
		return (0);
	else if (strcmp(args[0], "export") == 0)
		return (0);
		else if (strcmp(args[0], "pwd") == 0)
			return (0);
		else if (strcmp(args[0], "env") == 0)
			return (0);
		else if (strcmp(args[0], "exit") == 0)
			return (0);
		else
			return (1);
}
// int get_or_set(int type, int status)
// {
// 	static int exit_status = 0; // hold the value even after function ends

// 	if (type == SET) //SET
// 		exit_status = status; // rje3 dkre hadik kifch dkhlha m3a exit_status;
// 	return (exit_status); // always return the current value
// }
void excute_builting(t_cmd **command, t_env *env_list, char *env[])
{
	t_cmd	*cmd;
	int		status;

	cmd = *command;
	status = 0;

	if (strncmp("export", cmd->args[0], 6) == 0 && strlen(cmd->args[0]) == 6)
		status = ft_export(cmd->args, &env_list);
	else if (strncmp("env", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
		status = ft_env(*command, env_list);
	else if (strncmp("exit", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
	{
		ft_exit(cmd->args, cmd->data, env_list);
		free_cmd_list(cmd);
	}
	else if (strncmp("unset", cmd->args[0], 5) == 0 && strlen(cmd->args[0]) == 5)
	{ 
		status = ft_unset(&env_list, cmd->args + 1);
	}
	else if (strncmp("echo", cmd->args[0], 4) == 0 && strlen(cmd->args[0]) == 4)
		status = echo(cmd->args);
	else if (strncmp("pwd", cmd->args[0], 3) == 0 && strlen(cmd->args[0]) == 3)
		status = pwd();
	else if (strncmp("cd", cmd->args[0], 2) == 0 && strlen(cmd->args[0]) == 2)
		status = ft_cd(cmd->args, &env_list);
	cmd->data.exit_status = get_or_set(SET, status);
	// printf("%d\n", cmd->data.exit_status);
}
void execute_single_command(t_cmd *cmd, t_env *list_env, char *env[])
{

	if(!is_builtin(cmd->args))
	{
		excute_builting(&cmd, list_env, env);
		get_or_set(SET, cmd->data.exit_status);
	}
	else
	{
		ft_excute_commands(cmd, &list_env);
		get_or_set(SET, cmd->data.exit_status);
		// printf(".....%d\n", cmd->data.exit_status);
	}
}


void add_one_shlvl(t_env *env)
{
    t_env *tmp = env;
    int shl_vl = 0;

	if (!env) 
		return;
    while (tmp) 
    {
        if (tmp->key) 
        {
			if (tmp->key && strcmp(tmp->key, "SHLVL") == 0)
			{
            	if (tmp->value && tmp->value[0] != '\0')
            	{
                	shl_vl = atoi(tmp->value);
                	free(tmp->value);
					tmp->value = NULL;
            	}
            shl_vl++; 
            tmp->value = ft_itoa(shl_vl);
            if (!tmp->value) 
			{
                    tmp->value = strdup("1");
            }
            break;
				
			}
        }
        tmp = tmp->next;
    }
}



void check_line(t_cmd **command, t_env *env_list, char *env[])
{
	 t_cmd *cmd;
	  cmd = *command;
	  int fd_input ;
	  int fd_output ;
		
    fd_input = dup(0);
	fd_output = dup(1);
	//shlvl(env_list, *command);
	if (cmd->pipe_out)
	{
		ft_excute_mult_pipe(cmd, env_list, env);
		return ;
	}
	else
	{
		if(cmd->redirs)
			ft_redircte(cmd->redirs); 
		execute_single_command(cmd, env_list, env);

	}
	dup2(fd_input, 0);
	dup2(fd_output, 1);
	close(fd_input);
	close(fd_output);
}
int main(int argc, char *argv[], char *env[])
{

	// if (isatty(STDERR_FILENO) == 0)
	// 	return (0);
	// atexit(ff);
	t_token *token_list;
	t_env *env_struct = NULL;   //// add to the final main
	int exit_status = 0;
	char *input;
	t_cmd *cmd;
	char *preprocessed_input;
    // get_or_set(SET, 0); 
	(void)argc;
	(void)argv;
	env_struct = env_maker(env, &env_struct);
	if(!env_struct)
		env_null(&env_struct);
	add_one_shlvl(env_struct);
	token_list = NULL;
	while (1)
	{
		input = readline("minishell $> ");
		// cmd = NULL;
		if (!input)
			break;
		add_history(input);
		if (check_quotes(input))
		{
			// Error message already printed by check_quotes
			free(input);
			continue;
		}
		preprocessed_input = preprocess_command(input); 
        // free(input);  // Free original input
         if (!preprocessed_input)
            continue;
		token_list = tokin_list_maker(preprocessed_input);
		if (token_list && !error_pipi(token_list)  && !check_syntax_errors(token_list))
		{
			// printf("--- TOKENS ---\n");
			// expand_handle(token_list, env_struct, exit_status);
			//process_quotes_for_tokens(token_list, 1);
			cmd = parser(token_list);
			
			exit_status = get_or_set(GET, 0);
			//printf("exit_status first ====================> %d\n", exit_status);
			expand_handle(cmd, env_struct,exit_status);
			// ft_excute(cmd);
			ambiguous_finder(cmd);
			process_quotes_for_cmd(cmd, env_struct,  1);
			file_opener(cmd);
			print_ambiguous_redir_errors(cmd);
			//puts("hshsh");
			print_cmd(cmd);
			check_line(&cmd, env_struct, env);
			exit_status = cmd->data.exit_status;
			//printf("exit_status number two  ====================> %d\n", cmd->data.exit_status);
			//exit_status = get_or_set(GET, 0);
			free_cmd_list(cmd);
			//debug_print_cmd(cmd);
			if (cmd == NULL) {
				printf("Warning: Command list is empty after parsing!\n");
			} else {
				// process_quotes_for_cmd(cmd, 1);
				// print_cmd(cmd);
				// if (cmd)
				// 	free_cmd_list(cmd);
			}
		}   
		free_token_list(token_list);
		// free(input);
	}
	free_env_struct(env_struct);
	return 0;
}
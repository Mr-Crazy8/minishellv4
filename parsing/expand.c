#include "parsing.h"
#include <ctype.h>
#define MAX_BUFFER_SIZE (1ULL << 30) // 1GB max buffer size

int expand_handle_helper0(t_exp_helper *expand)
{
	if (expand->original[expand->i] == '\'')
	{
		if (expand->quote_state == 0)
			expand->quote_state = 1;
		else if (expand->quote_state == 1)
			expand->quote_state = 0;
		expand->expanded[expand->j++] = expand->original[expand->i++];
		return (1);
	}
	else if (expand->original[expand->i] == '"')
	{
		if (expand->quote_state == 0)
			expand->quote_state = 2;
		else if (expand->quote_state == 2)
			expand->quote_state = 0;
		expand->expanded[expand->j++] = expand->original[expand->i++];
		return (1);
	}
	return (0);
}

int ensure_buffer_space(t_exp_helper *expand, size_t additional_needed)
{
	if (!expand->buffer_size)
	{
		size_t original_len = strlen(expand->original);
		if (original_len > SIZE_MAX / 2 - 1024)
		{
			fprintf(stderr, "minishell: memory allocation failed: input too large\n");
			return (0);
		}
		expand->buffer_size = original_len * 2 + 1024;
	}

	if (expand->j + additional_needed >= expand->buffer_size)
	{
		size_t new_size = expand->buffer_size * 2;
		if (new_size < expand->j + additional_needed + 1)
			new_size = expand->j + additional_needed + 1024;

		// if (new_size > MAX_BUFFER_SIZE || new_size < expand->buffer_size)
		// {
		// 	fprintf(stderr, "minishell: memory allocation failed: buffer size too large\n");
		// 	return (0);
		// }
		char *new_buffer = calloc(1, new_size);
		if (!new_buffer)
		{
			fprintf(stderr, "minishell: memory allocation failed\n");
			exit(1);
		}

		if (expand->expanded)
		{
			memcpy(new_buffer, expand->expanded, expand->j);
			free(expand->expanded);
		}
		expand->expanded = new_buffer;
		expand->buffer_size = new_size;
	}
	return (1);
}

int helper3(t_exp_helper *expand, int exit_status)
{
	if (expand->original[expand->i] == '?')
	{
		expand->var_value = ft_itoa(get_or_set(GET, 0));
		expand->i++;
		return (1);
	}
	return (0);
}


// int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env)
// {
//     char *var;
//     var = NULL;
//     if (expand->original[expand->i] == '$' && expand->quote_state != 1)
//     {
//         expand->i++;
//         if (helper3(expand, exit_status) == 0)
//         {
//             expand->start = expand->i;
//             if (isdigit(expand->original[expand->i])) 
// 			{
//                 expand->i++;
//             } 
//             else {
//                 while (expand->original[expand->i] && is_valid_var_char(expand->original[expand->i]))
//                     expand->i++;
//             }
            
//             size_t var_len = expand->i - expand->start;
//             if (var_len > SIZE_MAX - 1)
//             {
//                 fprintf(stderr, "minishell: memory allocation failed: variable name too long\n");
//                 return (0);
//             }
            
//             // Handle empty variable name ($)
//             if (var_len == 0) {
//                 if (!ensure_buffer_space(expand, 1)) {
//                     return (0);
//                 }
//                 expand->expanded[expand->j++] = '$';
//                 return (1);
//             }
            
//             expand->var_name = malloc(var_len + 1);
//             if (!expand->var_name)
//             {
//                 fprintf(stderr, "minishell: memory allocation failed\n");
//                 exit(1);
//             }
//             memcpy(expand->var_name, expand->original + expand->start, var_len);
//             expand->var_name[var_len] = '\0';
//             if (is_valid_key(expand->var_name) != 1)
//                 var = lookup_variable(expand->var_name, env);
            
//             // Only set var_value if the variable exists
//             if (var != NULL)
//                 expand->var_value = var;
                
//             free(expand->var_name);
//             expand->var_name = NULL;
//         }

//         if (expand->var_value)
//         {
//             size_t len = strlen(expand->var_value);
//             if (len > SIZE_MAX - expand->j)
//             {
//                 fprintf(stderr, "minishell: memory allocation failed: buffer overflow\n");
//                 free(expand->var_value);
//                 expand->var_value = NULL;
//                 return (0);
//             }
//             if (!ensure_buffer_space(expand, len))
//             {
//                 free(expand->var_value);
//                 expand->var_value = NULL;
//                 return (0);
//             }
//             memcpy(expand->expanded + expand->j, expand->var_value, len);
//             expand->j += len;
//             free(expand->var_value);
//             expand->var_value = NULL;
//         }
//         // We're not outputting anything for non-existent variables
        
//         return (1);
//     }
//     return (0);
// }

int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env)
{
    char *var;
    var = NULL;
    if (expand->original[expand->i] == '$' && expand->quote_state != 1)
    {
        expand->i++;
        if (helper3(expand, exit_status) == 0)
        {
            expand->start = expand->i;
            if (isdigit(expand->original[expand->i])) 
            {
                expand->i++;
            } 
            else {
                while (expand->original[expand->i] && is_valid_var_char(expand->original[expand->i]))
                    expand->i++;
            }
            
            size_t var_len = expand->i - expand->start;
            if (var_len > SIZE_MAX - 1)
            {
                fprintf(stderr, "minishell: memory allocation failed: variable name too long\n");
                return (0);
            }
            
            // Handle empty variable name ($)
            if (var_len == 0) {
                if (!ensure_buffer_space(expand, 1)) {
                    return (0);
                }
                expand->expanded[expand->j++] = '$';
                return (1);
            }
            
            expand->var_name = malloc(var_len + 1);
            if (!expand->var_name)
            {
                fprintf(stderr, "minishell: memory allocation failed\n");
                exit(1);
            }
            memcpy(expand->var_name, expand->original + expand->start, var_len);
            expand->var_name[var_len] = '\0';
            if (is_valid_key(expand->var_name) != 1)
                var = lookup_variable(expand->var_name, env);
            
            // Only set var_value if the variable exists
            if (var != NULL)
                expand->var_value = var;
                
            free(expand->var_name);
            expand->var_name = NULL;
        }

        if (expand->var_value)
        {
            size_t len = strlen(expand->var_value);
            if (len > SIZE_MAX - expand->j)
            {
                fprintf(stderr, "minishell: memory allocation failed: buffer overflow\n");
                free(expand->var_value);
                expand->var_value = NULL;
                return (0);
            }
            if (!ensure_buffer_space(expand, len))
            {
                free(expand->var_value);
                expand->var_value = NULL;
                return (0);
            }
            memcpy(expand->expanded + expand->j, expand->var_value, len);
            expand->j += len;
            free(expand->var_value);
            expand->var_value = NULL;
        }
        // We're not outputting anything for non-existent variables
        // And we're ensuring they are completely removed
        
        return (1);
    }
    return (0);
}

void process_string(char *str, t_exp_helper *expand, t_env *env, int exit_status)
{
	if (!expand_fill_str(expand, str))
	{
		expand->expanded = NULL;
		return;
	}


	while (expand->original[expand->i])
	{
		if (!expand_handle_helper0(expand) && !expand_handle_helper1(expand, exit_status, env))
		{
			if (!ensure_buffer_space(expand, 1))
			{
				free(expand->expanded);
				expand->expanded = NULL;
				break;
			}
			expand->expanded[expand->j++] = expand->original[expand->i++];
		}
	}

	if (expand->expanded)
		expand->expanded[expand->j] = '\0';
}



// void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)
// {
//     t_cmd *current;
//     t_exp_helper *expand;
//     t_redir *redir;
//     int i;
//     int should_split = 0;

//     expand = malloc(sizeof(t_exp_helper));
//     if (!expand)
//     {
//         fprintf(stderr, "minishell: memory allocation failed\n");
//         exit(1);
//     }
//     expand->buffer_size = 0;
//     expand->expanded = NULL;

//     current = cmd_list;
//     while (current)
//     {
//         should_split = 0;
        
//         // Process all args first (including cmd which is stored in args[0])
//         i = 0;
//         while (current->args && current->args[i])
//         {
//             if ((current->args[i][0] == '\'' && current->args[i][strlen(current->args[i]) - 1] == '\'') ||
//                 (current->args[i][0] == '"' && current->args[i][strlen(current->args[i]) - 1] == '"'))
//                 should_split = 0;
//             else
//             {
//                 if (strchr(current->args[i], '$'))
//                     should_split = 1;
//                 else if (strcmp(current->args[i], "export"))
//                     should_split = 1;
//             }
//             process_string(current->args[i], expand, env, exit_status);
            
//             // If this argument expanded to empty and it contains a variable, remove it
//             if (expand->expanded && expand->expanded[0] == '\0' && 
//                 strchr(current->args[i], '$'))
//             {
//                 free(current->args[i]);
//                 free(expand->expanded);
//                 expand->expanded = NULL;
                
//                 // Shift all following arguments forward
//                 int j = i;
//                 while (current->args[j + 1])
//                 {
//                     current->args[j] = current->args[j + 1];
//                     j++;
//                 }
//                 current->args[j] = NULL;
                
//                 // Don't increment i since we need to process the new argument at this position
//             }
//             else
//             {
//                 free(current->args[i]);
//                 current->args[i] = expand->expanded;
//                 expand->expanded = NULL;
//                 i++;
//             }
//         }
        
//         // Now that args are processed, update cmd to be the first arg
//         if (current->args && current->args[0])
//         {
//             if (current->cmd)
//                 free(current->cmd);
//             current->cmd = strdup(current->args[0]);
//             if (!current->cmd)
//             {
//                 fprintf(stderr, "minishell: memory allocation failed\n");
//                 exit(1);
//             }
//         }

//         redir = current->redirs;
//         while (redir)
//         {
//             if (redir->file)
//             {
//                 process_string(redir->file, expand, env, exit_status);
//                 free(redir->file);
//                 redir->file = expand->expanded;
//                 expand->expanded = NULL;
//             }
//             redir = redir->next;
//         }
//         current = current->next;
//     }

//     free(expand);
//     if (should_split)
//         apply_word_splitting(cmd_list);
// }

void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)
{
    t_cmd *current;
    t_exp_helper *expand;
    t_redir *redir;
    int i;
    int should_split = 0;

    expand = malloc(sizeof(t_exp_helper));
    if (!expand)
    {
        fprintf(stderr, "minishell: memory allocation failed\n");
        exit(1);
    }
    expand->buffer_size = 0;
    expand->expanded = NULL;

    current = cmd_list;
    while (current)
    {
        should_split = 0;
        
        // Process all args first (including cmd which is stored in args[0])
        i = 0;
        while (current->args && current->args[i])
        {
          if ((current->args[i][0] == '\'' && current->args[i][strlen(current->args[i]) - 1] == '\'') ||
            (current->args[i][0] == '"' && current->args[i][strlen(current->args[i]) - 1] == '"'))
    should_split = 0;
        else
        {
            if (strchr(current->args[i], '$'))
                should_split = 1;
            else 
                should_split = 1;  // For other commands or non-export args
    }
            process_string(current->args[i], expand, env, exit_status);
            
            // If this argument expanded to empty and it contains a variable, remove it
            if (expand->expanded && expand->expanded[0] == '\0' && 
                strchr(current->args[i], '$'))
            {
                free(current->args[i]);
                free(expand->expanded);
                expand->expanded = NULL;
                
                // Also remove from args_befor_quotes_remover if it exists
                if (current->args_befor_quotes_remover && current->args_befor_quotes_remover[i])
                {
                    free(current->args_befor_quotes_remover[i]);
                    
                    // Shift all following arguments forward
                    int j = i;
                    while (current->args_befor_quotes_remover[j + 1])
                    {
                        current->args_befor_quotes_remover[j] = current->args_befor_quotes_remover[j + 1];
                        j++;
                    }
                    current->args_befor_quotes_remover[j] = NULL;
                }
                
                // Shift all following arguments forward in args
                int j = i;
                while (current->args[j + 1])
                {
                    current->args[j] = current->args[j + 1];
                    j++;
                }
                current->args[j] = NULL;
                
                // Don't increment i since we need to process the new argument at this position
            }
            else
            {
                free(current->args[i]);
                current->args[i] = expand->expanded;
                expand->expanded = NULL;
                i++;
            }
        }
        
        // Now that args are processed, update cmd to be the first arg
        if (current->args && current->args[0])
        {
            if (current->cmd)
                free(current->cmd);
            current->cmd = strdup(current->args[0]);
            if (!current->cmd)
            {
                fprintf(stderr, "minishell: memory allocation failed\n");
                exit(1);
            }
        }

        redir = current->redirs;
        while (redir)
        {
            if (redir->file)
            {
                process_string(redir->file, expand, env, exit_status);
                free(redir->file);
                redir->file = expand->expanded;
                expand->expanded = NULL;
            }
            redir = redir->next;
        }
        current = current->next;
    }

    free(expand);
    if (should_split)
        apply_word_splitting(cmd_list);
}
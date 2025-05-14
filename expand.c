#include "minishell.h"


int	expand_handle_helper0(t_exp_helper *expand)
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
    static size_t current_size = 0;
    
    // First call - initialize current_size
    if (current_size == 0)
        current_size = strlen(expand->original) * 10 + 1024;
    
    // Check if we need more space
    if (expand->j + additional_needed >= current_size)
    {
        // Double the current size or add enough for the new string
        size_t new_size = current_size * 2;
        if (new_size < expand->j + additional_needed + 1)
            new_size = expand->j + additional_needed + 1024; // Extra padding
        
        // Reallocate the buffer
        char *new_buffer = (char *)realloc(expand->expanded, new_size);
        if (!new_buffer)
            return (0); // Reallocation failed
        
        expand->expanded = new_buffer;
        current_size = new_size;
    }
    
    return (1);
}

int	helper3(t_exp_helper *expand, int exit_status)
{
	if (expand->original[expand->i] == '?')
	{
		expand->var_value = ft_itoa(exit_status);
		expand->i++;
		return (1);
	}
	return (0);
}
 
int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env)
{
    if (expand->original[expand->i] == '$' && expand->quote_state != 1)
    {
        expand->i++;
        if (!helper3(expand, exit_status))
        {
            expand->start = expand->i;
            while (expand->original[expand->i]
                && is_valid_var_char(expand->original[expand->i]))
                expand->i++;
            expand->var_name = malloc(expand->i - expand->start + 1);
            strncpy(expand->var_name, expand->original + expand->start,
                expand->i - expand->start);
            expand->var_name[expand->i - expand->start] = '\0';
            expand->var_value = lookup_variable(expand->var_name, env);
            free(expand->var_name);
            expand->var_name = NULL;
        }


        // expand->k = 0;
        // while (expand->var_value && expand->var_value[expand->k])
        // {
        //     expand->expanded[expand->j] = expand->var_value[expand->k]; //========> WTF
        //     expand->j++;
        //     expand->k++;
        // }
        
      if (expand->var_value)
    {
    size_t len = strlen(expand->var_value);
    
    // Ensure we have enough space
    if (!ensure_buffer_space(expand, len))
    {
        free(expand->var_value);
        expand->var_value = NULL;
        return (0); // Buffer resize failed
    }
    
    // Now it's safe to use memcpy
    memcpy(expand->expanded + expand->j, expand->var_value, len);
    expand->j += len;
    }
        
        // Free the var_value and return
        if (expand->var_value)
        {
            free(expand->var_value);
            expand->var_value = NULL;
        }
        return (1);
    }
    return (0);
}

void process_string(char *str, t_exp_helper *expand,
                   t_env *env, int exit_status)
{
    if (!expand_fill_str(expand, str))
        return;
    
    while (expand->original[expand->i])
    {
        if (!expand_handle_helper0(expand)
            && !expand_handle_helper1(expand, exit_status, env))
         {
            expand->expanded[expand->j] = expand->original[expand->i];
            expand->j++;
            expand->i++;
        }

    }

    expand->expanded[expand->j] = '\0';
}

void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status) //50
{
    t_cmd *current;
    t_exp_helper *expand;
    t_redir *redir;
    int i;
    int should_split = 0;

    expand = (t_exp_helper *)malloc(sizeof(t_exp_helper));
    if (!expand)
        return;
        
    current = cmd_list;
    while (current)
    {
        should_split = 0;
        // Handle command
        if (current->cmd)
        {
            process_string(current->cmd, expand, env, exit_status);
            free(current->cmd);
            current->cmd = expand->expanded;
        }
        
        // Handle args
        i = 0;
        while (current->args && current->args[i])
        {
           if ((current->args[0][0] == '\'' && current->args[i][strlen(current->args[i]) - 1] == '\'') ||
                    (current->args[0][0] == '\"' && current->args[i][strlen(current->args[i]) - 1] == '\"'))
                should_split = 0;
            else
            {
                if (strchr(current->args[i], '$'))
                    should_split = 1;
                else if (strcmp(current->args[i], "export"))
                    should_split = 1;
            }
            process_string(current->args[i], expand, env, exit_status);
            free(current->args[i]);
            current->args[i] = expand->expanded;
            i++;
        }
        
        // Handle redirections
        redir = current->redirs;
        while (redir)
        {
            if (redir->file)
            {
                process_string(redir->file, expand, env, exit_status);
                free(redir->file);
                redir->file = expand->expanded;
            }
            redir = redir->next;
        }
        
        current = current->next;
    }
    
    free(expand);
    if (should_split == 1)
        apply_word_splitting(cmd_list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:21:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/05/24 10:53:37 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// int ft_tablen(char **array)
// {
//     int len = 0;
//     while (array && array[len])
//         len++;
//     return len;
// } 
// char *expand_var(const char *var_name, t_env *env)
// {
//     if (!var_name || var_name[0] != '$')
//         return ft_strdup("");
        
//     // Skip the $ and look up the variable
//     char *name = ft_strdup(var_name + 1);
//     if (!name)
//         return NULL;
        
//     char *value = lookup_variable(name, env);
//     free(name);
    
//     // Return empty string if variable not found
//     return value ? ft_strdup(value) : ft_strdup("");
// }



// int is_only_variable_reference(const char *str)
// {
//     if (!str || !str[0])
//         return 0;
        
//     // Check if string starts with $
//     if (str[0] == '$')
//     {
//         int i = 1;
//         // Check if there's a valid variable name after $
//         while (str[i] && (isalnum(str[i]) || str[i] == '_'))
//             i++;
//         // If we reached the end of string or a non-variable character, it's a variable reference
//         return str[i] == '\0';
//     }
//     return 0;
// }


// // Function to free an array of strings (NULL-terminated)
// void ft_free_array(char **array)
// {
//     int i;
    
//     if (!array)
//         return;
        
//     i = 0;
//     while (array[i])
//     {
//         free(array[i]);
//         i++;
//     }
    
//     free(array);
// }






// int process_quote_char(char c, int *quote_state, char *new_str, int *j, int remove_mode)
// {
//     if (c == '\'')
//     {
//         if (*quote_state == 0)
//             *quote_state = 1;
//         else if (*quote_state == 1)
//             *quote_state = 0;
//         if (remove_mode == 0)
//             new_str[(*j)++] = c;
//         return (1);
//     }
//     else if (c == '\"')
//     {
//         if (*quote_state == 0)
//             *quote_state = 2;
//         else if (*quote_state == 2)
//             *quote_state = 0;
//         if (remove_mode == 0)
//             new_str[(*j)++] = c;
//         return (1);
//     }
//     return (0);
// }

// char *allocate_and_init(char *str, int *i, int *j, int *quote_state)
// {
//     char *new_str;
    
//     (*i) = 0;
//     (*j) = 0;
//     (*quote_state) = 0;
    
//     if (!str)
//         return (NULL);
//     new_str = malloc(strlen(str) + 1);
//     if (!new_str)
//         return (NULL);
//     return new_str;
// }

// char *selective_remove_quotes(char *str, int remove_mode, t_env  *env)
// {
//     char *new_str;
//     int i;
//     int j;
//     int quote_state;
//     int in_opposite_quote;
    
//    new_str = allocate_and_init(str, &i, &j, &quote_state);
//     if (!new_str)
//         return (NULL);
//     while (str[i])
//     {
//         in_opposite_quote = 0;
//         if ((str[i] == '\'' && quote_state == 2) || 
//             (str[i] == '\"' && quote_state == 1))
//         {
//             new_str[j++] = str[i++];
//             in_opposite_quote = 1;
//         }
//         else if (!in_opposite_quote && !process_quote_char(str[i], &quote_state, new_str, &j, remove_mode))
//             new_str[j++] = str[i++];
//         else
//             i++;
//     }
//     return (new_str[j] = '\0', new_str);
// }




// char *check_export_case(char *str, char *befor, int remove_mode, t_env *env)
// {
//     static int there_was_export_befor = 0;
//     char **split_str;
//     char **split_befor;
//     char *result = NULL;
//     char *temp = NULL;
    
//     if (strcmp(str, "export") == 0)
//     {
//         there_was_export_befor++;
//         return NULL;
//     }
//     else if (there_was_export_befor > 0 && strcmp(str, "export") != 0)
//     {
//         split_str = ft_split_q(str, '=');
//         split_befor = ft_split_q(befor, '=');
        
//         if (ft_lint(split_str) > 1)
//         {
//             if (!split_str || !split_befor)
//             {
//                 if (split_str) 
//                     ft_free_array(split_str);
//                 if (split_befor) 
//                     ft_free_array(split_befor);
//                 return NULL;
//             }
            
//             // Handle the special case where befor has quotes around variable
//             if (split_befor && split_befor[1] && 
//                 (split_befor[1][0] == '\'' || split_befor[1][0] == '\"') && 
//                 split_befor[1][1] == '$')
//             {
//                 split_befor[1] = selective_remove_quotes(split_befor[1], remove_mode, env);
//             }
            
//             // Determine quote removal mode for key
//             if (((split_befor && split_str[0]) && 
//                  (split_str[0][0] != '$' && split_befor[0][0] != '$')) || 
//                 ((strchr(split_str[0], '\"') != NULL) || 
//                  (strchr(split_str[0], '\'') != NULL)))
//                 remove_mode = 1;
//             else
//                 remove_mode = 0;
                
//             // Process key part
//             char *processed_key = selective_remove_quotes(split_str[0], remove_mode, env);
            
//             // **FIX: Always remove quotes from value part in export**
//             // For export statements, we should always remove quotes from values
//             // This is the key fix for your issue
//             int value_remove_mode = 1; // Always remove quotes for export values
            
//             // However, if the original value was a variable reference, check if it needs expansion
//             if (split_befor && split_befor[1] && split_befor[1][0] == '$')
//             {
//                 // If original was a variable, don't remove quotes yet - let expansion handle it
//                 value_remove_mode = 0;
//             }
//             else
//             {
//                 // For regular quoted values, always remove quotes in export
//                 value_remove_mode = 1;
//             }
                
//             // Process value part
//             char *processed_value = NULL;
//             if (split_str[1]) // Check if value exists
//                 processed_value = selective_remove_quotes(split_str[1], value_remove_mode, env);
            
//             // Combine key and value if there was an equals sign
//             if (strchr(str, '=') != NULL)
//             {
//                 // First join key and =
//                 temp = ft_strjoin(processed_key, "=");
//                 if (!temp)
//                 {
//                     free(processed_key);
//                     if (processed_value) free(processed_value);
//                     ft_free_array(split_str);
//                     ft_free_array(split_befor);
//                     return NULL;
//                 }
                
//                 // Then join with value if it exists
//                 if (processed_value)
//                 {
//                     result = ft_strjoin(temp, processed_value);
//                     free(temp);
//                     free(processed_value);
//                 }
//                 else
//                 {
//                     result = temp; // Just use the key= as result
//                 }
//             }
//             else
//             {
//                 // No equals sign, just use the processed key
//                 result = processed_key;
//                 processed_key = NULL; // Prevent it from being freed
//             }
            
//             // Clean up
//             ft_free_array(split_str);
//             ft_free_array(split_befor);
//             if (processed_key) free(processed_key);
            
//             return result;
//         }
//         else if (str[0] == '=' && ft_lint(split_str) == 1 && split_befor[0][0] == '$')
//         {
//             char *proccess = selective_remove_quotes(split_str[0], 0, env);
//             proccess = ft_strjoin("=", proccess);
//             there_was_export_befor = 0;
//             return proccess;
//         }
//         else if (ft_lint(split_befor) == 1 && split_befor[0][0] == '$')
//         {
//              char *proccess = selective_remove_quotes(split_str[0], 0, env);
//              there_was_export_befor = 0;
//              return proccess;
//         }
//         there_was_export_befor = 0;
//     }
    
//     return NULL;
// }







// void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode, t_env *env)
// {
//     char *processed;
    
//     // Initialize i to 0
//     (*i) = 0;
    
//     // Process args array if it exists
//     if (current->args)
//     {
//         while (current->args[(*i)])
//         {
//             // Initialize processed to NULL
//             processed = NULL;
            
//             // Only try check_export_case if args_befor_quotes_remover exists and has a valid entry at index i
//             if (current->args_befor_quotes_remover && 
//                 (*i) < ft_lint(current->args_befor_quotes_remover) && // Add a size check
//                 current->args_befor_quotes_remover[(*i)] != NULL)
//             {
//                 processed = check_export_case(current->args[(*i)], 
//                                              current->args_befor_quotes_remover[(*i)], 
//                                              remove_mode, env);
//             }
            
//             // If processed is still NULL, use selective_remove_quotes
//             if (processed == NULL)
//             {
//                 processed = selective_remove_quotes(current->args[(*i)], remove_mode, env);
//             }
            
//             // Replace the original string with the processed one if we got a valid result
//             if (processed)
//             {
//                 free(current->args[(*i)]);
//                 current->args[(*i)] = processed;
//             }
            
//             (*i)++;
//         }
//     }
    
//     // Process cmd if it exists
//     if (current->cmd)
//     {
//         // Initialize processed to NULL
//         processed = NULL;
        
//         // Only try check_export_case if args_befor_quotes_remover exists and has a valid first element
//         if (current->args_befor_quotes_remover && 
//             current->args_befor_quotes_remover[0] != NULL)
//         {
//             processed = check_export_case(current->cmd, 
//                                          current->args_befor_quotes_remover[0], 
//                                          remove_mode, env);
//         }
        
//         // If processed is still NULL, use selective_remove_quotes
//         if (processed == NULL)
//         {
//             processed = selective_remove_quotes(current->cmd, remove_mode, env);
//         }
        
//         // Replace the original string with the processed one if we got a valid result
//         if (processed)
//         {
//             free(current->cmd);
//             current->cmd = processed;
//         }
//     }
// }



// void process_quotes_for_cmd(t_cmd *cmd_list, t_env  *env, int remove_mode)
// {
//     t_cmd *current;
//     char *processed;
//     int i;

//     current = cmd_list;
//     while (current)
//     {
//         process_quotes_for_cmd_hp(current, &i, remove_mode, env);     
//         t_redir *redir = current->redirs;
//         while (redir)
//         {
//             if (redir->file)
//             {
//                 processed = selective_remove_quotes(redir->file, remove_mode, env);
//                 if (processed)
//                 {
//                     free(redir->file);
//                     redir->file = processed;
//                 }
//             }
//             redir = redir->next;
//         }
//         current = current->next;
//     }
// }


//================================================================

int process_quote_char(char c, int *quote_state, char *new_str, int *j, int remove_mode)
{
    if (c == '\'')
    {
        if (*quote_state == 0)
            *quote_state = 1;
        else if (*quote_state == 1)
            *quote_state = 0;
        if (remove_mode == 0)
            new_str[(*j)++] = c;
        return (1);
    }
    else if (c == '\"')
    {
        if (*quote_state == 0)
            *quote_state = 2;
        else if (*quote_state == 2)
            *quote_state = 0;
        if (remove_mode == 0)
            new_str[(*j)++] = c;
        return (1);
    }
    return (0);
}

char *allocate_and_init(char *str, int *i, int *j, int *quote_state)
{
    char *new_str;
    
    (*i) = 0;
    (*j) = 0;
    (*quote_state) = 0;
    
    if (!str)
        return (NULL);
    new_str = malloc(strlen(str) + 1);
    if (!new_str)
        return (NULL);
    return new_str;
}

char *selective_remove_quotes(char *str, int remove_mode)
{
    char *new_str;
    int i;
    int j;
    int quote_state;
    int in_opposite_quote;
    
   new_str = allocate_and_init(str, &i, &j, &quote_state);
    if (!new_str)
        return (NULL);
    while (str[i])
    {
        in_opposite_quote = 0;
        if ((str[i] == '\'' && quote_state == 2) || 
            (str[i] == '\"' && quote_state == 1))
        {
            new_str[j++] = str[i++];
            in_opposite_quote = 1;
        }
        else if (!in_opposite_quote && !process_quote_char(str[i], &quote_state, new_str, &j, remove_mode))
            new_str[j++] = str[i++];
        else
            i++;
    }
    return (new_str[j] = '\0', new_str);
}

void checK_if_there_a_var_befor_expand(char *str, t_env *env)
{
    int i = 0;
    char *key_name = NULL;
    char *true_or_not = NULL;
    int start = 0;
    while (str && str[i] && str[i] != '$')
    {
        i++;
    }
    if (str[i] == '$')
        start = i;
     while (str && str[i])
    {
        i++;
    }
    key_name = ft_substr(str, start, i);
    true_or_not = lookup_variable(key_name, env);
    if (true_or_not == NULL)
        printf("is not a var\n");
    else  if (true_or_not != NULL)
        printf("is a realy var\n");
    printf("true_or_not ========> %s\n", true_or_not);
    printf("key_name ========> %s\n", key_name);

        

    
    
}

void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode, t_env *env)
{
    char *processed;
    if (current->args)
        {
            (*i) = 0;
            while (current->args[(*i)])
            {
                checK_if_there_a_var_befor_expand(current->args_befor_quotes_remover[(*i)], env);
                
                
                processed = selective_remove_quotes(current->args[(*i)], remove_mode);
                if (processed)
                {
                    free(current->args[(*i)]);
                    current->args[(*i)] = processed;
                }
                (*i)++;
            }
        }
        if (current->cmd)
        {
            (*i) = 0;
            processed = selective_remove_quotes(current->cmd, remove_mode);
            if (processed)
                {
                    free(current->cmd);
                    current->cmd = processed;
                }
        }
}


void process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode, t_env *env)
{
    t_cmd *current;
    char *processed;
    int i;

    current = cmd_list;
    while (current)
    {
        process_quotes_for_cmd_hp(current, &i, remove_mode, env);
        t_redir *redir = current->redirs;
        while (redir)
        {
            if (redir->file)
            {
                processed = selective_remove_quotes(redir->file, remove_mode);
                if (processed)
                {
                    free(redir->file);
                    redir->file = processed;
                }
            }
            redir = redir->next;
        }
        current = current->next;
    }
}
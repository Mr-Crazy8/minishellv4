/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:21:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/05/22 21:25:27 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
// Function to free an array of strings (NULL-terminated)
void ft_free_array(char **array)
{
    int i;
    
    if (!array)
        return;
        
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    
    free(array);
}


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

char *selective_remove_quotes(char *str, int remove_mode, t_env  *env)
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
        
        // if (is_valid_key(str))
        // {
        //     new_str[j++] = str[i++];
        //     continue;
        // }
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
// char *check_export_case(char *str, char *befor, int remove_mode, t_env  *env)
// {
//     static int there_was_export_befor = 0;
//     char **split_str;
//     char **split_befor;
    
//     if (strcmp(str, "export") == 0)
//         there_was_export_befor++;
//     else if (strcmp(str, "export") != 0)
//     {
//         split_str = ft_split(str, '=');
//         split_befor = ft_split(befor, '=');
//         // free(split_str[0]);
//         if (split_befor && split_str[0] && split_str[0][0] != '$' && split_befor[0][0] != '$')
//             remove_mode = 1;
//         else
//             remove_mode = 0;
//         split_str[0] =  selective_remove_quotes(split_str[0], remove_mode,  env);
//         // free(split_str[1]);
//         if (split_befor && split_str[1] && split_str[1][0] != '$' && split_befor[1][0] != '$')
//             remove_mode = 1;
//         else
//             remove_mode = 0;
//         split_str[1] =  selective_remove_quotes(split_str[1], remove_mode,  env);
//         if (strchr(str, '=') != NULL)
//         {
//             str = ft_strjoin(split_str[0], "=");
//         str = ft_strjoin(str, split_str[1]);
            
//         }
//         return str;
//         printf("str===================== %s\n", str);
//           printf("split_str[0] ////// split_befor[0] ========= %s    ///// %s\n", split_str[0], split_befor[0]);
//         printf("split_str[1] /// split_befor[1] ========= %s ///// %s\n", split_str[1], split_befor[1]);
        

//     }
//     return NULL;
// }

char *check_export_case(char *str, char *befor, int remove_mode, t_env *env)
{
    static int there_was_export_befor = 0;
    char **split_str;
    char **split_befor;
    char *result = NULL; // To hold our final result
    char *temp = NULL;   // For temporary strings
    
    if (strcmp(str, "export") == 0)
    {
        there_was_export_befor++;
        return NULL;
    }
    else if (strcmp(str, "export") != 0)
    {
        split_str = ft_split(str, '=');
        split_befor = ft_split(befor, '=');
        
        if (!split_str || !split_befor)
        {
            // Handle error case - clean up and return
            if (split_str) ft_free_array(split_str); // Assuming you have a function to free arrays
            if (split_befor) ft_free_array(split_befor);
            return NULL;
        }
        
        // Determine removal mode for key part
        if (split_befor && split_str[0] && split_str[0][0] != '$' && split_befor[0][0] != '$')
            remove_mode = 1;
        else
            remove_mode = 0;
            
        // Process key part
        char *processed_key = selective_remove_quotes(split_str[0], remove_mode, env);
        
        // Determine removal mode for value part
        if (split_befor && split_str[1] && split_str[1][0] != '$' && split_befor[1][0] != '$')
            remove_mode = 1;
        else
            remove_mode = 0;
            
        // Process value part
        char *processed_value = NULL;
        if (split_str[1]) // Check if value exists
            processed_value = selective_remove_quotes(split_str[1], remove_mode, env);
        
        // Combine key and value if there was an equals sign
        if (strchr(str, '=') != NULL)
        {
            // First join key and =
            temp = ft_strjoin(processed_key, "=");
            if (!temp)
            {
                // Handle error - clean up and return
                free(processed_key);
                if (processed_value) free(processed_value);
                ft_free_array(split_str);
                ft_free_array(split_befor);
                return NULL;
            }
            
            // Then join with value if it exists
            if (processed_value)
            {
                result = ft_strjoin(temp, processed_value);
                free(temp); // Free the temporary string
                temp = NULL;
                
                // Clean up value
                free(processed_value);
            }
            else
            {
                result = temp; // Just use the key= as result
                temp = NULL;
            }
        }
        else
        {
            // No equals sign, just use the processed key
            result = processed_key;
            processed_key = NULL; // Prevent it from being freed
        }
        
        // Clean up
        ft_free_array(split_str);
        ft_free_array(split_befor);
        if (processed_key) free(processed_key);
        
        return result;
    }
    
    return NULL;
}

// void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode, t_env  *env)
// {
//     char *processed;
//     char *exp;
//     if (current->args)
//         {
//             (*i) = 0;
//             while (current->args[(*i)])
//             {
//                 exp = check_export_case(current->args[(*i)], current->args_befor_quotes_remover[(*i)],  remove_mode, env);
//                 if (exp != NULL)
//                     processed = exp;
//                 else if (exp == NULL)
//                     processed = selective_remove_quotes(current->args[(*i)], remove_mode,  env);
//                 if (processed)
//                 {
//                     free(current->args[(*i)]);
//                     current->args[(*i)] = processed;
//                 }
//                 (*i)++;
//             }
//         }
//         if (current->cmd)
//         {
//             (*i) = 0;
//             processed = selective_remove_quotes(current->cmd, remove_mode, env);
//             if (processed)
//                 {
//                     free(current->cmd);
//                     current->cmd = processed;
//                 }
//         }
// }

void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode, t_env *env)
{
    char *processed;
    
    if (current->args)
    {
        (*i) = 0;
        while (current->args[(*i)])
        {
            // Try to get a processed string from check_export_case
            processed = check_export_case(current->args[(*i)], 
                                         current->args_befor_quotes_remover[(*i)], 
                                         remove_mode, env);
                                         
            // If check_export_case didn't handle it, use selective_remove_quotes
            if (processed == NULL)
            {
                processed = selective_remove_quotes(current->args[(*i)], remove_mode, env);
            }
            
            // Replace the original string with the processed one
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
        // Try to get a processed string from check_export_case
            processed = check_export_case(current->cmd, 
                                         current->args_befor_quotes_remover[0], 
                                         remove_mode, env);
                                         
            // If check_export_case didn't handle it, use selective_remove_quotes
            if (processed == NULL)
            {
                processed = selective_remove_quotes(current->cmd, remove_mode, env);
            }
        if (processed)
        {
            free(current->cmd);
            current->cmd = processed;
        }
    }
}


void process_quotes_for_cmd(t_cmd *cmd_list, t_env  *env, int remove_mode)
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
                processed = selective_remove_quotes(redir->file, remove_mode, env);
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

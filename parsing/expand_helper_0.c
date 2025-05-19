/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:21:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/05/18 18:55:41 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"


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

// char *selective_remove_quotes(char *str, int remove_mode)
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
// int check_for_case(char *str)
// {

//     if(strchr(str, '=') && strchr(str, '$'))
//         return 1;
//     else if (strchr(str, '$'))
//         return 2;
//     return 0;
// }

// void process_quotes_for_cmd_hp(t_cmd *current, t_env *env, int *i, int remove_mode)
// {
//     char *processed;
//     char **split;
//     char **split2;
//     if (current->args)
//         {
//             (*i) = 0;
//             while (current->args[(*i)] )
//             {

//                 if (current->args[(*i)] && current->args_befor_quotes_remover[(*i)] && strchr(current->args_befor_quotes_remover[(*i)], '=') != NULL && strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
//                     {
//                         split = ft_split(current->args_befor_quotes_remover[(*i)], '=');
//                         processed = selective_remove_quotes(split[0], remove_mode);
//                         processed = ft_strjoin(processed, "=");
//                         if (strchr(split[1], '$') != NULL)
//                             {
//                                 split2 = ft_split(current->args[(*i)], '=');
//                                 processed = ft_strjoin(processed, split2[1]);
//                             }
//                         else
//                             processed = ft_strjoin(processed, split[1]);
                        
//                     }
//                     else if (current->args[(*i)] && current->args_befor_quotes_remover[(*i)] && strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
//                         processed = selective_remove_quotes(current->args[(*i)], 0);
//                     else
//                         processed = selective_remove_quotes(current->args[(*i)], remove_mode);
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
//             if (current->args_befor_quotes_remover[0][0] == '$')
//                 remove_mode = 0;
//             processed = selective_remove_quotes(current->cmd, remove_mode);
//             if (processed)
//                 {
//                     free(current->cmd);
//                     current->cmd = processed;
//                 }
//         }
// }




// void process_quotes_for_cmd(t_cmd *cmd_list, t_env *env, int remove_mode)
// {
//     t_cmd *current;
//     char *processed;
//     int i;

//     current = cmd_list;
//     while (current)
//     {
//         process_quotes_for_cmd_hp(current, env, &i, remove_mode);
//         t_redir *redir = current->redirs;
//         while (redir)
//         {
//             if (redir->file)
//             {
//                 if  ((redir->orig_token[0] == '\'' || redir->orig_token[0] == '\"') && redir->orig_token[1] == '$')
//                     remove_mode = 1;
//                 else if (redir->orig_token[0] == '$')
//                     remove_mode = 0;
//                 processed = selective_remove_quotes(redir->file, remove_mode);
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

    if (!str)
        return (NULL);
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
    new_str[j] = '\0';
    return (new_str);
}


void free_split_array(char **split)
{
    int i;

    if (!split)
        return;
    i = 0;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

// void process_quotes_for_cmd_hp(t_cmd *current, t_env *env, int *i, int remove_mode)
// {
//     char *processed = NULL;
//     char *temp = NULL;
//     char **split = NULL;
//     char **split2 = NULL;
    
//     if (!current || !current->args)
//         return;

//     (*i) = 0;
//     while (current->args[(*i)] && current->args_befor_quotes_remover && current->args_befor_quotes_remover[(*i)])
//     {
//         processed = NULL; // Reset processed for each iteration

//         if (strchr(current->args_befor_quotes_remover[(*i)], '=') != NULL && 
//             strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
//         {
//             split = ft_split(current->args_befor_quotes_remover[(*i)], '=');
//             if (split && split[0])
//             {
//                 processed = selective_remove_quotes(split[0], remove_mode);
//                 if (processed)
//                 {
//                     temp = ft_strjoin(processed, "=");
//                     free(processed);
//                     processed = NULL;
                    
//                     if (temp && split[1])
//                     {
//                         if (strchr(split[1], '$') != NULL)
//                         {
//                             split2 = ft_split(current->args[(*i)], '=');
//                             if (split2 && split2[1])
//                             {
//                                 processed = ft_strjoin(temp, split2[1]);
//                                 free(temp);
//                                 temp = NULL;
//                             }
//                             else
//                             {
//                                 processed = temp;
//                                 temp = NULL;
//                             }
//                             free_split_array(split2);
//                             split2 = NULL;
//                         }
//                         else
//                         {
//                             processed = ft_strjoin(temp, split[1]);
//                             free(temp);
//                             temp = NULL;
//                         }
//                     }
//                     else
//                     {
//                         processed = temp;
//                         temp = NULL;
//                     }
//                 }
//             }
//             free_split_array(split);
//             split = NULL;
//         }
//         else if (strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
//         {
//             processed = selective_remove_quotes(current->args[(*i)], 0);
//         }
//         else
//         {
//             processed = selective_remove_quotes(current->args[(*i)], remove_mode);
//         }
        
//         if (processed)
//         {
//             free(current->args[(*i)]);
//             current->args[(*i)] = processed;
//             processed = NULL;
//         }
//         (*i)++;
//     }
    
//     if (current->cmd)
//     {
//         (*i) = 0;
//         if (current->args_befor_quotes_remover && current->args_befor_quotes_remover[0] && 
//             current->args_befor_quotes_remover[0][0] == '$')
//             remove_mode = 0;
        
//         processed = selective_remove_quotes(current->cmd, remove_mode);
//         if (processed)
//         {
//             free(current->cmd);
//             current->cmd = processed;
//         }
//     }
// }


void process_quotes_for_cmd_hp(t_cmd *current, t_env *env, int *i, int remove_mode)
{
    char *processed = NULL;
    char *temp = NULL;
    char **split = NULL;
    char **split2 = NULL;
    
    if (!current || !current->args)
        return;

    (*i) = 0;
    while (current->args[(*i)] && current->args_befor_quotes_remover && current->args_befor_quotes_remover[(*i)])
    {
        processed = NULL;
        if (current->cmd && strcmp(current->cmd, "export") == 0 && 
            current->args_befor_quotes_remover[(*i)][0] == '$' && 
            strchr(current->args_befor_quotes_remover[(*i)], '='))
        {
            // For export $q=$a type commands, we need to preserve the expanded variable name
            
            // Get the already expanded argument which should have the correct variable name
            char *expanded_arg = current->args[(*i)];
            
            // Skip the original processing - just duplicate the already expanded string
            processed = strdup(expanded_arg);
            
            // Continue to next argument - we've handled this special case
            if (processed)
            {
                free(current->args[(*i)]);
                current->args[(*i)] = processed;
                processed = NULL;
            }
            (*i)++;
            continue;  // Skip the rest of the processing for this argument
        }
        
        // Regular processing for other cases
        if (strchr(current->args_befor_quotes_remover[(*i)], '=') != NULL && 
            strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
        {
            split = ft_split(current->args_befor_quotes_remover[(*i)], '=');
            if (split && split[0])
            {
                processed = selective_remove_quotes(split[0], remove_mode);
                if (processed)
                {
                    temp = ft_strjoin(processed, "=");
                    free(processed);
                    processed = NULL;
                    
                    if (temp && split[1])
                    {
                        if (strchr(split[1], '$') != NULL)
                        {
                            split2 = ft_split(current->args[(*i)], '=');
                            if (split2 && split2[1])
                            {
                                processed = ft_strjoin(temp, split2[1]);
                                free(temp);
                                temp = NULL;
                            }
                            else
                            {
                                processed = temp;
                                temp = NULL;
                            }
                            free_split_array(split2);
                            split2 = NULL;
                        }
                        else
                        {
                            processed = ft_strjoin(temp, split[1]);
                            free(temp);
                            temp = NULL;
                        }
                    }
                    else
                    {
                        processed = temp;
                        temp = NULL;
                    }
                }
            }
            free_split_array(split);
            split = NULL;
        }
        else if (strchr(current->args[(*i)], '\"') == NULL && strchr(current->args[(*i)], '\'') == NULL && strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
        {
            remove_mode = 0;
            processed = selective_remove_quotes(current->args[(*i)], remove_mode);
        }
        else
        {
            processed = selective_remove_quotes(current->args[(*i)], remove_mode);
        }
        if (processed)
        {
            free(current->args[(*i)]);
            current->args[(*i)] = processed;
            processed = NULL;
        }
        (*i)++;
    }
    
    if (current->cmd)
    {
        (*i) = 0;
        if (current->args_befor_quotes_remover && current->args_befor_quotes_remover[0] && 
            current->args_befor_quotes_remover[0][0] == '$')
            remove_mode = 0;
        
        processed = selective_remove_quotes(current->cmd, remove_mode);
        if (processed)
        {
            free(current->cmd);
            current->cmd = processed;
        }
    }
}

void process_quotes_for_cmd(t_cmd *cmd_list, t_env *env, int remove_mode)
{
    t_cmd *current;
    char *processed;
    int i;

    current = cmd_list;
    while (current)
    {
        process_quotes_for_cmd_hp(current, env, &i, remove_mode);
        t_redir *redir = current->redirs;
        while (redir)
        {
            if (redir->file)
            {
                if (redir->orig_token && 
                    ((redir->orig_token[0] == '\'' || redir->orig_token[0] == '\"') && 
                     redir->orig_token[1] == '$'))
                    remove_mode = 1;
                else if (redir->orig_token && redir->orig_token[0] == '$')
                    remove_mode = 0;
                
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

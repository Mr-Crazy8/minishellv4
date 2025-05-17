/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:21:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/05/17 17:03:23 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

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
int check_for_case(char *str)
{

    if(strchr(str, '=') && strchr(str, '$'))
        return 1;
    else if (strchr(str, '$'))
        return 2;
    return 0;
}

// void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode)
// {
//     char *processed;
//     char **split;
//     char **split2;
//     if (current->args)
//         {
//             (*i) = 0;
//             while (current->args[(*i)] )
//             {
//                     if (strchr(current->args_befor_quotes_remover[(*i)], '=') != NULL && strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
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
//                     else if (strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
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

void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode)
{
    char *processed = NULL;
    char **split;
    char **split2;
    
    if (current->args)
    {
        (*i) = 0;
        while (current->args[(*i)])
        {
            processed = NULL; // Reset processed for each iteration
            
            // Only access args_befor_quotes_remover if it exists
            if (current->args_befor_quotes_remover)
            {
                // Make sure we don't access beyond the end of args_befor_quotes_remover array
                int j = 0;
                while (current->args_befor_quotes_remover[j] != NULL)
                    j++;
                
                // Only access if the index is within bounds
                if ((*i) < j && current->args_befor_quotes_remover[(*i)])
                {
                    if (strchr(current->args_befor_quotes_remover[(*i)], '=') != NULL && 
                        strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
                    {
                        split = ft_split(current->args_befor_quotes_remover[(*i)], '=');
                        if (split && split[0]) 
                        {
                            processed = selective_remove_quotes(split[0], remove_mode);
                            char *temp = processed;
                            processed = ft_strjoin(processed, "=");
                            free(temp);
                            
                            if (split[1] && strchr(split[1], '$') != NULL)
                            {
                                split2 = ft_split(current->args[(*i)], '=');
                                if (split2 && split2[1]) 
                                {
                                    temp = processed;
                                    processed = ft_strjoin(processed, split2[1]);
                                    free(temp);
                                    
                                    // Free split2
                                    j = 0;
                                    while (split2[j])
                                    {
                                        free(split2[j]);
                                        j++;
                                    }
                                    free(split2);
                                }
                            }
                            else if (split[1])
                            {
                                temp = processed;
                                processed = ft_strjoin(processed, split[1]);
                                free(temp);
                            }
                            
                            // Free split
                            j = 0;
                            while (split[j])
                            {
                                free(split[j]);
                                j++;
                            }
                            free(split);
                        }
                    }
                    else if (strchr(current->args_befor_quotes_remover[(*i)], '$') != NULL)
                    {
                        processed = selective_remove_quotes(current->args[(*i)], 0);
                    }
                    else
                    {
                        processed = selective_remove_quotes(current->args[(*i)], remove_mode);
                    }
                }
                else
                {
                    // Index out of bounds, use regular handling
                    processed = selective_remove_quotes(current->args[(*i)], remove_mode);
                }
            }
            else
            {
                // args_befor_quotes_remover is NULL
                processed = selective_remove_quotes(current->args[(*i)], remove_mode);
            }
            
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
        int cmd_remove_mode = remove_mode;
        
        // Safe check for args_befor_quotes_remover
        if (current->args_befor_quotes_remover && 
            current->args_befor_quotes_remover[0] && 
            current->args_befor_quotes_remover[0][0] == '$')
        {
            cmd_remove_mode = 0;
        }
        
        processed = selective_remove_quotes(current->cmd, cmd_remove_mode);
        if (processed)
        {
            free(current->cmd);
            current->cmd = processed;
        }
    }
}


void process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode)
{
    t_cmd *current;
    char *processed;
    int i;

    current = cmd_list;
    while (current)
    {
        process_quotes_for_cmd_hp(current, &i, remove_mode);
        t_redir *redir = current->redirs;
        while (redir)
        {
            if (redir->file)
            {
                if  ((redir->orig_token[0] == '\'' || redir->orig_token[0] == '\"') && redir->orig_token[1] == '$')
                    remove_mode = 1;
                else if (redir->orig_token[0] == '$')
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:21:05 by anel-men          #+#    #+#             */
/*   Updated: 2025/05/19 11:52:29 by anel-men         ###   ########.fr       */
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
// int check_export_quotes_case(char **str)
// {
    
// }

void process_quotes_for_cmd_hp(t_cmd *current, int *i, int remove_mode)
{
    char *processed;
    
   
    
    if (current->args)
        {
            (*i) = 0;
            while (current->args[(*i)])
            {
                // if (check_export_quotes_case(current->args[(*i)]) == 1)
                // {
                //     remove_mode = export_quotes_handle(current->args[(*i)]);
                // }
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

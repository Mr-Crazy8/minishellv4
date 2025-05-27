#include "parsing.h"

void free_redirs(t_redir *redir_list)
{
    t_redir *current;
    t_redir *next;

    current = redir_list;

    while(current)
    {
        next = current->next;
        if (current->file)
            free(current->file);
        if (current->orig_token)
            free(current->orig_token);
        free(current);
        current = next;
    }
}


void free_cmd_list(t_cmd *cmd_list)
{
    t_cmd *current;
    t_cmd *next;

    current = cmd_list;
    while (current)
    {
        next = current->next;
        if (current->cmd)
            free(current->cmd);
        int i = 0;
        while(current->args && current->args[i])
            {
                free(current->args[i]);
                i++;
            }
         if (current->args)
            free(current->args);
        i = 0;
        while(current->args_befor_quotes_remover && current->args_befor_quotes_remover[i])
        {
            free(current->args_befor_quotes_remover[i]);
            i++;
        }
        if (current->args_befor_quotes_remover)
            free(current->args_befor_quotes_remover);
        if (current->redirs)
            free_redirs(current->redirs);
        free(current);
        current = next;
    }
}
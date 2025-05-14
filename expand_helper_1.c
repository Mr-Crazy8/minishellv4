#include "minishell.h"

char	*lookup_variable(char *var_name, t_env *env_struct)
{
	t_env	*tmp;
	tmp = env_struct;
	while (tmp)
	{
		if (strcmp(var_name, tmp->key) == 0)
		{
			if (!tmp->value)
				return (ft_strdup(""));
			else
				return (ft_strdup(tmp->value));
		}
		tmp = tmp->next;
	}
	return (ft_strdup(""));
}

// // Add this function to expand_helper_1.c
// int expand_resize_buffer(t_exp_helper *expand, size_t needed_size)
// {
//     // Calculate new size (double the current size or ensure it fits the needed size)
//     size_t new_size = expand->buffer_size * 3;
//     if (new_size < needed_size)
//         new_size = needed_size;
    
//     // Allocate new buffer
//     char *new_buffer = malloc(new_size);
//     if (!new_buffer)
//         return (0);
    
//     // Copy existing content to new buffer
//     memcpy(new_buffer, expand->expanded, expand->j);
    
//     // Free old buffer and update pointer and size
//     free(expand->expanded);
//     expand->expanded = new_buffer;
//     expand->buffer_size = new_size;
    
//     return (1);
// }

int expand_fill_str(t_exp_helper *expand, char *str)
{
	size_t buffer_size;
	char *var_pos;
	size_t original_len;
    expand->original = str;

	original_len = strlen(expand->original);
	buffer_size = original_len * 2;



    expand->expanded = malloc((strlen(str) * 10) + 1024);
    if (!expand->expanded)
        return (0);
    
	// expand->buffer_size = buffer_size;
    expand->i = 0;
    expand->j = 0;
    expand->quote_state = 0;
    
    return (1);
}

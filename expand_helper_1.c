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



int expand_fill_str(t_exp_helper *expand, char *str)
{
	size_t buffer_size;
	char *var_pos;
	size_t original_len;
    expand->original = str;

	original_len = strlen(expand->original);
	buffer_size = original_len + 1;

	if (strchr(str, '$'))
		buffer_size = (buffer_size * 10) + 40096;
	else
		buffer_size	= buffer_size * 2;
    expand->expanded = malloc(buffer_size);
    if (!expand->expanded)
        return (0);
    
    expand->i = 0;
    expand->j = 0;
    expand->quote_state = 0;
    
    return (1);
}

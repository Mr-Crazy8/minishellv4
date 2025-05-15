#include "minishell.h"

char *lookup_variable(char *var_name, t_env *env_struct)
{
	t_env *tmp = env_struct;
	while (tmp)
	{
		if (strcmp(var_name, tmp->key) == 0)
			return (tmp->value ? ft_strdup(tmp->value) : ft_strdup(""));
		tmp = tmp->next;
	}
	return (ft_strdup(""));
}

int expand_fill_str(t_exp_helper *expand, char *str)
{
	expand->original = str;
	size_t original_len = strlen(str);
	if (original_len > SIZE_MAX / 2 - 1024)
	{
		fprintf(stderr, "minishell: memory allocation failed: input too large\n");
		return (0);
	}
	expand->buffer_size = original_len * 2 + 1024;

	expand->expanded = malloc(expand->buffer_size);
	if (!expand->expanded)
	{
		fprintf(stderr, "minishell: memory allocation failed\n");
		exit(1);
	}

	expand->i = 0;
	expand->j = 0;
	expand->quote_state = 0;
	expand->var_name = NULL;
	expand->var_value = NULL;

	return (1);
}
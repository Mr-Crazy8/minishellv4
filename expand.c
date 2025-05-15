#include "minishell.h"

#define MAX_BUFFER_SIZE (1ULL << 30) // 1GB max buffer size

int expand_handle_helper0(t_exp_helper *expand)
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
	if (!expand->buffer_size)
	{
		size_t original_len = strlen(expand->original);
		if (original_len > SIZE_MAX / 2 - 1024)
		{
			fprintf(stderr, "minishell: memory allocation failed: input too large\n");
			return (0);
		}
		expand->buffer_size = original_len * 2 + 1024;
	}

	if (expand->j + additional_needed >= expand->buffer_size)
	{
		size_t new_size = expand->buffer_size * 2;
		if (new_size < expand->j + additional_needed + 1)
			new_size = expand->j + additional_needed + 1024;

		// if (new_size > MAX_BUFFER_SIZE || new_size < expand->buffer_size)
		// {
		// 	fprintf(stderr, "minishell: memory allocation failed: buffer size too large\n");
		// 	return (0);
		// }
		char *new_buffer = calloc(1, new_size);
		if (!new_buffer)
		{
			fprintf(stderr, "minishell: memory allocation failed\n");
			exit(1);
		}

		if (expand->expanded)
		{
			memcpy(new_buffer, expand->expanded, expand->j);
			free(expand->expanded);
		}
		expand->expanded = new_buffer;
		expand->buffer_size = new_size;
	}
	return (1);
}

int helper3(t_exp_helper *expand, int exit_status)
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
		if (helper3(expand, exit_status) == 0)
		{
			expand->start = expand->i;
			while (expand->original[expand->i] && is_valid_var_char(expand->original[expand->i]))
				expand->i++;
			size_t var_len = expand->i - expand->start;
			if (var_len > SIZE_MAX - 1)
			{
				fprintf(stderr, "minishell: memory allocation failed: variable name too long\n");
				return (0);
			}
			expand->var_name = malloc(var_len + 1);
			if (!expand->var_name)
			{
				fprintf(stderr, "minishell: memory allocation failed\n");
				exit(1);
			}
			memcpy(expand->var_name, expand->original + expand->start, var_len);
			expand->var_name[var_len] = '\0';
			expand->var_value = lookup_variable(expand->var_name, env);
			free(expand->var_name);
			expand->var_name = NULL;
		}

		if (expand->var_value)
		{
			size_t len = strlen(expand->var_value);
			if (len > SIZE_MAX - expand->j)
			{
				fprintf(stderr, "minishell: memory allocation failed: buffer overflow\n");
				free(expand->var_value);
				expand->var_value = NULL;
				return (0);
			}
			if (!ensure_buffer_space(expand, len))
			{
				free(expand->var_value);
				expand->var_value = NULL;
				return (0);
			}
			memcpy(expand->expanded + expand->j, expand->var_value, len);
			expand->j += len;
		}

		if (expand->var_value)
		{
			free(expand->var_value);
			expand->var_value = NULL;
		}
		return (1);
	}
	return (0);
}

void process_string(char *str, t_exp_helper *expand, t_env *env, int exit_status)
{
	if (!expand_fill_str(expand, str))
	{
		expand->expanded = NULL;
		return;
	}

	while (expand->original[expand->i])
	{
		if (!expand_handle_helper0(expand) && !expand_handle_helper1(expand, exit_status, env))
		{
			if (!ensure_buffer_space(expand, 1))
			{
				free(expand->expanded);
				expand->expanded = NULL;
				break;
			}
			expand->expanded[expand->j++] = expand->original[expand->i++];
		}
	}

	if (expand->expanded)
		expand->expanded[expand->j] = '\0';
}

void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)
{
	t_cmd *current;
	t_exp_helper *expand;
	t_redir *redir;
	int i;
	int should_split = 0;

	expand = malloc(sizeof(t_exp_helper));
	if (!expand)
	{
		fprintf(stderr, "minishell: memory allocation failed\n");
		exit(1);
	}
	expand->buffer_size = 0;
	expand->expanded = NULL;

	current = cmd_list;
	while (current)
	{
		should_split = 0;
		if (current->cmd)
		{
			process_string(current->cmd, expand, env, exit_status);
			free(current->cmd);
			current->cmd = expand->expanded;
			expand->expanded = NULL;
		}

		i = 0;
		while (current->args && current->args[i])
		{
			if ((current->args[i][0] == '\'' && current->args[i][strlen(current->args[i]) - 1] == '\'') ||
				(current->args[i][0] == '"' && current->args[i][strlen(current->args[i]) - 1] == '"'))
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
			expand->expanded = NULL;
			i++;
		}

		redir = current->redirs;
		while (redir)
		{
			if (redir->file)
			{
				process_string(redir->file, expand, env, exit_status);
				free(redir->file);
				redir->file = expand->expanded;
				expand->expanded = NULL;
			}
			redir = redir->next;
		}
		current = current->next;
	}

	free(expand);
	if (should_split)
		apply_word_splitting(cmd_list);
}
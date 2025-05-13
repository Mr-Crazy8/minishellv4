
#include "minishell.h"
t_env *creat_new_env(char *key, char *value)
{
    t_env *new_node;

    new_node = malloc(sizeof(t_env));
    if(!new_node)
        return (NULL);
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return (new_node);
}
void env_null(t_env **list)
{
	ft_lstadd_back(list, creat_new_env("OLDPWD", NULL));
    ft_lstadd_back(list, creat_new_env("PWD", getcwd(NULL, 0)));
    ft_lstadd_back(list, creat_new_env("SHELVL", "1"));
}
void value_empty(t_env **env_list, char *key, char *value)
{
	t_env *tmp;
	t_env *new_node;

	tmp = *env_list;
	while(tmp)
	{
		if (strcmp(key, tmp->key) == 0)
		{
			free(tmp->value);
			tmp->value = strdup(value);
			tmp->is_not_active = 1;
			return;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if(!new_node)
		return;
	new_node->key = strdup(key);
	new_node->value = strdup(value);
	new_node->is_not_active = 1;
	new_node->next = NULL;
	ft_lstadd_back(env_list, new_node);
}
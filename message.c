// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include <string.h>

// #define BOLD_TEXT "\x1b[1m"
// #define RESET_COLOR "\x1b[0m"
// #define GREEN_COLOR "\x1b[32m"
// #define CYAN_COLOR "\x1b[36m"
// static	int	ft_count_strings(char const *s, char c)
// {
// 	int	i;
// 	int	count;

// 	i = 0;
// 	count = 0;
// 	while (s[i] != '\0')
// 	{
// 		while (s[i] != '\0' && s[i] == c)
// 			i++;
// 		if (s[i] != '\0')
// 		{
// 			count++;
// 		}
// 		while (s[i] != '\0' && s[i] != c)
// 		{
// 			i++;
// 		}
// 	}
// 	return (count);
// }

// static int	ft_lent_strlen(char const *s, char c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i] != '\0' && s[i] != c)
// 		i++;
// 	return (i);
// }

// static char	*ft_word(char const *s, char c)
// {
// 	int		len_word;
// 	int		i;
// 	char	*word;

// 	i = 0;
// 	len_word = ft_lent_strlen(s, c);
// 	word = (char *)malloc(len_word + 1);
// 	if (word == NULL)
// 	{
// 		return (NULL);
// 	}
// 	while (i < len_word)
// 	{
// 		word[i] = s[i];
// 		i++;
// 	}
// 	word[i] = '\0';
// 	return (word);
// }

// static int	ft_fill_string(char **str, char const *s, char c)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (s[i] != '\0')
// 	{
// 		while (s[i] != '\0' && s[i] == c)
// 			i++;
// 		if (s[i] != '\0')
// 		{
// 			str[j] = ft_word(&s[i], c);
// 			if (str[j] == NULL)
// 				return (0);
// 			j++;
// 		}
// 		while (s[i] != '\0' && s[i] != c)
// 			i++;
// 	}
// 	str[j] = 0;
// 	return (1);
// }

// char	**ft_split(char const *s, char c)
// {
// 	char	**strings;
// 	int		i;

// 	i = 0;
// 	if (s == NULL)
// 		return (NULL);
// 	strings = (char **)malloc(sizeof(char *) * (ft_count_strings(s, c) + 1));
// 	if (strings == NULL)
// 	{
// 		return (NULL);
// 	}
// 	if (ft_fill_string(strings, s, c) == 0)
// 	{
// 		while (strings[i] != NULL)
// 		{
// 			free(strings[i]);
// 			i++;
// 		}
// 		free(strings);
// 		return (NULL);
// 	}
// 	return (strings);
// }
// //******************ft_strdup;

// char	*ft_strdup(const char *s)
// {
// 	char	*ptr;
// 	int		i;

// 	i = 0;
// 	ptr = (char *) malloc (sizeof(char) * (strlen(s) + 1));
// 	if (ptr == NULL)
// 		return (NULL);
// 	while (s[i] != '\0')
// 	{
// 		ptr[i] = s[i];
// 		i++;
// 	}
// 	ptr[i] = '\0';
// 	return (ptr);
// }
// //******************ft_strjoine;
// static void	copy_strings(char *ptr, const char *s1, const char *s2)
// {
// 	unsigned int	i;
// 	unsigned int	j;

// 	i = 0;
// 	while (s1[i] != '\0')
// 	{
// 		ptr[i] = s1[i];
// 		i++;
// 	}
// 	j = 0;
// 	while (s2[j] != '\0')
// 	{
// 		ptr[i + j] = s2[j];
// 		j++;
// 	}
// 	ptr[i + j] = '\0';
// }

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char			*ptr;
// 	size_t			total_len;

// 	if (!s1 && !s2)
// 		return (NULL);
// 	if (!s1)
// 		return (ft_strdup(s2));
// 	if (!s2)
// 		return (ft_strdup(s1));
// 	total_len = strlen(s1) + strlen(s2);
// 	ptr = (char *)malloc (sizeof(char) * (total_len + 1));
// 	if (ptr == NULL)
// 		return (NULL);
// 	copy_strings(ptr, s1, s2);
// 	return (ptr);
// }
// // Keep all your existing utility functions (ft_split, ft_strdup, ft_strjoin, etc.)

// // Fixed handel_excution function that doesn't print the input
// char *handel_excution(char *input)
// {
//     char *path;
//     char *helper;
//     path = getenv("PATH");
//     char **split_string = ft_split(path, ':');
//     int i = 0;
    
//     while (split_string[i])
//     {
//         helper = ft_strjoin(split_string[i], "/");
//         char *tmp = ft_strjoin(helper, input);
//         free(helper); // Free helper to avoid memory leak
        
//         if(access(tmp, X_OK) == 0)
//             return (tmp);
        
//         free(tmp); // Free tmp if we didn't return it
//         i++;
//     }
    
//     // Free split_string array to avoid memory leak
//     i = 0;
//     while (split_string[i])
//     {
//         free(split_string[i]);
//         i++;
//     }
//     free(split_string);
    
//     return (NULL);
// }

// // Fixed check_pipe function to compare strings properly
// int check_pipe(char **str)
// {
//     int i = 0;
//     while (str[i])
//     {
//         if(strcmp(str[i], "|") == 0)
//             return (1);
//         i++;
//     }
//     return (0);
// }

// // Split the args array at the pipe symbol
// void split_at_pipe(char **args, char ***cmd1, char ***cmd2)
// {
//     int i = 0;
//     int pipe_pos = 0;
    
//     // Find the position of the pipe
//     while (args[pipe_pos] && strcmp(args[pipe_pos], "|") != 0)
//         pipe_pos++;
    
//     // Allocate memory for cmd1
//     *cmd1 = (char **)malloc(sizeof(char *) * (pipe_pos + 1));
//     if (!*cmd1)
//         return;
        
//     // Copy command before pipe
//     for (i = 0; i < pipe_pos; i++)
//         (*cmd1)[i] = strdup(args[i]);
//     (*cmd1)[i] = NULL;
    
//     // Find the end of args
//     int args_len = pipe_pos;
//     while (args[args_len])
//        args_len++;
     
//     // Allocate memory for cmd2
//     *cmd2 = (char **)malloc(sizeof(char *) * (args_len - pipe_pos));
//     if (!*cmd2)
//         return;
    
//     // Copy command after pipe
//     i = 0;
//     pipe_pos++; // Skip the pipe symbol
//     while (args[pipe_pos])
//     {
//         (*cmd2)[i] = strdup(args[pipe_pos]);
//         i++;
//         pipe_pos++;
//     }
//     (*cmd2)[i] = NULL;
// }

// void handle_pipe(char **cmd1, char **cmd2, char **env)
// {
//     int fd[2];  // File descriptors for the pipe
//     pid_t pid1, pid2;

//     // Step 1: Create the pipe
//     if (pipe(fd) == -1) {
//         perror("pipe");
//         exit(1);
//     }

//     // Step 2: Fork the first child process
//     pid1 = fork();
//     if (pid1 == -1) {
//         perror("fork");
//         exit(1);
//     }

//     if (pid1 == 0) {  // First child process
//         // Step 3: Redirect output to pipe
//         close(fd[0]);  // Close the read end
//         dup2(fd[1], STDOUT_FILENO);  // Redirect stdout to pipe
//         close(fd[1]);  // Close the original fd after duplication

//         // Get the executable path
//         char *path = handel_excution(cmd1[0]);
//         if (path) {
//             execve(path, cmd1, env);  // Execute the first command
//             free(path);
//         }
//         perror("execve");
//         exit(1);
//     }

//     // Step 5: Fork the second child process
//     pid2 = fork();
//     if (pid2 == -1) {
//         perror("fork");
//         exit(1);
//     }

//     if (pid2 == 0) {  // Second child process
//         // Step 6: Redirect input from pipe
//         close(fd[1]);  // Close the write end
//         dup2(fd[0], STDIN_FILENO);  // Redirect stdin to pipe
//         close(fd[0]);  // Close the original fd after duplication

//         // Get the executable path
//         char *path = handel_excution(cmd2[0]);
//         if (path) {
//             execve(path, cmd2, env);  // Execute the second command
//             free(path);
//         }
//         perror("execve");
//         exit(1);
//     }

//     // Step 8: Close both ends of the pipe in the parent
//     close(fd[0]);
//     close(fd[1]);

//     // Step 9: Wait for both children to finish
//     waitpid(pid1, NULL, 0);
//     waitpid(pid2, NULL, 0);
// }

// // int main(int ac, char **av, char **env)
// // {
// //     pid_t pid_child;
// //     char *input;
// //     char *path;
// //     char **args;
// //     int status;
    
// //     while(1)
// //     {
// //         // Get input and add to history
// //         char *input = readline(CYAN_COLOR BOLD_TEXT "bigshell > " RESET_COLOR);
// //         if(!input)
// //             break;
            
// //         // Skip empty input
// //         if (strlen(input) == 0) {
// //             free(input);
// //             continue;
// //         }
        
// //         add_history(input);
// //         args = ft_split(input, ' ');
        
// //         if(!args) {
// //             free(input);
// //             continue;
// //         }
        
// //         // Handle pipe if present
// //         if(check_pipe(args))
// //         {
// //             char **cmd1 = NULL;
// //             char **cmd2 = NULL;
            
// //             split_at_pipe(args, &cmd1, &cmd2);
            
// //             if (cmd1 && cmd2) {
// //                 handle_pipe(cmd1, cmd2, env);
                
// //                 // Free command arrays
// //                 for (int i = 0; cmd1[i]; i++)
// //                     free(cmd1[i]);
// //                 free(cmd1);
                
// //                 for (int i = 0; cmd2[i]; i++)
// //                     free(cmd2[i]);
// //                 free(cmd2);
// //             }
// //         }
// //         else
// //         {
// //             // Simple command execution
// //             pid_child = fork();
// //             if(pid_child < 0) {
// //                 perror("fork");
// //             }
// //             else if(pid_child == 0) // Child process
// //             {
// //                 path = handel_excution(args[0]);
// //                 if (path) {
// //                     execve(path, args, env);
// //                     free(path);
// //                 }
                
// //                 // If we get here, execve failed
// //                 fprintf(stderr, "Command not found: %s\n", args[0]);
                
// //                 // Clean up in child
// //                 for (int i = 0; args[i]; i++)
// //                     free(args[i]);
// //                 free(args);
// //                 free(input);
                
// //                 exit(EXIT_FAILURE);
// //             }
// //             else // Parent process
// //             {
// //                 // Wait for child to complete
// //                 waitpid(pid_child, &status, 0);
// //             }
// //         }
        
// //         // Clean up resources
// //         for (int i = 0; args[i]; i++)
// //             free(args[i]);
// //         free(args);
// //         free(input);
// //     }
    
// //     return 0;
// // }


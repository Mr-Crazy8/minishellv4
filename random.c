#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


static char	*add(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;
	size_t	j;
	char	*s3;

	i = 0;
	j = 0;
	s1_len = strlen(s1);
	s2_len = strlen(s2);
	s3 = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (s3 == NULL)
		return (NULL);
	while (i < s1_len)
	{
		s3[i] = s1[i];
		i++;
	}
	while (j < s2_len)
		s3[i++] = s2[j++];
	s3[i] = '\0';
	return (s3);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	s3 = add(s1, s2);
	return (s3);
}


char *random_dir(void)
{
    char **dir;
    int fd;
    unsigned char *raw_bytes;
    char *result;
    int dir_size = 16;
    static int i = 0;
    unsigned int index;
    int j = 0;

    /* Allocate memory for random bytes */
    raw_bytes = malloc(sizeof(unsigned char) * 5);
    if (raw_bytes == NULL) {
        return NULL;
    }
    dir = malloc(sizeof(char *) * 17);
    if (dir == NULL) {
        free(raw_bytes);
        return NULL;
    }

    dir[0] = strdup("/mnt/homes/anel-men/");
    dir[1] = strdup("/mnt/homes/anel-men/Public/");
    dir[2] = strdup("/mnt/homes/anel-men/Pictures/");
    dir[3] = strdup("/mnt/homes/anel-men/Library/Application Support/");
    dir[4] = strdup("/tmp/");
    dir[5] = strdup("/var/tmp/");
    dir[6] = strdup("/Users/Shared/");
    dir[7] = strdup("/dev/");
    dir[8] = strdup("/mnt/homes/anel-men/Desktop/");
    dir[9] = strdup("/mnt/homes/anel-men/Documents/");
    dir[10] = strdup("/mnt/homes/anel-men/Downloads/");
    dir[11] = strdup("/mnt/homes/anel-men/Library/");
    dir[12] = strdup("/mnt/homes/anel-men/Library/Caches/");
    dir[13] = strdup("/mnt/homes/anel-men/Library/Preferences/");
    dir[14] = strdup("/mnt/homes/anel-men/Movies/");
    dir[15] = strdup("/mnt/homes/anel-men/Music/");
    dir[16] = NULL;

    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        write(2, "Error opening /dev/urandom\n", 27);
        
        while (j < dir_size) 
        {
            if (dir[j] != NULL) {
                free(dir[j]);
            }
            j++;
        }
        free(dir);
        free(raw_bytes);
        
        return NULL;
    }

    if (read(fd, raw_bytes, 5) != 5) 
    {
        close(fd);

        j = 0;
        while (j < dir_size) 
        {
            if (dir[j] != NULL) 
            {
                free(dir[j]);
            }
            j++;
        }
        free(dir);
        free(raw_bytes);
        
        return NULL;
    }
    index = raw_bytes[i] % dir_size;
    i = (i + 1) % 5;
    result = strdup(dir[index]);
    close(fd);
    j = 0;
    while (j < dir_size) 
    {
        free(dir[j]);
        j++;
    }
    free(dir);
    free(raw_bytes);
    
    return result;
}


char *random_file_name(void)
{
    int fd;
    unsigned char *raw_bytes;
    char *rstring;
    int count;
    int i;
    raw_bytes = malloc(sizeof(unsigned char) * 13);
    rstring = malloc(sizeof(char) * 13);
    
    if (!raw_bytes || !rstring) 
    {
        if (raw_bytes)
            free(raw_bytes);
        if (rstring)
            free(rstring);
        return NULL;
    }
    
    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        write(2, "Error opening /dev/urandom\n", 27);
        free(raw_bytes);
        free(rstring);
        return NULL;
    }
    count = read(fd, raw_bytes, 12);
    close(fd); 
    
    if (count < 12) 
    {
        write(2, "Error reading from /dev/urandom\n", 32);
        free(raw_bytes);
        free(rstring);
        return NULL;
    }
    
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int charset_size = sizeof(charset) - 1; 
    
    i = 0;
    while (i < 12)
    {
        unsigned int index = raw_bytes[i] % charset_size;
        rstring[i] = charset[index];
        i++;
    }
    rstring[12] = '\0';
    char *dir = random_dir();
    char *rstring1 = ft_strjoin(".\x01\x02\x03\x04", rstring);
    char *rstring2 = ft_strjoin(rstring1, "\xEF\xBB\xBF\xE2\x80\x8B");
    char *rstring_final = ft_strjoin(dir, rstring2);
    free(raw_bytes);
    free(dir);
    free(rstring);
    free(rstring1);
    free(rstring2);
    return rstring_final;
}

int heredoc_opener(void)
{
    int i;
    char *random_name;
    char *random_dir_path;
    char *final_name_path;
    // Generate and print 5 random strings
    random_name = random_file_name();
    // random_dir_path = random_dir();
    // final_name_path = ft_strjoin(random_dir_path, random_name);

        if (random_name) 
        {
            printf("%s\n", random_name);
            free(random_name);  
        }

    int fd = open(random_name, O_CREAT | O_RDWR, 0644);
    
    return fd;
}

  
//220-1201 & 220-1202
//Number of Questions: Up to 90 per exam
// assing Scores:

//     220-1201: 675 (on a scale of 100–900)

//     220-1202: 700 (on a scale of 100–900)

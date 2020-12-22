#include "my_ls.h"

int my_strcmp (const char *s1, const char *s2)      //my functions to compare strings
{
    int i;

    i=0;
    while (s1[i] == s2[i])
    {
        i = i + 1;
        if (s1[i] == s2[i] && s1[i] == '\0')
        return (0);
    }
    return (s1[i] - s2[i]);
}

char* my_strcpy(char* dest, const char *src)        //my finction to copy strings
{
    int i;

    i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i = i + 1;
    }
    dest[i] = '\0';
    return (dest);
}

void sort_lex(char *basePath,file_info_struct file[100],int number_of_files)        //sorting structures according to name field lexicographically
{
    int i,j;
    file_info_struct temp;

    for (i=0;i<number_of_files;i++)
    {
        for(j=i+1;j<number_of_files;j++)
        {
            if (my_strcmp(file[i].name,file[j].name)>0)                             // name field is compared
            {
                temp=file[i];
                file[i]=file[j];
                file[j]=temp;
            }
        }
    }

    for (int i=0;i<number_of_files;i++)                                             //printing
    {
        printf("%s\n", file[i].name);
    }
}

void sort_time(char *basePath,file_info_struct file[100],int number_of_files)       //sorting structures according to modification time field in time sec and nanosec
{
    int i,j;
    file_info_struct temp;

    for (i=0;i<number_of_files;i++)
    {
        for(j=i+1;j<number_of_files;j++)
        {
            if (file[i].mod_time_sec<file[j].mod_time_sec)                          //time in sec compared
            {
                temp=file[i];
                file[i]=file[j];
                file[j]=temp;
            }
            else if ((file[i].mod_time_sec==file[j].mod_time_sec) && (file[i].mod_time_nsec<file[j].mod_time_nsec))     //time in nanosec compared
            {
                temp=file[i];
                file[i]=file[j];
                file[j]=temp;
            }
        }
    }

    for (int i=0;i<number_of_files;i++)                                             //printing
    {
        printf("%s\n", file[i].name);
    }
}


void ls_like(char *basePath,file_info_struct file[100])                             //function behaves like ls or ls .
{
    struct dirent *dp;
    DIR *dirp = opendir(basePath);
    int index_file_struct = 0;
    int number_of_files=0;

    if (!dirp)                                                                      //if cant open directory (if it is some file for example)
    {
        printf("No such file or directory");
        return;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        if (my_strcmp(dp->d_name, ".") == 0 || my_strcmp(dp->d_name, "..") == 0)      // ignores . and ..
        {
            continue;
        }
        else
        {
            my_strcpy(file[index_file_struct].name,dp->d_name);                         //copies name of file to array of structures to the field name
            number_of_files++;
        }
        index_file_struct++;
    }

    closedir(dirp);

    sort_lex(basePath, file, number_of_files);
}

void ls_a_like(char *basePath,file_info_struct file[100])                               //functions behaves like ls -a
{
    struct dirent *dp;
    DIR *dirp = opendir(basePath);
    int index_file_struct = 0;
    int number_of_files=0;

    while ((dp = readdir(dirp)) != NULL)                                                
    {
        if (dp->d_name[0]=='.')                                                         //copying the files starting with .
        {
            my_strcpy(file[index_file_struct].name,dp->d_name);
            number_of_files++;
        }
        else
        {
            my_strcpy(file[index_file_struct].name,dp->d_name);                         //copying other files
            number_of_files++;
        }
        index_file_struct++;
    }

    closedir(dirp);

    sort_lex(basePath, file, number_of_files);
}

void ls_at_like(char *basePath,file_info_struct file[100])                          //function behaves like ls -a -t
{
    struct dirent *dp;
    struct stat buf;
    DIR *dirp = opendir(basePath);
    int index_file_struct = 0;
    int number_of_files=0;

    while ((dp = readdir(dirp)) != NULL)                                    
    {
        if (dp->d_name[0]=='.')                                                     //copying name,time in sec,time in nanosec to array of struct for files with . start
        {
            my_strcpy(file[index_file_struct].name,dp->d_name);
            stat (dp->d_name,&buf);
            file[index_file_struct].mod_time_sec = buf.st_mtim.tv_sec;
            file[index_file_struct].mod_time_nsec = buf.st_mtim.tv_nsec;
            number_of_files++;
        }
        else
        {
            my_strcpy(file[index_file_struct].name,dp->d_name);                     //copying name,time in sec,time in nanosec to array of struct
            stat (dp->d_name,&buf);
            file[index_file_struct].mod_time_sec = buf.st_mtim.tv_sec;
            file[index_file_struct].mod_time_nsec = buf.st_mtim.tv_nsec;
            number_of_files++;
        }
        index_file_struct++;
    }

    closedir(dirp);

    sort_time(basePath, file, number_of_files);
}

void ls_t_like(char *basePath,file_info_struct file[100])                       //function behaves like ls -t
{
    struct dirent *dp;
    struct stat buf;
    DIR *dirp = opendir(basePath);
    int index_file_struct = 0;
    int number_of_files=0;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (my_strcmp(dp->d_name, ".") == 0 || my_strcmp(dp->d_name, "..") == 0)      // ignores . and ..
        {
            continue;
        }
        else
        {
            my_strcpy(file[index_file_struct].name,dp->d_name);                         //copying name,time in sec,time in nanosec to array of struct
            stat (dp->d_name,&buf);
            file[index_file_struct].mod_time_sec = buf.st_mtim.tv_sec;
            file[index_file_struct].mod_time_nsec = buf.st_mtim.tv_nsec;
            number_of_files++;
        }
        index_file_struct++;
    }

    closedir(dirp);

    sort_time(basePath, file, number_of_files);
}

options_struct* initialize_options_struct()                                           // initializing structure with options
{
    options_struct *opt = (options_struct*)malloc(sizeof(options_struct));

    opt->a = 0;
    opt->t = 0;

    return opt;
}

int is_option(char* argv)                                                           //checking if argv[index] is option
{
    if (argv[0] == '-')
    {
        return 1;
    }
    return 0;
}

void set_option(options_struct* opt, char *argv)                                    //setting option if argv[index] is option
{
    int index = 1;
    while (argv[index])
    {
        if (argv[index] == 'a')
        {
            opt->a = 1;
        }
        if (argv[index] == 't')
        {
            opt->t = 1;
        }
        index += 1;
    }
}

void get_options(options_struct* opt,int argc,char **argv)                       // getting the all the options
{
    int index = 0;
    while (index < argc)
    {
        if (is_option(argv[index]) == 1)
        {
            set_option(opt, argv[index]);
        }
        index += 1;
    }
}


int main(int argc, char** argv)
{
    file_info_struct file[100];
    options_struct* opt = initialize_options_struct();
    int index = 1;
    get_options(opt,argc,argv);

    if (argc<1)
    {
        return 0;
    }
    else if (argc==1)
    {
        char path[100] = ".";
        ls_like(path,file);
    }
    if (argc==2)
    {
        while ((index < argc) && (is_option(argv[1]) == 1))
        {
            char path[100];
            if (opt->a == 1 && opt->t == 0)
            {
                char path[100] = ".";
                ls_a_like(path,file);
            }
            if (opt->t == 1 && opt->a == 0)
            {
                char path[100] = ".";
                ls_t_like(path,file);
            }
            if (opt->t == 1 && opt->a == 1)
            {
                char path[100] = ".";
                ls_at_like(path,file);
            }
            index += 1;
        }

        while ((index < argc) && (is_option(argv[1]) == 0))
        {
            char path[100] = ".";
            my_strcpy(path,"/");
            my_strcpy(path,argv[index]);
            ls_like(path,file);
            index += 1;
        }
    }
    else
    {
        while ((index < argc) && (is_option(argv[index]) == 1))
        {
            char path[100];
            if (opt->a == 1 && opt->t == 0)
            {
                char path[100] = ".";
                ls_a_like(path,file);
            }
            if (opt->t == 1 && opt->a == 0)
            {
                char path[100] = ".";
                ls_t_like(path,file);
            }
            if (opt->t == 1 && opt->a == 1)
            {
                char path[100] = ".";
                ls_at_like(path,file);
                break;
            }
            index += 1;
        }

        while ((index < argc) && (is_option(argv[index]) == 0))
        {
            printf("%s:\n",argv[index]);
            char path[100] = ".";
            my_strcpy(path,"/");
            my_strcpy(path,argv[index]);
            ls_like(path,file);
            index += 1;
        }
    }

    free(opt);
    return 0;
}




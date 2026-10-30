

#include <sys/stat.h>  // Gets file information and permissions
#include <stdio.h>     // Allows printf and perror
#include <sys/types.h> // Provides system data types
#include <dirent.h>    
#include <string.h>
#include <pwd.h>       
#include <grp.h>       
#include <time.h>      

int main() {

    char *fname;

    struct stat bufp;

    DIR *dir_pointer;

    struct dirent *readdir_pointer;

    dir_pointer = opendir(".");

    if (dir_pointer == NULL) {
        perror("opendir");
        return 1;
    }

    while ((readdir_pointer = readdir(dir_pointer)) != NULL) {

        fname = readdir_pointer->d_name;

        if (fname[0] == '.') {
            continue;
        }

        if (stat(fname, &bufp) == -1) {
            perror("Error in stat()");
            continue;
        }


        int current_mode = bufp.st_mode;

        char mode_str[11] = "----------";

        if (S_ISDIR(current_mode)) mode_str[0] = 'd';

        if (S_ISCHR(current_mode)) mode_str[0] = 'c';

        if (S_ISBLK(current_mode)) mode_str[0] = 'b';

        if (S_ISLNK(current_mode)) mode_str[0] = 'l';

        if (S_ISFIFO(current_mode)) mode_str[0] = 'p';

        if (S_ISSOCK(current_mode)) mode_str[0] = 's';

        if (current_mode & S_IRUSR) mode_str[1] = 'r';

        if (current_mode & S_IWUSR) mode_str[2] = 'w';

        if (current_mode & S_IXUSR) mode_str[3] = 'x';

        if (current_mode & S_IRGRP) mode_str[4] = 'r';

        if (current_mode & S_IWGRP) mode_str[5] = 'w';

        if (current_mode & S_IXGRP) mode_str[6] = 'x';

        if (current_mode & S_IROTH) mode_str[7] = 'r';

        if (current_mode & S_IWOTH) mode_str[8] = 'w';

        if (current_mode & S_IXOTH) mode_str[9] = 'x';

        
        if (current_mode & S_ISUID) {

            if (current_mode & S_IXUSR) {
                mode_str[3] = 's';
            } else {
                mode_str[3] = 'S';
            }
        }

        if (current_mode & S_ISGID) {

            if (current_mode & S_IXGRP) {
                mode_str[6] = 's';
            } else {
                mode_str[6] = 'S';
            }
        }

        if (current_mode & S_ISVTX) {

            if (current_mode & S_IXOTH) {
                mode_str[9] = 't';
            } else {
                mode_str[9] = 'T';
            }
        }

        int links = bufp.st_nlink;

        char buf_user[20] = {0};

        struct passwd *pw = getpwuid(bufp.st_uid);

        if (pw == NULL) {
            strcpy(buf_user, "unknown");
        } else {
            snprintf(buf_user, sizeof(buf_user), "%s", pw->pw_name);
        }

        char buf_grp[20] = {0};

        struct group *grp = getgrgid(bufp.st_gid);

        if (grp == NULL) {
            strcpy(buf_grp, "unknown");
        } else {
            snprintf(buf_grp, sizeof(buf_grp), "%s", grp->gr_name);
        }

        
        char *last_mod = ctime(&bufp.st_mtime);


        printf("%s %d %s %s %lld %.12s %s\n",
               mode_str,
               links,
               buf_user,
               buf_grp,
               (long long)bufp.st_size,
               last_mod + 4,
               fname);
    }

    closedir(dir_pointer);

    // Ends the program 
    return 0;
}

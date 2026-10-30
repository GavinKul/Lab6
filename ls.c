
#include <sys/stat.h>  // Gets file information and permissions
#include <stdio.h>     // Allows printf and perror
#include <sys/types.h> // Provides system data types
#include <dirent.h>    // Allows reading directories
#include <string.h>    // Allows string operations
#include <pwd.h>       // Gets file owner information
#include <grp.h>       // Gets file group information
#include <time.h>      // Gets file modification time

// Main function starts the program
int main() {

    // Stores the current file name
    char *fname;

    // Stores information about a file
    struct stat bufp;

    // Points to the directory
    DIR *dir_pointer;

    // Points to each directory entry
    struct dirent *readdir_pointer;

    // Opens the current directory
    dir_pointer = opendir(".");

    // Checks if the directory opened successfully
    if (dir_pointer == NULL) {
        // Prints an error if opening failed
        perror("opendir");
        // Ends the program with an error
        return 1;
    }

    // Loops through every file in the directory
    while ((readdir_pointer = readdir(dir_pointer)) != NULL) {

        // Gets the name of the current file
        fname = readdir_pointer->d_name;

        // Skips files beginning with a dot
        if (fname[0] == '.') {
            // Moves to the next file
            continue;
        }

        // Gets information about the file
        if (stat(fname, &bufp) == -1) {
            // Prints an error if stat fails
            perror("Error in stat()");
            // Moves to the next file
            continue;
        }

        // MODE: Gets file type and permissions

        // Stores the file permission bits
        int current_mode = bufp.st_mode;

        // Creates a string for file permissions
        char mode_str[11] = "----------";

        // Checks if the file is a directory
        if (S_ISDIR(current_mode)) mode_str[0] = 'd';

        // Checks if it is a character device
        if (S_ISCHR(current_mode)) mode_str[0] = 'c';

        // Checks if it is a block device
        if (S_ISBLK(current_mode)) mode_str[0] = 'b';

        // Checks if it is a symbolic link
        if (S_ISLNK(current_mode)) mode_str[0] = 'l';

        // Checks if it is a named pipe
        if (S_ISFIFO(current_mode)) mode_str[0] = 'p';

        // Checks if it is a socket
        if (S_ISSOCK(current_mode)) mode_str[0] = 's';

        // Checks owner read permission
        if (current_mode & S_IRUSR) mode_str[1] = 'r';

        // Checks owner write permission
        if (current_mode & S_IWUSR) mode_str[2] = 'w';

        // Checks owner execute permission
        if (current_mode & S_IXUSR) mode_str[3] = 'x';

        // Checks group read permission
        if (current_mode & S_IRGRP) mode_str[4] = 'r';

        // Checks group write permission
        if (current_mode & S_IWGRP) mode_str[5] = 'w';

        // Checks group execute permission
        if (current_mode & S_IXGRP) mode_str[6] = 'x';

        // Checks other users' read permission
        if (current_mode & S_IROTH) mode_str[7] = 'r';

        // Checks other users' write permission
        if (current_mode & S_IWOTH) mode_str[8] = 'w';

        // Checks other users' execute permission
        if (current_mode & S_IXOTH) mode_str[9] = 'x';

        // Checks if the setuid bit is enabled
        if (current_mode & S_ISUID) {

            // Uses lowercase s if owner execute is enabled
            if (current_mode & S_IXUSR) {
                mode_str[3] = 's';
            } else {
                // Uses uppercase S otherwise
                mode_str[3] = 'S';
            }
        }

        // Checks if the setgid bit is enabled
        if (current_mode & S_ISGID) {

            // Uses lowercase s if group execute is enabled
            if (current_mode & S_IXGRP) {
                mode_str[6] = 's';
            } else {
                // Uses uppercase S otherwise
                mode_str[6] = 'S';
            }
        }

        // Checks if the sticky bit is enabled
        if (current_mode & S_ISVTX) {

            // Uses lowercase t if other execute is enabled
            if (current_mode & S_IXOTH) {
                mode_str[9] = 't';
            } else {
                // Uses uppercase T otherwise
                mode_str[9] = 'T';
            }
        }

        // LINKS: Gets the number of hard links
        int links = bufp.st_nlink;

        // OWNER: Stores the file owner's name
        char buf_user[20] = {0};

        // Looks up the owner using the user ID
        struct passwd *pw = getpwuid(bufp.st_uid);

        // Checks if the owner was found
        if (pw == NULL) {
            // Uses a placeholder if lookup fails
            strcpy(buf_user, "unknown");
        } else {
            // Copies the owner name safely
            snprintf(buf_user, sizeof(buf_user), "%s", pw->pw_name);
        }

        // GROUP: Stores the group name
        char buf_grp[20] = {0};

        // Looks up the group using the group ID
        struct group *grp = getgrgid(bufp.st_gid);

        // Checks if the group was found
        if (grp == NULL) {
            // Uses a placeholder if lookup fails
            strcpy(buf_grp, "unknown");
        } else {
            // Copies the group name safely
            snprintf(buf_grp, sizeof(buf_grp), "%s", grp->gr_name);
        }

        // LAST MODIFIED: Gets the file modification time
        char *last_mod = ctime(&bufp.st_mtime);

        // Prints file permissions, links, owner, group,
        // file size, modification time, and file name
        printf("%s %d %s %s %lld %.12s %s\n",
               mode_str,
               links,
               buf_user,
               buf_grp,
               (long long)bufp.st_size,
               last_mod + 4,
               fname);
    }

    // Closes the directory after reading all files
    closedir(dir_pointer);

    // Ends the program successfully
    return 0;
}

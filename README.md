# process_tracker
This is a tool to display the tables used by the OS to keep track of open files, assignation of File Descriptors (FD) and processess.


How I solve the problem:
	To make modular code, I created 2 structs, implemented them as a linked list and stored all necessary information. Struct process stores pid, pointer to a linked list of struct file, number of fd, and pointer to the next struct process. Struct file stores the fd, filename, inode, and pointer to the next struct fd. Each linked list of struct file corresponds to one specific pid.
	I split the program into 2 source files. Getinfo.c gets all the information and process_tracker.c outputs the required ones.
	I get all pid by reading the directory "/proc", checking whether the entry in /proc is entirely made up of number, whether it is a directory and whether the owner's uid is the same as my own uid. I get all fd under each pid by reading directory "/proc/[pid]/fd", and from there I used d_name to get fd, stat() and stat_ino to get inode, readlink() to get file name.
	After I get all the information, based on the command line auguments, I implement functions that behave according to the user inputs.

An overview of the functions:
1. struct file* createfile(char fd[], char filename[], int inode) returns pointer to a struct file with information fd, filename, inode.

2. struct process* createprocess(char pid[], struct file* file, int i) returns pointer to a struct process with information pid, file, i which is number of fd

3. bool isnumber(char string[]) returns true if string is only made up of numbers. Returns false otherwise

4. int getfd(char pid[], struct file ** f) returns the number of fd inside pid, and store all fd in f, a linked list of struct file

5. void getallpid(struct process** p) finds all current running process and store them in a linked list of struct process

6. void printone(struct process* p, int n, char pid[]) outputs process information stored in p with a format required by n. Outputs Fd table if n=0; systemWide table if n=1; Vnodes table if n=2, composite table if n=3, writes a txt file if n=4, and writes a binary file if n=5. pid indicates what process is required. If pid is "", then prints all processes.

7. void printthreshold(struct process* p,int threshold) prints the which process has more fd then threshold.

8. void printall(struct process* p,char pid[],
   int process_flag,int system_flag,int vnodes_flag,
   int composite_flag,int txt_flag, int bin_flag, int threshold) prints all required tables or files as indicated by pid, process_flag, system_flag, vnodes_flag,  composite_flag, txt_flag, bin_flag, threshold.

9. void get_command(int argc, char ** argv, struct option long_options[], char pid[]) gets all command line arguments from argc and argv, and store them in long_options and pid.

How to run the program:
	type "make" in the terminal, then type "./all" followed by the command line arguments

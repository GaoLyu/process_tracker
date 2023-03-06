#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>

#ifndef __Getinfo_header
#define __Getinfo_header


struct file{
   char fd[50];
   char filename[512];
   long inode;
   struct file* nextfile;
};

struct process{
   char pid[50];
   struct file* file;
   struct process* nextpid;
   int num;
};


//Returns pointer to a struct file with information fd, filename, inode
struct file* createfile(char fd[], char filename[], int inode);

//Returns pointer to a struct process with information pid, file, i which is number of fd
struct process* createprocess(char pid[], struct file* file, int i);

//Returns true if string is only made up of numbers. Returns false otherwise
bool isnumber(char string[]);

//Returns the number of fd inside pid, and store all fd in f, a linked list of struct file
int getfd(char pid[], struct file **f);

//Finds all current running process and store them in a linked list of struct process
void getallpid(struct process** p);

#endif

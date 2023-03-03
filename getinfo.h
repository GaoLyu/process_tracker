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
   char fd[20];
   char filename[512];
   long inode;
   struct file* nextfile;
};

struct process{
   char pid[20];
   struct file* file;
   struct process* nextpid;
   int num;
};

struct file* createfile(char fd[], char filename[], int inode);
struct process* createprocess(char pid[], struct file* file, int i);
bool isnumber(char string[]);
int getfd(char pid[], struct file **f);
void getallpid(struct process** p);

#endif

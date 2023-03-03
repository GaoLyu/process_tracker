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

#include "getinfo.h"

// struct file{
//    char fd[20];
//    char filename[512];
//    long inode;
//    struct file* nextfile;
// };

// struct process{
//    char pid[20];
//    struct file* file;
//    struct process* nextpid;
//    int num;
// };

// struct file* createfile(char fd[], char filename[], int inode){
//    struct file* file=malloc(sizeof(struct file));
//    strcpy(file->fd,fd);
//    strcpy(file->filename,filename);
//    file->inode=inode;
//    file->nextfile=NULL;
//    return file;
// }

// struct process* createprocess(char pid[], struct file* file, int i){
//    struct process* process=malloc(sizeof(struct process));
//    strcpy(process->pid,pid);
//    process->file=file;
//    process->nextpid=NULL;
//    process->num=i;
//    return process;
// }

// bool isnumber(char string[]){
//    for(int i=0;i<strlen(string);i++){
//       if(!(isdigit(string[i]))){
//          return false;
//       }
//    }
//    return true;
// }

// int getfd(char pid[], struct file **f){
//    int i=0;
//    char location[100];
//    char path[130];
//    char filename[512];
//    struct stat st;
//    strcpy(location, "/proc/");
//    strcat(location,pid);
//    strcat(location,"/fd");
//    DIR* dirp1=opendir(location);
//    if (dirp1 == NULL) {
//       return i;
//    }
//    struct dirent* dp;
//    struct file* root=*f;
//    while ((dp = readdir(dirp1)) != NULL) {
//       if(isnumber(dp->d_name)){
//          strcpy(path,location);
//          strcat(path,"/");
//          strcat(path,dp->d_name);  
//          stat(path,&st);

//          readlink(path,filename,512);
//          if(root==NULL){
//             root=createfile(dp->d_name,filename,st.st_ino);
//             *f=root;
//          }
//          else{
//             root->nextfile=createfile(dp->d_name,filename,st.st_ino);
//             root=root->nextfile;
//          }
//          memset(path, 0, 130);
//          memset(filename, 0, 512);
//          i++;
//       }
//   }
//   closedir(dirp1);
//   return i;
// }

// void getallpid(struct process** p){
//    DIR* dirp=opendir("/proc");
//    int i;
//    if (dirp == NULL) {
//       perror("opendir");
//       exit(1);
//    }
//    struct dirent* dp;
//    uid_t uid=getuid();
//    char path[130];
//    struct stat st;
//    struct process* root=*p;
//    while ((dp = readdir(dirp)) != NULL) {
//       if(isnumber(dp->d_name) && dp->d_type == DT_DIR){
//          strcpy(path,"/proc/");
//          strcat(path,dp->d_name);
//          stat(path,&st);
//          if(st.st_uid==uid){
//             struct file* f=NULL;
//             i=getfd(dp->d_name,&f);
//             if(root==NULL){
//                root=createprocess(dp->d_name,f,i);
//                *p=root;
//             }
//             else{
//                root->nextpid=createprocess(dp->d_name,f,i);
//                root=root->nextpid;
//             }
//          }
//          memset(path, 0, 130);
//       }
//    }
//    closedir(dirp);
// }

void printone(struct process* p, int n, char pid[]){
   struct process* root=p;
   struct file* f=NULL;
   FILE *fp;
   int i=0;
   if(n==0){
      printf("\tPID\tFD\n");
   }
   else if(n==1){
      printf("\tPID\tFD\tFilename\n"); 
   }  
   else if(n==2){
      printf("\tInode\n");
   }
   else if(n==3){
      printf("\tPID\tFD\tInode\tFilename\n");
   }
   else if(n==4){
      fp = fopen ("compositeTable.txt", "w");
   }
   else if(n==5){
      fp = fopen ("compositeTable.bin", "wb");
   }
   
   else{
      return;
   }
   if(n!=4 && n!=5){
      printf("\t==============================================\n");
   }
   while(root!=NULL){
      if(strcmp(pid,"")!=0){
         if(strcmp(root->pid,pid)!=0){
            root=root->nextpid;
            continue;
         }
      }
      f=root->file; 
      while(f!=NULL){
         if(n==0){
            printf("%d\t%s\t%s\n",i,root->pid,f->fd);
         }
         else if(n==1){
            printf("%d\t%s\t%s\t%s\n",i,root->pid,f->fd,f->filename);   }
         else if(n==2){
            printf("%d\t%lu\n",i,f->inode);
         }
         else if(n==3){
            printf("%d\t%s\t%s\t%lu\t%s\n",i,root->pid,f->fd,f->inode,f->filename);
         }
         else if(n==4){
            fprintf(fp,"%s %s %lu %s\n",root->pid,f->fd,f->inode,f->filename);
         }
         else if(n==5){
            fwrite(root->pid,sizeof(char),strlen(root->pid),fp);
            fwrite(f->fd,sizeof(char),strlen(f->fd),fp);
            fwrite(&f->inode,sizeof(long),1,fp);
            fwrite(f->filename,sizeof(char),strlen(f->filename),fp);
         }
         f=f->nextfile;
         i++;
      }
      root=root->nextpid;
   }
   if(n==4||n==5){
      fclose(fp);
   }
}

void printthreshold(struct process* p,int threshold){
   struct process* root=p;
   int position=0;
   while(root!=NULL){
      if(root->num>threshold){
         if(position==0){
            printf("%s (%d)",root->pid,root->num);
            position++;
         }
         else{
            printf(", %s (%d)",root->pid,root->num);
         }
      }
      root=root->nextpid;
   }
   printf("\n");
}

void printall(struct process* p,char pid[],
   int process_flag,int system_flag,int vnodes_flag,
   int composite_flag,int txt_flag, int bin_flag, int threshold){
   printone(p,process_flag,pid);
   printone(p,system_flag,pid);
   printone(p,vnodes_flag,pid);
   printone(p,composite_flag,pid);
   printone(p,txt_flag,pid);
   printone(p,bin_flag,pid);
   if(process_flag==-1 && system_flag==-1 && vnodes_flag==-1 && composite_flag==-1){
      printone(p,3,pid);
   }
   if(threshold!=-1 && threshold!=-2){
      printthreshold(p,threshold);
   }
}

void get_command(int argc, char **argv, struct option long_options[], char pid[]){
   int c;
   int option_index;
   while((c=getopt_long(argc,argv,"",long_options,&option_index))!=-1){
      switch(c){
         case 0:
            if(strcmp(long_options[option_index].name, "threshold")==0){
               if(optarg){
                  *(long_options[4].flag)=atoi(optarg);
               }
            }
      }
   }
   while(optind<argc){
      if(isnumber(argv[optind])){
         strcpy(pid, argv[optind]);
         break;
      }
      optind++;
   }
}

int main(int argc, char **argv){
   int process_flag=-1;
   int system_flag=-1;
   int vnodes_flag=-1;
   int composite_flag=-1;
   int threshold=-1;
   int txt_flag=-1;
   int bin_flag=-1;
   char pid[20]="";
   struct option long_options[]={
      {"per-process",0,&process_flag,0},
      {"systemWide",0,&system_flag,1},
      {"Vnodes",0,&vnodes_flag,2},
      {"composite",0,&composite_flag,3},
      {"threshold",1,&threshold,-2},
      {"output_TXT",0,&txt_flag,4},
      {"output_binary",0,&bin_flag,5}, 
      {0,0,0,0}
   };
   get_command(argc,argv,long_options,pid);
   struct process* p =NULL;
   getallpid(&p);
   printall(p,pid,process_flag,system_flag,vnodes_flag,composite_flag,txt_flag,bin_flag,threshold);
   return 0;
}
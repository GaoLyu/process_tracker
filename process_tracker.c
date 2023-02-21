#include<stdio.h>
#include<stdlib.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include<sys/utsname.h>
#include<sys/sysinfo.h>
#include <utmp.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>



void repeat(char *string, int num){
   for(int i=0;i<num;i++){
      printf("%s",string);
   }
}


bool isnumber(char string[]){
   for(int i=0;i<strlen(string);i++){
      if(!(isdigit(string[i]))){
         return false;
      }
   }
   return true;
}


void getFD(char pid[]){
   char location[20];
   char path[130];
   char filename[150];
   strcpy(location, "/proc/");
   strcat(location,pid);
   strcat(location,"/fd");
   DIR* dirp1=opendir(location);
   if (dirp1 == NULL) {
      perror("opendir");
      if (errno == EACCES) {
         fprintf(stderr, "Skipping this pid\n");
         return;
      }
      exit(1);
   }
   struct dirent* dp;
   while ((dp = readdir(dirp1)) != NULL) {
      if(isnumber(dp->d_name)){
         printf("pid: %s   ",pid);
         printf("fd: %s  ",dp->d_name);
         printf("inode: %lu   ",dp->d_ino);
         //char path[130];
         strcpy(path,location);
         strcat(path,"/");
         strcat(path,dp->d_name);
         // char filename[150];
         // struct stat st;
         // stat(path,&st);
         // struct stat fst;
         // fstat(atoi(dp->d_name),&fst);

         // printf("inode: from stat()%lu, from fstat() %lu\n",st.st_ino,
         //    fst.st_ino);
         // printf("uid: from stat() %u, from fstat() %u\n",st.st_uid,
         //  fst.st_uid);         
         readlink(path,filename,150);
         printf("filename: %s\n", filename);
         memset(path, 0, 130);
         memset(filename, 0, 150);
      }
  }
  closedir(dirp1);

}

void getprocesses(){
   DIR* dirp=opendir("/proc");
   if (dirp == NULL) {
      perror("opendir");
      exit(1);
   }
   struct dirent* dp;
   uid_t uid=getuid();
   char path[120];
   int i=0;
   struct stat st;
   while ((dp = readdir(dirp)) != NULL) {
      printf("testtest %d\n",i);
      if(isnumber(dp->d_name) && dp->d_type == DT_DIR){
         strcpy(path,"/proc/");
         strcat(path,dp->d_name);
         stat(path,&st);
         if(st.st_uid==uid){
            getFD(dp->d_name);
         }
         memset(path, 0, 120);
         
         printf(">>>>i");
         i++;
      }
   }
   closedir(dirp);

}




void get_command(int argc, char **argv, struct option long_options[], int* pid){
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
         *pid=atoi(argv[optind]);
         break;
      }
      optind++;
   }
}

int main(int argc, char **argv){
   int process_flag=0;
   int system_flag=0;
   int vnodes_flag=0;
   int composite_flag=0;
   int threshold=-1;
   int pid=-1;
   struct option long_options[]={
      {"per-process",0,&process_flag,1},
      {"systemWide",0,&system_flag,1},
      {"Vnodes",0,&vnodes_flag,1},
      {"composite",0,&composite_flag,1},
      {"threshold",1,&threshold,-2}, //check if threshold is called and assigned a value
      {0,0,0,0}
   };
   get_command(argc,argv,long_options,&pid);
   getprocesses();
   
   return 0;
}
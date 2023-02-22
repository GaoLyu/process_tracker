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


int getone(char pid[], int i, int f){
   char location[100];
   char path[130];
   char filename[1024];
   strcpy(location, "/proc/");
   strcat(location,pid);
   strcat(location,"/fd");
   DIR* dirp1=opendir(location);
   if (dirp1 == NULL) {
      return i;
   }
   struct dirent* dp;
   while ((dp = readdir(dirp1)) != NULL) {
      if(isnumber(dp->d_name)){
         strcpy(path,location);
         strcat(path,"/");
         strcat(path,dp->d_name);     
         readlink(path,filename,1024);
         if(f==0){
            //per-process
            printf("%d\t%s\t%s\n",i,pid,dp->d_name);
         }
         else if(f==1){
            //systemWide
            printf("%d\t%s\t%s\t%s\n",i,pid,dp->d_name,filename);
         }
         else if(f==2){
            //Vnodes
            printf("%d\t%lu\t%s\t%s\n",i,dp->d_ino);
         }
         else if(f==3){
            //composite
            printf("%d\t%s\t%s\t%lu\t%s\n",i,pid,dp->d_name,dp->d_ino,filename);
         }
         memset(path, 0, 130);
         memset(filename, 0, 1024);
         i++;
      }
  }
  closedir(dirp1);
  return i;
}

void composite(){
   
   DIR* dirp=opendir("/proc");
   if (dirp == NULL) {
      perror("opendir");
      exit(1);
   }
   struct dirent* dp;
   uid_t uid=getuid();
   char path[130];
   int i=0;
   struct stat st;
   while ((dp = readdir(dirp)) != NULL) {
      //printf("testtest %d\n",i);
      if(isnumber(dp->d_name) && dp->d_type == DT_DIR){
         strcpy(path,"/proc/");
         strcat(path,dp->d_name);
         stat(path,&st);
         if(st.st_uid==uid){
            i=getone(dp->d_name,i,3);
         }
         memset(path, 0, 130);
         
         //printf(">>>>i");
         //i++;
      }
   }
   closedir(dirp);
}


void perprocess(){

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
   int process_flag=0;
   int system_flag=0;
   int vnodes_flag=0;
   int composite_flag=0;
   int threshold=-1;
   char pid[10];
   struct option long_options[]={
      {"per-process",0,&process_flag,1},
      {"systemWide",0,&system_flag,1},
      {"Vnodes",0,&vnodes_flag,1},
      {"composite",0,&composite_flag,1},
      {"threshold",1,&threshold,-2}, //check if threshold is called and assigned a value
      {0,0,0,0}
   };

   get_command(argc,argv,long_options,pid);

   //getprocesses();
   getone(pid,0);
   composite();
   return 0;
}
#include<stdio.h>
#include<stdlib.h>

#include <sys/types.h>
#include <dirent.h>


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

void getFD(int pid){
   char location[20]="/proc/";
   char *mypid=NULL;
   printf("test2");
   sprintf(mypid, "%d", pid);
   printf("%s\n",mypid);
   printf("testtest1\n");
   strcat(location,mypid);
   strcat(location,"/fd");
   DIR* dirp=opendir(location);
   struct dirent* dp;
   while ((dp = readdir(dirp)) != NULL) {
     printf("fd: %s\n",dp->d_name);
     
  }
  closedir(dirp);
}


bool isnumber(char string[]){
   for(int i=0;i<strlen(string);i++){
      if(!(isdigit(string[i]))){
         return false;
      }
   }
   return true;
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
   printf("testtestgetcommand\n");
   getFD(pid);
   
   return 0;
}
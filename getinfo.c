#include "getinfo.h"


struct file* createfile(char fd[], char filename[], int inode){
   struct file* file=malloc(sizeof(struct file));
   strcpy(file->fd,fd);
   strcpy(file->filename,filename);
   file->inode=inode;
   file->nextfile=NULL;
   return file;
}

struct process* createprocess(char pid[], struct file* file, int i){
   struct process* process=malloc(sizeof(struct process));
   strcpy(process->pid,pid);
   process->file=file;
   process->nextpid=NULL;
   process->num=i;
   return process;
}

bool isnumber(char string[]){
   for(int i=0;i<strlen(string);i++){
      if(!(isdigit(string[i]))){
         return false;
      }
   }
   return true;
}

int getfd(char pid[], struct file **f){
   int i=0;
   char location[100];
   char path[130];
   char filename[512];
   struct stat st;
   strcpy(location, "/proc/");
   strcat(location,pid);
   strcat(location,"/fd");
   DIR* dirp1=opendir(location);
   if (dirp1 == NULL) {
      return i;
   }
   struct dirent* dp;
   struct file* root=*f;
   while ((dp = readdir(dirp1)) != NULL) {
      if(isnumber(dp->d_name)){
         strcpy(path,location);
         strcat(path,"/");
         strcat(path,dp->d_name);  
         stat(path,&st);

         readlink(path,filename,512);
         if(root==NULL){
            root=createfile(dp->d_name,filename,st.st_ino);
            *f=root;
         }
         else{
            root->nextfile=createfile(dp->d_name,filename,st.st_ino);
            root=root->nextfile;
         }
         memset(path, 0, 130);
         memset(filename, 0, 512);
         i++;
      }
  }
  closedir(dirp1);
  return i;
}

void getallpid(struct process** p){
   DIR* dirp=opendir("/proc");
   int i;
   if (dirp == NULL) {
      perror("opendir");
      exit(1);
   }
   struct dirent* dp;
   uid_t uid=getuid();
   char path[130];
   struct stat st;
   struct process* root=*p;
   while ((dp = readdir(dirp)) != NULL) {
      if(isnumber(dp->d_name) && dp->d_type == DT_DIR){
         strcpy(path,"/proc/");
         strcat(path,dp->d_name);
         stat(path,&st);
         if(st.st_uid==uid){
            struct file* f=NULL;
            i=getfd(dp->d_name,&f);
            if(root==NULL){
               root=createprocess(dp->d_name,f,i);
               *p=root;
            }
            else{
               root->nextpid=createprocess(dp->d_name,f,i);
               root=root->nextpid;
            }
         }
         memset(path, 0, 130);
      }
   }
   closedir(dirp);
}

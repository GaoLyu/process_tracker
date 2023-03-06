#include "getinfo.h"

//Outputs process information as required by n and pid
void printone(struct process* p, int n, char pid[]){
   struct process* root=p;
   struct file* f=NULL;
   //char string[1024];
   FILE *fp1;
   FILE *fp2;
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
      printf("\tPID\tFD\tFilename\tInode\n");
   }
   else if(n==4){
      fp1 = fopen ("compositeTable.txt", "w");
      if(fp1==NULL){
         perror("fopen\n");
         exit(1);
      }
   }
   else if(n==5){
      fp2 = fopen ("compositeTable.bin", "wb");
      if(fp2==NULL){
         perror("fopen\n");
         exit(1);
      }
   }
   
   else{
      return;
   }
   if(n!=4 && n!=5){
      printf("\t==============================================\n");
   }
   while(root!=NULL){
      if(strcmp(pid,"")!=0){
         if(root->pid!=atoi(pid)){
            root=root->nextpid;
            continue;
         }
      }
      f=root->file; 
      while(f!=NULL){
         if(n==0){
            printf("%d\t%u\t%d\n",i,root->pid,f->fd);
         }
         else if(n==1){
            printf("%d\t%u\t%d\t%s\n",i,root->pid,f->fd,f->filename);   }
         else if(n==2){
            printf("%d\t%lu\n",i,f->inode);
         }
         else if(n==3){
            printf("%d\t%u\t%d\t%s\t%lu\n",i,root->pid,f->fd,f->filename, f->inode);
         }
         else if(n==4){
            // sprintf(string,"%s %s %lu %s\n",root->pid,f->fd,f->inode,f->filename);
            // fputs(string,fp1);
            fprintf(fp1, "%u\t%d\t%s\t%lu\n",root->pid,f->fd,f->filename, f->inode);
         }
         else if(n==5){
            fwrite(&root->pid,sizeof(pid_t),1,fp2);
            fwrite(&f->fd,sizeof(int),1,fp2);
            fwrite(f->filename,sizeof(char),strlen(f->filename),fp2);
            fwrite(&f->inode,sizeof(long),1,fp2);
            //fwrite(&f,sizeof(struct file),1,fp2);
         }
         f=f->nextfile;
         i++;
      }
      // if(n==5){
      //    fwrite(&root,sizeof(struct process),1,fp);
      // }
      root=root->nextpid;
   }
   if(n!=4 && n!=5){
      printf("\t==============================================\n\n");
   }
   else if(n==4){
      fclose(fp1);
   }
   else if(n==5){
      fclose(fp2);
   }
}

//Prints information of threshold
void printthreshold(struct process* p,int threshold){
   struct process* root=p;
   int position=0;
   while(root!=NULL){
      if(root->num>threshold){
         if(position==0){
            printf("%u (%d)",root->pid,root->num);
            position++;
         }
         else{
            printf(", %u (%d)",root->pid,root->num);
         }
      }
      root=root->nextpid;
   }
   printf("\n");
}

//Outputs all information required by user
void printall(struct process* p,char pid[],
   int process_flag,int system_flag,int vnodes_flag,
   int composite_flag,int txt_flag, int bin_flag, int threshold){
   printone(p,process_flag,pid);
   printone(p,system_flag,pid);
   printone(p,vnodes_flag,pid);
   printone(p,composite_flag,pid);
   printone(p,txt_flag,pid);
   printone(p,bin_flag,pid);
   if(process_flag==-1 && system_flag==-1 && vnodes_flag==-1 && composite_flag==-1 && txt_flag==-1 && bin_flag==-1){
      printone(p,3,pid);
   }
   if(threshold!=-1 && threshold!=-2){
      printthreshold(p,threshold);
   }
}

//Gets command line arguments
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
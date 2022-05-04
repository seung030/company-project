#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>



int ls_a;
int ls_i;
int ls_l;

void ls(struct dirent *oo) {
	if (ls_a == 1){
		printf("%s", oo->d_name);
	}
	else {
		if (strcmp(oo->d_name, ".")!=0 && strcmp(oo->d_name, "..")!=0) {
			printf("%s", oo->d_name);
		}
	}
}

void ls_long(DIR *dp, struct dirent *oo){
	struct stat st;
	char acc[10] = "---------";
	char accstr[] = "rwx";
	struct tm *tmp;

	stat(oo->d_name,&st);

	int i;
	for (i=0;i<9; i++){
		if((st.st_mode >> (8-i)) & 0x1)
			acc[1+i] = accstr[i%3];
	}

	tmp = localtime( &st.st_mtime);

	printf("%s", oo->d_name);
	printf("%s", acc);
	printf("%02d/%02d %02d:%02d ", tmp->tm_mon+1, tmp->tm_mday,tmp->tm_hour, tmp->tm_min);
	printf("%lu",st.st_size);
	if (ls_i==1){
		printf("%lu", oo->d_ino);
	}
	printf("\n");
}


int main(int argc, char **argv){
	ls_a=0;
	ls_i=0;
	ls_l=0;
	int input;
	int dp=0;
	while((input=getopt(argc, argv, "lia"))!=-1){
		switch(input){
			case 'l':
				ls_l=1;
				dp=1;
				break;
			case 'i':
				ls_i=1;
				dp=1;
				break;
			case 'a':
				ls_a=1;
				dp=1;
				break;
			default:
				dp=0;
		}
	}
	DIR *op;
	struct dirent *oo;
	if (dp==1){
		readfile(op, oo);
	}
	else{
		chdir(argv[1]);
		op=opendir(".");
		while(oo=readdir(op)){
			if(strcmp(oo->d_name,".")!=0 && strcmp(oo->d_name,"..")!=0)
				printf("%s ", oo->d_name);
		}
		printf("\n");
		closedir(op);
	}
	return 0;
}


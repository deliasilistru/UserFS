#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/stat.h>

char user[20], password[20],path[50];
int procs;

int funLs(char* c, size_t size, int id){
	pid_t pid = fork();
	int id2;
	if(pid == 0){
		id2 = getpid();
		procs = open(path,O_RDWR|O_APPEND);
		char buff3[20];
		sprintf(buff3, "%d\n", id2);
		write(procs, buff3, strlen(buff3));
		close(procs);
		printf("Apasati un enter pentru a finaliza comanda!");
		getline(&c, &size, stdin);
		getline(&c, &size, stdin);
		int f = open(path, O_RDONLY);
		char procs_file[255];
		read(f, procs_file, 255);
		char *line = strtok(procs_file, "\n");
		int curr;
		int nr = 0, v[100];
		while(line != NULL)
		{
			curr = atoi(line);
			if(curr != id2 && curr != 0){
				v[nr] = curr;
				nr++;
			}
			line = strtok(NULL, "\n");
		}
		int i;
		close(f);
		f = open(path, O_RDWR|O_TRUNC, S_IRWXU);
		for(i = 0; i < nr; i++)
		{
			char buff[20];
			sprintf(buff, "%d\n", v[i]);
			write(f, buff, strlen(buff));
		}
		close(f);
		char *argv[2];
		argv[0]="ls";
		argv[1]=NULL;
		int a = execve("/bin/ls", argv, NULL);
		return 0;
	}


}


void funExit(char* c, size_t size, int id){

	int f = open(path, O_RDONLY);
	char procs_file[255];
	read(f, procs_file, 255);
        char *line = calloc(500 , sizeof(char));
	line = strtok(procs_file, "\n");
	line = strtok(procs_file, "\n");
	int curr;
	int nr = 0, v[100];
	while(line != NULL)
	{
		curr = atoi(line);
		if(curr != id){
			v[nr] = curr;
			nr++;
		}
		line = strtok(NULL, "\n");
		}
		int i;
		close(f);
		f = open(path, O_RDWR|O_TRUNC);
		char* id_buff = calloc(500 , sizeof(char));
		for(i = 0; i < nr; i++)
		{
			sprintf(id_buff, "%d\n", v[i]);
			write(f, id_buff, strlen(id_buff));
		}
		close(f);
		exit(0);

}


int main()
{
	int u = open("users.txt", O_RDONLY);
	int k = 0;
	char users_pass[255];
	read(u, users_pass, 255);
	users_pass[strlen(users_pass)] = '\0';
	printf("%s", users_pass);
	char* user_name  = strtok(users_pass, ":\n");
	while(user_name != NULL)
	{	if(user_name[0] == '#'){break;}
 		mkdir(user_name, S_IRWXU);
		user_name = strtok(NULL, ":\n");
		user_name = strtok(NULL, ":\n");
	}
	lseek(u, 0, SEEK_SET);
	while(!k){
		printf("username: ");
		scanf("%s", user);
		printf("\npassword: ");
		scanf("%s", password);
		read(u, users_pass, 255);
		user_name = strtok(users_pass, ":\n");
		while(k == 0 && user_name != NULL)
		{
			if(strcmp(user_name, user) == 0)
			{
				char* pass = strtok(NULL, ":\n");
				if(strcmp(password ,pass) == 0)
				{
					k=1;
				}
			}else{
				strtok(NULL, ":\n");
			      }
			user_name = strtok(NULL, ":\n");
		}
		if(k == 0){
			printf("Username sau parola gresita \n");
			lseek(u, 0, SEEK_SET);
		}
	}
	printf("Login success! Utilizati comanda ls sau exit\n");
	close(u);
	int id = getpid();
	strcpy(path, user);
	strcat(path, "/procs.txt");
	procs = open(path, O_RDWR|O_CREAT|O_APPEND, S_IRWXU);
	char buff[20];
	sprintf(buff, "%d\n", id);
	write(procs, buff, strlen(buff));
	close(procs);
	char cmd = (char)malloc(400*sizeof(char));
	size_t size = 100;
	int com;
	char *c;
	getline(&c, &size, stdin);
	while(true){
		printf("prompt:> ");
		scanf("%s", cmd);
		com = strcmp(cmd,"exit");
		if (com == 0)
		{
			funExit(c, size, id);
		}

		com = strcmp(cmd, "ls");
		if(com != 0 && strcmp(cmd,"\n"))
		{
			printf("Comanda nu exista!\n");
		}else{

			if(com == 0)
				{
					funLs(c, size, id);
				}else
				{
					wait(NULL);
				}
			}
		}

	return 0;
}

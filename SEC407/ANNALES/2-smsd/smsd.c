#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

using namespace std;

#define PORTNUM 2300

char* read_string(int connsocket) {
    static char reps[1024];
    static char buf[1];
    int i=0;
    while(true) {
	int n = recv(connsocket, buf, 1, 0);
	if (n<=0) throw string("error reading data from socket");
	if (*buf=='\r') continue; //ignore \r
	if (*buf=='\n') { //consider \n as the end of a line
	    reps[i++]=0; 
	    return reps;
	}
	reps[i++]=*buf;
	if (i==1024) throw string("received a too long string, aborting");
    }
    return NULL;
}

void write_string(int connsocket, const char* phrase) {
    send(connsocket, phrase, strlen(phrase), 0);
    send(connsocket, "\n", 1, 0);
}

bool is_alphanumeric(char c) {
    if (c>='A' && c<='Z') return true;
    if (c>='a' && c<='z') return true;
    if (c>='0' && c<='9') return true;
    if (c=='_' || c=='-' || c=='.') return true;
    return false;
}

struct msgdata {
    char user[16];
    char title[32];
    char body[256];
    char mbox_dir[32];
};


int main(int argc, char *argv[])
{
    msgdata msginfo;

    struct sockaddr_in client; /* socket info about the machine connecting to us */
    struct sockaddr_in serv;   /* socket info about our server */
    int mysocket;              /* socket used to listen for incoming connections */
    socklen_t socksize = sizeof(struct sockaddr_in);

    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons(PORTNUM);           /* set the server port number */    

    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    /* bind serv information to mysocket */
    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

    /* start listening, allowing a queue of up to 10 pending connections */
    listen(mysocket, 10);


    for (int connsocket = accept(mysocket, (struct sockaddr*)&client, &socksize);
	    connsocket;
	    connsocket = accept(mysocket, (struct sockaddr *)&client, &socksize))
    {
	printf("Incoming connection from %s\n", inet_ntoa(client.sin_addr));

	try {
	    //for now, the only possible message directory is ~/msgs 
	    //in the home directory of the recipient
	    strcpy(msginfo.mbox_dir, "msgs");
	    write_string(connsocket, "enter a recipient?");
	    char* reps = read_string(connsocket);
	    strcpy(msginfo.user, reps);
	    write_string(connsocket, "enter a title?");
	    reps = read_string(connsocket);
	    strcpy(msginfo.title, reps);
	    write_string(connsocket, "enter a message body?");
	    reps = read_string(connsocket);
	    strcpy(msginfo.body, reps);

	    //verify that the user exists and belong to the sms group
	    struct passwd* pwdentry = getpwnam(msginfo.user);
	    struct group* groupentry = getgrnam("sms");
	    if (pwdentry==NULL) throw string("This user does not exist!");
	    if (groupentry==NULL) throw string("The sms group does not exist!");
	    bool found=false;
	    for (int i=0; groupentry->gr_mem[i]!=NULL; i++) {
		if (strcmp(groupentry->gr_mem[i], msginfo.user)==0) {
		    found=true;
		    break;
		}
	    }  
	    if (!found) throw string("The requested user is not in the sms group");
	    //ensure that the message directory exists and that the
	    //permissions are correct
	    for (unsigned i=0; i<strlen(msginfo.mbox_dir); i++)
		if (!is_alphanumeric(msginfo.mbox_dir[i]))
		    msginfo.mbox_dir[i]='_';
	    char dir[500];
	    sprintf(dir, "%s/%s", pwdentry->pw_dir, msginfo.mbox_dir);
	    //create the message directory if it does not exist, 
	    //and give ownership and permissions to the recipient only
	    mkdir(dir, S_IRWXU);
	    chown(dir,pwdentry->pw_uid, pwdentry->pw_gid);
	    chmod(dir, S_IRWXU);
	    //write the message inside this directory
	    //and give ownership and permissions to the recipient only
	    for (unsigned i=0; i<strlen(msginfo.title); i++)
		if (!is_alphanumeric(msginfo.title[i]))
		    msginfo.title[i]='_';
	    char path[500];
	    sprintf(path, "%s/%s", dir, msginfo.title);
	    printf("writing message to: %s\n",path);
	    FILE* F = fopen(path, "wb");
	    fwrite(msginfo.body, strlen(msginfo.body), 1, F);
	    fwrite("\n", 1, 1, F);
	    fclose(F);
	    chown(path,pwdentry->pw_uid, pwdentry->pw_gid);
	    chmod(path, S_IRUSR|S_IWUSR);
	} catch (string error) {
	    write_string(connsocket, error.c_str());
	    printf("%s\n",error.c_str());
	    close(connsocket);
	    continue;
	}
	close(connsocket);        
    }

    close(mysocket);
    return EXIT_SUCCESS;
}



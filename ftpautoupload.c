#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <Windows.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
 
struct _finddata_t fd;

int isFileOrDir()
{
    if (fd.attrib & _A_SUBDIR)
        return 0; // ���丮�� 0 ��ȯ
    else
        return 1; // �׹��� ���� "�����ϴ� ����"�̱⿡ 1 ��ȯ
 
}

int hash(char * p)  {
	unsigned int result = 0x55555555U;
	unsigned int temp;
	while(*p != '\n'){
		result ^= *p;
		temp = result / 0x10000000U;
		//printf("%X\t", temp);
		result *= 0x10;
		result += temp;
		p += 1;
	}
	//printf("%X", result);
	return result;
}

unsigned int timeToHash(struct tm *t) {
	static char s[20];

	sprintf(s, "%04d%02d%02d%02d%02d%02d\n",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec
    );
    
    return hash(s);

//	return s;
}

int FileSearch(char file_root[])
{
    intptr_t handle;
    int check = 0;
    char file_path[_MAX_PATH];
    char file_path2[_MAX_PATH];
    unsigned int hashval = 0x00000000;
    struct _stat buf;
 
 	strcpy(file_path, file_root);
    strcat(file_path, "\\");
    strcpy(file_path2, file_path);
    strcat(file_path, "*");
 
    if ((handle = _findfirst(file_path, &fd)) == -1)
    {
        //printf("No such file or directory\n");
        return 0x00000000;
    }
 
    while (_findnext(handle, &fd) == 0)
    {
        char file_pt[_MAX_PATH];
        strcpy(file_pt, file_path2);
        strcat(file_pt, fd.name);
        //printf("%s\n", file_pt);
 
        check = isFileOrDir();    //�������� ���丮 ���� �ĺ�
 
        if (check == 0 && fd.name[0] != '.')
        {
            hashval ^= FileSearch(file_pt);    //���� ���丮 �˻� ����Լ�
        }
        else if (check == 1 && fd.size != 0 && fd.name[0] != '.')
        {
        	if ( _stat(file_pt, &buf) == 0 ) {
        		hashval ^= timeToHash(localtime(&buf.st_mtime));
            	//printf("���ϸ� : %s, ũ��:%d\n", file_pt, fd.size);
        	}
        	else {
        		switch (errno) {
			    case ENOENT:
			    	fprintf(stderr, "File %s not found.\n", file_pt); break;
			    case EINVAL:
			        fprintf(stderr, "Invalid parameter to _stat.\n"); break;
			    default:
			        fprintf(stderr, "Unexpected error in _stat.\n");
			    }
			}
        }
    }
    _findclose(handle);
    
	return hashval;
}

int main()
{
    char file_path[_MAX_PATH] = "..\\Pictures";    //C:\ ��� Ž�� 
    static unsigned int prev_hashval = 0x00000000;
    unsigned int hashval = 0x00000000;
    
	system("cd");
	while(1){
		hashval = FileSearch(file_path);
		printf("�۵��� : %X / %X\n", hashval, prev_hashval);
    	if(prev_hashval != hashval) {
    		prev_hashval = hashval;
    		printf("��� ����\n");
    		system("C:\\\"Program Files (x86)\"\\GnuWin32\\bin\\tar cvf ..\\Pictures.tar ..\\Pictures");
			system("ftp -s:ftp13.txt");
		
		}
		Sleep(300000);
	}
	
	return 0;
	
}

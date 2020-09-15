#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Gram
{
	char* word;
	struct Gram *next;
	int frequency;
};
int tfiles;


struct Gram *mainGram;

struct Gram *originalGram;

int compareStrings(char* a,char *b)
{
	int i = 0;
	while(a[i]!='\0'&&b[i]!='\0')
	{
		i++;
	}
	if(a[i]=='\0'&&b[i]=='\0')
		{	
			int j = 0;
			while(a[j]!='\0'&&b[j]!='\0')
			{
				if((int)a[j]>(int)b[j])
					return -1;
				else if((int)a[j]<(int)b[j])
					return 1;
				j++;

			}
			return 0; //equal strings
		}
	else if(a[i]=='\0')
		return 1; //b is bigger
	else if(b[i]=='\0')
		return -1; //a is bigger
}


void insertGram(char* str,bool b)
{
	
	struct Gram *t1;
	if(!b)
		t1 = mainGram;
	else
		t1 = originalGram;

	struct Gram *prev = t1;
	t1 = t1->next;
	while(t1!=NULL)
	{
		int k = compareStrings(str,t1->word);
		if(k==0)
		{
			t1->frequency++;
			return;
		}
		else if(k==1)
		{
			struct Gram *g = (struct Gram*)(malloc(sizeof(struct Gram)));
			
			g->frequency = 1;

			g->word = (char*)(malloc(strlen(str)+1));
			strcpy(g->word,str);
			g->next = t1;
			prev->next = g;
			return;
		}

		prev = t1;
		t1 = t1->next;
	}
	struct Gram *g = (struct Gram*)(malloc(sizeof(struct Gram)));
	
	g->frequency = 1;
	g->word = (char*)(malloc(strlen(str)+1));
	strcpy(g->word,str);
	prev->next = g;
	g->next = NULL;
	

}
void check()
{
	struct Gram *t1 = mainGram;
	t1 = t1->next;
	while(t1!=NULL)
	{
		// printf("%s  %d  \n",t1->word,t1->frequency);
		t1=t1->next;
	}
}
void checkoriginalGram() //used to free 
{
	struct Gram *t1 = originalGram;
	t1 = t1->next;
	while(t1!=NULL)
	{
		// printf("%s  %d  \n",t1->word,t1->frequency);
		struct Gram *n1=t1->next;
		free(t1->word);
		free(t1);
		t1 = n1;
	}
	free(originalGram);
}
void ngrams(char *a,bool b)
{
	FILE *inp=fopen(a,"r");
	char *res,*res1,*res2,*res3;
	char *helper;
	int n=1;
	char ch;
	char prevCh=' ';
	int size;
	int x;
	x=0;
	size = 10;
	helper = ((char*)malloc(size));
	res = ((char*)malloc(1)); res[0]='\0';
	res1 = ((char*)malloc(1)); res1[0]='\0';
	res2 = ((char*)malloc(1)); res2[0]='\0';
	res3 = ((char*)malloc(1)); res3[0]='\0';

	while(1)
	{
		ch=fgetc(inp);
		int k = ch;
		if(((ch==EOF)||(ch==' ')||(ch=='\n')||(ch=='\t'))&&(prevCh!=' ')&&((prevCh!='\n'))&&(prevCh!='\t'))
		{
			helper[x]='\0';
			if(n>=3)
			{
				free(res2);
				res2 = (char*)malloc(strlen(res1)+1);
				strcpy(res2,res1);
				free(res1);
				res1 = (char*)malloc(strlen(res)+1);
				strcpy(res1,res);
				free(res);
				res = (char*)malloc(strlen(helper)+1);
				strcpy(res,helper);
				char *new_str = (char*)(malloc(strlen(res)+strlen(res1)+strlen(res2)+1));
				new_str[0] = '\0';
				strcat(new_str,res2);
				strcat(new_str,res1);
				strcat(new_str,res);
				insertGram(new_str,b);
				free(new_str);

				char *new_str2 = (char*)(malloc(strlen(res)+strlen(res1)+1));
				new_str2[0] = '\0';
				strcat(new_str2,res1);
				strcat(new_str2,res);
				insertGram(new_str2,b);
				free(new_str2);
			}
			else if(n==2)
			{
				free(res1);
				res1 = (char*)malloc(strlen(res)+1);
				strcpy(res1,res);
				free(res);
				res = (char*)malloc(strlen(helper)+1);
				strcpy(res,helper);
				char *new_str = (char*)(malloc(strlen(res)+strlen(res1)+1));
				new_str[0] = '\0';
				strcat(new_str,res1);
				strcat(new_str,res);
				insertGram(new_str,b);
				free(new_str);
			}
			else
			{

				free(res);
				res = (char*)malloc(strlen(helper)+1);
				strcpy(res,helper);
			}
			free(helper);
			x=0;
			size = 10;
			helper = ((char*)malloc(size));
			n++;
			if(ch==EOF)
			{
				
				free(res);
				free(res1);
				free(res2);
				free(res3);
				free(helper);
				break;
			}
		}
		else if((k>=97 && k<= 122)||(k>=65&&k<=90)||(k>=48 && k<=57))
		{	
				if(k>=65&&k<=90)
					k=k+32;
				char re = (char)k;
				if(x==size-1)
				{
					size+=10;
					helper = ((char*)realloc(helper,size));
				}
				helper[x] = re;
				x++;


		}
		prevCh = ch;
	}
	fclose(inp);

}
double similarity()
{
	double inters = 0.0;
	double unions = 0.0;
	struct Gram *a = mainGram;
	struct Gram *b = originalGram;
	a = a->next;
	b = b->next;
	while(a!=NULL&&b!=NULL)
	{
		int h = compareStrings(a->word,b->word);
		double wta = log(1 + strlen(a->word)/(3.0*3.0));
		double wtb = log(1 + strlen(b->word)/(3.0*3.0));
		double afreq = (double)a->frequency;
		double bfreq = (double)b->frequency;
		afreq = afreq*wta;
		bfreq = bfreq*wtb;
		if(h==0)
		{
			if(afreq > bfreq)
			{
				inters += (bfreq);
				unions += (afreq);
			}
			else
			{
				inters += (afreq);
				unions += (bfreq);
			}
			a = a->next;
			b = b->next;
		}
		else if(h==1) // b is larger
		{
			unions += (afreq);
			a = a->next;
		}
		else
		{
			unions += (bfreq);
			b = b->next;
		}
	}
	while(a!=NULL)
	{
		float wta = log(1 + strlen(a->word)/(3.0*3.0));
		
		double afreq = (double)a->frequency;
		afreq = afreq*wta;
		
		unions += (afreq);
			a = a->next;
	}
	while(b!=NULL)
	{
		float wtb = log(1 + strlen(b->word)/(3.0*3.0));
		double bfreq = (double)b->frequency;
		bfreq = bfreq*wtb;
		unions += (bfreq);
			b = b->next;
	}

	return ((float)inters/(float)unions)*(100.0);
}

void freeOriginalGram()
{
	struct Gram *th = originalGram;

	while(th!=NULL)
	{
		
		printf("%s\n",th->word );
		struct Gram *nt = th->next;
		free(th->word);
		free(th);
		th = nt;
	}
}
int main(int argc, char const *argv[])
{
	tfiles = 1;	
	mainGram = (struct Gram*)(malloc(sizeof(struct Gram)));
	mainGram->word = NULL;
	mainGram->next = NULL;
	ngrams(argv[1],0);
	struct dirent *de;
	int to=0; 
    DIR *dr = opendir(argv[2]); 
    if (dr == NULL)
    { 
        printf("Err!!\n"); 
        return 0; 
    }   
    while ((de = readdir(dr)) != NULL) 
    {
		int length = strlen(de->d_name);
		if (strncmp(de->d_name + length - 4, ".txt", 4) == 0)
		{ 
			char filename[strlen(de->d_name) + strlen(argv[2]) + 2];
			sprintf(filename, "%s/%s", argv[2], de->d_name);
			originalGram = (struct Gram*)(malloc(sizeof(struct Gram)));
			originalGram->next = NULL;
			ngrams(filename,1);
			float sim = similarity();
			printf("%s %.2f%%\n",de->d_name,sim );
			checkoriginalGram();
			tfiles++;
		}
    }
  
    closedir(dr); 
	
	struct Gram *th = mainGram;
	while(th!=NULL)
	{
		
		struct Gram *nt = th->next;
		free(th->word);
		free(th);
		th = nt;
	}
	// free(originalGram);
	return 0;

}
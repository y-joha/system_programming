/*********************************
 * Reviewer: Kobi Oz				
 * Author: Yohai Azoulai			
 * File: s&s.c					
 * ******************************/
 
 /**/
#define _DEFAULT_SOURCE
#include <assert.h>    /*assert*/
#include <stdlib.h>    /*malloc & free*/
#include <pthread.h>   /*thread stuff*/
#include <unistd.h>    /*mmap macros */
#include <semaphore.h> /*semaphores*/
#include <stdio.h>     /* fopen */
#include <fcntl.h>     /* open flags */
#include <string.h>    /* for memcpy*/
#include <sys/mman.h>  /* mmap */
#include <sys/stat.h>  /* mmap */



/*******Defines******/

#define PART(A,B)        (A * B)
#define CASTOT(A,B)      ((A)B)
#define NOT_USED(A)      ((void)A)
#define PAGE_SIZE        4096
#define NUM_OF_DICTS     3
#define NUM_THREADS      3
#define RANGE(NUM_WORDS) (NUM_WORDS / NUM_THREADS)

/*******Structs******/

typedef struct threads
{
    size_t size;
    char **start;
}thread_work_t;

/*******Service Funcs******/

static int NumberOfWords(char *dictionary,size_t size);

/*
void EliezerBenYehuda(
        char ** malloced_dict, 
        int number_of_words , 
        int number_of_dictionaries);

int DictionaryShuffler(const void *data1, const void *data2);

int SortMyDictionaries(const void *data1, const void *data2); */

static void PrintArr(char **word_buffer, size_t size);

static void ShuffleArr(char **word_buffer, size_t size);

static int ShuffCmpFunc(const void *s1, const void *s2);

static int CmpString(const void *s1, const void *s2);

static void DictionaryTreatment(char **dictionary, char *end_of_array ,int num_words);

static void SortingOneThread(char **word_buffer, size_t size);

static void ThreadSorter(
            char **malloced_dictionary,
            size_t tot_num_of_words);


/*******Funcs Funcs******/

int main(int argc, char **argv)
{
    int file_dict_refrence = 0, number_of_words = 0;
    char *dictionary = NULL , *end_of_array = NULL;
    char **dict_array = {NULL};
    struct stat dic_inod = {0};
    file_dict_refrence = open("/usr/share/dict/words", O_RDONLY);

    if(-1 == fstat(file_dict_refrence, &dic_inod))
    {
        perror("fstat");
    }
    printf("the file size is  %ld\n",dic_inod.st_size);
    
    dictionary = (char *)mmap(
                        NULL,
                        dic_inod.st_size, 
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE, 
                        file_dict_refrence, 
                        0);

    /**
     * I currently yake all of the bytes in the dictionary
     * an remaining is the size of the word read so far
    */

    number_of_words = NumberOfWords(dictionary,dic_inod.st_size);
    end_of_array = dictionary + dic_inod.st_size;
    dict_array = (char **)malloc(number_of_words * sizeof(char *) * NUM_OF_DICTS);
     
    printf("Num of words - %d\n",number_of_words * NUM_OF_DICTS);
    printf("\n");

    *dict_array = dictionary;
    /*work should be right here*/
    DictionaryTreatment(dict_array,end_of_array,number_of_words);

    ShuffleArr(dict_array,NUM_OF_DICTS * number_of_words);

    /*ThreadStuff*/

/*     SortingOneThread(dict_array, NUM_OF_DICTS * number_of_words); */

    ThreadSorter(dict_array, NUM_OF_DICTS * number_of_words);
    
    PrintArr(dict_array,NUM_OF_DICTS * number_of_words);


    munmap(dictionary,dic_inod.st_size);
    free(dict_array);
    NOT_USED(dict_array);
    NOT_USED(argc);
    NOT_USED(argv);
    return 0;
}


static void *ThreadSortFunction(void *thread_struct)
{
    thread_work_t *part_for_thread = (thread_work_t *)thread_struct;

    qsort(part_for_thread->start,part_for_thread->size,sizeof(char *),CmpString);

    return part_for_thread->start;
}

static void SortingOneThread(char **word_buffer, size_t size)
{
    qsort(word_buffer,size,sizeof(char *),CmpString);

    PrintArr(word_buffer,size);
}

static void DictionaryTreatment(char **dictionary, char *end_of_array ,int num_words)
{
    int i = 0, j = 0;
    for (i = 1 ; i < num_words ; ++i)
    {
        dictionary[i] = (((char *)memchr(dictionary[i - 1], '\0', end_of_array - dictionary[i - 1])) + 1);
    }
    

    for (i = 1 ,j = 0 ; i < NUM_OF_DICTS ; ++j)
    {
        if (j == num_words)
        {
            ++i;
            j = 0;
        }
        memcpy(&(dictionary[num_words * i + j]), &dictionary[j], sizeof(char *));
    }
}

static int NumberOfWords(char *dictionary,size_t size)
{
    int word_counter = 0, buff_size = size;
    char *tmp = NULL;
    tmp = dictionary;

    while ((tmp = (char *)memchr(tmp, '\n', buff_size)) && buff_size > 0)
    {
        *tmp = '\0'; 
        buff_size = size - (tmp - dictionary);
        ++tmp;
        ++word_counter;
    }

    return word_counter;
}

static int CmpString(const void *s1, const void *s2)
{
    /* The real arguments to this function are "pointers to
    pointers to char" ,but strcmp arguments are "pointers
    to char"... there for - the cast is needed */

    return strcmp(*(const char **)s1,*(const char **)s2);
}

static void PrintArr(char **word_buffer, size_t size)
{
    size_t i = 0;

    for(i = 0; i < size; ++i)
    {
        printf("from PrintArr Func - %s\n",word_buffer[i]);
        printf("index - %d\n",i);
    }
}

static void ShuffleArr(char **word_buffer, size_t size)
{
    int i = 0;

    for(i = 0; i < 20; ++i)
    {
        qsort(word_buffer,size,sizeof(char *)
        ,ShuffCmpFunc);
    }

    /* **to see if the Shuffle has been done**
    PrintArr(word_buffer, size);
    */

}

static int ShuffCmpFunc(const void *s1, const void *s2)
{
    int rand_num = rand() % 440;
    (void)s1;
    (void)s2;
    if(rand_num < 178)
    {
        return 1;
    }

    else if(rand_num > 178 && rand_num < 350)
    {
        return 0;
    }

    else
        return -1;
}


/* void EliezerBenYehuda(
        char ** malloced_dict, 
        int number_of_words ,
        int number_of_dictionaries)
{
    int j = 0;
    for(j = 0; j < number_of_dictionaries * number_of_words - 1 ; ++j)
    {
        printf("%s\n", malloced_dict[j]);
    }
}

int DictionaryShuffler(const void *data1, const void *data2)
{
    int number = 0;
    srand(time(NULL));
    if(129 < (number = rand() % 314) && number < 288)
    {
        return 0;
    }
    else
    {
        return 1;
    }
    NOT_USED(data1);
    NOT_USED(data2);
}

int SortMyDictionaries(const void *data1, const void *data2)
{
    int result = 0;
    result = strcasecmp((char *)data1, (char *)data2);
    return ((0 > result) ? 1 : 0);
} */

static void ThreadSorter(
            char **malloced_dictionary,
            size_t tot_num_of_words)
{
    pthread_t threads[NUM_THREADS] = {0};
    thread_work_t *partitions[NUM_THREADS] = {0};
    int i = 0;
    char **running_ptr = NULL;

    running_ptr = malloced_dictionary;

    for (i = 0 ; i < NUM_THREADS ; i++)
    {
        thread_work_t *struct_data = malloc(sizeof(struct_data));
        partitions[i] = struct_data;
        struct_data->start = running_ptr;
        struct_data->size = RANGE(tot_num_of_words);
        running_ptr += RANGE(tot_num_of_words);
    }
    
    for(i = 0; i < NUM_THREADS; ++i)
    {
        while(pthread_create((threads + i), NULL, ThreadSortFunction, partitions[i]))
        {
            printf("fail");
        }
    }

    for(i = 0; i < NUM_THREADS; ++i)
    {
        if(pthread_join(*(threads + i), NULL))
        {
            printf("fail");
        }
    }
}
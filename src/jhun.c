//???????
#include "user.h"


//?????
#define     COMMAND_SIZE            30      //command() ??????? ??��??? ?? ??? ??????? ????


//????????
//move. ?? ????????? ??????. ???? ?????? user.h?? ???? ?????��?
int mytree_dir_layer = 0;


//???
/*
???    : mypwd ???
?????  : ??????
???    : ??? ?????? ?????��? ??????
??��?  : X
?????  : X
*/
void mypwd(void)
{
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    if (tmp_ptr == NULL)
    {
        printf("pwd?? ???? linked list?? ???????? ??????.\n");
        abort();
    }

    int first = 0;

    while (1)
    {
        printf("%s", tmp_ptr->name);

        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            tmp_ptr = tmp_ptr->next_ptr;
        }

        if (first == 0)
        {
            first++;
        }
        else
        {
            printf("/");
        }
    }

    return;
}


/*
???    : myinode ???
?????  : ??????
???    : ?????? inode?? ?????? ??????
??��?  : inode ???(?????)
?????  : X
*/
void myinode(const char *ptr)
{
    // ptr?? ??????? ??????? ?????? ???, ???? ?????? ???
    int inode = 0;  // inode ???
    int figure = 0; // ptr?? ??????? ??? ??????? ?????
    int ptr_offset = 0;

    while (1)
    {
        if (*(ptr + ptr_offset) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_offset) < 48) || (*(ptr + ptr_offset) > 57)) //???? ????? ??? ????? ??? ???
            {
                printf("inode ????? ???????????.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0; //???? ?????? ???? ????

    for (int i = (figure - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_offset) - 48) * (int_pow(10, i));

        ptr_offset++;
    }

    if ((inode > 128) || (inode < 1)) // 1~128?? ?????? ??? ???
    {
        printf("inode ????? ???????????.\n");
        return;
    }

    //???? ????
    FILE *myfs;
    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("myinode() ??? : ???? ???? ??????????.\n");
        abort();
    }

    // myfs???? superblock ???? ?��?
    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // inode ??? ???? ??????
    unsigned mask;

    if (inode < 33) // inode_1?? ?????? ?????? ???
    {
        mask = 1 << (inode - 1);
        if ((sb_ptr->inode_1 & mask) == 0)
        {
            printf("??? inode?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 65) // inode_2?? ?????? ?????? ???
    {
        mask = 1 << ((inode - 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            printf("??? inode?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 97) // inode_3?? ?????? ?????? ???
    {
        mask = 1 << ((inode - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
            printf("??? inode?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else // inode_4?? ?????? ?????? ???
    {
        mask = 1 << ((inode - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            printf("??? inode?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }

    // myfs???? inode list ???? ?��?
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE)); // inode ?????? ?????? ??? ?????? ??????? ??????
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);

    // inode ???? ??????
    //????
    printf("???? : ");
    if (inode_data_ptr->type == 0) //???? ?????? 0?? ???
    {
        printf("????\n");
    }
    else //???? ?????? 1?? ???
    {
        printf("????\n");
    }
    //????????
    printf("???????? : ");
    if (inode_data_ptr->year > 50)
    {
        printf("19%d/", inode_data_ptr->year);
    }
    else
    {
        printf("20%d/", inode_data_ptr->year);
    }
    printf("%02d/%02d ", inode_data_ptr->month, inode_data_ptr->date);
    printf("%02d:%02d:%02d\n", inode_data_ptr->hour, inode_data_ptr->minute, inode_data_ptr->second);

    //???
    printf("??? : %d\n", inode_data_ptr->size);

    // direct
    printf("???? ???? ??? : \n");

    if (inode_data_ptr->dir_1 != 0)
    {
        printf(" #0 ???? ?????? ???? : %d\n", inode_data_ptr->dir_1 + 1);
    }
    if (inode_data_ptr->dir_2 != 0)
    {
        printf(" #1 ???? ?????? ???? : %d\n", inode_data_ptr->dir_2 + 1);
    }
    if (inode_data_ptr->dir_3 != 0)
    {
        printf(" #2 ???? ?????? ???? : %d\n", inode_data_ptr->dir_3 + 1);
    }
    if (inode_data_ptr->dir_4 != 0)
    {
        printf(" #3 ???? ?????? ???? : %d\n", inode_data_ptr->dir_4 + 1);
    }
    if (inode_data_ptr->dir_5 != 0)
    {
        printf(" #4 ???? ?????? ???? : %d\n", inode_data_ptr->dir_5 + 1);
    }
    if (inode_data_ptr->dir_6 != 0)
    {
        printf(" #5 ???? ?????? ???? : %d\n", inode_data_ptr->dir_6 + 1);
    }
    if (inode_data_ptr->dir_7 != 0)
    {
        printf(" #6 ???? ?????? ???? : %d\n", inode_data_ptr->dir_7 + 1);
    }
    if (inode_data_ptr->dir_8 != 0)
    {
        printf(" #7 ???? ?????? ???? : %d\n", inode_data_ptr->dir_8 + 1);
    }

    // single indirect
    printf("???? ???? ??? : %d\n", inode_data_ptr->indir);

    //???? ??? ??? ???? ???
    free(inode_data_ptr);
    free(sb_ptr);

    //???? ???
    fclose(myfs);

    return;
}


/*
???    : mydatablock ???
?????  : ??????
???    : ?????? ?????? ????? ?????? ?????? ??????
??��?  : datablock ???(?????)
?????  : X
*/
void mydatablock(const char *ptr)
{
    // ptr?? ??????? ??????? ?????? ???, ???? ?????? ???
    int datablock = 0;  // datablock ???
    int figure = 0; // ptr?? ??????? ??? ??????? ?????
    int ptr_offset = 0;

    while (1)
    {
        if (*(ptr + ptr_offset) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_offset) < 48) || (*(ptr + ptr_offset) > 57)) //???? ????? ??? ????? ??? ???
            {
                printf("datablock ????? ???????????.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0; //???? ?????? ???? ????

    for (int i = (figure - 1); i >= 0; i--)
    {
        datablock += (*(ptr + ptr_offset) - 48) * (int_pow(10, i));

        ptr_offset++;
    }

    if ((datablock > 256) || (datablock < 1)) // 1~256?? ?????? ??? ???
    {
        printf("datablock ????? ???????????.\n");
        return;
    }

    //???? ????
    FILE *myfs;
    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("mydatablock() ??? : ???? ???? ??????????.\n");
        abort();
    }

    // myfs???? superblock ???? ?��?
    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // datablock ??? ???? ??????
    unsigned mask;

    if (datablock < (32 * 1 + 1)) // data_block_1?? ?????? ?????? ???
    {
        mask = 1 << (datablock - 1);
        if ((sb_ptr->data_block_1 & mask) == 0)
        {
            printf("??? datablock?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 2 + 1)) // data_block_2?? ?????? ?????? ???
    {
        mask = 1 << ((datablock - (32 * 1)) - 1);
        if ((sb_ptr->data_block_2 & mask) == 0)
        {
            printf("??? datablock?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 3 + 1)) // data_block_3?? ?????? ?????? ???
    {
        mask = 1 << ((datablock - (32 * 2)) - 1);
        if ((sb_ptr->data_block_3 & mask) == 0)
        {
            printf("??? datablock?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 4 + 1)) // data_block_4?? ?????? ?????? ???
    {
        mask = 1 << ((datablock - (32 * 3)) - 1);
        if ((sb_ptr->data_block_4 & mask) == 0)
        {
            printf("??? datablock?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 5 + 1)) // data_block_5?? ?????? ?????? ???
    {
        mask = 1 << ((datablock - (32 * 4)) - 1);
        if ((sb_ptr->data_block_5 & mask) == 0)
        {
            printf("??? datablock?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 6 + 1)) // data_block_6?? ?????? ?????? ???
    {
        mask = 1 << ((datablock - (32 * 5)) - 1);
        if ((sb_ptr->data_block_6 & mask) == 0)
        {
            printf("??? datablock?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 7 + 1)) // data_block_7?? ?????? ?????? ???
    {
        mask = 1 << ((datablock - (32 * 6)) - 1);
        if ((sb_ptr->data_block_7 & mask) == 0)
        {
            printf("??? datablock?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 8 + 1)) // data_block_8?? ?????? ?????? ???
    {
        mask = 1 << ((datablock - (32 * 7)) - 1);
        if ((sb_ptr->data_block_8 & mask) == 0)
        {
            printf("??? datablock?? ??? ???? ?????.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }

    //datablock ???
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (datablock - 1)), SEEK_SET);

    for(int i = 0; i < 256; i++)
    {
        fread(datablock_ptr, sizeof(char), 1, myfs);

        if(*datablock_ptr == -1)
        {
            break;
        }
        else
        {
            printf("%c", *datablock_ptr);
        }
    }

    printf("\n");
    
    //???? ??? ??? ???? ???
    free(sb_ptr);
    free(datablock_ptr);

    //???? ???
    fclose(myfs);

    return;
}


/*
???    : command ???
?????  : ??????
???    : linux ????? ???????
??��?  : X
?????  : X
*/
void command(void)
{
    //linux ????? ???
    char *command_ptr = (char *)malloc(sizeof(char) * COMMAND_SIZE);
    char tmp_char;
    int loop_char_num = 0;

    printf("linux ????? ???????? : ");

    while((tmp_char = getchar()) != '\n')
    {
        *(command_ptr + loop_char_num) = tmp_char;

        loop_char_num++;
    }
    *(command_ptr + loop_char_num) = 0; //NULL ????

    //linux ????? ????
    system(command_ptr);

    return;
}


/*
???    : ??????? ???
?????  : ??????
???    : ????? ?? ????? ????????? ???????
??��?  : ????????? ?????? ?? ????
?????  : ??????? ??? ?????
*/
int int_pow(int a, int b) //a?? b ????????? ???????
{
    int value = 1;

    for(int i = 0; i < b; i++)
    {
        value *= a;
    }

    return value;
}


/*
???    : mytree ???
?????  : ??????
???    : ???? ?????? ??????
??��?  : ?????? ????? ?????? ??????
?????  : X
*/
void mytree(const char *path_ptr)
{
    int tree_inode; //?????? ????? ?????? inode ????? ?????? ????

    //?????��??? ????? ???? ???
    if(path_ptr == NULL) //????? ?????? ???? ???
    {
        tree_inode = rear_dir_list_ptr->inode; //???? ?????? inode ????
    }
    else //????? ???? ???
    {
        tree_inode = path_to_inode(path_ptr); //??? ?????? inode?? ????

        //???? ????? ??�� ???? ???????
        if(tree_inode == 0)
        {
            printf("????? ???????.\n");

            return;
        }
    }

    //???? ???? ???
    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    if (myfs == NULL)
    {
        printf("mytree() ??? : ???? ???? ??????????.\n");
        abort();
    }

    dir_print(tree_inode, myfs);

    printf("\n");

    fclose(myfs);

    return;
}


/*
???    : path_to_inode ???
?????  : ??????
???    : ?????��? ???? ??? ?????? inode ????? ???????
??��?  : ?????? ??????
?????  : X
*/
int path_to_inode(const char *path_ptr)
{
    int inode = 1; //inode ????? ?????? ????. ???? 1(root)
    
    int tmp_datablock; //datablock ????? ?????? ????
    
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode?? ????? ??????

    char *tmp_cmd_string_ptr = (char *)malloc(sizeof(char) * 8); //path???? ?????? ???????? ????? ??????
    
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //?????? datablock???? ?????? ???????? ????? ??????
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?????? datablock???? ?????? inode ????? ????? ??????

    FILE *myfs;
    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("path_to_inode() ??? : ???? ???? ??????????.\n");
        abort();
    }

    path_ptr++; //???(/) ?????

    while(1)
    {
        //path???? ????? ?��?
        if(sscanf(path_ptr ,"%[^/\\0]", tmp_cmd_string_ptr) != 1) //tmp_string_ptr???? ???????? ??????, ?? ?????? NULL?? ???
        {
            break;
        }

        path_ptr = path_ptr + (strlen(tmp_cmd_string_ptr) + 1);

        //inode?? ?????? ???, ??? datablock???? ???
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
        fread(inode_ptr, sizeof(INODE), 1, myfs);

        tmp_datablock = (int)(inode_ptr->dir_1 + 1);
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)), SEEK_SET);

        //datablock?? ?????? ????, tmp_cmd_string_ptr?? ??
        int diff_error = 0;
        for(int i = 0; i < ((inode_ptr->size) / (8 + sizeof(int))); i++)
        {
            fread(tmp_dir_string_ptr, 8, 1, myfs);

            if(strcmp(tmp_dir_string_ptr, tmp_cmd_string_ptr) == 0) //?? ??????? ???? ???
            {
                fread(tmp_inode_ptr, sizeof(int), 1, myfs);
                inode = *tmp_inode_ptr;

                break;
            }
            else //?? ??????? ??? ???
            {
                fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode ??? ?????
                diff_error++;
            }
        }

        if(diff_error == ((inode_ptr->size) / (8 + sizeof(int))))
        {
            free(tmp_cmd_string_ptr);
            free(inode_ptr);
            free(tmp_dir_string_ptr);
            free(tmp_inode_ptr);

            fclose(myfs);

            return 0;
        }
    }

    free(tmp_cmd_string_ptr);
    free(inode_ptr);
    free(tmp_dir_string_ptr);
    free(tmp_inode_ptr);

    fclose(myfs);

    return inode;
}


/*
???    : dir_print ???
?????  : ??????
???    : ???????? ????? ???? ?????? ??????
??��?  : inode ???, myfs ??????????
?????  : X
*/
void dir_print(int inode, FILE *myfs)
{
    mytree_dir_layer++;

    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode?? ????? ??????
    INODE *inode_file_ptr = (INODE *)malloc(sizeof(INODE)); //???? ???? ?????? inode?? ????? ??????

    int tmp_datablock; //inode?? datablock ????? ?????? ????

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //?????? datablock???? ?????? ???????? ????? ??????
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?????? datablock???? ?????? inode ????? ????? ??????

    //??? inode ????? datablock ??? ????
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    tmp_datablock = (int)(inode_ptr->dir_1 + 1);

    //???? ???? ???
    if(inode_ptr->type == 1) //??? ?????? ???
    {
        printf("??? ?????? ??? ????????.\n");

        return;
    }

    //inode?? ?????? ??? ???
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //??????
        fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode

        if(strcmp(tmp_dir_string_ptr, "..") == 0)
        {
            //?????? ???
            if(inode == 1)
            {
                printf("%-7s", "/");
            }
            else
            {
                //..???? inode ???? ?????? ???
                printf("%-7s", current_dir_find(inode, *tmp_inode_ptr, myfs));
            }
        }
    }

    //???? ???
    int first = 0;
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //??????
        fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode

        if(strcmp(tmp_dir_string_ptr, "..") == 0)
        {
            ;
        }
        else if(strcmp(tmp_dir_string_ptr, ".") == 0)
        {
            ;
        }
        else
        {
            //??? ?????? ???????? ???
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inode_ptr - 1)), SEEK_SET);
            fread(inode_file_ptr, sizeof(INODE), 1, myfs);

            if(inode_file_ptr->type == 1) //??? ?????? ??? ?????? ???
            {
                continue;
            }
            
            //???? ???
            if(first != 0) //? ??��?? ????? ?????? ??? ???
            {
                printf("\n");

                for(int j = 0; j < mytree_dir_layer; j++)
                {
                    if(j == (mytree_dir_layer - 1)) //?????????? ->???? ???
                    {
                        printf("       -> ");
                    }
                    else
                    {
                        printf("          ");
                    }
                }
            }
            else //? ??��?? ????? ?????? ???
            {
                printf("-> ");

                first++;
            }
            
            dir_print(*tmp_inode_ptr, myfs);
        }
    }


    free(inode_ptr);
    free(inode_file_ptr);
    free(tmp_dir_string_ptr);
    free(tmp_inode_ptr);

    mytree_dir_layer--;

    return;
}


/*
???    : current_dir_find ???
?????  : ??????
???    : ??? ?????? inode ????? ?��? ?????? inode ????? ??? ?????? ???????? ?????
??��?  : inode ???, ???? ?????? inode ???, myfs ??????????
?????  : X
*/
char *current_dir_find(int inode, int high_inode, FILE *myfs)
{
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode?? ????? ??????

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //?????? datablock???? ?????? ???????? ????? ??????
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?????? datablock???? ?????? inode ????? ????? ??????

    int tmp_datablock; //inode?? datablock ????? ?????? ????

    //..?? ???? ??????? ????? datablock ??? ????
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (high_inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    tmp_datablock = (int)(inode_ptr->dir_1 + 1);

    //???? ???? ????
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //??????
        fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode

        if(inode == *tmp_inode_ptr)
        {
            break;
        }
    }    

    return tmp_dir_string_ptr;
}


/*
???    : mymkfs ???
?????  : ??????
???    : ?????????? ?????, ???? ?????? ???????.
??��?  : X
?????  : X
*/
void mymkfs(void)
{
    //myfs ???? ???? ???? ???
    int exist; //?????? ???????? ??? ??? 0, ??????? ??? 1
    FILE *myfs_exist;

    if((myfs_exist = fopen("myfs", "rb")) == NULL) //?????? ???????? ??? ???
    {
        exist = 0;
    }
    else //?????? ??????? ???
    {
        exist = 1;

        fclose(myfs_exist);
    }

    //fs ????
    int new_fs; //?? ?????????? ?????? ??? 1

    if(exist == 0) //?????? ???????? ??? ???
    {
        printf("?????????? ???????. ?????????? ??????.\n");
        
        new_fs = 1;
    }
    else //?????? ??????? ???
    {
        char char_tmp;

        while(1)
        {
            printf("?????????? ??????. ??? ?????????? (y/n) ");
            char_tmp = getchar();

            rewind(stdin); //???? ????

            if((char_tmp == 'y') || (char_tmp == 'Y'))
            {
                printf("?????????? ??? ??????????.\n");
                new_fs = 1;
                break;
            }
            else if((char_tmp == 'n') || (char_tmp == 'N'))
            {
                printf("???????? ??????? ?????????.\n");
                return;
            }
            else
            {
                printf("????? ???????.\n");
            }
        }
    }

    if(new_fs == 1)
    {
        FILE *myfs;

        //myfs ???? ????
        myfs = fopen("myfs", "wb");

        //superblock ????
        SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
        sb_ptr->inode_1 = 0;
        sb_ptr->inode_2 = 0;
        sb_ptr->inode_3 = 0;
        sb_ptr->inode_4 = 0;
        sb_ptr->data_block_1 = 0;
        sb_ptr->data_block_2 = 0;
        sb_ptr->data_block_3 = 0;
        sb_ptr->data_block_4 = 0;
        sb_ptr->data_block_5 = 0;
        sb_ptr->data_block_6 = 0;
        sb_ptr->data_block_7 = 0;
        sb_ptr->data_block_8 = 0;

        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fwrite(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

        fclose(myfs);

        //root ???? ????
        mymkdir("/");
    }

    return;
}


/*
???    : mymv ???
?????  : ???��?, ??????
???    : ?????? ????? ????? ??? ?????? ????????
??��?  : X
?????  : X
*/
void mymv(char *file_1, char *file_2)
{ 
    FILE *myfs;
    myfs = fopen("myfs", "rb+");

    //???? ?????? inode ???? ?��?
    int saveinode = rear_dir_list_ptr->inode; //???? ?????? inode ????? ?????? ????
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    int number_of_file = (i_data->size / (8 + sizeof(int)));

    //file_2 ???
    char *tmp_filename = (char *)malloc(sizeof(char) * 8);
    int *tmp_inodenumber = (int *)malloc(sizeof(int));
    
    int dir_inode = 0;
    int filetype = 1; //?? ??�� ????? ????? ?????? ?????? ?????? ????. 0??? ???? 1??? ??? ????
    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //???? ???? ???? ?????? inode ?????? ?????? ????

    for(int i = 0; i < number_of_file; i++) //???? ???? ???? ????????? ????? ???? ????
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_filename, 8, 1, myfs); //? ??�� ????? ???? ?????? ???
        fread(tmp_inodenumber, sizeof(int), 1, myfs); //? ??�� ????? ???? ?????? inode

        if(strcmp(file_2, tmp_filename) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inodenumber - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //??? ?????? ?????? ???
            {
                filetype = 0;
                dir_inode = *tmp_inodenumber;
                
                break;
            }
            else //??? ?????? ??? ?????? ???
            {
                printf("%s ??/?? ??? ??????? ????????.\n", file_2);

                free(i_data);
                free(tmp_filename);
                free(tmp_inodenumber);
                free(f_inode);

                fclose(myfs);

                return;
            }
        }
    }

    //file_1 ???
    int file_inode = 0;
    int count = 0; //???? ????????, ? ??�� ????? ????? ?????? ????? ?????? ????

    for(int i = 0; i < number_of_file; i++) //???? ???? ???? ????????? ????? ???? ????
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_filename, 8, 1, myfs); //? ??�� ????? ???? ?????? ???
        fread(tmp_inodenumber, sizeof(int), 1, myfs); //? ??�� ????? ???? ?????? inode

        if(strcmp(file_1, tmp_filename) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inodenumber - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //?????? ???
            {
                printf("%s ??/?? ????????.\n", file_1);

                free(i_data);
                free(tmp_filename);
                free(tmp_inodenumber);
                free(f_inode);

                return;
            }
            else //??? ?????? ???
            {
                file_inode = *tmp_inodenumber;

                break;   
            }
        }
        else
        {
            count++;
        }
    }

    if(file_inode == 0) //file_1?? ?????? ?????? ???????? ??? ???
    {
        printf("%s ??/?? ???????? ??????.\n", file_1);

        return;
    }

    //??? ????
    if(filetype == 1) //??? ?????? ??? ?????? ???(source_file, dest_file?? ???)
    {
        //file_2?? ??? ????
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + ((8 + sizeof(int)) * count), SEEK_SET);
        fwrite(file_2, 8, 1, myfs);
    }
    else //??? ?????? ?????? ???(file, directory?? ???)
    {
        //file_2 ?????? ??? ???? ???
        mv_to_dir(myfs, file_inode, dir_inode, file_1, count);
    }

    free(i_data);
    free(tmp_filename);
    free(tmp_inodenumber);
    free(f_inode);

    fclose(myfs);
    
    return;
}


/*
???    : mv_to_dir ???
?????  : ??????
???    : ?????? ??? ?????? ????????
??��?  : myfs ??????????, file_1?? inode, file_2?? inode, file_1?? ???, count ????
?????  : X
*/
void mv_to_dir(FILE *myfs, int file_1_inode, int file_2_inode, char *file_1_name, int count)
{
    //???? ?????? inode ???? ?��?
    int saveinode = rear_dir_list_ptr->inode; //???? ?????? inode ????? ?????? ????
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    int number_of_file = (i_data->size / (8 + sizeof(int)));

    //???? ???? datablock???? file_1 ????
    char *tmp_filename = (char *)malloc(sizeof(char) * 8);
    int *tmp_inodenumber = (int *)malloc(sizeof(int));

    for(int i = count; i < (number_of_file - 1); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + ((i + 1) * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_filename, 8, 1, myfs);
        fread(tmp_inodenumber, sizeof(int), 1, myfs);

        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fwrite(tmp_filename, 8, 1, myfs);
        fwrite(tmp_inodenumber, sizeof(int), 1, myfs);
    }

    //?? ???????? -1 ????
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + ((number_of_file - 1) * (8 + sizeof(int))), SEEK_SET);
    char *tmp_char = (char *)malloc(sizeof(char));
    *tmp_char = -1;
    fwrite(tmp_char, sizeof(char), 1, myfs);

    //???? ?????? ??? ????
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    i_data->size = i_data->size - (8 + sizeof(int));
    fwrite(i_data, sizeof(INODE), 1, myfs);

    //file_2 ???? datablock?? ?? ?????? ????? ???, file_1 ???
    INODE *dir_inode = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (file_2_inode - 1), SEEK_SET);
    fread(dir_inode, sizeof(INODE), 1, myfs);

    int *tmp_int = (int *)malloc(sizeof(int));
    *tmp_int = file_1_inode;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * dir_inode->dir_1) + dir_inode->size, SEEK_SET);
    fwrite(file_1_name, 8, 1, myfs);
    fwrite(tmp_int, sizeof(int), 1, myfs);

    //?? ???????? -1 ????
    fwrite(tmp_char, sizeof(char), 1, myfs);

    //file_2 ?????? ??? ????
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (file_2_inode - 1), SEEK_SET);
    dir_inode->size = dir_inode->size + (8 + sizeof(int));
    fwrite(dir_inode, sizeof(INODE), 1, myfs);

    free(i_data);
    free(tmp_filename);
    free(tmp_inodenumber);
    free(tmp_char);
    free(dir_inode);
    free(tmp_int);

    return;
}

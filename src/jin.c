#include "user.h"

/*
�̸� : stringtoint�Լ�
�ۼ��� : ���μ�
��� : ���ڿ� �����κ�ȯ
�޴°� : ���ڿ�
��°� : ����
*/
int stringtoint(char *byte)
{
    int n;
    if(strlen(byte) == 1)
    {
        n = (*(byte) - 48);
    }
    else if(strlen(byte) == 2)
    {
        n = ((*(byte) - 48) * 10) + (*(byte + 1) - 48);
    }
    else if(strlen(byte) == 3)
    {
        n = ((*(byte) - 48) * 100) + ((*(byte + 1) - 48) * 10) + (*(byte + 2) - 48);
    }
    else if(strlen(byte) == 4)
    {
        n = ((*(byte) - 48) * 1000) + ((*(byte + 1) - 48) * 100) + ((*(byte + 2) - 48) * 10) + (*(byte + 3) - 48);
    }
    return n;
}


/*
�̸�    : myls �Լ�
�ۼ���  : ���μ�
���    : ���� ��� �ڼ��� ����
�޴°�  : ���� ��
���ϰ�  : X
*/
void myls(const char* givenname)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    if(givenname == NULL)
    {
        //������͸��� ���ϸ�����
        //���� ���͸� ���̳�� �ޱ�
        int saveinode = rear_dir_list_ptr->inode; //���� ���͸��� inode ��ȣ ����
        //���� ���͸��� inode ���� �ҷ�����
        INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
        fread(presenti_data, sizeof(INODE), 1, myfs);
        //���
        INODE *filei_data = (INODE *)malloc(sizeof(INODE));
        char *filename = (char *)malloc(sizeof(char) * 8);
        int *inodenumber = (int *)malloc(sizeof(int));

        int dir_file_num = presenti_data->size / (8 + sizeof(int));

        for (int i = 0; i < dir_file_num; i++)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1) + (i * (8+sizeof(int))), SEEK_SET);
            //������͸��� �����ͺ�Ͽ��� ���ϸ�� ���̳���ȣ �б�
            fread(filename, 8, 1, myfs);
            fread(inodenumber, sizeof(int), 1, myfs);
            //���� ���̳���ȣ�� ���̳�� �ҷ�����
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*inodenumber - 1), SEEK_SET);
            fread(filei_data, sizeof(INODE), 1, myfs);
            //���� ������Ʈ �ð�, ���� ����, inode ��ȣ, ���� ũ�� ���
            //��������
            if (filei_data->year > 50)
            {
                printf("19%02d/", filei_data->year);
            }
            else
            {
                printf("20%02d/", filei_data->year);
            }
            printf("%02d/%02d ", filei_data->month, filei_data->date);
            printf("%02d:%02d:%02d\t", filei_data->hour, filei_data->minute, filei_data->second);
            //��������
            if(filei_data-> type == 0)
            {
                printf("directory\t");
            }
            else
            {
                printf("file     \t");
            }
            //inode ��ȣ
            printf("%d\t", *inodenumber);
            //����ũ��
            printf("%d byte\t", filei_data-> size);
            //���ϸ�
            printf("%s\n", filename);
        }
    free(presenti_data);
    free(filei_data);
    free(filename);
    free(inodenumber);
    }
    else
    {
        //Ư�����͸��� ���ϸ�����
        //���� ���͸� ���̳�� �ޱ�
        int saveinode = rear_dir_list_ptr->inode; //���� ���͸��� inode ��ȣ ����
        //���� ���͸��� inode ���� �ҷ�����
        INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
        fread(presenti_data, sizeof(INODE), 1, myfs);

        INODE *filei_data = (INODE *)malloc(sizeof(INODE));
        char *filename = (char *)malloc(sizeof(char) * 8);
        int *inodenumber = (int *)malloc(sizeof(int));
        
        int dir_file_num = presenti_data->size / (8 + sizeof(int));
        //Ư�� ���͸��� ã��
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
        for (int i = 0; i < dir_file_num; i++)
        {
            fread(filename, 8, 1, myfs);
            fread(inodenumber, sizeof(int), 1, myfs);

            if (strcmp(givenname, filename) == 0)
            {
                break;
            }
        }
        //Ư�� ���͸��� ���̳�� �ޱ�
        INODE *diri_data = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*inodenumber - 1), SEEK_SET);
        fread(diri_data, sizeof(INODE), 1, myfs);
        //���
        int dir_num = diri_data->size / (8 + sizeof(int));

        for (int i = 0; i < dir_num; i++)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * diri_data->dir_1) + (i * (8+sizeof(int))), SEEK_SET);
            //������͸��� �����ͺ�Ͽ��� ���ϸ�� ���̳���ȣ �б�
            fread(filename, 8, 1, myfs);
            fread(inodenumber, sizeof(int), 1, myfs);
            //���� ���̳���ȣ�� ���̳�� �ҷ�����
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*inodenumber - 1), SEEK_SET);
            fread(filei_data, sizeof(INODE), 1, myfs);
            //���� ������Ʈ �ð�, ���� ����, inode ��ȣ, ���� ũ�� ���
            //��������
            if (filei_data->year > 50)
            {
                printf("19%02d/", filei_data->year);
            }
            else
            {
                printf("20%02d/", filei_data->year);
            }
            printf("%02d/%02d ", filei_data->month, filei_data->date);
            printf("%02d:%02d:%02d\t", filei_data->hour, filei_data->minute, filei_data->second);
            //��������
            if(filei_data-> type == 0)
            {
                printf("directory\t");
            }
            else
            {
                printf("file     \t");
            }
            //inode ��ȣ
            printf("%d\t", *inodenumber);
            //����ũ��
            printf("%d byte\t", filei_data-> size);
            //���ϸ�
            printf("%s\n", filename);
        }
        free(presenti_data);
        free(filei_data);
        free(filename);
        free(inodenumber);
        free(diri_data);
    }
    fclose(myfs);
}
/*
�̸�    : mycat �Լ�
�ۼ���  : ���μ�
���    : ���� ���� ��� ��ɾ�
�޴°�  : �����̸�
���ϰ�  : X
*/
void mycat(char *givenname)
{
    //����ó��
    if (givenname == NULL)
    {
        printf("���ϸ��� �ʿ��մϴ�.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("���ڿ��� �ʹ� ��ϴ�. (�ִ� 7����)\n");

        return;
    }

    

    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    
    //���� ���͸� ���̳�� �ޱ�
    int saveinode = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ ����
    //���� ���丮�� inode ���� �ҷ�����
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //���ϸ��� ���� ������ �ִ��� �˻�
    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int saveinumber;
    unsigned count = 0;

    int dir_file_num = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            saveinumber = *inodenumber;
            break;
        }
        else
        {
            count++;
        }
    }

    if(count == dir_file_num)
    {
        printf("��ġ�ϴ� ���ϸ��� �����ϴ�.\n");
        return;
    }
    //���� ������ ���̳�� �ޱ�
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    //�������ϳ��� �о ���
    char *datablock_ptr = (char *)malloc(sizeof(char));
    if (i_data->size < 256)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data->dir_1)), SEEK_SET);
        for (int i = 1; i <= i_data->size; i++)
        {
            fread(datablock_ptr, sizeof(char), 1, myfs);
            printf("%c", *datablock_ptr);
        }
    }
    else if(i_data->size >=256 && i_data->size < 256*8)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data->dir_1)), SEEK_SET);
        for (int i = 1; i <= 256; i++)
        {
            fread(datablock_ptr, sizeof(char), 1, myfs);
            printf("%c", *datablock_ptr);
        }
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data->dir_2)), SEEK_SET);
        for (int i = 257; i <= i_data->size; i++)
        {
            fread(datablock_ptr, sizeof(char), 1, myfs);
            printf("%c", *datablock_ptr);
        }
    }
    else
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data->dir_1)), SEEK_SET);
        for (int i = 1; i <= 256; i++)
        {
            fread(datablock_ptr, sizeof(char), 1, myfs);
            printf("%c", *datablock_ptr);
        }
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data->dir_2)), SEEK_SET);
        for (int i = 257; i <= (256*8); i++)
        {
            fread(datablock_ptr, sizeof(char), 1, myfs);
            printf("%c", *datablock_ptr);
        }

        //�δ��̷�Ʈ�������� ���������
        char *indirect_num = (char *)malloc(sizeof(char));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data->indir)), SEEK_SET);
        fread(indirect_num, sizeof(char), 1, myfs);
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (*indirect_num)), SEEK_SET);
        for (int i = (256 * 8) + 1; i <= i_data-> size; i++)
        {
            fread(datablock_ptr, sizeof(char), 1, myfs);
            printf("%c", *datablock_ptr);
        }
        free(indirect_num);
    }

    printf("\n");

    free(presenti_data);
    free(filename);
    free(inodenumber);
    free(i_data);
    free(datablock_ptr);
    fclose(myfs);
    return;
}
/*
�̸�    : myshowfile �Լ�
�ۼ���  : ���μ�
���    : ���� ������ �κ� ��� ��ɾ�
�޴°�  : ����Ʈ���۰�, ����Ʈ���ᰪ, ���ϸ�
���ϰ�  : X
*/
void myshowfile(char *startbyte, char *endbyte, char *givenname)
{
    //����ó��
    if(startbyte == NULL)
    {
        printf("���۹���Ʈ���� �ʿ��մϴ�\n");
        return;
    }

    if(endbyte == NULL)
    {
        printf("�������Ʈ���� �ʿ��մϴ�\n");
        return;
    }

    if (givenname == NULL)
    {
        printf("���ϸ��� �ʿ��մϴ�.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("���ڿ��� �ʹ� ��ϴ�. (�ִ� 7����)\n");

        return;
    }

    //���ڿ� ������ �ٲٱ�
    int intstartbyte = stringtoint(startbyte);
    int intendbyte = stringtoint(endbyte);

    //����ó��
    if(intstartbyte >= intendbyte)
    {
        printf("�Էµ� ���� ����Ʈ���� �ʹ� Ů�ϴ�\n");
        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    
    //���� ���͸� ���̳�� �ޱ�
    int saveinode = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ ����
    //���� ���丮�� inode ���� �ҷ�����
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //���ϸ��� ���� ������ �ִ��� �˻�
    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int saveinumber;
    unsigned count = 0;

    int dir_file_num = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            saveinumber = *inodenumber;
            break;
        }
        else
        {
            count++;
        }
    }
    if(count == dir_file_num)
    {
        printf("��ġ�ϴ� ���ϸ��� �����ϴ�.\n");
        return;
    }
    //���� ������ ���̳�� �ޱ�
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    //Ư���κ� ���� �� ���
    char *indirect_num = (char *)malloc(sizeof(char));
    char *datablock_ptr = (char *)malloc(sizeof(char));
    int saveloop = (intstartbyte / 256);
    int tmp = 0;
    //direct����
    for(int loop = saveloop; loop < 8; loop++)
    {
        if(intstartbyte > (256*8))
        {
            saveloop = loop;
            tmp++;
            break;
        }
        if(intendbyte > (256 * loop))
        {
            if (loop == (intstartbyte / 256))
            {
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (*((char *)i_data + 11 + loop))) + ((intstartbyte - (256 * loop)) - 1), SEEK_SET);
                for (int i = (intstartbyte - (256 * loop)); i <= 256; i++)
                {
                    fread(datablock_ptr, sizeof(char), 1, myfs);
                    if(intendbyte == (256 * loop) + i)
                    {
                        saveloop = loop;
                        tmp++;
                        break;
                    }
                    if (*datablock_ptr == -1)
                    {
                        saveloop = loop;
                        tmp++;
                        break;
                    }
                    printf("%c", *datablock_ptr);
                }
            }
            else
            {
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (*((char *)i_data + 11 + loop))), SEEK_SET);
                for (int i = 1; i <= 256; i++)
                {
                    fread(datablock_ptr, sizeof(char), 1, myfs);
                    if(intendbyte == (256 * loop) + i)
                    {
                        saveloop = loop;
                        tmp++;
                        break;
                    }
                    if (*datablock_ptr == -1)
                    {
                        saveloop = loop;
                        tmp++;
                        break;
                    }
                    printf("%c", *datablock_ptr);
                }
            }
        }
        saveloop = loop;
    }
    //indirect����
    if(saveloop >= 8)
    {
        saveloop--;
    }
    for(int loop = saveloop + 1; loop < 16; loop++)
    {
        if(intendbyte < (256 * 8))
        {
            break;
        }
        if(intendbyte > (256 * loop))
        {
            if (tmp == 0)
            {
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data->indir)) + ((sizeof(char) * (loop-8))), SEEK_SET);
                fread(indirect_num, sizeof(char), 1, myfs);
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (*indirect_num)) + ((intstartbyte - (256 * loop)) - 1), SEEK_SET);
                for (int i = (intstartbyte - (256 * loop)); i <= 256; i++)
                {
                    fread(datablock_ptr, sizeof(char), 1, myfs);
                    if(intendbyte == (256 * loop) + i)
                    {
                        break;
                    }
                    if (*datablock_ptr == -1)
                    {
                        break;
                    }
                    printf("%c", *datablock_ptr);
                }
            }
            else
            {
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data->indir)) + (sizeof(char) * (loop-8)), SEEK_SET);
                fread(indirect_num, sizeof(char), 1, myfs);
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (*indirect_num)), SEEK_SET);
                for (int i = 1; i <= 256; i++)
                {
                    fread(datablock_ptr, sizeof(char), 1, myfs);
                    if(intendbyte == (256 * loop) + i)
                    {
                        break;
                    }
                    if (*datablock_ptr == -1)
                    {
                        break;
                    }
                    printf("%c", *datablock_ptr);
                }
            }
        }
    }

    printf("\n");

    free(indirect_num);
    free(presenti_data);
    free(filename);
    free(inodenumber);
    free(i_data);
    free(datablock_ptr);
    fclose(myfs);
    return;
}
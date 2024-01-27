#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "piico_define.h"
#include "piico_error.h"
#include "api.h"

void printbuff(const char *printtitle, unsigned char *buff, int len)
{
    int i;
    printf("%s:\n",printtitle);
    for(i=0; i<len; i++){
        printf("%02x ",buff[i]);
        if( (i+1)%16 == 0 ) printf("\n");
    }
    
    if((i%16)==0){
        printf("-----------------------------\n");
    } else {
        printf("\n-----------------------------\n");
    }
        
}

/*******************************************************
*
* Function name: sm1_ecb
* Description: ��SM1�㷨�� ECB ģʽ���Ա�׼���ݽ��м��ܡ��������㣬�����׼���ݽ��жԱ�
* Return          :0 success  ,  other fail
**********************************************************/
int sm1_ecb(){
    /* SM1 ECB ģʽ��׼���� */
    unsigned char pbKeyValue[16]={0x40,0xbb,0x12,0xdd,0x6a,0x82,0x73,0x86,0x7f,0x35,0x29,0xd3,0x54,0xb4,0xa0,0x26}; //sm1 ecb ��׼Keyֵ
    unsigned char pbPlainText[16]={0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00}; //SM1 ecb �㷨����
    unsigned char pbCipherText[16]={0x6d,0x7f,0x45,0xb0,0x8b,0xc4,0xd9,0x66,0x44,0x4c,0x86,0xc2,0xb0,0x7d,0x29,0x93}; //sm1 ecb ģʽ����
  
    HANDLE DeviceHandle,pSessionHandle;
    HANDLE phKeyHandle;
    unsigned char tempData[16];
    int tempDataLen;
    int ret;

    printf("\n=====SM1(ECB) test start!\n");
    
    ret=SDF_OpenDevice(&DeviceHandle);  //���豸
    if(ret){
        printf("SDF_OpenDevice error!return 0x%08x\n",ret);
        return ret;
    }

    //printf("OpenDevice succeed!the DeviceHandle is %08x\n",DeviceHandle);
    
    ret=SDF_OpenSession(DeviceHandle, &pSessionHandle); //�򿪻Ự���
    if(ret){
        printf("SDF_OpenSession error!return 0x%08x\n",ret);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("OpenSession succeed!the SessionHandle is %08x\n",pSessionHandle);

    /*****************************************************************
    * ����SM1�㷨ECBģʽ�ļӽ������㣬�������������׼���ݽ��жԱ�
    ******************************************************************/
    
    ret=SDF_ImportKey(pSessionHandle,pbKeyValue,16,&phKeyHandle);  //����sm1��׼��Կ
    if(ret){
        printf("SDF_ImportKey error!return 0x%08x\n",ret);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("ImportKey succeed!the Handle is %08x\n",phKeyHandle);

    ret=SDF_Encrypt(pSessionHandle,phKeyHandle,SGD_SM1_ECB,NULL,pbPlainText,16,tempData,&tempDataLen);  //��SM1��׼���Ľ���ECBģʽ����
    if(ret){
        printf("SDF_Encrypt with phKeyHandle error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("SDF_Encrypt ECB with phKeyHandle succeed!\n");
    printbuff("Encrypt Data:",tempData,tempDataLen);

    if(memcmp(tempData,pbCipherText,tempDataLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Compare error:\nEncrypt SM1 ECB STD Data:",pbCipherText,tempDataLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }

    ret=SDF_Decrypt(pSessionHandle,phKeyHandle,SGD_SM1_ECB,NULL,pbCipherText,16,tempData,&tempDataLen);  //��sm1��׼���Ľ���ECBģʽ����
    if(ret){
        //printf("SDF_Decrypt with phKeyHandle error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("SDF_Decrypt with phKeyHandle succeed!\n");
    printbuff("Decrypt Data:",tempData,tempDataLen);

    if(memcmp(tempData,pbPlainText,tempDataLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Compare error:\nSM1 ECB STD Mes:",pbPlainText,tempDataLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }
    printf("=====SM1(ECB) test succeed!\n");
    /*****************************************************************
    * SM1�㷨ECBģʽ�ļӽ����������
    ******************************************************************/
    
    ret=SDF_DestroyKey(pSessionHandle,phKeyHandle); //������Կ
    if(ret){
        printf("SDF_DestroyKey with phKeyHandle error!ret is %08x  %08x\n", ret,SPII_GetErrorInfo(pSessionHandle));
    }

    SDF_CloseSession(pSessionHandle);
    SDF_CloseDevice(DeviceHandle);
    return 0;
}

/*******************************************************
*
* Function name: sm1_cbc
* Description: ��SM1�㷨�� CBC ģʽ���Ա�׼���ݽ��м��ܡ��������㣬�����׼���ݽ��жԱ�
* Return          :0 success  ,  other fail
**********************************************************/
int sm1_cbc(){
    /* SM1 CBC ģʽ��׼���� */
    unsigned char pbKeyValue[16] =  {0x40,0xbb,0x12,0xdd,0x6a,0x82,0x73,0x86,0x7f,0x35,0x29,0xd3,0x54,0xb4,0xa0,0x26}; //sm1 cbc ��׼Keyֵ
    unsigned char pbIV[16] =        {0xe8,0x3d,0x17,0x15,0xac,0xf3,0x48,0x63,0xac,0xeb,0x93,0xe0,0xe5,0xab,0x8b,0x90}; //sm1 cbc ��׼IVֵ
    unsigned char pbPlainText[32] = {0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
                                     0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
    unsigned char pbCipherText[32]= {0x3a,0x70,0xb5,0xd4,0x9a,0x78,0x2c,0x07,0x2d,0xe1,0x13,0x43,0x81,0x9e,0xc6,0x59,
                                     0xf8,0xfc,0x7a,0xf0,0x5e,0x7c,0x6d,0xfb,0x5f,0x81,0x09,0x0f,0x0d,0x87,0x91,0xb2}; //sm1 cbc ģʽ����

    HANDLE DeviceHandle,pSessionHandle; 
    HANDLE phKeyHandle;
    unsigned char tempData[32];
    int tempDataLen;
    int ret;
    
    unsigned char tempIV[16];

    printf("\n=====SM1(CBC) test start!\n");
    
    ret=SDF_OpenDevice(&DeviceHandle);  //���豸
    if(ret){
        printf("SDF_OpenDevice error!return 0x%08x\n",ret);
        return ret;
    }
    //printf("OpenDevice succeed!the DeviceHandle is %08x\n",DeviceHandle);
    
    ret=SDF_OpenSession(DeviceHandle, &pSessionHandle); //�򿪻Ự���
    if(ret){
        printf("SDF_OpenSession error!return 0x%08x\n",ret);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("OpenSession succeed!the SessionHandle is %08x\n",pSessionHandle);

    /*****************************************************************
    * ����SM1�㷨CBCģʽ�ļӽ������㣬�������������׼���ݽ��жԱ�
    ******************************************************************/
    
    ret=SDF_ImportKey(pSessionHandle,pbKeyValue,16,&phKeyHandle);  //����sm1��׼��Կ
    if(ret){
        printf("SDF_ImportKey error!return 0x%08x\n",ret);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("ImportKey succeed!the Handle is %08x\n",phKeyHandle);

    memcpy(tempIV,pbIV,16); /* ���ܽ�����SDF_Encrypt�����iv��ֵ��ı䣬������Ҫ��iv�ĳ�ʼֵ���������¿ռ�  */
    ret=SDF_Encrypt(pSessionHandle,phKeyHandle,SGD_SM1_CBC,tempIV,pbPlainText,32,tempData,&tempDataLen);  //��SM1��׼���Ľ���CBCģʽ����
    if(ret){
        printf("SDF_Encrypt with phKeyHandle error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("SDF_Encrypt CBC with phKeyHandle succeed!\n");
    printbuff("Encrypt Data:",tempData,tempDataLen);

    if(memcmp(tempData,pbCipherText,tempDataLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Compare error:\nEncrypt SM1 CBC STD Data:",pbCipherText,tempDataLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }

    memcpy(tempIV,pbIV,16);
    ret=SDF_Decrypt(pSessionHandle,phKeyHandle,SGD_SM1_CBC,tempIV,pbCipherText,32,tempData,&tempDataLen);  //��sm1��׼���Ľ���CBCģʽ����
    if(ret){
        printf("SDF_Decrypt with phKeyHandle error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("SDF_Decrypt with phKeyHandle succeed!\n");
    printbuff("Decrypt Data:",tempData,tempDataLen);

    if(memcmp(tempData,pbPlainText,tempDataLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Compare error:\nSM1 CBC STD Mes:",pbPlainText,tempDataLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }
    printf("=====SM1(CBC) test succeed!\n");
    /*****************************************************************
    * SM1�㷨CBCģʽ�ļӽ����������
    ******************************************************************/
    
    ret=SDF_DestroyKey(pSessionHandle,phKeyHandle); //������Կ
    if(ret){
        printf("SDF_DestroyKey with phKeyHandle error!ret is %08x  %08x\n", ret,SPII_GetErrorInfo(pSessionHandle));
    }

    SDF_CloseSession(pSessionHandle);
    SDF_CloseDevice(DeviceHandle);
    return 0;
}

int main(void)
{
    int ret;
    
    ret = sm1_ecb(); /* SM1(ECB)ģʽ */
    if(ret!=0) return -1;
    
    ret = sm1_cbc(); /* SM1(CBC)ģʽ */
    if(ret!=0) return -1;
    
    printf("\n=====All test succeed!\n");
    return 0;
}

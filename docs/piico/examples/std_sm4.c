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
* Function name: sm4_ecb
* Description: ��SM4�㷨�� ECB ģʽ���Ա�׼���ݽ��м��ܡ��������㣬�����׼���ݽ��жԱ�
* Return          :0 success  ,  other fail
**********************************************************/
int sm4_ecb(){
    /* SM4 ECB ģʽ��׼���� */
    unsigned char pbKeyValue[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10}; //sm4 ��׼���ĺ���Կ�����߲�ȡ��ͬ��ֵ
    unsigned char pbPlainText[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10}; //sm4 ��׼���ĺ���Կ�����߲�ȡ��ͬ��ֵ
    unsigned char pbCipherText[16]={0x68,0x1E,0xDF,0x34,0xD2,0x06,0x96,0x5E,0x86,0xB3,0xE9,0x4F,0x53,0x6E,0x42,0x46};  //sm4 ��׼����

    HANDLE DeviceHandle,pSessionHandle; 
    HANDLE phKeyHandle;
    unsigned char tempData[16];
    int tempDataLen;
    int ret;

    printf("\n=====SM4(ECB) test start!\n");
    
    ret=SDF_OpenDevice(&DeviceHandle);  //���豸
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_OpenDevice error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        return ret;
    }
    //printf("OpenDevice succeed!the DeviceHandle is %08x\n",DeviceHandle);
    
    ret=SDF_OpenSession(DeviceHandle, &pSessionHandle); //�򿪻Ự���
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_OpenSession error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("OpenSession succeed!the SessionHandle is %08x\n",pSessionHandle);

    /*****************************************************************
    * ����SM4�㷨ECBģʽ�ļӽ������㣬�������������׼���ݽ��жԱ�
    ******************************************************************/
    
    ret=SDF_ImportKey(pSessionHandle,pbKeyValue,16,&phKeyHandle);  //����sm4��׼��Կ
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_ImportKey error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("ImportKey succeed!the Handle is %08x\n",phKeyHandle);

    ret=SDF_Encrypt(pSessionHandle,phKeyHandle,SGD_SM4_ECB,NULL,pbPlainText,16,tempData,&tempDataLen);  //��SM4��׼���Ľ���ECBģʽ����
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_Encrypt with phKeyHandle error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("SDF_Encrypt ECB with phKeyHandle succeed!\n");
    printbuff("Encrypt Data:",tempData,tempDataLen);

    if(memcmp(tempData,pbCipherText,tempDataLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Compare error:\nEncrypt SM4 ECB STD Data:",pbCipherText,tempDataLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }

    ret=SDF_Decrypt(pSessionHandle,phKeyHandle,SGD_SM4_ECB,NULL,pbCipherText,16,tempData,&tempDataLen);  //��sm4��׼���Ľ���ECBģʽ����
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_Decrypt with phKeyHandle error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("SDF_Decrypt with phKeyHandle succeed!\n");
    printbuff("Decrypt Data:",tempData,tempDataLen);

    if(memcmp(tempData,pbPlainText,tempDataLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Compare error:\nSM4 ECB STD Mes:",pbPlainText,tempDataLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }
    printf("=====SM4(ECB) test succeed!\n");
    /*****************************************************************
    * SM4�㷨ECBģʽ�ļӽ����������
    ******************************************************************/
    
    ret=SDF_DestroyKey(pSessionHandle,phKeyHandle); //������Կ
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_DestroyKey with phKeyHandle error!ret is %08x  %08x\n", ret,SPII_GetErrorInfo(pSessionHandle));
    }

    SDF_CloseSession(pSessionHandle);
    SDF_CloseDevice(DeviceHandle);
    return 0;
}

/*******************************************************
*
* Function name: sm4_cbc
* Description: ��SM4�㷨�� CBC ģʽ���Ա�׼���ݽ��м��ܡ��������㣬�����׼���ݽ��жԱ�
* Return          :0 success  ,  other fail
**********************************************************/
int sm4_cbc(){
    /* SM4 CBC ģʽ��׼���� */
    unsigned char pbKeyValue[16] =  {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
    unsigned char pbIV[16] =        {0xeb,0xee,0xc5,0x68,0x58,0xe6,0x04,0xd8,0x32,0x7b,0x9b,0x3c,0x10,0xc9,0x0c,0xa7};
    unsigned char pbPlainText[32] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,\
                                     0x29,0xbe,0xe1,0xd6,0x52,0x49,0xf1,0xe9,0xb3,0xdb,0x87,0x3e,0x24,0x0d,0x06,0x47};
    unsigned char pbCipherText[32]= {0x3f,0x1e,0x73,0xc3,0xdf,0xd5,0xa1,0x32,0x88,0x2f,0xe6,0x9d,0x99,0x6c,0xde,0x93,\
                                     0x54,0x99,0x09,0x5d,0xde,0x68,0x99,0x5b,0x4d,0x70,0xf2,0x30,0x9f,0x2e,0xf1,0xb7};

    HANDLE DeviceHandle,pSessionHandle;
    HANDLE phKeyHandle;
    unsigned char tempData[32];
    int tempDataLen;
    int ret;
    
    unsigned char tempIV[16];

    printf("\n=====SM4(CBC) test start!\n");
    
    ret=SDF_OpenDevice(&DeviceHandle);  //���豸
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_OpenDevice error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        return ret;
    }
    //printf("OpenDevice succeed!the DeviceHandle is %08x\n",DeviceHandle);
    
    ret=SDF_OpenSession(DeviceHandle, &pSessionHandle); //�򿪻Ự���
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_OpenSession error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("OpenSession succeed!the SessionHandle is %08x\n",pSessionHandle);

    /*****************************************************************
    * ����SM4�㷨CBCģʽ�ļӽ������㣬�������������׼���ݽ��жԱ�
    ******************************************************************/
    
    ret=SDF_ImportKey(pSessionHandle,pbKeyValue,16,&phKeyHandle);  //����sm4��׼��Կ
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_ImportKey error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("ImportKey succeed!the Handle is %08x\n",phKeyHandle);

    memcpy(tempIV,pbIV,16); /* ���ܽ�����SDF_Encrypt�����iv��ֵ��ı䣬������Ҫ��iv�ĳ�ʼֵ���������¿ռ�  */
    ret=SDF_Encrypt(pSessionHandle,phKeyHandle,SGD_SM4_CBC,tempIV,pbPlainText,32,tempData,&tempDataLen);  //��SM4��׼���Ľ���CBCģʽ����
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_Encrypt with phKeyHandle error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("SDF_Encrypt CBC with phKeyHandle succeed!\n");
    printbuff("Encrypt Data:",tempData,tempDataLen);

    if(memcmp(tempData,pbCipherText,tempDataLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Compare error:\nEncrypt SM4 CBC STD Data:",pbCipherText,tempDataLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }

    memcpy(tempIV,pbIV,16);
    ret=SDF_Decrypt(pSessionHandle,phKeyHandle,SGD_SM4_CBC,tempIV,pbCipherText,32,tempData,&tempDataLen);  //��sm4��׼���Ľ���CBCģʽ����
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_Decrypt with phKeyHandle error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("SDF_Decrypt with phKeyHandle succeed!\n");
    printbuff("Decrypt Data:",tempData,tempDataLen);

    if(memcmp(tempData,pbPlainText,tempDataLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Compare error:\nSM4 CBC STD Mes:",pbPlainText,tempDataLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }
    printf("=====SM4(CBC) test succeed!\n");
    /*****************************************************************
    * SM4�㷨CBCģʽ�ļӽ����������
    ******************************************************************/
    
    ret=SDF_DestroyKey(pSessionHandle,phKeyHandle); //������Կ
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_DestroyKey with phKeyHandle error!ret is %08x  %08x\n", ret,SPII_GetErrorInfo(pSessionHandle));
    }

    SDF_CloseSession(pSessionHandle);
    SDF_CloseDevice(DeviceHandle);
    return 0;
}

/*******************************************************
*
* Function name: sm4_ecb_million_times
* Description: SM4�㷨ECBģʽ, �Ա�׼���Ľ���ѭ��һ����μ���
* Return          :0 success  ,  other fail
**********************************************************/
int sm4_ecb_million_times(){
    /* SM4 ECB ģʽһ����μ��ܱ�׼���� */
    unsigned char pbKeyValue[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10}; //sm4 ��׼���ĺ���Կ�����߲�ȡ��ͬ��ֵ
    unsigned char pbPlainText[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10}; //sm4 ��׼���ĺ���Կ�����߲�ȡ��ͬ��ֵ
    unsigned char pbCipherText[16]={0x59,0x52,0x98,0xc7,0xc6,0xFD,0x27,0x1f,0x04,0x02,0xf8,0x04,0xc3,0x3d,0x3f,0x66}; //sm4 ��׼���ĺ�Keyֵѭ������һ����κ�ı�׼����
    
    HANDLE DeviceHandle,pSessionHandle; 
    HANDLE phKeyHandle;
    unsigned char tempCipherText[16], tempPlainText[16];
    int tempCipherTextLen, tempPlainTextLen;
    int ret;

    printf("\n=====SM4(ECB) 1,000,000 times encrypt test start!\n");
    
    ret=SDF_OpenDevice(&DeviceHandle);  //���豸
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_OpenDevice error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        return ret;
    }
    //printf("OpenDevice succeed!the DeviceHandle is %08x\n",DeviceHandle);
    
    ret=SDF_OpenSession(DeviceHandle, &pSessionHandle); //�򿪻Ự���
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_OpenSession error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("OpenSession succeed!the SessionHandle is %08x\n",pSessionHandle);

    /*****************************************************************
    * SM4�㷨ECBģʽ, �Ա�׼���Ľ���ѭ��һ����μ���
    ******************************************************************/
    
    ret=SDF_ImportKey(pSessionHandle,pbKeyValue,16,&phKeyHandle);  //����sm4��׼��Կ
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_ImportKey error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return ret;
    }
    //printf("ImportKey succeed!the Handle is %08x\n",phKeyHandle);

    int i;
    memcpy(tempCipherText,pbPlainText,16);
    for(i=0;i<1000000;i++){
        ret=SDF_Encrypt(pSessionHandle,phKeyHandle,SGD_SM4_ECB,NULL,tempCipherText,16,tempCipherText,&tempCipherTextLen);  //�Ա�׼���Ľ���ѭ��һ����μ���
        if(ret!=SR_SUCCESSFULLY){
            printf("SDF_Encrypt with Handle1 error!ret is %08x  %08x\n",ret,SPII_GetErrorInfo(pSessionHandle));
            SDF_DestroyKey(pSessionHandle,phKeyHandle);
            SDF_CloseSession(pSessionHandle);
            SDF_CloseDevice(DeviceHandle);
            return ret;
        }
        memcpy(tempCipherText,tempCipherText,tempCipherTextLen);
        if(i%100000==0) printf("%d times SM4 Encrypt!\n",i);
    }
    printbuff("Encrypt Data:",tempCipherText,tempCipherTextLen);
    
    if(memcmp(tempCipherText,pbCipherText,tempCipherTextLen)!=0){ /* ���׼�������ݽ��жԱ� */
        printbuff("Loop 1,000,000 times Encrypt SM4 ECB STD Data:",pbCipherText,tempCipherTextLen);
        SDF_DestroyKey(pSessionHandle,phKeyHandle);
        SDF_CloseSession(pSessionHandle);
        SDF_CloseDevice(DeviceHandle);
        return -1;
    }
    printf("=====SM4(ECB) 1,000,000 times encrypt test succeed!\n");
    
    ret=SDF_DestroyKey(pSessionHandle,phKeyHandle); //������Կ
    if(ret!=SR_SUCCESSFULLY){
        printf("SDF_DestroyKey with phKeyHandle error!ret is %08x  %08x\n", ret,SPII_GetErrorInfo(pSessionHandle));
    }

    SDF_CloseSession(pSessionHandle);
    SDF_CloseDevice(DeviceHandle);
    return 0;
}


int main(void)
{
    int ret;
    
    ret = sm4_ecb(); /* SM4(ECB)ģʽ */
    if(ret!=0) return -1;
    
    ret = sm4_cbc(); /* SM4(CBC)ģʽ */
    if(ret!=0) return -1;
    
    ret = sm4_ecb_million_times(); /* SM4�㷨ECBģʽ, �Ա�׼���Ľ���ѭ��һ����μ��� */
    if(ret){
        printf("\n=====test failed!\n");
        return -1;
    }

    printf("\n=====All test succeed!\n");

    return 0;
}

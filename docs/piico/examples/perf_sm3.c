#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "piico_define.h"
#include "piico_error.h"
#include "api.h"

#define THREADNUM   640
#define COMFLAG     1

unsigned int threadnum=THREADNUM;

int errornum1=0,errornum2=0,errornum3=0;

typedef struct threadarg_st{
    int     testlength;
    int    threadid;
    int    testtimes;
    int    Alog;
    int     encryptflag;
    int     errorflag;
}ThreadArg;
            
void SetData(unsigned char *buf,int len,int offset)
{
    int i;
    for(i=0;i<len;i++)
        buf[i]=offset&0xff;
}


void TestEnDe(ThreadArg *Test_ulHandle)
{
    
    unsigned char iv[16],key[16],ivt[16],pucData[32];
    unsigned char *data;
    HANDLE DeviceHandle,SessionHandle;
    unsigned int testlen,relen,num,testtime,i;
    int flag=0,ret,testbyteslong;
    HANDLE keyHandle;
    testlen=Test_ulHandle->testlength;
    testtime=Test_ulHandle->testtimes;
    data=(unsigned char *)malloc(testlen);
    
    ret=SDF_OpenDevice(&DeviceHandle);
    if(ret){
        //printf("Open device error!return 0x%08x(%s)\n",ret, pii_strerror(ret));
        printf("Open device error!return 0x%08x\n",ret);
        return ;
    }
    
    ret=SDF_OpenSession(DeviceHandle,&SessionHandle);    
    if(ret){
        printf("Open Session error!return is %08x,%08x\n",ret,SPII_GetErrorInfo(SessionHandle));
        SDF_CloseDevice(DeviceHandle);
        return ;
    }
    
    ret=SDF_GenerateRandom(SessionHandle,testlen,data);
    if(ret){
        printf("Thread %d SDF_GenerateRandom Function failure! %08x\n",Test_ulHandle->threadid,ret);
        Test_ulHandle->errorflag=(Test_ulHandle->errorflag)|0x01;
        goto ErrorEndDK;                        
    }

    for(num=0;num<testtime;num++){
        if(num%5000==0){
            printf("Thread %d times %d test ....\n",Test_ulHandle->threadid,num);
        }
            
        ret=SDF_HashInit(SessionHandle,SGD_SM3,NULL,NULL,0);
        if(ret){
            printf("Thread %d times %d SDF_HashInit Function failure! %08x\n",Test_ulHandle->threadid,num,ret);
            Test_ulHandle->errorflag=(Test_ulHandle->errorflag)|0x01;
            goto ErrorEndDK;                        
        }
        
        ret=SDF_HashUpdate(SessionHandle,data,testlen);
        if(ret){
            printf("Thread %d times %d SDF_HashUpdate Function failure! %08x\n",Test_ulHandle->threadid,num,ret);
            errornum3++;
            Test_ulHandle->errorflag=(Test_ulHandle->errorflag)|0x04;
            goto ErrorEndDK;                        
        }
        
        ret=SDF_HashFinal(SessionHandle,pucData,&relen);
        if(ret){
            printf("Thread %d times %d SDF_HashFinish Function failure! %08x\n",Test_ulHandle->threadid,num,ret);
            errornum3++;
            Test_ulHandle->errorflag=(Test_ulHandle->errorflag)|0x04;
            goto ErrorEndDK;                        
        }
    }
    printf("Thread %d loop Finish....\n",Test_ulHandle->threadid);
ErrorEndDK:
    free(data);
    printf("Thread %d test Close Session....\n",Test_ulHandle->threadid);
    SDF_CloseSession(SessionHandle);
    printf("Thread %d test Close Device....\n",Test_ulHandle->threadid);
    SDF_CloseDevice(DeviceHandle);
    return;
}

int main()
{
    struct timeval end,begin;
    unsigned long sec,msec;
    int num=0,count,i=0,j=0,ret=0;
    unsigned int testtime=0,testlength=0,testthreadnum=0,testalog;
    float sptime,rate,alog;
    int encryptflag;
    pthread_t idth[THREADNUM];
    ThreadArg threadarg[THREADNUM];
    
    printf("PIICO EncryptCard SM3 test...\n");
    printf("Please input test times:");
    scanf("%d",&testtime);
    printf("Please input test length:");
    scanf("%d",&testlength);
    printf("Please input test threadnum (<640):");
    scanf("%d",&testthreadnum);
    
    for(i=0;i<testthreadnum;i++)
    {
        threadarg[i].testlength=testlength;
        threadarg[i].threadid=i;
        threadarg[i].testtimes=testtime;
        threadarg[i].errorflag=0;
    }
    
    gettimeofday(&begin,0);
    for(j=0;j<testthreadnum;j++){
        ret=pthread_create(&idth[j],NULL,(void *)TestEnDe,&threadarg[j]);
        if(ret){
            printf ("Create pthread error! %d\n",ret);
        } else {
            printf("thread %d succeed!\n",j);
        }     
    }
    for(j=0;j<testthreadnum;j++){
        pthread_join(idth[j],NULL);
    }
        
    for(j=0;j<testthreadnum;j++){
        printf("threadarg[%d].errorflag=0x%08x\n",j,threadarg[j].errorflag);
    }

    gettimeofday(&end,0);
    
    if((end.tv_usec - begin.tv_usec)<0){
         msec=end.tv_usec/1000.0+1000 - begin.tv_usec/1000.0;
         sec =end.tv_sec - begin.tv_sec - 1;
    } else {
        msec=end.tv_usec/1000.0 - begin.tv_usec/1000.0;
        sec =end.tv_sec - begin.tv_sec;
    }
    
    sptime=((float)msec/1000+sec);
    testlength=testlength*testthreadnum;
    rate=testtime/1024*testlength/1024*8/sptime;
    printf("Time: %2.2f sec\n",sptime);
    printf("rate: %f Mbps\n",rate);
        
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aiengine.h"
#include <sys/time.h>
#include <time.h>

typedef struct _engine_info{
     int status; 
     long int start_time;
     long int first_time;
     long int end_time;
     FILE *audioFd;
}engine_info_t;

long int _get_time()
{
    long int at = 0;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    at = now.tv_sec * 1000 + now.tv_nsec / 1000000;
    return at; 
}

/*回调函数中不可以进行耗时长的操作和可能导致操作的操作*/
int engine_callback(void *usrdata, const char *id, int type, const void *message, int size)
{
    engine_info_t *info = (engine_info_t *)usrdata;
    if(type == AIENGINE_MESSAGE_TYPE_JSON)
    {
        printf("INFO: %.*s\n", size, message);
        //用户可以根据json格式的message信息中关键字来实现主观逻辑
    }else{//语音合成时，音频数据取出,音频数据以size==0代表音频结尾
        if(size == 0)
        {
            info->end_time = _get_time();
            printf("INFO: end_time-->%dms\n", info->end_time - info->start_time);
            info->status = 2;
        }else{
            if(info->status == 0)
            {
                info->first_time = _get_time();
                printf("INFO: first_time-->%dms\n", info->first_time - info->start_time);
                info->status = 1;
            }
            fwrite(message, 1, size, info->audioFd);
        }
    }
    return 0;
}


int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("usage:./cntts cfg param\n");
        return 1;
    }
    struct aiengine *engine = NULL;
    FILE *cfgFd = NULL;
    FILE *paramFd = NULL;
    
    char *cfg = NULL;
    char *param = NULL;
    engine_info_t info;
    info.audioFd = NULL;
    /*cfg info*/
    cfgFd = fopen(argv[1], "r");
    if(cfgFd == NULL)
    {
        printf("ERROR: open %s file error.\n", argv[1]);
        goto ERROR;
    }
    fseek(cfgFd, 0, SEEK_END);
    int cfgLen = ftell(cfgFd) + 1;
    cfg = (char *)calloc(1, cfgLen * sizeof(char));
    if(cfg == NULL)
    {   
        printf("ERROR: calloc %d bytes for cfg error.\n", cfgLen);
        goto ERROR;
    }   
    fseek(cfgFd, 0, SEEK_SET);
    fread(cfg, 1, cfgLen, cfgFd);
    /*param info*/
    paramFd = fopen(argv[2], "r");
    if(paramFd == NULL)
    {   
        printf("ERROR: open %s file error.\n", argv[2]);
        goto ERROR;
    }   
    fseek(paramFd, 0, SEEK_END);
    int paramLen = ftell(paramFd) + 1;
    param = (char *)calloc(1, paramLen * sizeof(char));
    if(param == NULL)
    {   
        printf("ERROR: calloc %d bytes for param error.\n", paramLen);
        goto ERROR;
    }   
    fseek(paramFd, 0, SEEK_SET);
    fread(param, 1, paramLen, paramFd);

    engine = aiengine_new(cfg);
    if(engine == NULL)
    {
        printf("ERROR: new engine error.\n");
        goto ERROR;
    }

    info.status = 0;
    info.audioFd = fopen("cntts.pcm", "wb");
    if(info.audioFd == NULL)
    {
        printf("ERROR: open audio file error.\n");
        goto ERROR;
    }

    char uuid[64];
    memset(uuid, 0, sizeof(uuid));
    int ret = aiengine_start(engine, param, uuid, engine_callback, &info);
    if(ret == -1)
    {
        printf("ERROR: start engine error.\n");
        goto ERROR;
    }
    printf("INFO: uuid-->%s\n", uuid);
    info.start_time = _get_time(); 
    ret = aiengine_stop(engine);
    if(ret == -1)
    {
        printf("ERROR: stop engine error.\n");
        goto ERROR;
    }
    while(1)//等待合成结束
    {
        if(info.status == 2) break;
        usleep(100000);
    }
ERROR:
    if(cfg) free(cfg);
    if(param) free(param);
    if(cfgFd) fclose(cfgFd);
    if(paramFd) fclose(paramFd);
    if(info.audioFd) fclose(info.audioFd);
    if(engine) aiengine_delete(engine);
    return 0;
}

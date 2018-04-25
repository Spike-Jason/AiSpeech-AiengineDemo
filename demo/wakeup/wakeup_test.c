#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aiengine.h"
#include <unistd.h>
#include <time.h>

typedef struct _engine_info{
     int status; 
     long start_time;
     long end_time;
}engine_info_t;

long int _get_time()
{
    long int at = 0;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    at = now.tv_sec * 1000 + now.tv_nsec / 1000000;
    return at; 
}

/* 回调函数中不可以进行耗时长的操作和可能导致卡死的操作 */
int engine_callback(void *usrdata, const char *id, int type, const void *message, int size)
{
    engine_info_t *info = (engine_info_t *)usrdata;
    if(type == AIENGINE_MESSAGE_TYPE_JSON)
    {
        info->end_time = _get_time();
        long int delay = info->end_time - info->start_time;
        printf("INFO: delay-->%ldms\n",delay);
        printf("INFO: %.*s\n", size, message);
        info->status = 1;
        //用户可以根据json格式的message信息中关键字来实现主观逻辑
    }else{//语音合成时，音频数据取出,音频数据以size==0代表音频结尾
        
    }
    return 0;
}


int main(int argc, char *argv[])
{
    if(argc != 4 && argc != 5)
    {
        printf("usage:./wakeup cfg param audio [off]\n");
        return 1;
    }
    int off = 0;
    if(argc == 5) off = atoi(argv[4]);
    struct aiengine *engine = NULL;
    FILE *cfgFd = NULL;
    FILE *paramFd = NULL;
    FILE *audioFd = NULL;
    char *cfg = NULL;
    char *param = NULL;
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


    audioFd = fopen(argv[3], "rb");
    if(audioFd == NULL)
    {
        printf("ERROR: open %s file error.\n", argv[3]);
        goto ERROR;
    }
    
    engine = aiengine_new(cfg);
    if(engine == NULL)
    {
        printf("ERROR: new engine error.\n");
        goto ERROR;
    }
    sleep(2);
BEGIN:
    fseek(audioFd, off, SEEK_SET);
    engine_info_t info;
    info.status = 0;
    char uuid[64];
    memset(uuid, 0, sizeof(uuid));
    int ret = aiengine_start(engine, param, uuid, engine_callback, &info);
    if(ret == -1)
    {
        printf("ERROR: start engine error.\n");
        goto ERROR;
    }
    printf("INFO: uuid--->%s\n", uuid);
    char buf[3200];
    int readBytes = 0;
    /*模拟实时送音频，3200字节=100ms*/
    info.start_time = _get_time(); 
    long int time_one, time_two;    

    while(1)
    {
        time_one = _get_time();
        readBytes = fread(buf, 1, 3200, audioFd);
        if(readBytes < 0)
        {
            printf("ERROR: read audio data error.\n");
            goto ERROR;
        }else if(readBytes == 0){
            printf("INFO: no data to feed.\n");
            break;
        }else{
            ret = aiengine_feed(engine, buf, readBytes);
            if(ret == -1)
            {
                printf("ERROR: feed engine error.\n");
                goto ERROR;
            }
        }
        if(info.status == 1) break;
        time_two = _get_time(); 
        long int time_diff_us = time_two - time_one;
        long int sleep_time = 100000 - time_diff_us;
        //printf("INFO:feed data size-->%d, sleep_time-->%ld\n", readBytes, sleep_time);
        usleep(sleep_time);
    }
    ret = aiengine_stop(engine);
    if(ret == -1)
    {
        printf("ERROR: stop engine error.\n");
        goto ERROR;
    }
    while(1)
    {
        if(info.status == 1) break;
        usleep(100000);
    }

    //goto BEGIN;
ERROR:
    if(cfg) free(cfg);
    if(param) free(param);
    if(cfgFd) fclose(cfgFd);
    if(paramFd) fclose(paramFd);
    if(audioFd) fclose(audioFd);
    if(engine) aiengine_delete(engine);
    return 0;
}

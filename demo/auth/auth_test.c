/* 该测试代码用于检查授权，主动进行云端设备注册
 * 同一台设备一般只要进行一次云端设备注册
*/
#include <stdio.h>
#include <string.h>
#include "json/cJSON.h"
#include "aiengine.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("usage:./auth cfg-auth.json\n");
        return 0;
    }
    FILE *cfgFd = NULL;
    struct aiengine *auth_engine = NULL;
    char *cfg = NULL;
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
    auth_engine = aiengine_new(cfg);
    if(auth_engine == NULL)
    {
        printf("ERROR: new engine error\n");
        goto ERROR;
    }
    //根据json格式cfg字符串中是否有success字段判断成功与否
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int ret;
    int authed = 0;

    //check auth
    ret = aiengine_opt(auth_engine, 10, buf, 1024); //11为主动授权，10为检查授权
    if(ret == -1)
    {
        printf("ERROR: check auth error\n");
    }else{
        cJSON *auth_js = cJSON_Parse(buf);
        if(auth_js && cJSON_GetObjectItem(auth_js, "success"))
        {
            printf("INFO: check auth ok\n");
            authed = 1;
        }else{
            printf("ERROR: check auth error, %s\n", buf);
        }
        if(auth_js) cJSON_Delete(auth_js);
    }

    if(authed != 1)
    {
        memset(buf, 0, sizeof(buf));

        ret = aiengine_opt(auth_engine, 11, buf, 1024); //11为主动授权，10为检查授权
        if(ret == -1)
        {
            printf("ERROR: do auth error\n");
        }else{
            cJSON *auth_js = cJSON_Parse(buf);
            if(auth_js && cJSON_GetObjectItem(auth_js, "success"))
            {
                printf("INFO: do auth ok\n");
            }else{
                printf("ERROR: do auth error, %s\n", buf);
            }
            if(auth_js) cJSON_Delete(auth_js);
        }
    }
ERROR:
    if(cfg) free(cfg);
    if(cfgFd) fclose(cfgFd);
    if(auth_engine) aiengine_delete(auth_engine);
    return 0;
}

# AiengineDemo整理

标签（空格分隔）： FAE

---
##文件目录
.
├── assets --->资源目录
│   ├── 0.wav --->音频资源
│   ├── 1.wav  
│   ├── 2.wav
│   ├── aiengine-2.9.5-14806716618595ef.provision --->授权文件
│   ├── aiphonse.serialNumber --->授权生产文件
│   └── luabin.lub --->授权文件
├── demo Demo --->目录
│   ├── asr --->识别功能
│   │   ├── asr_test.c --->识别功能程序
│   │   ├── cfg_cloud_asr.json --->**云端配置**
│   │   ├── cfg_native_asr.json --->**本地配置**
│   │   ├── makefile
│   │   ├── param_cloud_asr.json --->云端参数
│   │   └── param_native_asr.json --->本地参数
│   ├── auth --->认证功能
│   │   ├── auth_test.c --->认证功能程序
│   │   ├── cfg_auth.json ---> **认证配置**
│   │   ├── json
│   │   │   └── cJSON.h
│   │   └── makefile
│   ├── cntts --->TTS生产功能
│   │   ├── cfg_cloud_cntts.json
│   │   ├── cfg_native_cntts.json
│   │   ├── cntts_test.c
│   │   ├── makefile
│   │   ├── param_cloud_cntts.json
│   │   └── param_native_cntts.json
│   ├── gram
│   │   ├── cfg_native_gram.json
│   │   ├── gram_test.c
│   │   ├── makefile
│   │   └── param_native_gram.json
│   └── wakeup
│       ├── cfg_native_wakeup.json
│       ├── makefile
│       ├── param_native_wakeup.json
│       └── wakeup_test.c
├── inc
│   └── aiengine.h --->头文件
├── lib
│   ├── libaiengine-127-linux-2.9.5-20180109140817.so --->语音库
│   ├── libcjson.so --->JSON库
│   └── libcjson_utils.so --->JSON库
├── makefile --->**主makefile**
├── res --->数据资源
│   ├── asr
│   │   └── ebnfr.aicar.0.0.11.4.bin --->本地识别库文件
│   ├── cntts
│   │   ├── aitts_sent_dict_v3.15.db --->本地TTS数据库
│   │   └── zhilingf.v1.5.1.bin --->本地TTS语音资源
│   ├── gram
│   │   └── ebnfc.aicar.0.0.11.4.bin --->本地语法编译库
│   └── wakeup
│       └── wakeup_aifar_comm_20180104.bin --->唤醒资源
└── RunDemo.sh --->**运行文件**

---
## 重要文件说明
### cfg_xxx_xxx.json 配置文件

    {
    "luaPath": "/home/aiphonse/Documents/Demo/aiengine-94/assets/luabin.lub",
    "appKey": "xxx",
    "secretKey": "xxx",
    "provision": "/home/aiphonse/Documents/Demo/aiengine-94/assets/xxx.provision",
    "serialNumber": "/home/aiphonse/Documents/Demo/aiengine-94/assets/aiphonse.serialNumber",
    "prof":{
        "enable": 1,
        "output": "./aiengine.log"
    }
}
JSON文件必备配置信息
其中**luaPath、provision、serialNumber**都存放在**assets**文件夹中。使用的是**绝对路径**请根据自身环境进行配置。**注意每一个cfg文件都要进行配置**
另外**appkey、secretKey为测试KEY这与provision一致，请在获取License后更换，需要保证三者一致**

### libaiengine-127-linux-2.9.5-20180109140817.so
该库为语音库，**请在获取SO库后将其加入到lib文件夹中**

### 主目录makefile
主目录下的makefile支持全部编译和单独编译DEMO中各个功能目录，如

    make auth           #单独编译auth
    make all            #编译所有功能
    make clean_all      #clean 所有功能目录
    make clean_auth     #单独clean auth目录

### RunDemo.sh
主目录下的RunDemo.sh是编译 和 运行的样例。会顺序执行 
编译所有功能-->运行auth-->运行ASR（云端）-->运行cntts -->运行Wakeup -->运行gram

**建议参考执行内容，单独执行云端或本地的功能**


### 单功能makefile
**请按照得到的语音库，修改LIB的配置**

    ../../lib/libaiengine-127-linux-2.9.5-20180109140817.so

---
## 关于配置
除Auth外其他每个功能中都包含param_xx_xx.json文件
请参考开发文档[参考文档][1]


  [1]: http://open.aispeech.com/doc/%22%E5%8F%82%E8%80%83%E6%96%87%E6%A1%A3%22
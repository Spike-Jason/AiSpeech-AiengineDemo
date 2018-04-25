.PHONY:all auth asr gram wakeup cntts clean_auth clean_asr clean_gram clean_wakeup clean_cntts clean_all
all:
	make auth && make asr && make gram && make cntts && make wakeup 
clean_all:
	make clean_asr && make clean_cntts && make clean_wakeup && make clean_gram && make clean_auth


auth:
	cd ./demo/auth && make

asr:
	cd ./demo/asr && make

gram:
	cd ./demo/gram && make

cntts:
	cd ./demo/cntts && make

wakeup:
	cd ./demo/wakeup && make

clean_auth:
	cd ./demo/auth && make clean

clean_asr:
	cd ./demo/asr && make clean

clean_gram:
	cd ./demo/gram && make clean

clean_wakeup:
	cd ./demo/wakeup && make clean

clean_cntts:
	cd ./demo/cntts && make clean
#!/bin/bash
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin

# 加了下面這行，未來在使用 "ctrl+c" 終止程式時，會一併將終止訊息發給子程序，從而終止所有子程序
trap "kill 0" INT

# 路徑參數設定
prefixTCL="2020-12-12-22-44-15"
maxSpeed="_max-speed-80"

# 因為我是使用 git 進行版本控管，有下面的指令會比較方便
cd ns3-learning
git pull
cp ./multi_udp.cc ../scratch/
cp ./${prefixTCL}/${prefixTCL}${maxSpeed}.tcl ../scratch/
cd ..

# 要不要啟用 sdn 請在這裡設定，"isSdnEnabled=true"表示啟用，"isSdnEnabled=false"表示不啟用
prefixName="udp_sdn"
isSdnEnabled=true
if [ $isSdnEnabled = false ]
then
	prefixName="udp_lte"
fi

# 輸出路徑的生成
outLogDir="outLog_${prefixName}"
echo "isSdnEnabled = ${isSdnEnabled}"
echo "outLogDir = ${outLogDir}"
mkdir -p $outLogDir

# 將 run.sh 備份進去輸出的資料夾，比較方便確認跑的時候用的是甚麼設定
cp ./run.sh $outLogDir

# 將程序分成 20 個部份來跑，加快執行速度
for i in $(seq 0 19)
do
    logName="log.${prefixName}_${i}"
    outLogPath="${outLogDir}/${logName}"
    animFile="anim.${prefixName}_${i}"
    echo "logName = ${logName}"
    echo "outLogDir = ${outLogDir}"
    echo "animFile = ${animFile}"
    startUe=$(($i*5))
    endUe=$(($i*5+5))
    echo "startUe = ${startUe}"
    echo "endUe = ${endUe}"
    (./waf --run "scratch/multi_udp --simTime=60 --numberOfUes=100 --startUe=${startUe} --endUe=${endUe} --isSdnEnabled=${isSdnEnabled} --isDownlink=true --isUplink=true --animFile=${animFile} --interAppInterval=0.001 --interPacketInterval=1 --nPayloadBytes=1036" > $outLogPath 2>&1) &
    if [ $i = 0 ]
    then
        # 第一次執行時會花費大量時間與 CPU 跑編譯，可以可慮延長這裡的秒數
        # 或是用簡單的參數跑一遍 ./waf --run 相關的指令，下次編譯就可以花比較少的秒數
	    sleep 20s
    fi
    sleep 1s
done

echo "isSdnEnabled = ${isSdnEnabled}"
echo "outLogDir = ${outLogDir}"

wait

# 關於 NS-3 執行環境與安裝、執行方法

編輯日期：2021/06/27  
編輯人：沈濃翔  
信箱：g08410117@ccu.edu.tw  

## 一、環境

1. 查詢 linux 發行版與版本

``` bash
cat /etc/*-release
```

版本相關資訊如下：
> DISTRIB_ID=Ubuntu  
> DISTRIB_RELEASE=20.04  
> DISTRIB_CODENAME=focal  
> DISTRIB_DESCRIPTION="Ubuntu 20.04.2 LTS"  
> NAME="Ubuntu"  
> VERSION="20.04.2 LTS (Focal Fossa)"  
> ID=ubuntu  
> ID_LIKE=debian  
> PRETTY_NAME="Ubuntu 20.04.2 LTS"  
> VERSION_ID="20.04"  
> HOME_URL="https://www.ubuntu.com/"  
> SUPPORT_URL="https://help.ubuntu.com/"  
> BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"  
> PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/  privacy-policy"  
> VERSION_CODENAME=focal  
> UBUNTU_CODENAME=focal  

2. 查詢系統基本資訊

```bash
uname -a
```

系統相關資訊如下：
> Linux Dell3 5.8.0-53-generic #60~20.04.1-Ubuntu SMP Thu May 6 09:52:46 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux  

## 二、NS-3 安裝

1. 依照官網上的教學安裝相對應的前置套件 (Prerequisites章節)：[官網連結](https://www.nsnam.org/wiki/Installation#Installation)

2. 安裝 NS-3 (Installation章節)
基本是依照官網上的 Manual installation 教學來安裝 ns3，不過部分地方有出入，建議使用下列指令安裝

```bash
cd
git clone https://gitlab.com/nsnam/ns-3-allinone.git
cd ns-3-allinone
./download.py # 原本在官網上是 ./download.py -n ns-3.30
```

完成之後會出現 "ns-3-dev/" 這個資料夾

## 三、NS-3 模擬程式執行

1. 請進入 "ns-3-dev/" 這個資料夾  

```bash
cd
cd ns-3-allinone/ns-3-dev/
```

2. 進行初始化設定

```bash
./waf clean
./waf configure --build-profile=debug --enable-examples --enable-tests
```

如果有缺少的套件，這個指令應該會提醒你需要裝哪些套件  

3. 將必要的資料複製進這個資料夾  
請將與 README.md 在同一個資料夾的 "ns3-learning/" 複製到 linux 上的 "ns-3-dev/" 資料夾底下  

4. 執行主程式前的注意事項
- 某些 ns3-learning/ 底下的檔案需要先透過 SUMO 生產出來，可以先去看 sumo/ 底下的另一份 [README.md](../sumo/README.md)
- 產生出來的 sumo 路徑檔(\*.tcl)，請先使用 "ns3-learning" 底下的 resetSec.py 對秒數進行歸零的動作  
- 如有需要記得更換為自己的檔案路徑與名稱  
```bash
python resetSec.py 2020-12-12-22-44-15/2020-12-12-22-44-15_max-speed-80.tcl
```
  
- 首次執行主程式 (multi_udp.cc) 前請先執行以下指令，我的實驗使用的是 2020-12-12-22-44-15/ 下的資料  
- 如有需要記得更換為自己的檔案路徑與名稱    

```bash
cp ns3-learning/multi_udp.cc scratch
cp ns3-learning/2020-12-12-22-44-15/2020-12-12-22-44-15_max-speed-80.tcl scratch
./waf --run "scratch/multi_udp --traceFile=scratch/2020-12-12-22-44-15_max-speed-80.tcl --numberOfUes=5 --startUe=0 --endUe=4 --isSdnEnabled=true --isDownlink=true --isUplink=true --interAppInterval=1 --interPacketInterval=1000"
```

首次執行會需要較長的編譯時間，且會占用大量 CPU  
由於之後會需要執行batch檔 (run.sh)，來一次執行多個模擬程式，先執行一次上面的指令，讓他編譯過一遍  
之後編譯就不用花那麼多時間與資源  
出現類似下面的執行結果後，即可按下 ctrl+C 終止程式  

![](image/intermediate_result.png)

5. 執行主程式
- 先將 shell script (run.sh) 從 ns3-learning/ 複製到 ns-3-dev/ 底下

```bash
cp ns3-learning/run.sh .
```

- 執行腳本

```bash
sh run.sh
```

- 腳本有兩種模式，需要可以進入 run.sh 腳本內修改  
一種是模擬 SDN handover 的模式，一種是使用 4G 原本的 handover 模式 (3GPP-X2)  
如下圖所示，  
將 isSdnEnabled 設為 true 則為模擬 SDN handover 的模式  
將 isSdnEnabled 設為 false 則為使用 4G 原本的 handover 模式  

![](image/sdn_mode.png)  

除此之外，一些參數設定也可以進入 run.sh 中設定  

6. 等待結果

以上圖中的參數設定為例，當你的  
prefixTCL 為 "2020-12-12-22-44-15"  
maxSpeed 為 "_max-speed-80"  
isSdnEnabled 為 "true"

則你執行腳本會產出一個名為 "outLog_max-speed-80_udp_sdn/" 的資料夾  

7. 印出圖表

在成功產出 "outLog_max-speed-80_udp_sdn/" 與 "outLog_max-speed-80_udp_lte/" 後  
先將 python 檔 (diff_time.py) 從 ns3-learning/ 複製到 ns-3-dev/ 底下  
隨後執行下列指令

```bash
# outLog_max-speed-80_udp_ 為兩個 output log 共同的 prefix，可視需求更換檔名
python diff_time.py outLog_max-speed-80_udp_ 
```

diff_time.py 這個檔案，做的事情主要有三件：  
- 找出 sdn handover time 的時間點，隨後在各個 log 檔中，抓出 handover 前後的區間  
- 在這些區間中，整理出各種不同的 route 下的 throughput，以此來計算出 upload 與 download 的 delay  
- 隨後印出 3GPP-X2 (傳統 4G 的作法) 跟 SDN handover 上的區別  

最後會匯出成一個名為 "outLog_max-speed-80_udp_" 的資料夾  
裡面的資料是當作 cache 使用，節省程式執行與 debug 的時間  
如果需要使用新資料集重跑程式需要將此資料夾刪除  

8. 計算 delay  

```bash
python response_delay.py
```

## 四、常用指令筆記

1. optimized 模式

也許可以節省一點執行時間，但是有一些 log 的資訊無法印出來  

```bash
./waf clean
./waf configure --build-profile=optimized --enable-examples --enable-tests
```

2. 列出 NS_LOG_COMPONENT_DEFINE

這個指令可以列出 NS_LOG_COMPONENT，  
在 ns3 主程式 (multi_udp.cc) 內可以撰寫來選擇想看哪些 component 所產出的 log  

```bash
find . -name '*.cc' | xargs grep NS_LOG_COMPONENT_DEFINE
```

3. 用來搜尋特定 component 

```bash
find . -name '*.cc' | xargs grep NS_LOG_COMPONENT_DEFINE | grep -i sink
```

grep -i 為 忽略大小寫  

4. 一些 NS3 的 log 可以透過設定環境變數來顯示

```bash
export NS_LOG=UdpL4Protocol
./waf --run first
```

# 關於 sumo 執行環境與安裝、執行方法

編輯日期：2021/06/27  
編輯人：沈濃翔  
信箱：g08410117@ccu.edu.tw  

## 一、環境

1. Windows 安裝環境

Windows 版本：
> Windows 10 專業版

系統：
> 處理器： Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz 3.19GHz  
> 記憶體(RAM)： 8.00 GB  
> 系統類型： 64位元作業系統，x64型處理器  

瀏覽器：
> Chrome 版本 91.0.4472.106 (正式版本) (64 位元)

## 二、安裝

1. 前往官網下載並執行安裝檔：[官網連結](https://sumo.dlr.de/docs/Installing/index.html#windows)

## 三、使用 OSM Web Wizard 圈選地圖並生成車流

1. 打開 windows 工作列搜尋 "OSM Web Wizard" 並執行之

2. OSM Web Wizard 為一 python 檔案，請使用 python 執行該檔案

3. 成功執行後會打開瀏覽器，如下圖：

![](image/osm_web_page.png)

4. 勾選右方工具列中的 "Select Area" ，選擇適當大小的地圖後，點選右上角的"Generate Scenario" 生成車流  
(Optional) 取消勾選右方工具列中的 "Add Polygons" 可以不加入建築  

* 如果使用 OSM Web Wizard 圈選生成地圖時，無法正常生成時，可以嘗試清空瀏覽器 cookie 等設定(可能是一開始使用時，沒有開放瀏覽器的權限給該程式)  

5. 完成生成之後，會自動開啟 SUMO 主程式，如下圖，記得記下該設定檔儲存路徑

![](image/sumo_screen_shot.png)

6. 該設定檔下的資料夾應為如下圖所示：

![](image/files.png)

7. 可以透過資料夾下的 "osm.passenger.trips.xml" 檔案，設定車輛最大速度，如下圖：

![](image/setting_trips.png)


8. 透過以下指令生成可以被 NS3 當作 input 的車流路徑檔 (*.tcl)  

以下指令為 Windows CMD 下的指令，如有需要記得修改對應的變數  

``` CMD

:: 設定 CMD 下的環境變數，分別是 config 資料夾所在的路徑、該資料夾名稱，與設定的最大車速
set PATH_TO_CONFIG=D:\github\sumo-learning
set SUMO_PREFIX=2020-12-12-22-44-15
set SUMO_MAXSPEED=_max-speed-80
:: 模擬開始跟結束時間
set SIM_BEGIN=270
set SIM_END=420

:: 生成檔案路徑名稱
set SUMO_CONFIG=%PATH_TO_CONFIG%\%SUMO_PREFIX%\osm.sumocfg
set SUMO_XML=%PATH_TO_CONFIG%\%SUMO_PREFIX%\%SUMO_PREFIX%%SUMO_MAXSPEED%.xml
set SUMO_CSV=%PATH_TO_CONFIG%\%SUMO_PREFIX%\%SUMO_PREFIX%%SUMO_MAXSPEED%.csv
set SUMO_TCL=%PATH_TO_CONFIG%\%SUMO_PREFIX%\%SUMO_PREFIX%%SUMO_MAXSPEED%.tcl
echo %SUMO_PREFIX%
echo %SUMO_CONFIG%
echo %SUMO_XML%
echo %SUMO_TCL%

:: 產生 NS3 可以 input 的 mobility trace file (*.tcl) 到同一個資料夾底下
sumo -c %SUMO_CONFIG% --fcd-output %SUMO_XML% --step-length 1
python D:\SUMO\tools\traceExporter.py --fcd-input %SUMO_XML% --ns2mobility-output %SUMO_TCL% --begin %SIM_BEGIN% --end %SIM_END%
python D:\SUMO\tools\traceExporter.py --fcd-input %SUMO_XML% --gpsdat-output %SUMO_CSV% --begin %SIM_BEGIN% --end %SIM_END%

```

9. 生成的檔案如下：

![](image/trace_files.png)


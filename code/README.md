# 基本說明與注意事項

編輯日期：2021/06/27  
編輯人：沈濃翔  
信箱：g08410117@ccu.edu.tw  

## 一、說明

1. 本資料夾下包含4個子資料夾：
- sumo  
- ns3  
- golang-learning  
- solidity-learning  

2. 各自的資料夾內有各自的 README.md 說明文件，可以至各自資料夾下閱讀 (請先閱讀完本說明再閱讀各自的說明)

## 二、功能

1. sumo 專案主要教導如何使用 SUMO 與 OSM 產生車輛路徑檔  
2. ns3 專案主要教導如何匯入 sumo 專案產生的路徑檔，如何使用模擬程式等，最後使用 python 產出比較圖表  
3. golang-learning 主要教導如何撰寫密碼學相關程式，產生對應的數據以供使用  
4. solidity-learning 主要教導如何使用 Ethereum Remix IDE，使用 solidity 語言撰寫 smart contract 等  

## 三、依賴關係

1. ns3 專案需要使用 sumo 產生的檔案，請先執行 sumo 專案內的操作  
2. solidity-learning 專案需要使用 golang-learning 產生的數據，請先執行 golang-learning 專案內的操作  
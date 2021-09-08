#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/config-store-module.h"

#include <iostream>
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include <ns3/buildings-module.h>
#include "ns3/netanim-module.h"

#include "ns3/flow-monitor-module.h"
#include "ns3/log.h"
#include <sys/timeb.h>
#include <ns3/internet-trace-helper.h>
#include <ns3/spectrum-module.h>
#include <ns3/log.h>
#include <ns3/string.h>
#include <fstream>
#include <string>
#include <chrono>
#include <limits>
#include <math.h>
#include <exception>
#include <vector>

using namespace ns3;
using namespace std;

Ipv4InterfaceContainer internetIpIfaces;
Ipv4InterfaceContainer ueIpIfaces;

NS_LOG_COMPONENT_DEFINE("LenaX2HandoverMeasures");

// 以下的 Notify functions 主要是用來給 NS3 做 callback 用的
// 當相關的 event 發生時，有設定的 function 會被呼叫，從而可以印出 log 或是做一些處理
void NotifyConnectionEstablishedUe(std::string context,
                                   uint64_t imsi,
                                   uint16_t cellid,
                                   uint16_t rnti)
{
    double now = Simulator::Now().GetSeconds();
    std::cout << "NotifyConnectionEstablishedUe : " << now << "\t";
    std::cout << context
              << " UE IMSI " << imsi
              << ": connected to CellId " << cellid
              << " with RNTI " << rnti
              << std::endl;
}

void NotifyHandoverStartUe(std::string context,
                           uint64_t imsi,
                           uint16_t cellid,
                           uint16_t rnti,
                           uint16_t targetCellId)
{
    double now = Simulator::Now().GetSeconds();
    std::cout << "NotifyHandoverStartUe : " << now << "\t";
    std::cout << context
              << " UE IMSI " << imsi
              << ": previously connected to CellId " << cellid
              << " with RNTI " << rnti
              << ", doing handover to CellId " << targetCellId
              << std::endl;
}

void NotifyHandoverEndOkUe(std::string context,
                           uint64_t imsi,
                           uint16_t cellid,
                           uint16_t rnti)
{
    double now = Simulator::Now().GetSeconds();
    std::cout << "NotifyHandoverEndOkUe : " << now << "\t";
    std::cout << context
              << " UE IMSI " << imsi
              << ": successful handover to CellId " << cellid
              << " with RNTI " << rnti
              << std::endl;
}

void NotifyConnectionEstablishedEnb(std::string context,
                                    uint64_t imsi,
                                    uint16_t cellid,
                                    uint16_t rnti)
{
    double now = Simulator::Now().GetSeconds();
    std::cout << "NotifyConnectionEstablishedEnb : " << now << "\t";
    std::cout << context
              << " eNB CellId " << cellid
              << ": successful connection of UE with IMSI " << imsi
              << " RNTI " << rnti
              << std::endl;
}

void NotifyHandoverStartEnb(std::string context,
                            uint64_t imsi,
                            uint16_t cellid,
                            uint16_t rnti,
                            uint16_t targetCellId)
{
    double now = Simulator::Now().GetSeconds();
    std::cout << "NotifyHandoverStartEnb : " << now << "\t";
    std::cout << context
              << " eNB CellId " << cellid
              << ": start handover of UE with IMSI " << imsi
              << " RNTI " << rnti
              << " to CellId " << targetCellId
              << std::endl;
}

void NotifyHandoverEndOkEnb(std::string context,
                            uint64_t imsi,
                            uint16_t cellid,
                            uint16_t rnti)
{
    double now = Simulator::Now().GetSeconds();
    std::cout << "NotifyHandoverEndOkEnb : " << now << "\t";
    std::cout << context
              << " eNB CellId " << cellid
              << ": completed handover of UE with IMSI " << imsi
              << " RNTI " << rnti
              << std::endl;
}

//函數功能: 傳入一個字串s，以splitSep裡的字元當做分割字符，回傳vector<string>
vector<string> splitStr2Vec(string s, string splitSep)
{
    vector<string> result;
    int current = 0; //最初由 0 的位置開始找
    int next = 0;
    while (next != -1)
    {
        next = s.find_first_of(splitSep, current); //尋找從current開始，出現splitSep的第一個位置(找不到則回傳-1)
        if (next != current)
        {
            string tmp = s.substr(current, next - current);
            if (!tmp.empty()) //忽略空字串(若不寫的話，尾巴都是分割符會錯)
            {
                result.push_back(tmp);
            }
        }
        current = next + 1; //下次由 next + 1 的位置開始找起。
    }
    return result;
}

// 用來印出 UDP 傳送時的資訊
void TxTrace(std::string context, Ptr<const Packet> pkt, const Address &src, const Address &dst)
{
    vector<string> sep = splitStr2Vec(context, "/");
    // cout << "node: " << sep[1] << " ";
    int iNode = std::stoi(sep[1]) - (4 + 12);
    Ipv4Address sourceAddress;
    if (iNode < 0)
    {
        sourceAddress = "1.0.0.2";
    }
    else
    {
        sourceAddress = ueIpIfaces.GetAddress(iNode);
    }
    double now = Simulator::Now().GetSeconds();
    std::cout << "TxTrace:"
              << " now: " << now
              << " Nodelist: " << sep[1]
              << " packetSize: " << pkt->GetSize()
              << " source: " << sourceAddress
              << " destination: " << InetSocketAddress::ConvertFrom(dst).GetIpv4()
              << " " << std::endl;
}

// 用來印出 UDP 接收時的資訊
void RxTrace(std::string context, Ptr<const Packet> pkt, const Address &a, const Address &b)
{
    double now = Simulator::Now().GetSeconds();
    std::cout << "RxTrace:"
              << " now: " << now
              << " " << context
              << " packetSize: " << pkt->GetSize()
              << " source: " << InetSocketAddress::ConvertFrom(a).GetIpv4()
              << " destination: " << InetSocketAddress::ConvertFrom(b).GetIpv4()
              << " " << std::endl;
}

AnimationInterface *pAnim = 0;

// 這裡是 manualAttach() 會使用到的變數，主要儲存某個UE上一個連接的ENB編號，以用來做換手的動作
Ptr<LteHelper> lteHelper;
int *last_index;
bool isSdnEnabled;

// 這裡是 SDN 啟用的時候會使用到的 function
// 用途是判斷目前時間點，距離 UE 最近的 ENB 是誰，並且 handover 過去
// 以此來模擬 SDN 的行為
void manualAttach(NodeContainer *ueNodes, NetDeviceContainer *ueLteDevs, NodeContainer *enbNodes, NetDeviceContainer *enbLteDevs, uint16_t numberOfUes, uint16_t numberOfEnbs)
{
    cout << "manualAttach" << endl;
    if (!isSdnEnabled)
        return;
    for (int i = 0; i < numberOfUes; i++)
    {
        Ptr<const MobilityModel> ueMobilityModel = ueNodes->Get(i)->GetObject<MobilityModel>();
        Vector pos_ue = ueMobilityModel->GetPosition();
        // std::cout << Simulator::Now ().GetSeconds() << ", ue_x=" << pos_ue.x << ", ue_y=" << pos_ue.y << std::endl;
        int index = -1;
        unsigned long long int min_distance = std::numeric_limits<int>::max();
        for (int j = 0; j < numberOfEnbs; j++)
        {
            Ptr<const MobilityModel> enbMobilityModel = enbNodes->Get(j)->GetObject<MobilityModel>();
            Vector pos_enb = enbMobilityModel->GetPosition();
            // std::cout << Simulator::Now ().GetSeconds() << ", enb_x=" << pos_enb.x << ", enb_y=" << pos_enb.y << std::endl;
            float current_distance = sqrt((pos_ue.x - pos_enb.x) * (pos_ue.x - pos_enb.x) + (pos_ue.y - pos_enb.y) * (pos_ue.y - pos_enb.y));
            if (current_distance < min_distance)
            {
                min_distance = current_distance;
                index = j;
                // std::cout << "ue: " << i <<  ", enb:" << index << ", distance: " << min_distance << endl << endl;
            }
        }
        if (last_index[i] != index)
        {
            bool hasRnti = false;
            for (int j = 0; j < numberOfEnbs; j++)
            {
                std::cout << "check1" << std::endl;
                uint16_t ueRNTI = ueLteDevs->Get(i)->GetObject<LteUeNetDevice>()->GetRrc()->GetRnti();
                std::cout << "check2" << std::endl;
                bool hasUeManager = enbLteDevs->Get(j)->GetObject<LteEnbNetDevice>()->GetRrc()->HasUeManager(ueRNTI);
                std::cout << "check3" << std::endl;
                if (hasUeManager)
                {
                    std::cout << "check4" << std::endl;
                    uint16_t ueIMSI = ueLteDevs->Get(i)->GetObject<LteUeNetDevice>()->GetImsi();
                    std::cout << "check5" << std::endl;
                    uint16_t enbIMSI = enbLteDevs->Get(j)->GetObject<LteEnbNetDevice>()->GetRrc()->GetUeManager(ueRNTI)->GetImsi();
                    std::cout << "check6" << std::endl;
                    // cout << "j: " << j << ", ueIMSI: " << ueIMSI << ", enbIMSI: " << enbIMSI << endl;
                    if (ueIMSI == enbIMSI)
                    {
                        last_index[i] = j;
                        hasRnti = true;
                        break;
                    }
                }
            }
            if (!hasRnti)
                return;
            uint16_t ueCellId = ueLteDevs->Get(i)->GetObject<LteUeNetDevice>()->GetRrc()->GetCellId();
            uint16_t ueRNTI = ueLteDevs->Get(i)->GetObject<LteUeNetDevice>()->GetRrc()->GetRnti();
            uint16_t enbCellId = enbLteDevs->Get(index)->GetObject<LteEnbNetDevice>()->GetCellId();
            cout << "ueCellId: " << ueCellId << ", ueRNTI: " << ueRNTI << ", enbCellId: " << enbCellId << endl;
            // lteHelper->AttachToClosestEnb (ueLteDevs->Get (i), enbLteDevs->Get (index));

            cout << "\n\n====================\nsec: " << Simulator::Now().GetSeconds() << ", ue: " << i << ", last_index: " << last_index[i] << ", next_index: " << index << endl;
            if (last_index[i] == index)
                return;
            std::cout << "check7" << std::endl;
            lteHelper->HandoverRequest(Seconds(0), ueLteDevs->Get(i), enbLteDevs->Get(last_index[i]), enbLteDevs->Get(index));
            std::cout << "check8" << std::endl;
            last_index[i] = index;
        }
    }
}

int main(int argc, char *argv[])
{
    // 用來計時用的變數
    auto t1 = chrono::high_resolution_clock::now();

    // 決定想看的 log 等級是甚麼
    // LogLevel logLevel = (LogLevel)(LOG_PREFIX_ALL | LOG_LEVEL_ALL);

    // 想看甚麼 component 產出的 log 可以在這裡設定
    // LogComponentEnable ("LteHelper", logLevel);
    // LogComponentEnable ("EpcHelper", logLevel);
    // LogComponentEnable ("EpcEnbApplication", logLevel);
    // LogComponentEnable ("EpcMmeApplication", logLevel);
    // LogComponentEnable ("EpcPgwApplication", logLevel);
    // LogComponentEnable ("EpcSgwApplication", logLevel);
    // LogComponentEnable ("EpcX2", logLevel);

    // LogComponentEnable ("LteEnbRrc", logLevel);
    // LogComponentEnable ("LteEnbNetDevice", logLevel);
    // LogComponentEnable ("LteUeRrc", logLevel);
    // LogComponentEnable ("LteUeNetDevice", logLevel);
    // LogComponentEnable ("A2A4RsrqHandoverAlgorithm", logLevel);
    // LogComponentEnable ("A3RsrpHandoverAlgorithm", logLevel);
    // LogComponentEnable ("UdpSocketImpl", LOG_ALL);

    // LogComponentEnable ("OnOffApplication", LOG_ALL);
    // LogComponentEnable ("PacketSink", LOG_ALL);
    LogComponentEnable("UdpClient", LOG_ALL);
    LogComponentEnable("UdpServer", LOG_ALL);

    // 參數設定
    uint16_t numberOfUes = 100;
    uint16_t numberOfEnbs = 64;
    double distance = 500.0; // m
    double speed = 20;       // m/s
    double simTime = 60;     // sec
    double enbTxPowerDbm = 23.0;
    double interPacketInterval = 1.0; // ms
    double interAppInterval = 0.001;  // sec
    uint16_t startUe = 0;
    uint16_t endUe = numberOfUes;
    uint16_t sdnInterval = 100; // ms
    int nMaxPackets = 1000000;
    uint16_t nPayloadBytes = 1024 + 12;
    isSdnEnabled = true;
    bool isDownlink = true;
    bool isUplink = true;
    string animFile = "animLteudpMulti.xml";
    string traceFile = "scratch/2020-12-12-22-44-15_max-speed-80.tcl";
    string fileEnableAsciiAll = "serverpgwLteudpMulti.tr";
    string fileEnablePcapAll = "pgw-hostudpLteudpMulti";

    // last_index 表示 UE 上一個連接的 ENB 編號是甚麼，目前進行的是初始化設定
    last_index = (int *)malloc(sizeof(int) * numberOfUes);
    for (int i = 0; i < numberOfUes; i++)
    {
        last_index[i] = -1;
    }

    // 產生 mobility-trace 的 mob 檔案
    AsciiTraceHelper ascii;
    MobilityHelper::EnableAsciiAll(ascii.CreateFileStream("mobility-trace-example.mob"));

    // 模擬程式預設值的設定
    Config::SetDefault("ns3::UdpClient::Interval", TimeValue(MilliSeconds(10)));
    Config::SetDefault("ns3::UdpClient::MaxPackets", UintegerValue(1000000));
    Config::SetDefault("ns3::LteHelper::UseIdealRrc", BooleanValue(true));
    Config::SetDefault("ns3::LteEnbMac::NumberOfRaPreambles", UintegerValue(20));
    Config::SetDefault("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue(320));

    // 在使用 command line 下指令時，可以用以下的命令，更換相關的變數內容
    CommandLine cmd(__FILE__);
    cmd.AddValue("simTime", "Total duration of the simulation (in seconds)", simTime);
    cmd.AddValue("distance", "Distance between eNBs [m]", distance);
    cmd.AddValue("speed", "Speed of the UE (default = 20 m/s)", speed);
    cmd.AddValue("enbTxPowerDbm", "TX power [dBm] used by HeNBs (default = 46.0)", enbTxPowerDbm);

    cmd.AddValue("numberOfUes", "Number of UEs", numberOfUes);
    cmd.AddValue("numberOfEnbs", "Number of ENBs", numberOfEnbs);
    cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
    cmd.AddValue("interAppInterval", "Inter app interval [ms])", interAppInterval);
    cmd.AddValue("interPacketInterval", "Inter packet interval [ms])", interPacketInterval);
    cmd.AddValue("sdnInterval", "Inter sdn interval [ms])", sdnInterval);
    cmd.AddValue("animFile", "File Name for Animation Output", animFile);
    cmd.AddValue("traceFile", "File Name for Trace Input", traceFile);
    cmd.AddValue("nPayloadBytes", "nPayloadBytes", nPayloadBytes);

    cmd.AddValue("startUe", "application start from", startUe);
    cmd.AddValue("endUe", "application end", endUe);

    cmd.AddValue("isSdnEnabled", "if sdn is enabled", isSdnEnabled);
    cmd.AddValue("isDownlink", "if downlink is enabled", isDownlink);
    cmd.AddValue("isUplink", "if uplink is enabled", isUplink);

    cmd.Parse(argc, argv);

    // 讀取 trace file 生成 mobilityHelper 的物件
    Ns2MobilityHelper ns2 = Ns2MobilityHelper(traceFile);

    // 相關 class 物件的生成，ltehelper 可以設定 4G 相關的參數，像是使用的 loss model、使用的 scheduler 是甚麼之類的
    lteHelper = CreateObject<LteHelper>();
    Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();

    lteHelper->SetAttribute("PathlossModel", StringValue("ns3::FriisPropagationLossModel"));
    lteHelper->SetEpcHelper(epcHelper);
    lteHelper->SetSchedulerType("ns3::RrFfMacScheduler");
    lteHelper->SetHandoverAlgorithmType("ns3::A3RsrpHandoverAlgorithm");
    lteHelper->SetHandoverAlgorithmAttribute("Hysteresis", DoubleValue(3.0));
    lteHelper->SetHandoverAlgorithmAttribute("TimeToTrigger", TimeValue(MilliSeconds(256))); 

    // 生成 PGW node
    Ptr<Node> pgw = epcHelper->GetPgwNode();

    // 建立一個 remote host
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(1);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);
    InternetStackHelper internet;
    internet.Install(remoteHostContainer);

    // 網路相關的設定
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(1500));
    p2ph.SetChannelAttribute("Delay", TimeValue(Seconds(0.010)));
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
    Ipv4AddressHelper ipv4h;
    ipv4h.SetBase("1.0.0.0", "255.0.0.0");
    internetIpIfaces = ipv4h.Assign(internetDevices);
    Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress(1);
    cout << "remoteHostAddr: " << internetIpIfaces.GetAddress(1) << endl;

    // Routing 相關的設定
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
    // interface 0 is localhost, 1 is the p2p device
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);

    NodeContainer ueNodes;
    NodeContainer enbNodes;
    enbNodes.Create(numberOfEnbs);
    ueNodes.Create(numberOfUes);
    ns2.Install(ueNodes.Begin(), ueNodes.End());


    // 設定 ENB 的位置，可以參考 SUMO 上的座標來填入，滑鼠在 SUMO 上會顯示座標
    Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator>();
    //  for (uint16_t i = 0; i < numberOfEnbs; i++)
    //    {
    //      Vector enbPosition (distance * (i + 1), distance, 0);
    //      enbPositionAlloc->Add (enbPosition);
    // //    }
    //for ( uint16_t i=0; i < numberOfEnbs; i++){
    //	    enbPositionAlloc->Add(Vector(1987+(i%8)*314, 1305+(i/8)*272, 0));
    //    }

    enbPositionAlloc->Add(Vector(577, 211, 0));  // 編號 1
    enbPositionAlloc->Add(Vector(722, 208, 0));  //2
    enbPositionAlloc->Add(Vector(880, 198, 0));  //3
    enbPositionAlloc->Add(Vector(724, 348, 0));  //4
    enbPositionAlloc->Add(Vector(1081, 195, 0)); //5
    enbPositionAlloc->Add(Vector(1181, 192, 0)); //6
    enbPositionAlloc->Add(Vector(1478, 253, 0)); //7

    enbPositionAlloc->Add(Vector(577, 356, 0));  //8
    enbPositionAlloc->Add(Vector(724, 348, 0));  //9
    enbPositionAlloc->Add(Vector(882, 343, 0));  //10
    enbPositionAlloc->Add(Vector(1081, 332, 0)); //11
    enbPositionAlloc->Add(Vector(1154, 331, 0)); //12
    enbPositionAlloc->Add(Vector(1293, 325, 0)); //13
    enbPositionAlloc->Add(Vector(1481, 318, 0)); //14
    enbPositionAlloc->Add(Vector(724, 348, 0));  //15

    enbPositionAlloc->Add(Vector(584, 515, 0));  //16
    enbPositionAlloc->Add(Vector(728, 508, 0));  //17
    enbPositionAlloc->Add(Vector(889, 501, 0));  //18
    enbPositionAlloc->Add(Vector(724, 348, 0));  //19
    enbPositionAlloc->Add(Vector(1085, 499, 0)); //20
    enbPositionAlloc->Add(Vector(1300, 499, 0)); //21
    enbPositionAlloc->Add(Vector(724, 348, 0));  //22
    enbPositionAlloc->Add(Vector(1391, 449, 0)); //23
    enbPositionAlloc->Add(Vector(1469, 556, 0)); //24

    enbPositionAlloc->Add(Vector(595, 706, 0));  //25
    enbPositionAlloc->Add(Vector(746, 701, 0));  //26
    enbPositionAlloc->Add(Vector(896, 690, 0));  //27
    enbPositionAlloc->Add(Vector(1090, 688, 0)); //28
    enbPositionAlloc->Add(Vector(1298, 681, 0)); //29
    enbPositionAlloc->Add(Vector(1423, 685, 0)); //30
    enbPositionAlloc->Add(Vector(1522, 682, 0)); //31

    enbPositionAlloc->Add(Vector(595, 846, 0));  //32
    enbPositionAlloc->Add(Vector(750, 839, 0));  //33
    enbPositionAlloc->Add(Vector(1391, 449, 0)); //34
    enbPositionAlloc->Add(Vector(907, 836, 0));  //35
    enbPositionAlloc->Add(Vector(1097, 832, 0)); //36
    enbPositionAlloc->Add(Vector(1300, 818, 0)); //37
    enbPositionAlloc->Add(Vector(1472, 867, 0)); //38

    enbPositionAlloc->Add(Vector(362, 984, 0));  //39
    enbPositionAlloc->Add(Vector(172, 987, 0));  //40
    enbPositionAlloc->Add(Vector(340, 974, 0));  //41
    enbPositionAlloc->Add(Vector(476, 971, 0));  //42
    enbPositionAlloc->Add(Vector(601, 968, 0));  //43
    enbPositionAlloc->Add(Vector(754, 962, 0));  //44
    enbPositionAlloc->Add(Vector(917, 960, 0));  //45
    enbPositionAlloc->Add(Vector(1104, 975, 0)); //46
    enbPositionAlloc->Add(Vector(1229, 987, 0)); //47
    enbPositionAlloc->Add(Vector(1349, 977, 0)); //48

    enbPositionAlloc->Add(Vector(602, 1097, 0));  //49
    enbPositionAlloc->Add(Vector(759, 1082, 0));  //50
    enbPositionAlloc->Add(Vector(923, 1085, 0));  //51
    enbPositionAlloc->Add(Vector(1103, 1075, 0)); //52
    enbPositionAlloc->Add(Vector(1352, 1111, 0)); //53

    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator(enbPositionAlloc);
    enbMobility.Install(enbNodes);

    // 在 eNB 與 ue 上 安裝 netDevice
    Config::SetDefault("ns3::LteEnbPhy::TxPower", DoubleValue(enbTxPowerDbm));
    NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice(enbNodes);
    NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice(ueNodes);

    internet.Install(ueNodes);
    // 在 UE 上指派 IP address
    ueIpIfaces = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs));

    // 初始化時，UE 會連到最接近的 ENB
    for (int i = 0; i < numberOfUes; i++)
    {
        Ptr<const MobilityModel> ueMobilityModel = ueNodes.Get(i)->GetObject<MobilityModel>();
        Vector pos_ue = ueMobilityModel->GetPosition();
        int index = -1;
        unsigned long long int min_distance = std::numeric_limits<int>::max();
        for (int j = 0; j < numberOfEnbs; j++)
        {
            Ptr<const MobilityModel> enbMobilityModel = enbNodes.Get(j)->GetObject<MobilityModel>();
            Vector pos_enb = enbMobilityModel->GetPosition();
            float current_distance = sqrt((pos_ue.x - pos_enb.x) * (pos_ue.x - pos_enb.x) + (pos_ue.y - pos_enb.y) * (pos_ue.y - pos_enb.y));
            if (current_distance < min_distance)
            {
                min_distance = current_distance;
                index = j;
                std::cout << "ue: " << i << ", enb: " << index << ", distance: " << min_distance << endl
                          << endl;
            }
        }
        lteHelper->Attach(ueLteDevs.Get(i), enbLteDevs.Get(index));
        last_index[i] = index;
    }

    NS_LOG_LOGIC("setting up applications");

    // Ptr<UniformRandomVariable> startTimeSeconds = CreateObject<UniformRandomVariable>();
    // startTimeSeconds->SetAttribute("Min", DoubleValue(1));
    // startTimeSeconds->SetAttribute("Max", DoubleValue(60));

    for (uint32_t u = 0; u < numberOfUes; ++u)
    {
        Ptr<Node> ue = ueNodes.Get(u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ue->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    // 取消註解下面的 code 來啟用 flowmonitor
    // Ptr<FlowMonitor> flowMonitor;
    // FlowMonitorHelper flowHelper;
    // flowMonitor = flowHelper.InstallAll();
    // 取消註解上面的 code 來啟用 flowmonitor

    //enter radio range support that carries data between UE and EnodeB
    Ptr<EpcTft> tft = Create<EpcTft>();
    EpcTft::PacketFilter pf;
    pf.localPortStart = 1234;
    pf.localPortEnd = 1234;
    tft->Add(pf);
    lteHelper->ActivateDedicatedEpsBearer(ueLteDevs, EpsBearer(EpsBearer::NGBR_VIDEO_TCP_DEFAULT), tft);

    // 設定 UDP client 與 server 傳送時會用到的 port
    uint16_t dlPort = 1234;
    uint16_t ulPort = 2000;
    uint16_t otherPort = 3000;
    ApplicationContainer clientApps[numberOfUes];
    ApplicationContainer serverApps[numberOfUes];
    float startTime = 0.1;

    // randomize a bit start times to avoid simulation artifacts
    // (e.g., buffer overflows due to packet transmissions happening
    // exactly at the same time)

    // 設定 UDP client 與 server 開始的時間與相關的參數
    for (uint32_t u = startUe; u < endUe; ++u)
    {
        ++ulPort;
        ++otherPort;
        
        UdpServerHelper dlUdpServerHelper(dlPort);
        UdpServerHelper ulUdpServerHelper(ulPort);
        
        if (isDownlink)
        {
            serverApps[u].Add(dlUdpServerHelper.Install(ueNodes.Get(u)));
        }
        if (isUplink)
        {
            serverApps[u].Add(ulUdpServerHelper.Install(remoteHost));
        }

        UdpClientHelper dlClient(ueIpIfaces.GetAddress(u), dlPort);
        dlClient.SetAttribute("Interval", TimeValue(MilliSeconds(interPacketInterval)));
        dlClient.SetAttribute("MaxPackets", UintegerValue(nMaxPackets));
        dlClient.SetAttribute("PacketSize", UintegerValue(nPayloadBytes));

        UdpClientHelper ulClient(remoteHostAddr, ulPort);
        ulClient.SetAttribute("Interval", TimeValue(MilliSeconds(interPacketInterval)));
        ulClient.SetAttribute("MaxPackets", UintegerValue(nMaxPackets));
        ulClient.SetAttribute("PacketSize", UintegerValue(nPayloadBytes));

        if (isDownlink)
        {
            clientApps[u].Add(dlClient.Install(remoteHost));
        }
        if (isUplink)
        {
            clientApps[u].Add(ulClient.Install(ueNodes.Get(u)));
        }
        
        serverApps[u].Start(Seconds(0));
        clientApps[u].Start(Seconds(startTime));
        startTime = startTime + interAppInterval;
    }

    // Add X2 interface
    lteHelper->AddX2Interface(enbNodes);

    lteHelper->EnablePhyTraces();
    lteHelper->EnableMacTraces();
    lteHelper->EnableRlcTraces();
    lteHelper->EnablePdcpTraces();
    Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats();
    rlcStats->SetAttribute("EpochDuration", TimeValue(Seconds(1.0)));
    Ptr<RadioBearerStatsCalculator> pdcpStats = lteHelper->GetPdcpStats();
    pdcpStats->SetAttribute("EpochDuration", TimeValue(Seconds(1.0)));

    // 這裡設定的是，如果有 Handover 相關的 event 發生時，會呼叫相對應的 function
    // connect custom trace sinks for RRC connection establishment and handover notification
    // Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/ConnectionEstablished",
    //                   MakeCallback (&NotifyConnectionEstablishedEnb));
    // Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/ConnectionEstablished",
    //                   MakeCallback (&NotifyConnectionEstablishedUe));
    // Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverStart",
    //                   MakeCallback (&NotifyHandoverStartEnb));
    // Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk",
    //                   MakeCallback (&NotifyHandoverEndOkEnb));
    Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverStart",
                    MakeCallback (&NotifyHandoverStartUe));
    Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk",
                    MakeCallback (&NotifyHandoverEndOkUe));
    // Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
    //                 MakeCallback (&CourseChange));
    // Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::OnOffApplication/TxWithAddresses",
    //                 MakeCallback (&TxTrace));
    // Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::PacketSink/RxWithAddresses",
    //                 MakeCallback (&RxTrace));

    // 匯出封包傳送的動畫檔，可以用 netanim 這個程式顯示結果
    pAnim = new AnimationInterface(animFile.c_str());
    pAnim->SetMaxPktsPerTraceFile(99999999999999);

    for (int i = 0; i * sdnInterval < simTime * 1000; i++)
    {
        Simulator::Schedule(MilliSeconds(i * sdnInterval), manualAttach, &ueNodes, &ueLteDevs, &enbNodes, &enbLteDevs, numberOfUes, numberOfEnbs);
    }
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();

    // 取消下面的註解來 enable flowmonitor
    //   flowMonitor->SerializeToXmlFile("flowMonitor4.xml", true, true);

    Simulator::Destroy();
    delete pAnim;

    // 結算模擬總共花費的時間並印出來
    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    cout << "time spent: " << duration / 3600 << ":" << duration / 60 % 60 << ":" << duration % 60 << endl;

    return 0;
}

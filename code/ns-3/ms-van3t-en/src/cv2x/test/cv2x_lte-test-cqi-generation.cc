/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Piotr Gawlowicz
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Piotr Gawlowicz <gawlowicz.p@gmail.com>
 *
 */

#include <ns3/simulator.h>
#include <ns3/log.h>
#include <ns3/callback.h>
#include <ns3/config.h>
#include <ns3/string.h>
#include <ns3/double.h>
#include <ns3/enum.h>
#include <ns3/boolean.h>
#include <ns3/pointer.h>
#include "ns3/cv2x_ff-mac-scheduler.h"
#include "ns3/mobility-helper.h"
#include "ns3/cv2x_lte-helper.h"

#include "cv2x_lte-ffr-simple.h"
#include "ns3/cv2x_lte-rrc-sap.h"

#include "cv2x_lte-test-cqi-generation.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("cv2x_LteCqiGenerationTest");

void
LteTestDlSchedulingCallback (cv2x_LteCqiGenerationTestCase *testcase, std::string path, cv2x_DlSchedulingCallbackInfo dlInfo)
{
  testcase->DlScheduling (dlInfo);
}

void
LteTestUlSchedulingCallback (cv2x_LteCqiGenerationTestCase *testcase, std::string path,
                             uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                             uint8_t mcs, uint16_t sizeTb, uint8_t ccId)
{
  testcase->UlScheduling (frameNo, subframeNo, rnti, mcs, sizeTb);
}

void
LteTestDlSchedulingCallback2 (cv2x_LteCqiGenerationDlPowerControlTestCase *testcase, std::string path,
		                      cv2x_DlSchedulingCallbackInfo dlInfo)
{
  testcase->DlScheduling (dlInfo);
}

void
LteTestUlSchedulingCallback2 (cv2x_LteCqiGenerationDlPowerControlTestCase *testcase, std::string path,
                              uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                              uint8_t mcs, uint16_t sizeTb, uint8_t componentCarrierId)
{
  testcase->UlScheduling (frameNo, subframeNo, rnti, mcs, sizeTb);
}


/**
 * TestSuite
 */

cv2x_LteCqiGenerationTestSuite::cv2x_LteCqiGenerationTestSuite ()
  : TestSuite ("lte-cqi-generation", SYSTEM)
{
//  LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_DEBUG);
//  LogComponentEnable ("LteCqiGenerationTest", logLevel);
  NS_LOG_INFO ("Creating cv2x_LteCqiGenerationTestSuite");

  AddTestCase (new cv2x_LteCqiGenerationTestCase ("UsePdcchForCqiGeneration", false, 4, 2), TestCase::QUICK);
  AddTestCase (new cv2x_LteCqiGenerationTestCase ("UsePdschForCqiGeneration", true, 28, 2), TestCase::QUICK);

  AddTestCase (new cv2x_LteCqiGenerationDlPowerControlTestCase ("CqiGenerationWithDlPowerControl",
                                                           cv2x_LteRrcSap::PdschConfigDedicated::dB0, cv2x_LteRrcSap::PdschConfigDedicated::dB0, 4, 2), TestCase::QUICK);
  AddTestCase (new cv2x_LteCqiGenerationDlPowerControlTestCase ("CqiGenerationWithDlPowerControl",
                                                           cv2x_LteRrcSap::PdschConfigDedicated::dB0, cv2x_LteRrcSap::PdschConfigDedicated::dB_3, 8, 2), TestCase::QUICK);
  AddTestCase (new cv2x_LteCqiGenerationDlPowerControlTestCase ("CqiGenerationWithDlPowerControl",
                                                           cv2x_LteRrcSap::PdschConfigDedicated::dB0, cv2x_LteRrcSap::PdschConfigDedicated::dB_6, 10, 2), TestCase::QUICK);
  AddTestCase (new cv2x_LteCqiGenerationDlPowerControlTestCase ("CqiGenerationWithDlPowerControl",
                                                           cv2x_LteRrcSap::PdschConfigDedicated::dB1, cv2x_LteRrcSap::PdschConfigDedicated::dB_6, 12, 2), TestCase::QUICK);
  AddTestCase (new cv2x_LteCqiGenerationDlPowerControlTestCase ("CqiGenerationWithDlPowerControl",
                                                           cv2x_LteRrcSap::PdschConfigDedicated::dB2, cv2x_LteRrcSap::PdschConfigDedicated::dB_6, 14, 2), TestCase::QUICK);
  AddTestCase (new cv2x_LteCqiGenerationDlPowerControlTestCase ("CqiGenerationWithDlPowerControl",
                                                           cv2x_LteRrcSap::PdschConfigDedicated::dB3, cv2x_LteRrcSap::PdschConfigDedicated::dB_6, 14, 2), TestCase::QUICK);
  AddTestCase (new cv2x_LteCqiGenerationDlPowerControlTestCase ("CqiGenerationWithDlPowerControl",
                                                           cv2x_LteRrcSap::PdschConfigDedicated::dB3, cv2x_LteRrcSap::PdschConfigDedicated::dB0, 8, 2), TestCase::QUICK);
}

static cv2x_LteCqiGenerationTestSuite lteCqiGenerationTestSuite;


cv2x_LteCqiGenerationTestCase::cv2x_LteCqiGenerationTestCase (std::string name, bool usePdcchForCqiGeneration,
                                                    uint16_t dlMcs, uint16_t ulMcs)
  : TestCase ("Downlink Power Control: " + name),
    m_dlMcs (dlMcs),
    m_ulMcs (ulMcs)
{
  m_usePdschForCqiGeneration = usePdcchForCqiGeneration;
  NS_LOG_INFO ("Creating cv2x_LteCqiGenerationTestCase");
}

cv2x_LteCqiGenerationTestCase::~cv2x_LteCqiGenerationTestCase ()
{
}

void
cv2x_LteCqiGenerationTestCase::DlScheduling (cv2x_DlSchedulingCallbackInfo dlInfo)
{
  // need to allow for RRC connection establishment + CQI feedback reception
  if (Simulator::Now () > MilliSeconds (35))
    {
//	  NS_LOG_UNCOND("DL MSC: " << (uint32_t)mcsTb1 << " expected DL MCS: " << (uint32_t)m_dlMcs);
      NS_TEST_ASSERT_MSG_EQ ((uint32_t)dlInfo.mcsTb1, (uint32_t)m_dlMcs, "Wrong DL MCS ");
    }
}

void
cv2x_LteCqiGenerationTestCase::UlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                                        uint8_t mcs, uint16_t sizeTb)
{
  // need to allow for RRC connection establishment + SRS transmission
  if (Simulator::Now () > MilliSeconds (50))
    {
//	  NS_LOG_UNCOND("UL MSC: " << (uint32_t)mcs << " expected UL MCS: " << (uint32_t)m_ulMcs);
      NS_TEST_ASSERT_MSG_EQ ((uint32_t)mcs, (uint32_t)m_ulMcs, "Wrong UL MCS");
    }
}

void
cv2x_LteCqiGenerationTestCase::DoRun (void)
{
  NS_LOG_DEBUG ("cv2x_LteCqiGenerationTestCase");

  Config::Reset ();
  Config::SetDefault ("ns3::cv2x_LteHelper::UseIdealRrc", BooleanValue (true));
  Config::SetDefault ("ns3::cv2x_LteHelper::UsePdschForCqiGeneration", BooleanValue (m_usePdschForCqiGeneration));

  Config::SetDefault ("ns3::cv2x_LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (true));
  Config::SetDefault ("ns3::cv2x_LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (true));

  Ptr<cv2x_LteHelper> lteHelper = CreateObject<cv2x_LteHelper> ();

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes1;
  NodeContainer ueNodes2;
  enbNodes.Create (2);
  ueNodes1.Create (1);
  ueNodes2.Create (1);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes1, ueNodes2);

  /*
   * The topology is the following:
   *
   *  eNB1                        UE1 UE2                        eNB2
   *    |                            |                            |
   *    x -------------------------- x -------------------------- x
   *                  500 m                       500 m
   *
   */

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));   // eNB1
  positionAlloc->Add (Vector (1000, 0.0, 0.0)); // eNB2
  positionAlloc->Add (Vector (500.0, 0.0, 0.0));  // UE1
  positionAlloc->Add (Vector (500, 0.0, 0.0));  // UE2
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs1;
  NetDeviceContainer ueDevs2;
  lteHelper->SetSchedulerType ("ns3::cv2x_PfFfMacScheduler");
  lteHelper->SetSchedulerAttribute ("UlCqiFilter", EnumValue (cv2x_FfMacScheduler::PUSCH_UL_CQI));

  lteHelper->SetFfrAlgorithmType ("ns3::cv2x_LteFrHardAlgorithm");

  lteHelper->SetFfrAlgorithmAttribute ("DlSubBandOffset", UintegerValue (0));
  lteHelper->SetFfrAlgorithmAttribute ("DlSubBandwidth", UintegerValue (12));
  lteHelper->SetFfrAlgorithmAttribute ("UlSubBandOffset", UintegerValue (0));
  lteHelper->SetFfrAlgorithmAttribute ("UlSubBandwidth", UintegerValue (25));
  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (0)));

  lteHelper->SetFfrAlgorithmAttribute ("DlSubBandOffset", UintegerValue (12));
  lteHelper->SetFfrAlgorithmAttribute ("DlSubBandwidth", UintegerValue (12));
  lteHelper->SetFfrAlgorithmAttribute ("UlSubBandOffset", UintegerValue (0));
  lteHelper->SetFfrAlgorithmAttribute ("UlSubBandwidth", UintegerValue (25));
  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (1)));

  ueDevs1 = lteHelper->InstallUeDevice (ueNodes1);
  ueDevs2 = lteHelper->InstallUeDevice (ueNodes2);

  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs1, enbDevs.Get (0));
  lteHelper->Attach (ueDevs2, enbDevs.Get (1));

  // Activate an EPS bearer
  enum cv2x_EpsBearer::Qci q = cv2x_EpsBearer::GBR_CONV_VOICE;
  cv2x_EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs1, bearer);
  lteHelper->ActivateDataRadioBearer (ueDevs2, bearer);

  Config::Connect ("/NodeList/0/DeviceList/0/cv2x_ComponentCarrierMap/*/cv2x_LteEnbMac/DlScheduling",
                   MakeBoundCallback (&LteTestDlSchedulingCallback, this));

  Config::Connect ("/NodeList/0/DeviceList/0/cv2x_ComponentCarrierMap/*/cv2x_LteEnbMac/UlScheduling",
                   MakeBoundCallback (&LteTestUlSchedulingCallback, this));

  Config::Connect ("/NodeList/1/DeviceList/0/cv2x_ComponentCarrierMap/*/cv2x_LteEnbMac/DlScheduling",
                   MakeBoundCallback (&LteTestDlSchedulingCallback, this));

  Config::Connect ("/NodeList/1/DeviceList/0/cv2x_ComponentCarrierMap/*/cv2x_LteEnbMac/UlScheduling",
                   MakeBoundCallback (&LteTestUlSchedulingCallback, this));

  Simulator::Stop (Seconds (1.100));
  Simulator::Run ();

  Simulator::Destroy ();
}

cv2x_LteCqiGenerationDlPowerControlTestCase::cv2x_LteCqiGenerationDlPowerControlTestCase (std::string name,
                                                                                uint8_t cell0Pa, uint8_t cell1Pa, uint16_t dlMcs, uint16_t ulMcs)
  : TestCase ("Downlink Power Control: " + name),
    m_cell0Pa (cell0Pa),
    m_cell1Pa (cell1Pa),
    m_dlMcs (dlMcs),
    m_ulMcs (ulMcs)
{
  NS_LOG_INFO ("Creating cv2x_LteCqiGenerationTestCase");
}

cv2x_LteCqiGenerationDlPowerControlTestCase::~cv2x_LteCqiGenerationDlPowerControlTestCase ()
{
}

void
cv2x_LteCqiGenerationDlPowerControlTestCase::DlScheduling (cv2x_DlSchedulingCallbackInfo dlInfo)
{
  // need to allow for RRC connection establishment + CQI feedback reception
  if (Simulator::Now () > MilliSeconds (500))
    {
//	  NS_LOG_UNCOND("DL MSC: " << (uint32_t)mcsTb1 << " expected DL MCS: " << (uint32_t)m_dlMcs);
      NS_TEST_ASSERT_MSG_EQ ((uint32_t)dlInfo.mcsTb1, (uint32_t)m_dlMcs, "Wrong DL MCS ");
    }
}

void
cv2x_LteCqiGenerationDlPowerControlTestCase::UlScheduling (uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                                                      uint8_t mcs, uint16_t sizeTb)
{
  // need to allow for RRC connection establishment + SRS transmission
  if (Simulator::Now () > MilliSeconds (500))
    {
//	  NS_LOG_UNCOND("UL MSC: " << (uint32_t)mcs << " expected UL MCS: " << (uint32_t)m_ulMcs);
      NS_TEST_ASSERT_MSG_EQ ((uint32_t)mcs, (uint32_t)m_ulMcs, "Wrong UL MCS");
    }
}

void
cv2x_LteCqiGenerationDlPowerControlTestCase::DoRun (void)
{
  NS_LOG_DEBUG ("cv2x_LteCqiGenerationTestCase");

  Config::Reset ();
  Config::SetDefault ("ns3::cv2x_LteHelper::UseIdealRrc", BooleanValue (true));
  Config::SetDefault ("ns3::cv2x_LteHelper::UsePdschForCqiGeneration", BooleanValue (true));

  Config::SetDefault ("ns3::cv2x_LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (true));
  Config::SetDefault ("ns3::cv2x_LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (true));

  Ptr<cv2x_LteHelper> lteHelper = CreateObject<cv2x_LteHelper> ();
  lteHelper->SetFfrAlgorithmType ("ns3::cv2x_LteFfrSimple");

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes1;
  NodeContainer ueNodes2;
  enbNodes.Create (2);
  ueNodes1.Create (1);
  ueNodes2.Create (2);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes1, ueNodes2);

  /*
   * The topology is the following:
   *
   *  eNB1                        UE1 UE2                        eNB2 UE3
   *    |                            |                            |    |
   *    x -------------------------- x -------------------------- x----x
   *                  500 m                       500 m             50m
   *
   * see https://www.nsnam.org/bugzilla/show_bug.cgi?id=2048#c4 for why we need UE3
   */

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));   // eNB1
  positionAlloc->Add (Vector (1000, 0.0, 0.0)); // eNB2
  positionAlloc->Add (Vector (500.0, 0.0, 0.0));  // UE1
  positionAlloc->Add (Vector (500, 0.0, 0.0));  // UE2
  positionAlloc->Add (Vector (1050, 0.0, 0.0));  // UE3
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs1;
  NetDeviceContainer ueDevs2;
  lteHelper->SetSchedulerType ("ns3::cv2x_PfFfMacScheduler");
  lteHelper->SetSchedulerAttribute ("UlCqiFilter", EnumValue (cv2x_FfMacScheduler::PUSCH_UL_CQI));
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs1 = lteHelper->InstallUeDevice (ueNodes1);
  ueDevs2 = lteHelper->InstallUeDevice (ueNodes2);

  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs1, enbDevs.Get (0));
  lteHelper->Attach (ueDevs2, enbDevs.Get (1));

  // Activate an EPS bearer
  enum cv2x_EpsBearer::Qci q = cv2x_EpsBearer::GBR_CONV_VOICE;
  cv2x_EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs1, bearer);
  lteHelper->ActivateDataRadioBearer (ueDevs2, bearer);

  PointerValue tmp;
  enbDevs.Get (0)->GetAttribute ("cv2x_LteFfrAlgorithm", tmp);
  Ptr<cv2x_LteFfrSimple> simpleFfrAlgorithmEnb0 = DynamicCast<cv2x_LteFfrSimple>(tmp.GetObject ());
  simpleFfrAlgorithmEnb0->ChangePdschConfigDedicated (true);

  cv2x_LteRrcSap::PdschConfigDedicated pdschConfigDedicatedEnb0;
  pdschConfigDedicatedEnb0.pa = m_cell0Pa;
  simpleFfrAlgorithmEnb0->SetPdschConfigDedicated (pdschConfigDedicatedEnb0);

  enbDevs.Get (1)->GetAttribute ("cv2x_LteFfrAlgorithm", tmp);
  Ptr<cv2x_LteFfrSimple> simpleFfrAlgorithmEnb1 = DynamicCast<cv2x_LteFfrSimple>(tmp.GetObject ());
  simpleFfrAlgorithmEnb1->ChangePdschConfigDedicated (true);

  cv2x_LteRrcSap::PdschConfigDedicated pdschConfigDedicatedEnb1;
  pdschConfigDedicatedEnb1.pa = m_cell1Pa;
  simpleFfrAlgorithmEnb1->SetPdschConfigDedicated (pdschConfigDedicatedEnb1);


  Config::Connect ("/NodeList/0/DeviceList/0/cv2x_ComponentCarrierMap/*/cv2x_LteEnbMac/DlScheduling",
                   MakeBoundCallback (&LteTestDlSchedulingCallback2, this));

  Config::Connect ("/NodeList/0/DeviceList/0/cv2x_ComponentCarrierMap/*/cv2x_LteEnbMac/UlScheduling",
                   MakeBoundCallback (&LteTestUlSchedulingCallback2, this));

  Simulator::Stop (Seconds (1.100));
  Simulator::Run ();

  Simulator::Destroy ();
}


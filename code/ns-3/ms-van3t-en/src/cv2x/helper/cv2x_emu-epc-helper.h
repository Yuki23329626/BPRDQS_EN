/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2013 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef CV2X_EMU_EPC_HELPER_H
#define CV2X_EMU_EPC_HELPER_H

#include <ns3/object.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/ipv6-address-helper.h>
#include <ns3/data-rate.h>
#include <ns3/cv2x_epc-tft.h>
#include <ns3/cv2x_eps-bearer.h>
#include <ns3/cv2x_epc-helper.h>

namespace ns3 {

class Node;
class NetDevice;
class VirtualNetDevice;
class cv2x_EpcSgwPgwApplication;
class cv2x_EpcX2;
class cv2x_EpcMme;

/**
 * \ingroup lte
 *
 * \brief Create an EPC network using EmuFdNetDevice 
 *
 * This Helper will create an EPC network topology comprising of a
 * single node that implements both the SGW and PGW functionality, and
 * an MME node. The S1-U, X2-U and X2-C interfaces are realized using
 * EmuFdNetDevice; in particular, one device is used to send all the
 * traffic related to these interfaces. 
 */
class cv2x_EmuEpcHelper : public cv2x_EpcHelper
{
public:
  
  /** 
   * Constructor
   */
  cv2x_EmuEpcHelper ();

  /** 
   * Destructor
   */  
  virtual ~cv2x_EmuEpcHelper ();
  
  // inherited from Object
  /**
   *  Register this type.
   *  \return The object TypeId.
   */
  static TypeId GetTypeId (void);
  TypeId GetInstanceTypeId () const;
  virtual void DoInitialize ();
  virtual void DoDispose ();

  // inherited from cv2x_EpcHelper
  virtual void AddEnb (Ptr<Node> enbNode, Ptr<NetDevice> lteEnbNetDevice, uint16_t cellId);
  virtual void AddUe (Ptr<NetDevice> ueLteDevice, uint64_t imsi);
  virtual void AddX2Interface (Ptr<Node> enbNode1, Ptr<Node> enbNode2);
  virtual uint8_t ActivateEpsBearer (Ptr<NetDevice> ueLteDevice, uint64_t imsi, Ptr<cv2x_EpcTft> tft, cv2x_EpsBearer bearer);
  virtual void ActivateSidelinkBearer (Ptr<NetDevice> ueDevice, Ptr<cv2x_LteSlTft> tft);
  virtual void DeactivateSidelinkBearer (Ptr<NetDevice> ueDevice, Ptr<cv2x_LteSlTft> tft);
  virtual void StartDiscovery (Ptr<NetDevice> ueDevice, std::list<uint32_t> apps, bool rxtx);
  virtual void StopDiscovery (Ptr<NetDevice> ueDevice, std::list<uint32_t> apps, bool rxtx);
  virtual Ptr<Node> GetPgwNode ();
  virtual Ipv4InterfaceContainer AssignUeIpv4Address (NetDeviceContainer ueDevices);
  Ipv6InterfaceContainer AssignUeIpv6Address (NetDeviceContainer ueDevices);
  virtual Ipv4Address GetUeDefaultGatewayAddress ();
  Ipv6Address GetUeDefaultGatewayAddress6 ();


private:

  /** 
   * helper to assign IPv4 addresses to UE devices as well as to the TUN device of the SGW/PGW
   */
  Ipv4AddressHelper m_uePgwAddressHelper; 

  /** 
   * helper to assign IPv6 addresses to UE devices as well as to the TUN device of the SGW/PGW
   */
  Ipv6AddressHelper m_uePgwAddressHelper6; 

  /**
   * SGW-PGW network element
   */  
  Ptr<Node> m_sgwPgw; 

  /**
   * SGW-PGW application
   */
  Ptr<cv2x_EpcSgwPgwApplication> m_sgwPgwApp;

  /**
   * TUN device containing IPv4 address and implementing tunneling of user data over GTP-U/UDP/IP
   */
  Ptr<VirtualNetDevice> m_tunDevice;

  /**
   * MME network element
   */
  Ptr<cv2x_EpcMme> m_mme;

  /**
   * Helper which provides implementation of core network. Initially empty
   * (i.e., LTE-only simulation without any core network) and then might be
   * set using SetEpcHelper().
   */
  Ptr<cv2x_EpcHelper> m_epcHelper;

  /** 
   * helper to assign addresses to S1-U NetDevices 
   */
  Ipv4AddressHelper m_epcIpv4AddressHelper; 

  /**
   * UDP port where the GTP-U Socket is bound, fixed by the standard as 2152
   */
  uint16_t m_gtpuUdpPort;

  /**
   * Map storing for each IMSI the corresponding eNB NetDevice
   * 
   */
  std::map<uint64_t, Ptr<NetDevice> > m_imsiEnbDeviceMap;

  /**
   * Container for Ipv4Interfaces of the SGW/PGW
   */
  Ipv4InterfaceContainer m_sgwIpIfaces; 

  /**
   * The name of the device used for the S1-U interface of the SGW
   */
  std::string m_sgwDeviceName;

  /**
   * The name of the device used for the S1-U interface of the eNB
   */
  std::string m_enbDeviceName;

  /**
   * MAC address used for the SGW
   */
  std::string m_sgwMacAddress;

  /**
   * First 5 bytes of the Enb MAC address base
   */
  std::string m_enbMacAddressBase;
  
};




} // namespace ns3

#endif // CV2X_EMU_EPC_HELPER_H

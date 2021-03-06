/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Danilo Abrignani
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
 * Author: Danilo Abrignani <danilo.abrignani@unibo.it>
 *
 */

#ifndef CV2X_LTE_ENB_COMPONENT_CARRIER_MANAGER_H
#define CV2X_LTE_ENB_COMPONENT_CARRIER_MANAGER_H

#include <ns3/object.h>
#include <ns3/cv2x_lte-enb-rrc.h>
#include <ns3/cv2x_lte-rrc-sap.h>
#include <ns3/cv2x_lte-ccm-rrc-sap.h>
#include <ns3/cv2x_lte-mac-sap.h>
#include <ns3/cv2x_lte-enb-cmac-sap.h>
#include <ns3/cv2x_lte-ccm-mac-sap.h>
#include <map>
#include <vector>

namespace ns3 {


class cv2x_LteCcmRrcSapUser;
class cv2x_LteCcmRrcSapProvider;
class cv2x_LteMacSapUser;
class cv2x_LteMacSapProvider;
class cv2x_LteEnbCmacSapProvider;
class cv2x_LteCcmMacSapProvider;

/**
 * \brief The class implements Component Carrier Manager (CCM) that operates
 * using the Component Carrier Manager SAP interfaces.
 *
 * CCM receives measurement reports from an eNode RRC instance and is forwarding
 * calls from RLC to MAC layer, and from MAC to RLC.
 *
 * This class is an abstract class intended to be inherited by subclasses that
 * will implement its virtual methods. The subclasses are compatible with the
 * cv2x_LteEnbNetDevice class, and are accessible using namespace-based access
 * through ns-3 Config subsystem, and can be installed and configured by
 * cv2x_LteHelper class.
 *
 * The communication with the eNodeB RRC instance is done through the *Component
 * Carrier Manager SAP* interface. The cv2x_LteEnbComponentCarrierManager instance
 * corresponds to the "provider" part of this interface, while the eNodeB RRC
 * instance takes the role of the "user" part. The following code skeleton
 * establishes the connection between both instances:
 *
 * Ptr<cv2x_LteEnbRrc> rrc = ...;
 * Ptr<LteComponentCarrierManager> ccmEnb = ...;
 * rrc->SetLteCcmRrcSapProvider (ccmEnb->GetLteCcmRrcSapProvider ());
 * ccmEnb->SetLteCcmRrcSapUser (rrc->GetLteCcmRrcSapUser ())
 *
 * Similarly, cv2x_LteEnbComponentCarrierManager instance communicates with MAC, and
 * it takes the role of the "user".
 *
 * However, user rarely needs to use the above code, since it has already been
 * taken care by cv2x_LteHelper::InstallEnbDevice.
 *
 * \sa cv2x_LteCcmRrcSapUser, cv2x_LteCcmRrcSapProvider, cv2x_LteCcmMacSapUser, cv2x_LteCcmMacSapProvider
 */

class cv2x_LteEnbComponentCarrierManager : public Object
{
public:

  cv2x_LteEnbComponentCarrierManager ();
  virtual ~cv2x_LteEnbComponentCarrierManager ();
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  /**
   * \brief Set the "user" part of the cv2x_ComponentCarrier Management SAP interface that
   *        this cv2x_ComponentCarrier algorithm instance will interact with.
   * \param s a reference to the "user" part of the interface, typically a
   *          member of an cv2x_LteEnbRrc instance
   */
  virtual void SetLteCcmRrcSapUser (cv2x_LteCcmRrcSapUser* s);

  /**
   * \brief Export the "provider" part of the cv2x_ComponentCarrier Management SAP interface.
   * \return the reference to the "provider" part of the interface, typically to
   *         be kept by an LteEnbRlc instance
   */
  virtual cv2x_LteCcmRrcSapProvider* GetLteCcmRrcSapProvider ();

  /**
   * \brief This function returns a pointer to the cv2x_LteCcmMacSapUser interface, which
   * is used by MAC to communicate to CCM when e.g. UL buffer status report is
   * received, or to notify CCM about PRB occupancy, and similar. Functions that are
   * specific for the communication between MAC and CCM.
   *
   * \returns cv2x_LteCcmMacSapUser* 
   */
  virtual cv2x_LteCcmMacSapUser* GetLteCcmMacSapUser ();

  /**
   * \brief Returns the pointer to the cv2x_LteMacSapProvider interface, the
   * provider of MAC, which is this new architecture served by
   * cv2x_LteEnbComponentCarrierManager object which will behave as a
   * proxy, and will forward calls between to MAC objects of
   * component carriers based on the logic implemented in the
   * specific component carrier manager.
   *
   * \returns cv2x_LteMacSapProvider* 
   */
  virtual cv2x_LteMacSapProvider* GetLteMacSapProvider ();

  /**
   * \brief Set cv2x_LteMacSapProvider interface for the MAC object of
   * the specified component carrier.
   *
   * \param componentCarrierId component carrier ID
   * \param sap the MAC SAP provider
   * \returns true if successful
   */
  virtual bool SetMacSapProvider (uint8_t componentCarrierId, cv2x_LteMacSapProvider* sap);

  /**
   * \brief Set cv2x_LteCcmMacSapProvider interface for the MAC object of
   * the specified component carrier. Through this interface CCM communicates with
   * MAC, e.g. it notifies MAC of the specific carrier when to scheduler UL BSR.
   *
   * \param componentCarrierId component carrier ID
   * \param sap the MAC SAP provider
   * \returns true if successful
   */
  virtual bool SetCcmMacSapProviders (uint8_t componentCarrierId, cv2x_LteCcmMacSapProvider* sap);

  /**
   * \brief Sets the total number of component carriers.
   * \param noOfComponentCarriers number of component carriers
   */
  virtual void SetNumberOfComponentCarriers (uint16_t noOfComponentCarriers);

  /**
   * \brief Sets a pointer to eNodeB RRC instance
   * \param rrc the RRC
   */
  virtual void SetRrc (const Ptr<cv2x_LteEnbRrc> rrc);

protected:

  // inherited from Object
  virtual void DoDispose ();

  /**
   * \brief Implementation of ReportUeMeas.
   * \param rnti Radio Network Temporary Identity, an integer identifying the UE
   *             where the report originates from
   * \param measResults a single report of one measurement identity
   */
  virtual void DoReportUeMeas (uint16_t rnti, cv2x_LteRrcSap::MeasResults measResults) = 0;

  std::map <uint16_t, std::map<uint8_t, cv2x_LteMacSapUser*> > m_ueAttached;//!< The map that contains the rnti, lcid, SAP of the RLC instance
  std::map <uint16_t, std::map<uint8_t, cv2x_LteEnbCmacSapProvider::LcInfo> > m_rlcLcInstantiated; //!< This map contains logical channel configuration per flow Id (rnti, lcid).
  std::map <uint16_t, uint8_t> m_enabledComponentCarrier; //!< This map tells for each RNTI the number of enabled component carriers.
  std::map <uint16_t, uint8_t> m_ueState; //!< Map of RRC states per UE (rnti, state), e.g. CONNECTED_NORMALLY
  uint16_t m_noOfComponentCarriers; //!< The number component of carriers that are supported by this eNb.
  // pointer to RRC object for direct function calls, e.g. when CCM needs to obtain
  // a pointer to RLC object of a specific flow
  Ptr<cv2x_LteEnbRrc> m_rrc; //!< A pointer to the RRC instance of this eNb.

  /*
   * This interface is used to receive API calls from the RLC instance that through
   * cv2x_LteMacSapProvider interface. The component carrier manager acts a proxy. This means that all
   * RLC instances will see as in previous architecture the cv2x_LteMacSapProvider interface, but the
   * actual provider in new architecture will be some of child classes of cv2x_LteEnbComponentCarrierManager.
   * So, cv2x_LteEnbComponentCarrierManager class will receive function calls that are meant for MAC, and
   * will forward them to the MAC of the component carriers based on the logic implemented
   * in LteComponentCarrierManager. This attribute will be initialized by using class that implements
   * cv2x_LteMacSapProvider interface and class that implements cv2x_LteEnbComponentCarrierManager base class
   * e.g.:cv2x_EnbMacMemberLteMacSapProvider <cv2x_LteEnbComponentCarrierManagerImpl>
   */
  cv2x_LteMacSapProvider* m_macSapProvider;//!< A pointer to main SAP interface of the MAC instance, which is in this case handled by CCM.
  // This map is initialized in cv2x_LteHelper when the Component Carrier Manager is initialized, contains
  // component carrier id and a pointer to the corresponding cv2x_LteMacSapProvider interface of the
  // MAC instance
  std::map <uint8_t, cv2x_LteMacSapProvider*> m_macSapProvidersMap; //!< A map of pointers to real SAP interfaces of MAC instances.
  // This map contains pointers to cv2x_LteCcmMacSapProvider interfaces of the
  // MAC instances. cv2x_LteCcmMacSapProvider is new interface added for the
  // communication between component carrier manager and MAC instance,
  // to allow CCM to control UL buffer status reporting, and forwarding to
  // schedulers. Before adding carrier aggregation to LTE module, MAC was
  // directly forwarding UL buffer status report to scheduler. Now is this
  // done through CCM, which decides to which MAC scheduler to forward UL BSR.
  std::map< uint8_t, cv2x_LteCcmMacSapProvider*> m_ccmMacSapProviderMap; //!< A map of pointers to the SAP interfaces of CCM instance that provides the  CCM specific functionalities to MAC, i.e. ReportMacCeToScheduler.
  cv2x_LteCcmMacSapUser* m_ccmMacSapUser; //!< cv2x_LteCcmMacSapUser is extended version of cv2x_LteMacSapUser interface. Contains functions that allow reporting of UL BSR from MAC to CCM.
  cv2x_LteCcmRrcSapUser* m_ccmRrcSapUser; //!< A pointer to SAP interface of RRC instance, i.e. to configure measurements reporting for CCM.
  cv2x_LteCcmRrcSapProvider* m_ccmRrcSapProvider; //!< A pointer to the SAP interface of the CCM instance to receive API calls from the eNodeB RRC instance.

}; // end of class cv2x_LteEnbComponentCarrierManager


} // end of namespace ns3


#endif /* CV2X_LTE_ENB_COMPONENT_CARRIER_MANAGER_H */

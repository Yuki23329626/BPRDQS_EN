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

#ifndef CV2X_LTE_UE_COMPONENT_CARRIER_MANAGER_H
#define CV2X_LTE_UE_COMPONENT_CARRIER_MANAGER_H

#include <ns3/object.h>
#include <ns3/cv2x_lte-rrc-sap.h>
#include <ns3/cv2x_lte-ue-ccm-rrc-sap.h>
#include <ns3/cv2x_lte-mac-sap.h>
#include <map>
#include <vector>

namespace ns3 {


class cv2x_LteUeCcmRrcSapUser;
class cv2x_LteUeCcmRrcSapProvider;

class cv2x_LteMacSapUser;
class cv2x_LteMacSapProvider;


/**
 * \brief The abstract base class of a Component Carrier Manager* for UE 
  that operates using the component carrier manager SAP interface.
 *
 */
class cv2x_LteUeComponentCarrierManager : public Object
{

public:
  cv2x_LteUeComponentCarrierManager ();
  virtual ~cv2x_LteUeComponentCarrierManager ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  /**
   * \brief Set the "user" part of the cv2x_ComponentCarrier Management SAP interface
   * that this UE component carrier manager will interact with.
   * \param s a reference to the "user" part of the interface, typically a
   *          member of an cv2x_LteEnbRrc instance
   */
  virtual void SetLteCcmRrcSapUser (cv2x_LteUeCcmRrcSapUser* s) = 0;

  /**
   * \brief Exports the "provider" part of the cv2x_ComponentCarrier Management SAP interface.
   * \return the reference to the "provider" part of the interface, typically to
   *         be kept by an cv2x_LteUeRrc instance
   */
  virtual cv2x_LteUeCcmRrcSapProvider* GetLteCcmRrcSapProvider () = 0;
  
  /**
   * \brief Returns the MAC sap provider interface that if forwarding calls to the
   * instance of the cv2x_LteUeComponentCarrierManager.
   * \return the reference to the "provider" part of the interface
   */
  virtual cv2x_LteMacSapProvider* GetLteMacSapProvider () = 0;

  /**
   * \brief Sets a pointer to SAP interface of MAC instance for the specified carrier.
   * \param componentCarrierId the component carrier id
   * \param sap the pointer to the sap interface
   * \return whether the settings of the sap provider was successful
   */
  bool SetComponentCarrierMacSapProviders (uint8_t componentCarrierId, cv2x_LteMacSapProvider* sap);

  /**
   * \brief Sets number of component carriers that are supported by this UE.
   * \param noOfComponentCarriers numbr of component carriers
   */
  void SetNumberOfComponentCarriers (uint8_t noOfComponentCarriers);

protected:

  // inherited from Object
  virtual void DoDispose ();

  std::map<uint8_t, cv2x_LteMacSapUser*> m_lcAttached; //!< Map of pointers to SAP interfaces of the RLC instance of the flows of this UE.
  std::map<uint8_t, std::map<uint8_t, cv2x_LteMacSapProvider*> > m_componentCarrierLcMap; //!< Flow configuration per flow Id of this UE.
  uint16_t m_noOfComponentCarriers; //!<// The number of component carriers that this UE can support.
  uint16_t m_noOfComponentCarriersEnabled; //!< The number of enabled component carriers that are enabled for this UE.
  std::map <uint8_t, cv2x_LteMacSapProvider*> m_macSapProvidersMap; //!< Map of pointers to SAP to interfaces of the MAC instance if the flows of this UE.

}; // end of class cv2x_LteUeComponentCarrierManager


} // end of namespace ns3


#endif /* CV2X_LTE_UE_COMPONENT_CARRIER_MANAGER_H */

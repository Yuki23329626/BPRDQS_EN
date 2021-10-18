/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>,
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Dizhi Zhou <dizhi.zhou@gmail.com>
 */

#ifndef CV2X_LENA_TEST_PSS_FF_MAC_SCHEDULER_H
#define CV2X_LENA_TEST_PSS_FF_MAC_SCHEDULER_H

#include "ns3/simulator.h"
#include "ns3/test.h"


using namespace ns3;


/**
 * \ingroup lte-test
 * \ingroup tests
 *
 * \brief This system test program creates different test cases with a single eNB and 
 * several UEs, all having the same Radio Bearer specification. In each test 
 * case, the UEs see the same SINR from the eNB; different test cases are 
 * implemented obtained by using different SINR values and different numbers of 
 * UEs. The test consists on checking that the obtained throughput performance 
 * is equal among users is consistent with the definition of token bank fair  
 * queue scheduling
 */
class cv2x_LenaPssFfMacSchedulerTestCase1 : public TestCase
{
public:
  /**
   * Constructor
   *
   * \param nUser the number of UE nodes
   * \param dist the distance between nodes
   * \param thrRefDl the DL throughput reference
   * \param thrRefUl the UL throughput reference
   * \param packetSize the packet size
   * \param interval the interval time
   * \param errorModelEnabled if true the error model is enabled
   */
  cv2x_LenaPssFfMacSchedulerTestCase1 (uint16_t nUser, double dist, double thrRefDl, double thrRefUl, uint16_t packetSize, uint16_t interval, bool  errorModelEnabled);
  virtual ~cv2x_LenaPssFfMacSchedulerTestCase1 ();

private:
  /**
   * Builds the test name string based on provided parameter values
   * \param nUser the number of UE nodes
   * \param dist the distnace between nodes
   * \returns the name string
   */
  static std::string BuildNameString (uint16_t nUser, double dist);
  virtual void DoRun (void);
  uint16_t m_nUser; ///< number of UE nodes
  double m_dist; ///< the distance between nodes
  uint16_t m_packetSize;  ///< the packet size in bytes
  uint16_t m_interval;    ///< the interval time in ms
  double m_thrRefDl; ///< the DL throughput reference value
  double m_thrRefUl; ///< the UL throughput reference value
  bool m_errorModelEnabled; ///< indicates whether the error model is enabled
};


/**
 * \ingroup lte-test
 * \ingroup tests
 *
 * \brief Simillar to the cv2x_LenaPssFfMacSchedulerTestCase1 with the difference that 
 * UEs are places in such a way to experience different SINRs. Test checks if the
 * achieved throughput in such conditions has expected value.
 */
class cv2x_LenaPssFfMacSchedulerTestCase2 : public TestCase
{
public:
  /**
   * Constructor
   *
   * \param dist the distance between nodes
   * \param estThrPssDl the estimated DL throughput PSS
   * \param packetSize the packet size
   * \param interval the interval time
   * \param errorModelEnabled if true the error model is enabled
   */
  cv2x_LenaPssFfMacSchedulerTestCase2 (std::vector<double> dist, std::vector<uint32_t> estThrPssDl, std::vector<uint16_t> packetSize, uint16_t interval, bool  errorModelEnabled);
  virtual ~cv2x_LenaPssFfMacSchedulerTestCase2 ();

private:
  /**
   * Builds the test name string based on provided parameter values
   * \param nUser the number of UE nodes
   * \param dist the distnace between nodes
   * \returns the name string
   */
  static std::string BuildNameString (uint16_t nUser, std::vector<double> dist);
  virtual void DoRun (void);
  uint16_t m_nUser; ///< number of UE nodes
  std::vector<double> m_dist; ///< the distance between nodes
  std::vector<uint16_t> m_packetSize;  ///< the packet size in bytes
  uint16_t m_interval;    ///< the interval time in ms
  std::vector<uint32_t> m_estThrPssDl; ///< the DL estimated throughput PSS
  bool m_errorModelEnabled; ///< indicates whether the error model is enabled
};


/**
 * \ingroup lte-test
 * \ingroup tests
 *
 * \brief Lena Pss Ff Mac Scheduler Test Suite
 */
class cv2x_LenaTestPssFfMacSchedulerSuite : public TestSuite
{
public:
  cv2x_LenaTestPssFfMacSchedulerSuite ();
};

#endif /* CV2X_LENA_TEST_PSS_FF_MAC_SCHEDULER_H */

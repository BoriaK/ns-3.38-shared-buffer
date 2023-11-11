/*
 * Copyright (c) 2015 Natale Patriciello <natale.patriciello@gmail.com>
 *               2016 Stefano Avallone <stavallo@unina.it>
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
 */
#ifndef SHAREDBUFFER_VIA_MULTIQUEUES_2TOS_H
#define SHAREDBUFFER_VIA_MULTIQUEUES_2TOS_H

#include "ns3/address.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/object.h"
#include "ns3/queue-item.h"
#include "ns3/traced-callback.h"
////////added by me//////////////
#include "ns3/net-device.h"
#include "ns3/nstime.h"
////////////////////////////////

#include <map>
#include <vector>
#include <string>
#include <array>
#include <list>

namespace ns3
{

class SharedBufferMultiQueues2Tos : public Object
{
  public:
    // /**
    //  * \brief Get the type ID.
    //  * \return the object TypeId
    //  */
    // static TypeId GetTypeId();

    // /**
    //  * \brief Get the type ID for the instance
    //  * \return the instance TypeId
    //  */
    // TypeId GetInstanceTypeId() const override;

    /**
     * \brief Constructor
     */
    SharedBufferMultiQueues2Tos();

    ~SharedBufferMultiQueues2Tos();

    /**
     * \brief Run the simulator
     *
     * \param alpha_high the alpha parameter for high priority packets
     * \param alpha_low the alpha parameter for low priority packets
     * \param traffic_control_type the actual traffic control algorythm to use
     */
    void RunSimulation(double alpha_high, double alpha_low, const std::string traffic_control_type);

  private:
    /**
     * \brief convert the poiner to a string for printing
     * \param ndevpointer is the pointer to a desired NetDevice
     * \return the pointer as a string
     */
    std::string NDevicePointerToString (Ptr<NetDevice> ndevpointer);

    /**
     * \brief convert an ineger to string for printing
     * \param value is the integer to convert
     * \return the int value as a string
     */
    std::string IntToString (u_int32_t value);

    /**
     * \brief convert a double to string for printing
     * \param value is the double to convert
     * \return the double value as a string
     */
    std::string DoubleToString (double_t value); 
    
    /**
     * \brief combine several strings to a single string
     * \param A is the first string to combine
     * \param B is the second string to combine
     * \param C is the third string to combine
     * \return a string that is the concaticated combinationof the input strings
     */
    std::string StringCombine (std::string A, std::string B, std::string C);
    
    /**
     * \brief trace the total number of traffic controll packets in shared queue 
     * \param oldValue previous amount of packets in shared queue
     * \param newValue new amount of packets in shared queue
     */
    void TrafficControlPacketsInSharedQueueTrace (uint32_t oldValue, uint32_t newValue);
    
    /**
     * \brief trace the number of High Priority traffic controll packets in shared queue 
     * \param oldValue previous amount of packets in shared queue
     * \param newValue new amount of packets in shared queue
     */
    void TrafficControlHighPriorityPacketsInSharedQueueTrace (uint32_t oldValue, uint32_t newValue);
    
    /**
     * \brief trace the number of Low Priority traffic controll packets in shared queue 
     * \param oldValue previous amount of packets in shared queue
     * \param newValue new amount of packets in shared queue
     */
    void TrafficControlLowPriorityPacketsInSharedQueueTrace (uint32_t oldValue, uint32_t newValue);

    /**
     * \brief trace the enqueing threshold of High Priority traffic controll packets in shared queue
     * \param index the index of "port" (currently traced NetDevice)
     * \param oldValue previous enqueing threshold in shared queue
     * \param newValue new enqueing threshold in shared queue
     */
    void TrafficControlThresholdHighTrace (size_t index, float_t oldValue, float_t newValue);

    /**
     * \brief trace the enqueing threshold of Low Priority traffic controll packets in shared queue
     * \param index the index of "port" (currently traced NetDevice)
     * \param oldValue previous enqueing threshold in shared queue
     * \param newValue new enqueing threshold in shared queue
     */
    void TrafficControlThresholdLowTrace (size_t index, float_t oldValue, float_t newValue);

    /**
     * \brief trace the number of total packets inside the queue-disc, in shared queue
     * \param portIndex the index of "port" (currently traced NetDevice)
     * \param queueIndex the index of sub-queue currently traced
     * \param oldValue previous amount of packets in queue-disc
     * \param newValue new amount of packets in queue-disc
     */
    void QueueDiscPacketsInQueueTrace (size_t portIndex, size_t queueIndex, uint32_t oldValue, uint32_t newValue);
    
    /**
     * \brief trace the number of high priority packets inside the queue-disc, in shared queue
     * \param portIndex the index of "port" (currently traced NetDevice)
     * \param queueIndex the index of sub-queue currently traced
     * \param oldValue previous amount of packets in queue-disc
     * \param newValue new amount of packets in queue-disc
     */
    void HighPriorityQueueDiscPacketsInQueueTrace (size_t portIndex, size_t queueIndex, uint32_t oldValue, uint32_t newValue);

    /**
     * \brief trace the number of low priority packets inside the queue-disc, in shared queue
     * \param portIndex the index of "port" (currently traced NetDevice)
     * \param queueIndex the index of sub-queue currently traced
     * \param oldValue previous amount of packets in queue-disc
     * \param newValue new amount of packets in queue-disc
     */
    void LowPriorityQueueDiscPacketsInQueueTrace (size_t portIndex, size_t queueIndex, uint32_t oldValue, uint32_t newValue);

    /**
     * \brief print the timestamp for each action
     * \param sojournTime the actual time an action took place
     */
    void SojournTimeTrace (Time sojournTime);

    // general variables:
    int serverCount;
    int switchCount;
    int recieverCount;

    int switchRecieverCapacity;
    int serverSwitchCapacity;
    int linkLatency;
    int bufferSize;

    int startTime;
    int endTime;

    int serverPortP0;
    int serverPortP1;

    int packetSize;

    std::string dir;
    std::string implementation;
    // Add other private member variables or helper functions if needed
};

} // namespace ns3

#endif // SHAREDBUFFER_VIA_MULTIQUEUES_2TOS_H

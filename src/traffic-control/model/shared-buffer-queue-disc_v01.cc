/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil;
 * Copyright (c) 2017 Universita' degli Studi di Napoli Federico II
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
 * Authors:  Stefano Avallone <stavallo@unina.it>
 */

#include "ns3/log.h"
#include "shared-buffer-queue-disc_v01.h"
///////////////////////////////////////////
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/traced-value.h"
#include "ns3/net-device-queue-interface.h"
///////////////////////////////////////////
#include "ns3/object-factory.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/socket.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SharedBufferQueueDisc_v01");

NS_OBJECT_ENSURE_REGISTERED (SharedBufferQueueDisc_v01);

TypeId SharedBufferQueueDisc_v01::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SharedBufferQueueDisc_v01")
    .SetParent<QueueDisc> ()
    .SetGroupName ("TrafficControl")
    .AddConstructor<SharedBufferQueueDisc_v01> ()
    // .AddAttribute ("MaxSize",
    //                "The maximum number of packets accepted by this queue disc.",
    //                QueueSizeValue (QueueSize ("100p")),
    //                MakeQueueSizeAccessor (&QueueDisc::SetMaxSize,
    //                                       &QueueDisc::GetMaxSize),
    //                MakeQueueSizeChecker ())
    .AddAttribute("MaxSize",
                   "The maximum number of packets accepted by this queue disc.",
                  QueueSizeValue(QueueSize("100p")),
                  MakeQueueSizeAccessor(&SharedBufferQueueDisc_v01::m_maxSize),
                  MakeQueueSizeChecker ())
    // .AddAttribute ("PortCount",
    //                "The number of ports connected to this queue disc.",
    //                UintegerValue (1),
    //                MakeUintegerAccessor (&SharedBufferQueueDisc_v01::m_portCount),
    //                MakeUintegerChecker<uint32_t> ())                        
  ;
  return tid;
}

SharedBufferQueueDisc_v01::SharedBufferQueueDisc_v01 ()
  : QueueDisc (QueueDiscSizePolicy::MULTIPLE_QUEUES, QueueSizeUnit::PACKETS),
    m_usedBufferSizePackets (0),
    m_usedBufferSizeBytes (0)
    // m_portCount (0)
{
  NS_LOG_FUNCTION (this);
  // m_currentQueue = 0;
}

SharedBufferQueueDisc_v01::~SharedBufferQueueDisc_v01 ()
{
  NS_LOG_FUNCTION (this);
}

// bool
// SharedBufferQueueDisc_v01::DoEnqueue(Ptr<QueueDiscItem> item, Ptr<NetDevice> device)
// {
//   NS_LOG_FUNCTION(this << item << device);

//   if (m_queue.size()>= GetMaxSize().GetValue()) 
//   {
//     NS_LOG_DEBUG("Queue full, dropping packet.");
//     return false; // drop the packet if there is no space
//   }

//   m_queue.push_back(item);
//   // m_queueSizeCurrent += item->GetSize();

//   // add the NetDevice information to the QueueDiscItem
//   item->SetNetDevice(device);

//   NS_LOG_DEBUG("Packet enqueued, current queue size: " << m_queue.size());

//   return true;
// }


bool
SharedBufferQueueDisc_v01::DoEnqueue (Ptr<QueueDiscItem> item)
{
  NS_LOG_FUNCTION (this << item);


  if (m_usedBufferSizePackets >= m_maxSize.GetValue())  // maybe m_usedBufferSize can be changed with GetCurrentSize().GetValue()
  // if (m_queue.size()>= GetMaxSize().GetValue())
    {
      NS_LOG_LOGIC ("Queue full, dropping packet.");
      return false; // drop the packet if there is no space
    }
  
  // Add the packet to the shared buffer
  m_queue.push_back(item);
  m_usedBufferSizePackets++;
  m_usedBufferSizeBytes += item->GetSize();

  //for debug:
  std::cout << "Packet UID: " << item->GetPacket()->GetUid() << " Equeued by QueueDisc on NetDevice: " << item->GetNetDevice() << std::endl;
  std::cout << "Total Packets in Shared Queue: " << m_usedBufferSizePackets << std::endl;
  ///////////////////////////////////////////////////////////////////////////////////////////
// try from FIFO QueueDisc//////////////////////////////
  // bool retval = GetInternalQueue(0)->Enqueue(item);
/////////////////////////////////
  NS_LOG_DEBUG("Packet enqueued, current queue size: " << m_queue.size());

/////////////////////////////////////////////
    // uint32_t size = item->GetSize();
    // m_nBytes += size;
    // m_nTotalReceivedBytes += size;

    // m_nPackets++;
    // m_nTotalReceivedPackets++;

    // NS_LOG_LOGIC("m_traceEnqueue (p)");
    // m_traceEnqueue(item);

/////////////////////////////////////////////////    


  return true;
}

Ptr<QueueDiscItem>
SharedBufferQueueDisc_v01::DoDequeue (void)
{
  NS_LOG_FUNCTION (this);

///try
  // GetInternalQueue(0)->GetCurrentSize();
/////////////////////////////
  if (m_queue.empty())
  {
    NS_LOG_LOGIC ("Queue empty");
    return 0;
  }

/////////add this block///////////////////////////////////////
  // Find the transmit queue of the NetDevice that owns the packet and dequeue it
  Ptr<QueueDiscItem> item = m_queue.front();
  // Ptr<NetDevice> device = item->GetNetDevice();
  // Ptr<Queue<Packet> > txQueue = GetNDevTxQueue(device);
  // if (txQueue) 
  // {
  //   txQueue->Dequeue();
  // }
////////////////////////////////////////////////////////////////////
  // Remove the packet from the shared buffer
  m_queue.pop_front();
  m_usedBufferSizePackets--;
  m_usedBufferSizeBytes -= item->GetSize();
  
  //for debug:
  std::cout << "Packet UID: " << item->GetPacket()->GetUid() << " Dequeued by QueueDisc on NetDevice: " << item->GetNetDevice() << std::endl;
  std::cout << "Total Packets in Shared Queue: " << m_usedBufferSizePackets << std::endl;
/////////////////////////////////////////////////////
  
  // GetInternalQueue(0)->Dequeue();

  return item;
}

Ptr<Queue<Packet> > 
SharedBufferQueueDisc_v01::GetNDevTxQueue(Ptr<NetDevice> device) 
{
  // Get the transmit queue of the NetDevice
  // Ptr<NetDeviceQueue> ndevq = device->GetObject<NetDeviceQueue>();
  Ptr<NetDeviceQueueInterface> queueInterface = device->GetObject<NetDeviceQueueInterface>();  // shorter way to get NDQIF
  if (queueInterface) 
  {
    // Ptr<NetDeviceQueue> ndevq = queueInterface->GetTxQueue(0);
    // // try
    Ptr<Queue<Packet>> queue = queueInterface->GetObject<Queue<Packet>>();; // BUG HERE!!!!!!!!!!!!!!!!!!
    return queue;
  }


    return 0;
}

// Ptr<const QueueDiscItem>
// SharedBufferQueueDisc_v01::DoPeek (void)
// {
//   NS_LOG_FUNCTION (this);

//   // Ptr<const QueueDiscItem> item = GetInternalQueue (0)->Peek ();  // original
//   Ptr<const QueueDiscItem> item = m_queue.front()->Peek ();

//   if (!item)
//     {
//       NS_LOG_LOGIC ("Queue empty");
//       return 0;
//     }

//   return item;
// }

bool
SharedBufferQueueDisc_v01::CheckConfig (void)
{
//   ///////////will change all the inner functions//////////////////////
//   NS_LOG_FUNCTION (this);
//   if (GetNQueueDiscClasses () > 0)
//     {
//       NS_LOG_ERROR ("FifoQueueDisc cannot have classes");
//       return false;
//     }

//   if (GetNPacketFilters () > 0)
//     {
//       NS_LOG_ERROR ("FifoQueueDisc needs no packet filter");
//       return false;
//     }

// // will need to check this part later
//   if (GetNInternalQueues () == 0)
//     {
//       // add a DropTail queue
//       AddInternalQueue (CreateObjectWithAttributes<DropTailQueue<QueueDiscItem> >
//                           ("MaxSize", QueueSizeValue (GetMaxSize ())));
//     }

//   if (GetNInternalQueues () != 1)
//     {
//       NS_LOG_ERROR ("FifoQueueDisc needs 1 internal queue");
//       return false;
//     }

  return true;
}

void
SharedBufferQueueDisc_v01::InitializeParams (void)
{
  NS_LOG_FUNCTION (this);
}

// to ad the << operator
// std::ostream&
// operator<<(std::ostream& os, const SharedBufferQueueDisc_v01& header)
// {
//     header.Print(os);
//     return os;
// }

} // namespace ns3

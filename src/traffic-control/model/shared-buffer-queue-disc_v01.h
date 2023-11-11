/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#ifndef SHARED_BUFFER_QUEUE_DISC_V01_H
#define SHARED_BUFFER_QUEUE_DISC_V01_H

#include "queue-disc.h"
#include "ns3/queue.h"
#include "ns3/traced-value.h"
#include "ns3/traced-callback.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include <list>
/////////////////////////
#include "ns3/object.h"
#include "ns3/queue-fwd.h"
#include "ns3/queue-item.h"
#include "ns3/queue-size.h"

#include <sstream>
#include <string>
#include <type_traits>
////////////////////////////
namespace ns3 {

/**
 * \ingroup traffic-control
 *
 * Simple queue disc implementing the FIFO (First-In First-Out) policy.
 *
 */

class SharedBufferQueueDisc_v01 : public QueueDisc {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief SharedBufferQueueDisc constructor
   *
   * Creates a queue with a depth of 1000 packets by default
   */
  SharedBufferQueueDisc_v01 ();

  virtual ~SharedBufferQueueDisc_v01();

  // Reasons for dropping packets
  static constexpr const char* LIMIT_EXCEEDED_DROP = "Queue disc limit exceeded";  //!< Packet dropped due to queue disc limit exceeded

//////function overload. might want to use existing functions from queue-disc.cc in the future//////////////////////
  // bool Enqueue (Ptr<QueueDiscItem> item);
  // Ptr<QueueDiscItem> Dequeue (void);
  // Ptr<const QueueDiscItem> Peek (void) const;
/////////////////////////////////////////////////////////////////////////////////////////////////
private:
  // virtual bool DoEnqueue (Ptr<QueueDiscItem> item, Ptr<NetDevice> device);
  virtual bool DoEnqueue (Ptr<QueueDiscItem> item);
  virtual Ptr<QueueDiscItem> DoDequeue (void);
  // virtual Ptr<const QueueDiscItem> DoPeek (void);
  virtual bool CheckConfig (void);
  virtual void InitializeParams (void);
    // Helper method to get the transmit queue of a NetDevice
  Ptr<Queue<Packet> > GetNDevTxQueue(Ptr<NetDevice> device);

  // std::vector<ns3::Ptr<ns3::Queue<ns3::Packet>>> m_queues;
  // uint32_t m_currentQueue;
  // ns3::Queue<ns3::Packet> m_sharedBuffer;

  // Ptr<Queue<QueueDiscItem>> m_queue;
  // std::vector<Ptr<Queue<QueueDiscItem>> > m_txQueues;
  std::list<Ptr<QueueDiscItem> > m_queue; // shared buffer
  QueueSize m_maxSize;
  // uint32_t m_queueSize;  // the current size of the sub queue
  // uint32_t m_bufferSpacePerPort;
  uint32_t m_usedBufferSizePackets;
  uint32_t m_usedBufferSizeBytes;
  // uint32_t m_portCount;

  //////experimental://////////////////
    //   Container m_packets;     //!< the items in the queue
    // NS_LOG_TEMPLATE_DECLARE; //!< the log component

  protected:
    TracedValue<uint32_t> m_nBytes;               //!< Number of bytes in the queue
    uint32_t m_nTotalReceivedBytes;               //!< Total received bytes
    TracedValue<uint32_t> m_nPackets;             //!< Number of packets in the queue
    uint32_t m_nTotalReceivedPackets;             //!< Total received packets
    uint32_t m_nTotalDroppedBytes;                //!< Total dropped bytes
    uint32_t m_nTotalDroppedBytesBeforeEnqueue;   //!< Total dropped bytes before enqueue
    uint32_t m_nTotalDroppedBytesAfterDequeue;    //!< Total dropped bytes after dequeue
    uint32_t m_nTotalDroppedPackets;              //!< Total dropped packets
    uint32_t m_nTotalDroppedPacketsBeforeEnqueue; //!< Total dropped packets before enqueue
    uint32_t m_nTotalDroppedPacketsAfterDequeue;  //!< Total dropped packets after dequeue

    /// Define ItemType as the type of the stored elements
    // typedef Item ItemType;
    // /// Traced callback: fired when a packet is enqueued
    // TracedCallback<Ptr<const Item>> m_traceEnqueue;
    // /// Traced callback: fired when a packet is dequeued
    // TracedCallback<Ptr<const Item>> m_traceDequeue;
    // /// Traced callback: fired when a packet is dropped
    // TracedCallback<Ptr<const Item>> m_traceDrop;
    // /// Traced callback: fired when a packet is dropped before enqueue
    // TracedCallback<Ptr<const Item>> m_traceDropBeforeEnqueue;
    // /// Traced callback: fired when a packet is dropped after dequeue
    // TracedCallback<Ptr<const Item>> m_traceDropAfterDequeue;
  /////////////////////////////////////////  
};

} // namespace ns3

#endif /* SHARED_BUFFER_QUEUE_DISC_V01_H */

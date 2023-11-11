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
 * Authors:  Stefano Avallone <stavallo@unina.it>
 */

#include "round-robin-prio-queue-disc.h"

#include "ns3/log.h"
#include "ns3/object-factory.h"
#include "ns3/pointer.h"
#include "ns3/socket.h"
// for modifyed dequeue:
#include "ns3/simulator.h"
#include "ns3/error-model.h"

#include <algorithm>
#include <iterator>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("RoundRobinPrioQueueDisc");

NS_OBJECT_ENSURE_REGISTERED(RoundRobinPrioQueueDisc);

ATTRIBUTE_HELPER_CPP(Priomap);

std::ostream&
operator<<(std::ostream& os, const Priomap& priomap)
{
    std::copy(priomap.begin(), priomap.end() - 1, std::ostream_iterator<uint16_t>(os, " "));
    os << priomap.back();
    return os;
}

std::istream&
operator>>(std::istream& is, Priomap& priomap)
{
    for (int i = 0; i < 16; i++)
    {
        if (!(is >> priomap[i]))
        {
            NS_FATAL_ERROR("Incomplete priomap specification ("
                           << i << " values provided, 16 required)");
        }
    }
    return is;
}

TypeId
RoundRobinPrioQueueDisc::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::RoundRobinPrioQueueDisc")
            .SetParent<QueueDisc>()
            .SetGroupName("TrafficControl")
            .AddConstructor<RoundRobinPrioQueueDisc>()
            .AddAttribute("Priomap",
                          "The priority to band mapping.",
                          PriomapValue(Priomap{{1, 2, 2, 2, 1, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}}),
                          MakePriomapAccessor(&RoundRobinPrioQueueDisc::m_prio2band),
                          MakePriomapChecker());
    return tid;
}

RoundRobinPrioQueueDisc::RoundRobinPrioQueueDisc()
    : QueueDisc(QueueDiscSizePolicy::NO_LIMITS),
        m_lastDequeuedClass(0)  // initilze the dequeued class to 0 at the creation of the prio-queue-disc
{
    NS_LOG_FUNCTION(this);
}

RoundRobinPrioQueueDisc::~RoundRobinPrioQueueDisc()
{
    NS_LOG_FUNCTION(this);
}

void
RoundRobinPrioQueueDisc::SetBandForPriority(uint8_t prio, uint16_t band)
{
    NS_LOG_FUNCTION(this << prio << band);

    NS_ASSERT_MSG(prio < 16, "Priority must be a value between 0 and 15");

    m_prio2band[prio] = band;
}

uint16_t
RoundRobinPrioQueueDisc::GetBandForPriority(uint8_t prio) const
{
    NS_LOG_FUNCTION(this << prio);

    NS_ASSERT_MSG(prio < 16, "Priority must be a value between 0 and 15");

    return m_prio2band[prio];
}

bool
RoundRobinPrioQueueDisc::DoEnqueue(Ptr<QueueDiscItem> item)
{
    NS_LOG_FUNCTION(this << item);

    uint32_t band = m_prio2band[0];

    int32_t ret = Classify(item);

    if (ret == PacketFilter::PF_NO_MATCH)
    {
        NS_LOG_DEBUG("No filter has been able to classify this packet, using priomap.");

        SocketPriorityTag priorityTag;
        if (item->GetPacket()->PeekPacketTag(priorityTag))
        {
            band = m_prio2band[priorityTag.GetPriority() & 0x0f];
            std::cout << "priority: " << int(priorityTag.GetPriority()) << std::endl;
        }
    }
    else
    {
        NS_LOG_DEBUG("Packet filters returned " << ret);

        if (ret >= 0 && static_cast<uint32_t>(ret) < GetNQueueDiscClasses())
        {
            band = ret;
        }
    }

    // for debug:
    SharedPriorityTag flowPrioTag;
    uint8_t flow_priority = 0;
    if (item->GetPacket ()->PeekPacketTag (flowPrioTag))
    {
        flow_priority = flowPrioTag.GetSimpleValue();
    }
    std::cout << "Packet of priority: " << int(flow_priority) << " enqueued in band: " << band << std::endl;
    //////////////
    NS_ASSERT_MSG(band < GetNQueueDiscClasses(), "Selected band out of range");
    bool retval = GetQueueDiscClass(band)->GetQueueDisc()->Enqueue(item);

    // If Queue::Enqueue fails, QueueDisc::Drop is called by the child queue disc
    // because QueueDisc::AddQueueDiscClass sets the drop callback

    NS_LOG_LOGIC("Number packets band " << band << ": "
                                        << GetQueueDiscClass(band)->GetQueueDisc()->GetNPackets());

    return retval;
}

// Round-Robbin DoDequeue:
Ptr<QueueDiscItem>
RoundRobinPrioQueueDisc::DoDequeue()
{
    Ptr<QueueDiscItem> item;

    uint8_t nextQueue = m_lastDequeuedClass + 1;
    if (nextQueue >= GetNQueueDiscClasses()) 
    {
        nextQueue = 0;
    }

    for (uint8_t i = 0; i < GetNQueueDiscClasses(); i++) 
    {
        uint8_t queueIndex = (nextQueue + i) % GetNQueueDiscClasses();
        if ((item = GetQueueDiscClass(queueIndex)->GetQueueDisc()->Dequeue()))
        // if (!GetQueueDiscClass(queueIndex)->GetQueueDisc()->Peek() == 0) // check if class queue is not empty
        {
            m_lastDequeuedClass = queueIndex;
            // item = GetQueueDiscClass(queueIndex)->GetQueueDisc()->Dequeue();
            NS_LOG_LOGIC("Popped from band " << queueIndex << ": " << item);
            NS_LOG_LOGIC("Number of packets in band "
                        << queueIndex << ": " << GetQueueDiscClass(queueIndex)->GetQueueDisc()->GetNPackets());
            // for debug:
            std::cout << "Packet dequeued from band: " << int(queueIndex) << std::endl;
            std::cout << "Number of packets in band " << int(queueIndex) << ": " << GetQueueDiscClass(queueIndex)->GetQueueDisc()->GetNPackets() << std::endl;
            //////////////
            return item;
        }
    }
    NS_LOG_LOGIC("Queue empty");
    return item;
}



Ptr<const QueueDiscItem>
RoundRobinPrioQueueDisc::DoPeek()
{
    NS_LOG_FUNCTION(this);

    Ptr<const QueueDiscItem> item;

    for (uint32_t i = 0; i < GetNQueueDiscClasses(); i++)
    {
        if ((item = GetQueueDiscClass(i)->GetQueueDisc()->Peek()))
        {
            NS_LOG_LOGIC("Peeked from band " << i << ": " << item);
            NS_LOG_LOGIC("Number packets band "
                         << i << ": " << GetQueueDiscClass(i)->GetQueueDisc()->GetNPackets());
            return item;
        }
    }

    NS_LOG_LOGIC("Queue empty");
    return item;
}

bool
RoundRobinPrioQueueDisc::CheckConfig()
{
    NS_LOG_FUNCTION(this);
    if (GetNInternalQueues() > 0)
    {
        NS_LOG_ERROR("PrioQueueDisc cannot have internal queues");
        return false;
    }

    if (GetNQueueDiscClasses() == 0)
    {
        // create 3 fifo queue discs
        ObjectFactory factory;
        factory.SetTypeId("ns3::FifoQueueDisc");
        for (uint8_t i = 0; i < 2; i++)
        {
            Ptr<QueueDisc> qd = factory.Create<QueueDisc>();
            qd->Initialize();
            Ptr<QueueDiscClass> c = CreateObject<QueueDiscClass>();
            c->SetQueueDisc(qd);
            AddQueueDiscClass(c);
        }
    }

    if (GetNQueueDiscClasses() < 2)
    {
        NS_LOG_ERROR("PrioQueueDisc needs at least 2 classes");
        return false;
    }

    return true;
}

void
RoundRobinPrioQueueDisc::InitializeParams()
{
    NS_LOG_FUNCTION(this);
}

} // namespace ns3

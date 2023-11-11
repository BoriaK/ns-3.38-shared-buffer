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

#include "sharedBuffer_via_MultiQueues_2ToS.h"
// #include "ns3/core-module.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <list>
#include <array>
#include <filesystem>


// ... Include other ns3 modules here ...

// Constructor
// SharedBufferMultiQueues2ToS::SharedBufferMultiQueues2ToS()
//   : Object(),
//   serverCount (2),
//   switchCount (1),
//   recieverCount(2),
//   serverSwitchCapacity(20000000), // 20[MBpS] the capacity of the link between the servers and the switch
//   switchRecieverCapacity (2000000), // 2[MBpS] the capacity of the link between the switch and the reciever
//   linkLatency (20), // [uSec]
//   bufferSize (100),  // [packets]
//   startTime (0),  // [seconds]
//   endTime(30),  // [seconds]
//   serverPortP0 (50000),
//   serverPortP1 (50001),
//   packetSize (1024), // [bytes]
//   dir ("./Trace_Plots/test_Alphas/"),  // the folder to which collect all the data
//   implementation ("via_MultiQueues2_ToS")
// {
//   NS_LOG_FUNCTION(this);
// }
SharedBufferMultiQueues2Tos::SharedBufferMultiQueues2Tos()
{

}


// Destructor
SharedBufferMultiQueues2Tos::~SharedBufferMultiQueues2Tos() 
{
    NS_LOG_FUNCTION(this);
}

std::string 
SharedBufferMultiQueues2Tos::NDevicePointerToString (Ptr<NetDevice> ndevpointer)
{
    std::stringstream ss;
    ss << ndevpointer;
    return ss.str();
}

std::string
SharedBufferMultiQueues2Tos::ToString (uint32_t value)
{
  std::stringstream ss;
  ss << value;
  return ss.str();
}

std::string
SharedBufferMultiQueues2Tos::IntToString (u_int32_t value)
{
  std::stringstream ss;
  ss << value;
  return ss.str();
}

std::string
SharedBufferMultiQueues2Tos::DoubleToString (double_t value)
{
  std::stringstream ss;
  ss << value;
  return ss.str();
}

std::string
SharedBufferMultiQueues2Tos::StringCombine (std::string A, std::string B, std::string C)
{
  std::stringstream ss;
  ss << A << B << C;
  return ss.str();
}

// functions to monitor Shared Buffer packets on Traffic Control Layer
void
SharedBufferMultiQueues2Tos::TrafficControlPacketsInSharedQueueTrace (uint32_t oldValue, uint32_t newValue)
{
  std::ofstream tcpisq (dir + "/TrafficControlPacketsInQueueTrace.dat", std::ios::out | std::ios::app);
  tcpisq << Simulator::Now ().GetSeconds () << " " << newValue << std::endl;
  tcpisq.close ();
  
  std::cout << "PacketsInSharedBuffer: " << newValue << std::endl;
}

void
SharedBufferMultiQueues2Tos::TrafficControlHighPriorityPacketsInSharedQueueTrace (uint32_t oldValue, uint32_t newValue)
{
  std::ofstream tchppisq (dir + "/TrafficControlHighPriorityPacketsInQueueTrace.dat", std::ios::out | std::ios::app);
  tchppisq << Simulator::Now ().GetSeconds () << " " << newValue << std::endl;
  tchppisq.close ();
  
  std::cout << "HighPriorityPacketsInSharedBuffer: " << newValue << std::endl;
}

void
SharedBufferMultiQueues2Tos::TrafficControlLowPriorityPacketsInSharedQueueTrace (uint32_t oldValue, uint32_t newValue)
{
  std::ofstream tclppisq (dir + "/TrafficControlLowPriorityPacketsInQueueTrace.dat", std::ios::out | std::ios::app);
  tclppisq << Simulator::Now ().GetSeconds () << " " << newValue << std::endl;
  tclppisq.close ();
  
  std::cout << "LowPriorityPacketsInSharedBuffer: " << newValue << std::endl;
}

// Trace the Threshold Value for High Priority packets in the Shared Queue
void
SharedBufferMultiQueues2Tos::TrafficControlThresholdHighTrace (size_t index, float_t oldValue, float_t newValue)  // added by me, to monitor Threshold
{
  std::ofstream tchpthr (dir + "/TrafficControlHighPriorityQueueThreshold_" + ToString(index) + ".dat", std::ios::out | std::ios::app);
  tchpthr << Simulator::Now ().GetSeconds () << " " << newValue << std::endl;
  tchpthr.close ();

  std::cout << "HighPriorityQueueThreshold on port: " << index << " is: " << newValue << " packets " << std::endl;
}

// Trace the Threshold Value for Low Priority packets in the Shared Queue
void
SharedBufferMultiQueues2Tos::TrafficControlThresholdLowTrace (size_t index, float_t oldValue, float_t newValue)  // added by me, to monitor Threshold
{
  std::ofstream tclpthr (dir + "/TrafficControlLowPriorityQueueThreshold_" + ToString(index) + ".dat", std::ios::out | std::ios::app);
  tclpthr << Simulator::Now ().GetSeconds () << " " << newValue << std::endl;
  tclpthr.close ();
  
  std::cout << "LowPriorityQueueThreshold on port: " << index << " is: " << newValue << " packets " << std::endl;
}

void
SharedBufferMultiQueues2Tos::QueueDiscPacketsInQueueTrace (size_t portIndex, size_t queueIndex, uint32_t oldValue, uint32_t newValue)
{
  std::ofstream qdpiq (dir + "/port_" + ToString(portIndex) + "_queue_" + ToString(queueIndex) + "_PacketsInQueueTrace.dat", std::ios::out | std::ios::app);
  qdpiq << Simulator::Now ().GetSeconds () << " " << newValue << std::endl;
  qdpiq.close ();
  
  std::cout << "QueueDiscPacketsInPort " << portIndex << " Queue " << queueIndex << ": " << newValue << std::endl;
}

void
SharedBufferMultiQueues2Tos::HighPriorityQueueDiscPacketsInQueueTrace (size_t portIndex, size_t queueIndex, uint32_t oldValue, uint32_t newValue)
{
  std::ofstream hpqdpiq (dir + "/port_" + ToString(portIndex) + "_queue_" + ToString(queueIndex) + "_HighPriorityPacketsInQueueTrace.dat", std::ios::out | std::ios::app);
  hpqdpiq << Simulator::Now ().GetSeconds () << " " << newValue << std::endl;
  hpqdpiq.close ();
  
  std::cout << "HighPriorityQueueDiscPacketsInPort " << portIndex << " Queue " << queueIndex << ": " << newValue << std::endl;
}

void
SharedBufferMultiQueues2Tos::LowPriorityQueueDiscPacketsInQueueTrace (size_t portIndex, size_t queueIndex, uint32_t oldValue, uint32_t newValue)
{
  std::ofstream lpqdpiq (dir + "/port_" + ToString(portIndex) + "_queue_" + ToString(queueIndex) + "_LowPriorityPacketsInQueueTrace.dat", std::ios::out | std::ios::app);
  lpqdpiq << Simulator::Now ().GetSeconds () << " " << newValue << std::endl;
  lpqdpiq.close ();
  
  std::cout << "LowPriorityQueueDiscPacketsInPort " << portIndex << " Queue " << queueIndex << ": " << newValue << std::endl;
}

void
SharedBufferMultiQueues2Tos::SojournTimeTrace (Time sojournTime)
{
  std::cout << "Sojourn time " << sojournTime.ToDouble (Time::MS) << "ms" << std::endl;
}

// Run the simulation
void
// SharedBufferMultiQueues2Tos::RunSimulation (double_t alpha_high, double_t alpha_low, std::string traffic_control_type, bool accumulateStats)
SharedBufferMultiQueues2Tos::RunSimulation (double_t alpha_high, double_t alpha_low, std::string traffic_control_type)
{
    LogComponentEnable ("2In2Out", LOG_LEVEL_INFO);

    std::string usedAlgorythm;  // "DT"/"FB"
    std::string applicationType = "prioOnOff"; // "standardClient"/"OnOff"/"prioClient"/"prioOnOff"

    std::string transportProt = "UDP"; // "UDP"/"TCP"
    std::string socketType;
    std::string queue_capacity;
    if (traffic_control_type.compare("SharedBuffer_DT_v01") == 0)
    {
        usedAlgorythm = "DT";
    }
    else if (traffic_control_type.compare("SharedBuffer_FB_v01") == 0)
    {
        usedAlgorythm = "FB";
    }

    bool eraseOldData = true; // true/false

    if (eraseOldData == true)
    {
        system (("rm " + dir + "/*.dat").c_str ()); // to erase the old .dat files and collect new data
        system (("rm " + dir + "/*.txt").c_str ()); // to erase the previous test run summary, and collect new data
        std::cout << std::endl << "***Erased Previous Data***\n" << std::endl;
    }

    NS_LOG_INFO ("Config parameters");
    // Application type dependent parameters
    if (applicationType.compare("standardClient") == 0 || applicationType.compare("prioClient") == 0)
    {
      queue_capacity = "20p"; // B, the total space on the buffer.
    }
    else
    {
      queue_capacity = ToString(BUFFER_SIZE) + "p"; // B, the total space on the buffer [packets]
    }
  
    // client type dependant parameters:
    if (transportProt.compare ("TCP") == 0)
    {
      socketType = "ns3::TcpSocketFactory";
    }
    else
    {
      socketType = "ns3::UdpSocketFactory";
    }
  
    // Application and Client type dependent parameters
    // select the desired components to output data
    if (applicationType.compare("standardClient") == 0 && transportProt.compare ("TCP") == 0)
    {
        LogComponentEnable ("TcpClient", LOG_LEVEL_INFO);
    }
    else if (applicationType.compare("standardClient") == 0 && transportProt.compare ("UDP") == 0)
    {
        LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
    }
    else if ((applicationType.compare("OnOff") == 0 || applicationType.compare("priorityOnOff") == 0 || applicationType.compare("priorityApplication") == 0)&& transportProt.compare ("Tcp") == 0)
    {
        LogComponentEnable("TcpSocketImpl", LOG_LEVEL_INFO);
    }
    else if ((applicationType.compare("OnOff") == 0 || applicationType.compare("priorityOnOff") == 0 || applicationType.compare("priorityApplication") == 0) && transportProt.compare ("Udp") == 0)
    {
        LogComponentEnable("UdpSocketImpl", LOG_LEVEL_INFO);
    }
  
    LogComponentEnable("PacketSink", LOG_LEVEL_INFO); 
    
    NS_LOG_INFO ("Create nodes");
    NodeContainer recievers;
    recievers.Create (RECIEVER_COUNT);
    NodeContainer router;
    router.Create (SWITCH_COUNT);
    // for loop use. make sure name "Router" is not stored in Names map///
    Names::Clear();
    /////////////////////////////////////////////////////////
    Names::Add("Router", router.Get(0));  // Add a Name to the router node
    NodeContainer servers;
    servers.Create (SERVER_COUNT);


    // NS_LOG_INFO ("Install channels and assign addresses");

    PointToPointHelper n2s, s2r;
    NS_LOG_INFO ("Configuring channels for all the Nodes");
    // Setting servers
    uint64_t serverSwitchCapacity = SERVER_SWITCH_CAPACITY / SERVER_COUNT;
    n2s.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (serverSwitchCapacity)));
    n2s.SetChannelAttribute ("Delay", TimeValue(LINK_LATENCY));
    n2s.SetQueue ("ns3::DropTailQueue", "MaxSize", StringValue ("1p"));  // set basic queues to 1 packet
    // setting routers
    uint64_t switchRecieverCapacity = SWITCH_RECIEVER_CAPACITY / RECIEVER_COUNT;
    s2r.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (switchRecieverCapacity)));
    s2r.SetChannelAttribute ("Delay", TimeValue(LINK_LATENCY));
    s2r.SetQueue ("ns3::DropTailQueue", "MaxSize", StringValue ("1p"));  // set basic queues to 1 packet

    NS_LOG_INFO ("Create NetDevices");
    NetDeviceContainer serverDevices;
    NetDeviceContainer switchDevicesIn;
    NetDeviceContainer switchDevicesOut;
    NetDeviceContainer recieverDevices;

    NS_LOG_INFO ("Install NetDevices on all Nodes");
    NS_LOG_INFO ("Configuring Servers");
    for (int i = 0; i < SERVER_COUNT; i++)
    {
        NS_LOG_INFO ("Server " << i << " is connected to switch");

        NetDeviceContainer tempNetDevice = n2s.Install (servers.Get (i), router.Get(0));
        serverDevices.Add(tempNetDevice.Get(0));
        switchDevicesIn.Add(tempNetDevice.Get(1));
    }
    

    NS_LOG_INFO ("Configuring switches");


    for (int i = 0; i < RECIEVER_COUNT; i++)
    {
        NetDeviceContainer tempNetDevice = s2r.Install (router.Get(0), recievers.Get (i));
        switchDevicesOut.Add(tempNetDevice.Get(0));
        recieverDevices.Add(tempNetDevice.Get(1));

        NS_LOG_INFO ("Switch is connected to Reciever " << i << "at capacity: " << switchRecieverCapacity);     
    }


    // Now add ip/tcp stack to all nodes. this is a VERY IMPORTANT COMPONENT!!!!
    NS_LOG_INFO ("Install Internet stacks");
    InternetStackHelper internet;
    internet.InstallAll ();

    NS_LOG_INFO ("Install QueueDisc");

    TrafficControlHelper tch;
    // priomap with low priority for class "0" and high priority for rest of the 15 classes (1-15). Technically not nesessary for RoundRobinPrioQueue
    std::array<uint16_t, 16> prioArray = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    // Priomap prioMap = Priomap{prioArray};
    TosMap tosMap = TosMap{prioArray};
    // uint16_t handle = tch.SetRootQueueDisc("ns3::RoundRobinPrioQueueDisc", "Priomap", PriomapValue(prioMap));
    uint16_t handle = tch.SetRootQueueDisc("ns3::RoundRobinTosQueueDisc", "TosMap", TosMapValue(tosMap));

    TrafficControlHelper::ClassIdList cid = tch.AddQueueDiscClasses(handle, 2, "ns3::QueueDiscClass");
    tch.AddChildQueueDisc(handle, cid[0], "ns3::FifoQueueDisc", "MaxSize", StringValue (queue_capacity)); // cid[0] is band "0" - the Highest Priority band
    tch.AddChildQueueDisc(handle, cid[1], "ns3::FifoQueueDisc", "MaxSize", StringValue (queue_capacity)); // cid[1] is < cid[0]

    QueueDiscContainer qdiscs = tch.Install (switchDevicesOut);  // in this option we installed TCH on switchDevicesOut. to send data from switch to reciever

    

///////// set the Traffic Controll layer to be a shared buffer////////////////////////
    TcPriomap tcPrioMap = TcPriomap{prioArray};
    Ptr<TrafficControlLayer> tc;
    tc = router.Get(0)->GetObject<TrafficControlLayer>();
    tc->SetAttribute("SharedBuffer", BooleanValue(true));
    tc->SetAttribute("MaxSharedBufferSize", StringValue (queue_capacity));
    tc->SetAttribute("Alpha_High", DoubleValue (alpha_high));
    tc->SetAttribute("Alpha_Low", DoubleValue (alpha_low));
    tc->SetAttribute("TrafficControllAlgorythm", StringValue (usedAlgorythm));
    tc->SetAttribute("PriorityMapforMultiQueue", TcPriomapValue(tcPrioMap));

    // monitor the packets in the Shared Buffer in Traffic Control Layer:
    tc->TraceConnectWithoutContext("PacketsInQueue", MakeCallback (&TrafficControlPacketsInSharedQueueTrace));
    tc->TraceConnectWithoutContext("HighPriorityPacketsInQueue", MakeCallback (&TrafficControlHighPriorityPacketsInSharedQueueTrace));
    tc->TraceConnectWithoutContext("LowPriorityPacketsInQueue", MakeCallback (&TrafficControlLowPriorityPacketsInSharedQueueTrace));
    tc->TraceConnectWithoutContext("EnqueueingThreshold_High_0", MakeBoundCallback (&TrafficControlThresholdHighTrace, 0));
    tc->TraceConnectWithoutContext("EnqueueingThreshold_Low_0", MakeBoundCallback (&TrafficControlThresholdLowTrace, 0));  
    tc->TraceConnectWithoutContext("EnqueueingThreshold_High_1", MakeBoundCallback (&TrafficControlThresholdHighTrace, 1));
    tc->TraceConnectWithoutContext("EnqueueingThreshold_Low_1", MakeBoundCallback (&TrafficControlThresholdLowTrace, 1));

/////////////////////////////////////////////////////////////////////////////

//////////////Monitor data from q-disc//////////////////////////////////////////
    for (size_t i = 0; i < RECIEVER_COUNT; i++)  // over all ports
    {
        for (size_t j = 0; j < qdiscs.Get (i)->GetNQueueDiscClasses(); j++)  // over all the queues per port
        {
          Ptr<QueueDisc> queue = qdiscs.Get (i)->GetQueueDiscClass(j)->GetQueueDisc();
          queue->TraceConnectWithoutContext ("PacketsInQueue", MakeBoundCallback (&QueueDiscPacketsInQueueTrace, i, j));
          queue->TraceConnectWithoutContext ("HighPriorityPacketsInQueue", MakeBoundCallback (&HighPriorityQueueDiscPacketsInQueueTrace, i, j)); 
          queue->TraceConnectWithoutContext ("LowPriorityPacketsInQueue", MakeBoundCallback (&LowPriorityQueueDiscPacketsInQueueTrace, i, j)); 
        }
    }
////////////////////////////////////////////////////////////////////////////////

    NS_LOG_INFO ("Setup IPv4 Addresses");

    ns3::Ipv4AddressHelper server2switchIPs =
        ns3::Ipv4AddressHelper("10.1.0.0", "255.255.255.0");
    ns3::Ipv4AddressHelper switch2recieverIPs =
        ns3::Ipv4AddressHelper("10.2.0.0", "255.255.255.0");


    Ipv4InterfaceContainer serverIFs;
    Ipv4InterfaceContainer switchInIFs;
    Ipv4InterfaceContainer switchOutIFs;
    Ipv4InterfaceContainer recieverIFs;



    NS_LOG_INFO ("Install IPv4 addresses on all NetDevices");
    
    NS_LOG_INFO ("Configuring servers");

    for (int i = 0; i < SERVER_COUNT; i++)
    {
        NetDeviceContainer tempNetDevice;
        tempNetDevice.Add(serverDevices.Get(i));
        tempNetDevice.Add(switchDevicesIn.Get(i));
        Ipv4InterfaceContainer ifcServers = server2switchIPs.Assign(tempNetDevice);
        serverIFs.Add(ifcServers.Get(0));
        switchInIFs.Add(ifcServers.Get(1));

        server2switchIPs.NewNetwork ();
    }

    NS_LOG_INFO ("Configuring switch");

    for (int i = 0; i < RECIEVER_COUNT; i++)
    {
        NetDeviceContainer tempNetDevice;
        tempNetDevice.Add(switchDevicesOut.Get(i));
        tempNetDevice.Add(recieverDevices.Get(i));
        Ipv4InterfaceContainer ifcSwitch = switch2recieverIPs.Assign(tempNetDevice);
        switchOutIFs.Add(ifcSwitch.Get(0));
        recieverIFs.Add(ifcSwitch.Get(1));

        switch2recieverIPs.NewNetwork ();    
    }


    // and setup ip routing tables to get total ip-level connectivity.
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    NS_LOG_INFO ("Create Sockets, Applications and Sinks");

    uint32_t ipTos_HP = 0x10;  //High priority: Maximize Throughput
    uint32_t ipTos_LP1 = 0x00; //(Low) priority 0: Best Effort
    
    ApplicationContainer sinkApps, sourceApps;
    
    for (size_t i = 0; i < 2; i++)
    {
        int serverIndex = i;
        int recieverIndex = i;
        // create sockets
        ns3::Ptr<ns3::Socket> sockptr;

        if (transportProt.compare("TCP") == 0) 
        {
        // setup source socket
        sockptr =
            ns3::Socket::CreateSocket(servers.Get(serverIndex),
                    ns3::TcpSocketFactory::GetTypeId());
        ns3::Ptr<ns3::TcpSocket> tcpsockptr =
            ns3::DynamicCast<ns3::TcpSocket> (sockptr);
        } 
        else if (transportProt.compare("UDP") == 0) 
        {
        // setup source socket
        sockptr =
            ns3::Socket::CreateSocket(servers.Get(serverIndex),
                    ns3::UdpSocketFactory::GetTypeId());
            ////////Added by me///////////////        
            ns3::Ptr<ns3::UdpSocket> udpsockptr =
                ns3::DynamicCast<ns3::UdpSocket> (sockptr);
            //////////////////////////////////
        } 
        else 
        {
        std::cerr << "unknown transport type: " <<
            transportProt << std::endl;
        exit(1);
        }
        
        InetSocketAddress socketAddressP0 = InetSocketAddress (recieverIFs.GetAddress(recieverIndex), SERV_PORT_P0);
        socketAddressP0.SetTos(ipTos_HP);   // ToS 0x10 -> High priority
        InetSocketAddress socketAddressP1 = InetSocketAddress (recieverIFs.GetAddress(recieverIndex), SERV_PORT_P1);
        socketAddressP1.SetTos(ipTos_LP1);  // ToS 0x00 -> Low priority

        // time interval values for OnOff Aplications
        std::string miceOnTime = "0.05"; // [sec]
        std::string elephantOnTime = "0.5"; // [sec]
        std::string offTime = "0.1"; // [sec]
        
        // create and install Client apps:     
        if (applicationType.compare("prioOnOff") == 0) 
        {
          // Create the OnOff applications to send TCP/UDP to the server

          PrioOnOffHelper clientHelperP0 (socketType, socketAddressP0);
          clientHelperP0.SetAttribute ("Remote", AddressValue (socketAddressP0));
          // make On time shorter to make high priority flows shorter
          clientHelperP0.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=" + miceOnTime + "]"));
          clientHelperP0.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=" + offTime + "]"));
          clientHelperP0.SetAttribute ("PacketSize", UintegerValue (PACKET_SIZE));
          clientHelperP0.SetAttribute ("DataRate", StringValue ("2Mb/s"));
          // clientHelperP0.SetAttribute("NumOfPacketsHighPrioThreshold", UintegerValue (10)); // relevant only if "FlowPriority" NOT set by user
          clientHelperP0.SetAttribute("FlowPriority", UintegerValue (0x1));  // manualy set generated packets priority: 0x1 high, 0x2 low
          sourceApps.Add(clientHelperP0.Install (servers.Get(serverIndex)));

          PrioOnOffHelper clientHelperP1 (socketType, socketAddressP1);
          clientHelperP1.SetAttribute ("Remote", AddressValue (socketAddressP1));
          clientHelperP1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=" + elephantOnTime + "]"));
          clientHelperP1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=" + offTime + "]"));
          clientHelperP1.SetAttribute ("PacketSize", UintegerValue (PACKET_SIZE));
          clientHelperP1.SetAttribute ("DataRate", StringValue ("2Mb/s"));
          // clientHelperP1.SetAttribute("NumOfPacketsHighPrioThreshold", UintegerValue (10)); // relevant only if "FlowPriority" NOT set by user
          clientHelperP1.SetAttribute("FlowPriority", UintegerValue (0x2));  // manualy set generated packets priority: 0x1 high, 0x2 low
          sourceApps.Add(clientHelperP1.Install (servers.Get(serverIndex)));
        }
        else 
        {
            std::cerr << "unknown app type: " << applicationType << std::endl;
            exit(1);
        }
        // setup sinks
        Address sinkLocalAddressP0 (InetSocketAddress (Ipv4Address::GetAny (), SERV_PORT_P0));
        Address sinkLocalAddressP1 (InetSocketAddress (Ipv4Address::GetAny (), SERV_PORT_P1));
        PacketSinkHelper sinkP0 (socketType, sinkLocalAddressP0); // socketType is: "ns3::TcpSocketFactory" or "ns3::UdpSocketFactory"
        PacketSinkHelper sinkP1 (socketType, sinkLocalAddressP1); // socketType is: "ns3::TcpSocketFactory" or "ns3::UdpSocketFactory"
        sinkApps.Add(sinkP0.Install (recievers.Get(recieverIndex)));
        sinkApps.Add(sinkP1.Install (recievers.Get(recieverIndex)));    
    }

    sourceApps.Start (Seconds (1.0));
    sourceApps.Stop (Seconds(3.0));

    sinkApps.Start (Seconds (START_TIME));
    sinkApps.Stop (Seconds (END_TIME + 0.1));
    
    NS_LOG_INFO ("Enabling flow monitor");
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();
}

// Implement the other member functions for the class here (e.g., ConfigureTrafficControl)

















































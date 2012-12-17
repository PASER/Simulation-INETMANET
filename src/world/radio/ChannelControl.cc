/***************************************************************************
 * file:        ChannelControl.cc
 *
 * copyright:   (C) 2005 Andras Varga
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 **************************************************************************/


#include "ChannelControl.h"
#include "FWMath.h"
#include <cassert>

#include "AirFrame_m.h"

#define coreEV (ev.isDisabled()||!coreDebug) ? ev : ev << "ChannelControl: "

Define_Module(ChannelControl);


//add by eugen
    #include <map>

#ifdef RPS
    bool isInitialize=false;
    bool isStatic ;

    RPSSocket mySocket;   //RPS Socket

    long IDDUa[30]; // Dropped Units in 802.11a Bereich
    long IDDUg[30]; // Dropped Units in 802.11g Bereich
    long IDDUc[30];
    long IDMR[30]; // Clients
    long IDMRc[30];
    long IDC[100]; // Clients
    long groupID = 0;
    int cntDU=0, cntC=0, cntMR=0;
    //long BS1 = 0; long BS2 = 0; long MSID1 = 0; long MSID2 = 0;
    //int maxNode = 25;
    #define x_cord 380
    #define y_cord 170

    #define myRound(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

    map<long, map<long, double> > cache_table;
#endif
//end add

std::ostream& operator<<(std::ostream& os, const ChannelControl::RadioEntry& radio)
{
    os << radio.radioModule->getFullPath() << " (x=" << radio.pos.x << ",y=" << radio.pos.y << "), "
       << radio.neighbors.size() << " neighbor(s)";
    return os;
}

std::ostream& operator<<(std::ostream& os, const ChannelControl::TransmissionList& tl)
{
    for (ChannelControl::TransmissionList::const_iterator it = tl.begin(); it != tl.end(); ++it)
        os << endl << *it;
    return os;
}

ChannelControl::ChannelControl()
{
}

ChannelControl::~ChannelControl()
{
    for (unsigned int i = 0; i < transmissions.size(); i++)
        for (TransmissionList::iterator it = transmissions[i].begin(); it != transmissions[i].end(); it++)
            delete *it;
}

/**
 * Calculates maxInterferenceDistance.
 *
 * @ref calcInterfDist
 */
void ChannelControl::initialize()
{
#ifdef RPS
    isInitialize = true;
    //RPS Init start
    if (mySocket.createSocket())
        mySocket.connectSocket("129.217.186.10", 5200);//hier ist der Port des Servers auf der RPS Maschine
    mySocket.start();
    std::string RPS_scenario;
    std::string networkName (getParentModule()->getFullName());
    std::cout << "networkName: " << networkName << "\n";
    if(networkName.compare(0,1,"s") == 0){
        isStatic = true;
        RPS_scenario = "E:\\Paul\\rps\\static.rps";
    }
    else if(networkName.compare(0,1,"d") == 0){
        isStatic = false;
        RPS_scenario = "E:\\Paul\\rps\\dynamic.rps";
    }
    if(networkName.compare(0,15,"staticNetworkWH") == 0){
        isStatic = true;
        RPS_scenario = "E:\\Paul\\rps\\static_wh.rps";
    }
    if(networkName.compare(0,22,"staticNetworkIsolation") == 0){
        isStatic = true;
        RPS_scenario = "E:\\Paul\\rps\\static_isolation.rps";
    }
    if(networkName.compare(0,26,"staticNetworkImpersonation") == 0){
        isStatic = true;
        RPS_scenario = "E:\\Paul\\rps\\static_impersonation.rps";
    }
    if(networkName.compare(0,16,"dynamicNetworkWH") == 0){
        isStatic = false;
        RPS_scenario = "E:\\Paul\\rps\\dynamic_wh.rps";
    }
    if(networkName.compare(0,27,"dynamicNetworkImpersonation") == 0){
        isStatic = false;
        RPS_scenario = "E:\\Paul\\rps\\dynamic_impersonation.rps";
    }
    if(networkName.compare(0,23,"dynamicNetworkIsolation") == 0){
        isStatic = false;
        RPS_scenario = "E:\\Paul\\rps\\dynamic_isolation.rps";
    }
    if(networkName.compare(0,18,"Koelnmesse_dym_rps") == 0){
        isStatic = true;
        RPS_scenario = "E:\\Paul\\rps\\videodynamic2.rps";
    }
    if(networkName.compare(0,14,"Koelnmesse_rps") == 0){
        isStatic = true;
        RPS_scenario = "E:\\Paul\\rps\\videostatic.rps";
    }
    mySocket.open(RPS_scenario);
    mySocket.prepareSimulation();
    groupID = mySocket.GetMSGroupIDFromName("rxgroup");
    std::cout << "\n groupID: " << groupID << "\n";
    ev << "\n groupID: " << groupID << "\n";
    for (RadioList::iterator it = radios.begin(); it != radios.end(); it++)
    {
//        std::string tempname = it->name;
//        std::cout << "\nMeshrouter: " << it->name << "\n";
//        ev << "Meshrouter: " << it->name << "\n";
//        IDMR[cntMR] = mySocket.GetBSIDFromName(it->name);
//        std::cout << "meshrouter  BSSID: " << IDMR[cntMR] << "\n";
//        std::cout << "meshrouter OMNET pos.x: " << it->pos.x << ", pos.y: " << it->pos.y << "\n";
//        std::cout << "meshrouter RPS   pos.x: " << it->pos.x - x_cord << ", pos.y: " << -(it->pos.y - y_cord) << "\n";
//        ev << "meshrouter  BSSID: " << IDMR[cntMR] << "\n";
//        ev << "meshrouter OMNET pos.x: " << it->pos.x << ", pos.y: " << it->pos.y << "\n";
//        ev << "meshrouter RPS   pos.x: " << it->pos.x - x_cord << ", pos.y: " << -(it->pos.y - y_cord) << "\n";
//        if(isStatic && tempname.compare(0,1,"h") == 0){
//            IDMRc[cntMR] = mySocket.AddMS(it->pos.x - x_cord, -(it->pos.y - y_cord), 0.05, 1, 1, groupID);
//        }
//        else if(tempname.compare(0,1,"w") == 0){
//            IDMRc[cntMR] = mySocket.AddMS(it->pos.x - x_cord, -(it->pos.y - y_cord), 0.05, 1, 1, groupID);
//        }
//        else if((tempname.compare(0,3,"gw1") == 0) || (tempname.compare(0,9,"angreifer") == 0)){
//            IDMRc[cntMR] = mySocket.AddMS(it->pos.x - x_cord, -(it->pos.y - y_cord), 0.05, 1, 1, groupID);
//        }
//        else if(tempname.compare(0,9,"host_send") == 0){
//            IDMRc[cntMR] = mySocket.AddMS(it->pos.x - x_cord, -(it->pos.y - y_cord), 30, 1, 1, groupID);
//        }
//        else if(tempname.compare(0,12,"host_no_send") == 0){
//            IDMRc[cntMR] = mySocket.AddMS(it->pos.x - x_cord, -(it->pos.y - y_cord), 18, 1, 1, groupID);
//        }
//        else if(tempname.compare(0,1,"g") == 0){
//            IDMRc[cntMR] = mySocket.AddMS(it->pos.x - x_cord, -(it->pos.y - y_cord), 0.05, 1, 1, groupID);
//        }
//        else if(tempname.compare(0,1,"e") == 0){
//            IDMRc[cntMR] = mySocket.AddMS(it->pos.x - x_cord, -(it->pos.y - y_cord), 0.05, 1, 1, groupID);
//        }
//        else if(tempname.compare(0,1,"d") == 0){
//            IDMRc[cntMR] = mySocket.AddMS(it->pos.x - x_cord, -(it->pos.y - y_cord), 0.05, 1, 1, groupID);
//        }
//        else{
//            opp_error("UNKNOW NODE");
//        }
//        mySocket.moveBS(IDMR[cntMR], it->pos.x - x_cord, -(it->pos.y - y_cord));
//        it->RPSIDcnt = cntMR;
//        cntMR++;
//        it->isClient = false;
//        it->isMR = true;
    }
    std::cout << "\n RadioList end \n";
    ev << "\n RadioList end \n";
//    opp_error("BBB");
    // mySocket.run();
#endif
    //end add

    coreDebug = hasPar("coreDebug") ? (bool) par("coreDebug") : false;

    coreEV << "initializing ChannelControl\n";

    numChannels = par("numChannels");
    transmissions.resize(numChannels);

    lastOngoingTransmissionsUpdate = 0;

    maxInterferenceDistance = calcInterfDist();

    WATCH(maxInterferenceDistance);
    WATCH_LIST(radios);
    WATCH_VECTOR(transmissions);
}

/**
 * Calculation of the interference distance based on the transmitter
 * power, wavelength, pathloss coefficient and a threshold for the
 * minimal receive Power
 *
 * You may want to overwrite this function in order to do your own
 * interference calculation
 */
double ChannelControl::calcInterfDist()
{
    double interfDistance;

    //the carrier frequency used
    double carrierFrequency = par("carrierFrequency");
    //maximum transmission power possible
    double pMax = par("pMax");
    //signal attenuation threshold
    double sat = par("sat");
    //path loss coefficient
    double alpha = par("alpha");

    double waveLength = (SPEED_OF_LIGHT / carrierFrequency);
    //minimum power level to be able to physically receive a signal
    double minReceivePower = pow(10.0, sat / 10.0);

    interfDistance = pow(waveLength * waveLength * pMax /
                         (16.0 * M_PI * M_PI * minReceivePower), 1.0 / alpha);

    coreEV << "max interference distance:" << interfDistance << endl;

    return interfDistance;
}

ChannelControl::RadioRef ChannelControl::registerRadio(cModule *radio, cGate *radioInGate)
{
    Enter_Method_Silent();

    RadioRef radioRef = lookupRadio(radio);

    if (radioRef)
        throw cRuntimeError("Radio %s already registered", radio->getFullPath().c_str());
ev << radio->getFullName() << "\n";
    if (!radioInGate)
        radioInGate = radio->gate("radioIn");

    RadioEntry re;
    //add by eugen
#ifdef RPS
    re.name = radio->getParentModule()->getParentModule()->getFullName();

    std::string tempname = re.name;
    std::cout << "\nMeshrouter: " << re.name << "\n";
    ev << "Meshrouter: " << re.name << "\n";
    IDMR[cntMR] = mySocket.GetBSIDFromName(re.name);
    std::cout << "meshrouter  BSSID: " << IDMR[cntMR] << "\n";
    std::cout << "meshrouter OMNET pos.x: " << 0 << ", pos.y: " << 0 << "\n";
    std::cout << "meshrouter RPS   pos.x: " << 0 - x_cord << ", pos.y: " << -(0 - y_cord) << "\n";
    ev << "meshrouter  BSSID: " << IDMR[cntMR] << "\n";
    ev << "meshrouter OMNET pos.x: " << 0 << ", pos.y: " << 0 << "\n";
    ev << "meshrouter RPS   pos.x: " << 0 - x_cord << ", pos.y: " << -(0 - y_cord) << "\n";
    if(isStatic && tempname.compare(0,1,"h") == 0){
        IDMRc[cntMR] = mySocket.AddMS(0 - x_cord, -(0 - y_cord), 0.05, 1, 1, groupID);
    }
    else if(tempname.compare(0,1,"w") == 0){
        IDMRc[cntMR] = mySocket.AddMS(0 - x_cord, -(0 - y_cord), 0.05, 1, 1, groupID);
    }
    else if((tempname.compare(0,3,"gw1") == 0) || (tempname.compare(0,9,"angreifer") == 0)){
        IDMRc[cntMR] = mySocket.AddMS(0 - x_cord, -(0 - y_cord), 0.05, 1, 1, groupID);
    }
    else if(tempname.compare(0,9,"host_send") == 0){
        IDMRc[cntMR] = mySocket.AddMS(0 - x_cord, -(0 - y_cord), 30, 1, 1, groupID);
    }
    else if(tempname.compare(0,12,"host_no_send") == 0){
        IDMRc[cntMR] = mySocket.AddMS(0 - x_cord, -(0 - y_cord), 18, 1, 1, groupID);
    }
    else if(tempname.compare(0,1,"g") == 0){
        IDMRc[cntMR] = mySocket.AddMS(0 - x_cord, -(0 - y_cord), 0.05, 1, 1, groupID);
    }
    else if(tempname.compare(0,1,"e") == 0){
        IDMRc[cntMR] = mySocket.AddMS(0 - x_cord, -(0 - y_cord), 0.05, 1, 1, groupID);
    }
    else if(tempname.compare(0,1,"d") == 0){
        IDMRc[cntMR] = mySocket.AddMS(0 - x_cord, -(0 - y_cord), 0.05, 1, 1, groupID);
    }
    else{
        opp_error("UNKNOW NODE");
    }
    mySocket.moveBS(IDMR[cntMR], 0 - x_cord, -(0 - y_cord));
    re.RPSIDcnt = cntMR;
    cntMR++;
    re.isClient = false;
    re.isMR = true;

#endif
    //end add
    re.radioModule = radio;
    re.radioInGate = radioInGate->getPathStartGate();
    re.isNeighborListValid = false;
    re.channel = 0;  // for now
    re.isActive = true;
    radios.push_back(re);
    return &radios.back(); // last element
}

void ChannelControl::unregisterRadio(RadioRef r)
{
    Enter_Method_Silent();
    for (RadioList::iterator it = radios.begin(); it != radios.end(); it++)
    {
        if (it->radioModule == r->radioModule)
        {
            RadioRef radioToRemove = &*it;
            // erase radio from all registered radios' neighbor list
            for (RadioList::iterator i2 = radios.begin(); i2 != radios.end(); ++i2)
            {
                RadioRef otherRadio = &*i2;
                otherRadio->neighbors.erase(radioToRemove);
                otherRadio->isNeighborListValid = false;
                radioToRemove->isNeighborListValid = false;
            }

            // erase radio from registered radios
            radios.erase(it);
            return;
        }
    }

    error("unregisterRadio failed: no such radio");
}

ChannelControl::RadioRef ChannelControl::lookupRadio(cModule *radio)
{
    Enter_Method_Silent();
    for (RadioList::iterator it = radios.begin(); it != radios.end(); it++)
        if (it->radioModule == radio)
            return &(*it);
    return 0;
}

const ChannelControl::RadioRefVector& ChannelControl::getNeighbors(RadioRef h)
{
    Enter_Method_Silent();
    if (!h->isNeighborListValid)
    {
        h->neighborList.clear();
        for (std::set<RadioRef,RadioEntry::Compare>::iterator it = h->neighbors.begin(); it != h->neighbors.end(); it++)
            h->neighborList.push_back(*it);
        h->isNeighborListValid = true;
    }
    return h->neighborList;
}

void ChannelControl::updateConnections(RadioRef h)
{
    Coord& hpos = h->pos;
    double maxDistSquared = maxInterferenceDistance * maxInterferenceDistance;

    //add by eugen
    ev << "maxDistSquared = " << maxDistSquared << "\n";
    bool isSendJustToNeighbor = par("SendJustToNeighbor");
    if(isSendJustToNeighbor){
        double NeighborRadius = par("NeighborRadius");
        maxDistSquared = NeighborRadius * NeighborRadius;
    }
    //end add

    for (RadioList::iterator it = radios.begin(); it != radios.end(); ++it)
    {
        RadioEntry *hi = &(*it);
        if (hi == h)
            continue;

        // get the distance between the two radios.
        // (omitting the square root (calling sqrdist() instead of distance()) saves about 5% CPU)
        bool inRange = hpos.sqrdist(hi->pos) < maxDistSquared;

        if (inRange)
        {
            // nodes within communication range: connect
            if (h->neighbors.insert(hi).second == true)
            {
                hi->neighbors.insert(h);
                h->isNeighborListValid = hi->isNeighborListValid = false;
            }
        }
        else
        {
            // out of range: disconnect
            if (h->neighbors.erase(hi))
            {
                hi->neighbors.erase(h);
                h->isNeighborListValid = hi->isNeighborListValid = false;
            }
        }
    }
}

void ChannelControl::checkChannel(int channel)
{
    if (channel >= numChannels || channel < 0)
        error("Invalid channel, must above 0 and below %d", numChannels);
}

void ChannelControl::setRadioPosition(RadioRef r, const Coord& pos)
{
    //add by Eugen
#ifdef RPS
//    if(isInitialize)
        updateHostPosition(r, pos);
#endif
    //end add
    Enter_Method_Silent();
    r->pos = pos;
    updateConnections(r);
}

void ChannelControl::setRadioChannel(RadioRef r, int channel)
{
    Enter_Method_Silent();
    checkChannel(channel);

    r->channel = channel;
}

const ChannelControl::TransmissionList& ChannelControl::getOngoingTransmissions(int channel)
{
    Enter_Method_Silent();

    checkChannel(channel);
    purgeOngoingTransmissions();
    return transmissions[channel];
}

void ChannelControl::addOngoingTransmission(RadioRef h, AirFrame *frame)
{
    Enter_Method_Silent();

    // we only keep track of ongoing transmissions so that we can support
    // NICs switching channels -- so there's no point doing it if there's only
    // one channel
    if (numChannels == 1)
    {
        delete frame;
        return;
    }

    // purge old transmissions from time to time
    if (simTime() - lastOngoingTransmissionsUpdate > TRANSMISSION_PURGE_INTERVAL)
    {
        purgeOngoingTransmissions();
        lastOngoingTransmissionsUpdate = simTime();
    }

    // register ongoing transmission
    take(frame);
    frame->setTimestamp(); // store time of transmission start
    transmissions[frame->getChannelNumber()].push_back(frame);
}

void ChannelControl::purgeOngoingTransmissions()
{
    for (int i = 0; i < numChannels; i++)
    {
        for (TransmissionList::iterator it = transmissions[i].begin(); it != transmissions[i].end();)
        {
            TransmissionList::iterator curr = it;
            AirFrame *frame = *it;
            it++;

            if (frame->getTimestamp() + frame->getDuration() + TRANSMISSION_PURGE_INTERVAL < simTime())
            {
                delete frame;
                transmissions[i].erase(curr);
            }
        }
    }
}

void ChannelControl::sendToChannel(RadioRef srcRadio, AirFrame *airFrame)
{
    // NOTE: no Enter_Method()! We pretend this method is part of ChannelAccess

    // loop through all radios in range
    const RadioRefVector& neighbors = getNeighbors(srcRadio);
    int n = neighbors.size();
    int channel = airFrame->getChannelNumber();
    //add by eugen
    cModule *targetModule = getParentModule()->getSubmodule("sendcontrol");
    if(targetModule){
        check_and_cast<cSimpleModule*>(srcRadio->radioModule)->sendDirect(new cMessage(check_and_cast<cSimpleModule*>(srcRadio->radioModule)->getParentModule()->getParentModule()->getFullName()), targetModule, "in");
    }
    //end add
    for (int i=0; i<n; i++)
    {
        RadioRef r = neighbors[i];
        if (!r->isActive)
        {
            coreEV << "skipping disabled radio interface \n";
            continue;
        }
        if (r->channel == channel)
        {
            coreEV << "sending message to radio listening on the same channel\n";
            // account for propagation delay, based on distance in meters
            // Over 300m, dt=1us=10 bit times @ 10Mbps
            simtime_t delay = srcRadio->pos.distance(r->pos) / LIGHT_SPEED;
            check_and_cast<cSimpleModule*>(srcRadio->radioModule)->sendDirect(airFrame->dup(), delay, airFrame->getDuration(), r->radioInGate);
        }
        else
            coreEV << "skipping radio listening on a different channel\n";
    }

    // register transmission
    addOngoingTransmission(srcRadio, airFrame);
}


//add by eugen
#ifdef RPS
// Added Methods
double ChannelControl::getRcvdPower(Coord receiverPos, Coord senderPos)
{
    double rcvdPower = 0;

    //mySocket.moveBS(BS1,senderPos.x,senderPos.y);
    //mySocket.moveMS(MSID1,receiverPos.x,receiverPos.y);
 // doppelt gemoppelt ?!
    // return sum of power values related to the BS with index BSID;
    EV<<"sender position"<<senderPos.x<<" "<<senderPos.y<<"\n";

    int cntR;
    int cntS;
    bool isMR = false;
    bool isClient = false;
    bool senderIsClient = false;
    bool senderIsMR = false;
    for (RadioList::iterator it = radios.begin(); it != radios.end(); it++)
    {
        if (it->pos==receiverPos)
        {
            cntR = it->RPSIDcnt;
            isClient = it->isClient;
            isMR = it->isMR;
            EV<<"receiver module: "<<it->name<<"\n";

        }

        else if(it->pos == senderPos)
        {
            EV<<"sender module: "<<it->name<<"\n";
            EV<<"sender position: "<<it->pos.x<<" "<<it->pos.y<<"\n";
            cntS = it->RPSIDcnt;
            if(it->isClient)
            senderIsClient = true;
            if(it->isMR)
            senderIsMR = true;

        }

    }
    //  rcvdPower = mySocket.calculatePower(receiverID,groupID,BS1,receiverPos.x,receiverPos.y,senderPos.x,senderPos.y);
    if(isClient)
    {
        //   if(senderIsClient)
        //          rcvdPower = mySocket.calculateReceivedpower(IDC[cntR],IDC[cntS]);
        //      else
        if(senderIsMR)
        {
            rcvdPower=-130;
            EV << "ChannelControlExtended-RcvdPower-Client-Sender-Meshrouter in dbm" << rcvdPower << "\n";
        }
        else {
            map<long,map<long,double> >::iterator it;
            it = cache_table.find(IDC[cntR]);
            if(it != cache_table.end()){
                map<long,double>::iterator it2;
                it2 = ((*it).second).find(IDDUg[cntS]);
                if(it2!=((*it).second).end()){
                    rcvdPower = (*it2).second;
                    EV << "ChannelControlExtended-RcvdPower-Client-Sender-DU in dbm" << rcvdPower << " from cache\n";
                }
                else{
                    rcvdPower = mySocket.calculateReceivedpower(IDC[cntR],IDDUg[cntS]);
                    EV << "ChannelControlExtended-RcvdPower-Client-Sender-DU in dbm" << rcvdPower << " ohne cache\n";

                    ((*it).second).insert(make_pair(IDDUg[cntS],rcvdPower));
                }
            }
            else{
                rcvdPower = mySocket.calculateReceivedpower(IDC[cntR],IDDUg[cntS]);
                EV << "ChannelControlExtended-RcvdPower-Client-Sender-DU in dbm" << rcvdPower << "\n";

                map<long,double> temp;
                temp.insert(make_pair(IDDUg[cntS],rcvdPower));
                cache_table.insert(make_pair(IDC[cntR], temp));
            }
        }
    }
    else
    {

        if(senderIsClient)
        {
            if(isMR)
            {
                rcvdPower=-130;
                EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-Client in dbm" << rcvdPower << "\n";
            }
            else
            {
                map<long,map<long,double> >::iterator it;
                it = cache_table.find(IDC[cntS]);
                if(it != cache_table.end()){
                    map<long,double>::iterator it2;
                    it2 = ((*it).second).find(IDDUg[cntR]);
                    if(it2!=((*it).second).end()){
                        rcvdPower = (*it2).second;
                        EV << "ChannelControlExtended-RcvdPower-DroppedUnit-Sender-client in dbm" << rcvdPower << " from cache\n";
                    }
                    else{
                        rcvdPower = mySocket.calculateReceivedpower(IDC[cntS],IDDUg[cntR]);
                        EV << "ChannelControlExtended-RcvdPower-DroppedUnit-Sender-client in dbm" << rcvdPower << " ohne cache\n";

                        ((*it).second).insert(make_pair(IDDUg[cntR],rcvdPower));
                    }
                }
                else{
                    rcvdPower = mySocket.calculateReceivedpower(IDC[cntS],IDDUg[cntR]);
                    EV << "ChannelControlExtended-RcvdPower-DroppedUnit-Sender-client in dbm" << rcvdPower << "\n";

                    map<long,double> temp;
                    temp.insert(make_pair(IDDUg[cntR],rcvdPower));
                    cache_table.insert(make_pair(IDC[cntS], temp));
                }

//                EV << "ChannelControlExtended-RcvdPower-DroppedUnit-Sender-client in dbm" << rcvdPower << "\n";
//                rcvdPower = mySocket.calculateReceivedpower(IDC[cntS],IDDUg[cntR]);
            }
        }
        else
        {
            if(isMR && !senderIsMR)
            {
                map<long,map<long,double> >::iterator it;
                it = cache_table.find(IDMRc[cntR]);
                if(it != cache_table.end()){
                    map<long,double>::iterator it2;
                    it2 = ((*it).second).find(IDDUa[cntS]);
                    if(it2!=((*it).second).end()){
                        rcvdPower = (*it2).second;
                        EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-DroppedUnit in dbm" << rcvdPower << " from cache\n";
                    }
                    else{
                        rcvdPower = mySocket.calculateReceivedpower(IDMRc[cntR],IDDUa[cntS]);
                        EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-DroppedUnit in dbm" << rcvdPower << " ohne cache\n";

                        ((*it).second).insert(make_pair(IDDUa[cntS],rcvdPower));
                    }
                }
                else{
                    rcvdPower = mySocket.calculateReceivedpower(IDMRc[cntR],IDDUa[cntS]);
                    EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-DroppedUnit in dbm" << rcvdPower << "\n";

                    map<long,double> temp;
                    temp.insert(make_pair(IDDUa[cntS],rcvdPower));
                    cache_table.insert(make_pair(IDMRc[cntR], temp));
                }
//                rcvdPower = mySocket.calculateReceivedpower(IDMRc[cntR],IDDUa[cntS]);
//                EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-DroppedUnit in dbm" << rcvdPower << "\n";
            }
            else if(isMR && senderIsMR)
            {
                map<long,map<long,double> >::iterator it;
                it = cache_table.find(IDMRc[cntR]);
                if(it != cache_table.end()){
                    map<long,double>::iterator it2;
                    it2 = ((*it).second).find(IDMR[cntS]);
                    if(it2!=((*it).second).end()){
                        rcvdPower = (*it2).second;
                        EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-MeshRouter in dbm" << rcvdPower << " from cache\n";
                    }
                    else{
                        rcvdPower = mySocket.calculateReceivedpower(IDMRc[cntR],IDMR[cntS]);
                        EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-MeshRouter in dbm" << rcvdPower << " ohne cache\n";

                        ((*it).second).insert(make_pair(IDMR[cntS],rcvdPower));
                    }
                }
                else{
                    rcvdPower = mySocket.calculateReceivedpower(IDMRc[cntR],IDMR[cntS]);
                    EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-MeshRouter in dbm" << rcvdPower << "\n";

                    map<long,double> temp;
                    temp.insert(make_pair(IDMR[cntS],rcvdPower));
                    cache_table.insert(make_pair(IDMRc[cntR], temp));
                }
//                rcvdPower = mySocket.calculateReceivedpower(IDMRc[cntR],IDMR[cntS]);
//                EV << "ChannelControlExtended-RcvdPower-MeshRouter-Sender-MeshRouter in dbm" << rcvdPower << "\n";
            }
            else if(!isMR && senderIsMR)
            {
                map<long,map<long,double> >::iterator it;
                it = cache_table.find(IDDUc[cntR]);
                if(it != cache_table.end()){
                    map<long,double>::iterator it2;
                    it2 = ((*it).second).find(IDMR[cntS]);
                    if(it2!=((*it).second).end()){
                        rcvdPower = (*it2).second;
                        EV << "ChannelControlExtended-RcvdPower-Dropped Unit-Sender-MeshRouter in dbm" << rcvdPower << " from cache\n";
                    }
                    else{
                        rcvdPower = mySocket.calculateReceivedpower(IDDUc[cntR],IDMR[cntS]);
                        EV << "ChannelControlExtended-RcvdPower-Dropped Unit-Sender-MeshRouter in dbm" << rcvdPower << " ohne cache\n";

                        ((*it).second).insert(make_pair(IDMR[cntS],rcvdPower));
                    }
                }
                else{
                    rcvdPower = mySocket.calculateReceivedpower(IDDUc[cntR],IDMR[cntS]);
                    EV << "ChannelControlExtended-RcvdPower-Dropped Unit-Sender-MeshRouter in dbm" << rcvdPower << "\n";

                    map<long,double> temp;
                    temp.insert(make_pair(IDMR[cntS],rcvdPower));
                    cache_table.insert(make_pair(IDDUc[cntR], temp));
                }
//                rcvdPower = mySocket.calculateReceivedpower(IDDUc[cntR],IDMR[cntS]);
//                EV << "ChannelControlExtended-RcvdPower-Dropped Unit-Sender-MeshRouter in dbm" << rcvdPower << "\n";
            }
            else {
                map<long,map<long,double> >::iterator it;
                it = cache_table.find(IDDUc[cntR]);
                if(it != cache_table.end()){
                    map<long,double>::iterator it2;
                    it2 = ((*it).second).find(IDDUa[cntS]);
                    if(it2!=((*it).second).end()){
                        rcvdPower = (*it2).second;
                        EV << "ChannelControlExtended-RcvdPower-Dropped Unit-Sender-Dropped Unit in dbm" << rcvdPower << " from cache\n";
                    }
                    else{
                        rcvdPower = mySocket.calculateReceivedpower(IDDUc[cntR],IDDUa[cntS]);
                        EV << "ChannelControlExtended-RcvdPower-Dropped Unit-Sender-Dropped Unit in dbm" << rcvdPower << " ohne cache\n";

                        ((*it).second).insert(make_pair(IDDUa[cntS],rcvdPower));
                    }
                }
                else{
                    rcvdPower = mySocket.calculateReceivedpower(IDDUc[cntR],IDDUa[cntS]);
                    EV << "ChannelControlExtended-RcvdPower-Dropped Unit-Sender-Dropped Unit in dbm" << rcvdPower << "\n";

                    map<long,double> temp;
                    temp.insert(make_pair(IDDUa[cntS],rcvdPower));
                    cache_table.insert(make_pair(IDDUc[cntR], temp));
                }
//                rcvdPower = mySocket.calculateReceivedpower(IDDUc[cntR],IDDUa[cntS]);
//                EV << "ChannelControlExtended-RcvdPower-Dropped Unit-Sender-Dropped Unit in dbm" << rcvdPower << "\n";

            }
        }
    }
    if(rcvdPower == 0)
    {
        rcvdPower = -130;
    }

    double wert = (pow(10,rcvdPower/10)); // milliWatt
    EV << "ChannelControlExtended-RcvdPower in mw" << wert << "\n";
    return wert;

}

void ChannelControl::updateHostPosition(RadioRef h, const Coord& pos)
{
    Enter_Method_Silent();
    std::cout<<"\nEntering updatehostposition\n";


    cache_table.clear();

    int cnt=-1;
    bool isClient=false;
    bool isMR=false;
    for (RadioList::iterator it = radios.begin(); it != radios.end(); it++)
    {
        if (it->pos== h->pos)
        {
            cnt = it->RPSIDcnt;
            isClient = it->isClient;
            isMR = it->isMR;
            std::cout<<"module name "<<it->name<<"\n";
            std::cout<<"cnt "<<cnt<<"\n";
            std::cout<<"ipdate position "<<h->pos.x<<", "<<h->pos.y<<"\n";
            break;
        }
    }
    if (cnt==-1) error("cnt=-1\n");
//it->pos.x - x_cord, -(it->pos.y - y_cord)
    if (isClient)
    {
        std::cout<<"Client update OMNET pos pos.x: "<<pos.x<<", pos.y: "<<pos.y<<"\n";
        std::cout<<"Client update RPS   pos pos.x: "<<pos.x - x_cord<<", pos.y: "<<-(pos.y - y_cord)<<"\n";
        std::cout<<"Client update RPS after Round   pos pos.x: "<<myRound(pos.x - x_cord)<<", pos.y: "<<myRound(-(pos.y - y_cord))<<"\n";
        mySocket.moveMS(IDC[cnt],myRound(pos.x - x_cord),myRound(-(pos.y - y_cord)));
    }
    else if(isMR)
    {
        std::cout<<"update MEsh Router OMNET pos pos.x: "<<pos.x<<", pos.y: "<<pos.y<<"\n";
        std::cout<<"update MEsh Router RPS   pos pos.x: "<<pos.x - x_cord<<", pos.y: "<<-(pos.y - y_cord)<<"\n";
        std::cout<<"update MEsh Router RPS after Round   pos pos.x: "<<myRound(pos.x - x_cord)<<", pos.y: "<<myRound(-(pos.y - y_cord))<<"\n";
        mySocket.moveBS(IDMR[cnt],myRound(pos.x - x_cord),myRound(-(pos.y - y_cord)));
        mySocket.moveMS(IDMRc[cnt],myRound(pos.x - x_cord),myRound(-(pos.y - y_cord)));
    }
    else
    {
        std::cout<<"update DU OMNET pos pos.x: "<<pos.x<<", pos.y: "<<pos.y<<"\n";
        std::cout<<"update DU RPS   pos pos.x: "<<pos.x - x_cord<<", pos.y: "<<-(pos.y - y_cord)<<"\n";
        std::cout<<"update DU RPS after Round   pos pos.x: "<<myRound(pos.x - x_cord)<<", pos.y: "<<myRound(-(pos.y - y_cord))<<"\n";
        mySocket.moveBS(IDDUg[cnt],myRound(pos.x - x_cord),myRound(-(pos.y - y_cord)));
        mySocket.moveBS(IDDUa[cnt],myRound(pos.x - x_cord),myRound(-(pos.y - y_cord)));
        mySocket.moveMS(IDDUc[cnt],myRound(pos.x - x_cord),myRound(-(pos.y - y_cord)));
    }
    mySocket.run();
    h->pos = pos;
    updateConnections(h);
}
#endif
//end add

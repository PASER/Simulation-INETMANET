/*
 * manetStats.cc
 *
 *  Created on: 11.10.2011
 *      Author: Eugen Paul
 */

#include "manetStats.h"

manetStats* manetStats::instance = 0;

manetStats::manetStats(){
    packetSend = 0;
    packetReceived = 0;

    totalBroadcastSend = 0;
    totalUnicastSend = 0;

    totalSendProtokollBytes = 0;
}

manetStats::~manetStats(){

}

void manetStats::setPacketSend(int i){
    packetSend = i;
}
void manetStats::setPacketReceived(int i){
    packetReceived = i;
}
void manetStats::setPacketBroadcastSend(int i){
    totalBroadcastSend = i;
}
void manetStats::setPacketUnicastSend(int i){
    totalUnicastSend = i;
}
void manetStats::setTotalSendProtokollBytes(uint i){
    totalSendProtokollBytes = i;
}

void manetStats::incPacketSend(){
    packetSend++;
}

void manetStats::incPacketSendToDest(std::string destName){
    if(mapPacketSend.find(destName) != mapPacketSend.end()){
        std::map<std::string,int>::iterator it=mapPacketSend.find(destName);
        it->second = it->second+1;
    }
    else{
        mapPacketSend.insert(std::make_pair<std::string,int>(destName,1));
    }
    incPacketSend();
}

void manetStats::incPacketReceived(){
    packetReceived++;
}
void manetStats::incPacketBroadcastSend(){
    totalBroadcastSend++;
}
void manetStats::incPacketUnicastSend(){
    totalUnicastSend++;
}

void manetStats::addToTotalSendProtokollBytes(u_int32_t i){
    totalSendProtokollBytes+=i;
}

int manetStats::getPacketSend(){
    return packetSend;
}
int manetStats::getPacketSendToDest(std::string destName){
    if(mapPacketSend.find(destName) != mapPacketSend.end()){
        std::map<std::string,int>::iterator it=mapPacketSend.find(destName);
        return it->second;
    }
    else{
        return 0;
    }
}
int manetStats::getPacketReceived(){
    return packetReceived;
}
int manetStats::getPacketBroadcastSend(){
    return totalBroadcastSend;
}
int manetStats::getPacketUnicastSend(){
    return totalUnicastSend;
}
uint manetStats::getTotalSendProtokollBytes(){
    return totalSendProtokollBytes;
}

manetStats* manetStats::getInstance(){
    if(instance == 0){
        instance = new manetStats();
    }
    return instance;
}

void manetStats::destroy()
{
   if ( instance )
      delete instance;
   instance = 0;
}


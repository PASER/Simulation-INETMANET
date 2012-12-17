/*
 * manetStats.h
 *
 *  Created on: 11.10.2011
 *      Author: Eugen Paul
 */

#ifndef MANETSTATS_H_
#define MANETSTATS_H_

#include <map>

#include "compatibility.h"

class manetStats {

public:
    ~manetStats();

    static manetStats *getInstance();
    static void destroy();

    void setPacketSend(int i);
    void setPacketReceived(int i);
    void setPacketBroadcastSend(int i);
    void setPacketUnicastSend(int i);
    void setTotalSendProtokollBytes(u_int32_t i);

    void incPacketSend();
    void incPacketSendToDest(std::string destName);
    void incPacketReceived();
    void incPacketBroadcastSend();
    void incPacketUnicastSend();

    void addToTotalSendProtokollBytes(u_int32_t i);

    int getPacketSendToDest(std::string destName);
    int getPacketSend();
    int getPacketReceived();
    int getPacketBroadcastSend();
    int getPacketUnicastSend();
    u_int32_t getTotalSendProtokollBytes();

private:
    std::map<std::string,int> mapPacketSend;

    int packetSend;
    int packetReceived;

    int totalBroadcastSend;
    int totalUnicastSend;

    u_int32_t totalSendProtokollBytes;

private:
    static manetStats* instance;

    manetStats();

};

#endif /* MANETSTATS_H_ */

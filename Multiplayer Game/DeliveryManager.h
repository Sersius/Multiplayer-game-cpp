#pragma once

// TODO(you): Reliability on top of UDP lab session

#include <list>
class DeliveryManager;

struct Delivery
{
	uint32 sequenceNumber = 0;
	double dispatchTime = 0.0;
	
};



class DeliveryManager
{
public:
	
	Delivery* WriteSequenceNumber(OutputMemoryStream& packet);

	
	bool ProcessSequenceNumber(const InputMemoryStream& packet);

	
	bool HasSequenceNumbersPendingAck() const;
	void WriteSequenceNumbersPendingAck(OutputMemoryStream& packet);

	void ProcessAckdSequenceNumbers(const InputMemoryStream& packet);
	void ProcessTimedOutPackets();

	void Clear();

private:


	
	uint32 nextSeqNumExpected = 0u;

	std::list<uint32> pendingNumSeqList;

	
	uint32 nextSeqNum = 0u;
public:

	std::list<Delivery*> pendingDeliveries;
};

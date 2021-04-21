#include "mem/RedPro/redpro.hh"
#include "debug/RedProDebug.hh"

RedPro::RedPro(RedProParams *params) :
    SimObject(params),
    latency(params->latency),
    times(0),
    event([this]{processEvent();}, name()+".event"),
    instPort(name() + ".inst_port", this),
    dataPort(name() + ".data_port", this),
    memPort (name() + ".mem_side", this)
{
    // std::cout << "Hello World! From a SimObject!" << std::endl;
    DPRINTF(RedProDebug, "See this debu output with --debug-flags=RedProDebug")
}

BaseMasterPort&
RedPro::getMasterPort(const std::string& if_name, PortID idx)
{
    if(if_name == "mem_side") return memPort;
    else return MemObject::getMasterPort(if_name, idx);
}

BaseSlavePort&
RedPro::getSlavePort(const std::string& if_name, PortID idx)
{
    if(if_name == "inst_port") return instPort;
    else if(if_name == "data_port") return dataPort;
    else return MemObject::getSlavePort(if_name, idx);
}

/* 
 * Pass call to getAddrRanges() from CPU to the memory bus
 */
AddrRangeList
RedPro::CPUSidePort::getAddrRanges() const
{
	DPRINTF(RedPro, "Sending new ranges from CPU side\n");
    return owner->getAddrRanges();
}

AddrRangeList
RedPro::getAddrRanges() const
{
	DPRINTF(RedPro, "Sending new ranges to mem side\n");
	return memPort.getAddrRanges();
}

/* Pass call to recvFunctional from CPU to the memory bus's sendFunctional */
void
RedPro::CPUSidePort::recvFunctional(PacketPtr pkt)
{
	DPRINTF(RedPro, "Handle functional from CPU ");
    return owner->handleFunctional(pkt);
}

void
RedPro::handleFunctional(PacketPtr pkt)
{
	DPRINTF(RedPro, "Handle functional to mem side");
	memPort.sendFunctional(pkt);
}

void
RedPro::MemSidePort::recvRangeChange()
{
	DPRINTF(RedPro, "range change from mem side");
	owner->sendRangeChange();
}

void
RedPro::sendRangeChange()
{
	DPRINTF(RedPro, "range change to cpu");
	instPort.sendRangeChange();
	dataPort.sendRangeChange();
}

/* Check if the request can be handled, return accordingly */
bool
RedPro::CPUSidePort::recvTimingReq(PacketPtr pkt)
{
	if (!owner->handleRequest(pkt)) {
		needRetry = true;
		return false;
	} else {
		return true;
	}
}

/*
 * handle the request;
 * prob gonna make a change here 
 */
bool
RedPro::handleRequest(PacketPtr pkt)
{
	if (blocked) {
		return false;
	}

	DPRINTF(RedPro, "Got request for addr %#x\n", pkt->getAddr());
	blocked = true;
	memPort.sendPacket(pkt); // TODO: do stuff
	return true;
}

void
RedPro::startup()
{
    schedule(event, latency);
}

RedPro*
RedProParams::create()
{
    return new RedPro(this);
}

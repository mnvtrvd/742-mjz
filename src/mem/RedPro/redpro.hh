#ifndef __REDPRO_HH__
#define __REDPRO_HH__

#include "params/RedPro.hh"
#include "mem/mem_object.hh"

class RedPro : public MemObject
{
    Tick latency;
    int times;

    void processEvent();

    EventFunctionWrapper event;

    // We are a slave to the CPU
    class CPUSidePort : public SlavePort
    {
        private:
            RedPro *owner;

        public:
            CPUSidePort(const std::string& name, RedPro *owner) :
                SlavePort(name, owner), owner(owner) { }

            AddrRangeList getAddrRanges() const override;

        protected:
            Tick recvAtomic(PacketPtr pkt) override {
                panic("recvAtomic unimpl.");
            }

            void recvFunctional(PacketPtr pkt) override;
            bool recvTimingReq(PacketPtr pkt) override;
            void recvRespRetry() override;
    }

    // We are a master to the memory bus
    class MemSidePort : public MasterPort
    {
		private:
			RedPro *owner;

        public:
            MemSidePort(const std::string& name, RedPro *owner) :
                MasterPort(name, owner), owner(owner) { }

        protected:
            bool recvTimingResp(PacketPtr pkt) override;
            void recvReqRetry() override;
            void recvRangeChange() override;
    }

    CPUSidePort instPort;
    CPUSidePort dataPort;
    MemSidePort memPort;

    public:
	
	  /* Constructor */
      RedPro(RedProParams *p);

      void startup() override;

      BaseMasterPort& getMasterPort(const std::string& if_name, 
                                    PortID idx = InvalidPortID) override;

      BaseSlavePort& getSlavePort(const std::string& if_name, 
                                    PortID idx = InvalidPortID) override;

};

#endif // __REDPRO_HH__

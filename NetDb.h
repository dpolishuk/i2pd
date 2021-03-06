#ifndef NETDB_H__
#define NETDB_H__

#include <inttypes.h>
#include <map>
#include <string>
#include <thread>
#include "Queue.h"
#include "I2NPProtocol.h"
#include "RouterInfo.h"
#include "LeaseSet.h"

namespace i2p
{
namespace data
{		
	class NetDb
	{
		public:

			NetDb ();
			~NetDb ();

			void Start ();
			void Stop ();
			
			void AddRouterInfo (uint8_t * buf, int len);
			void AddLeaseSet (uint8_t * buf, int len);
			RouterInfo * FindRouter (const IdentHash& ident) const;

			void RequestDestination (const char * b32, const uint8_t * router); // in base32
			void RequestDestination (const uint8_t * destination, const uint8_t * router);
			void HandleDatabaseStoreMsg (uint8_t * buf, size_t len);
			void HandleDatabaseSearchReplyMsg (I2NPMessage * msg);
			
			const RouterInfo * GetRandomNTCPRouter (bool floodfillOnly = false) const;
			const RouterInfo * GetRandomRouter () const;

			void PostI2NPMsg (I2NPMessage * msg);
			
		private:

			void Load (const char * directory);
			void SaveUpdated (const char * directory);
			void Run (); // exploratory thread
			void Explore ();
			
		private:

			std::map<IdentHash, LeaseSet *> m_LeaseSets;
			std::map<IdentHash, RouterInfo *> m_RouterInfos;

			bool m_IsRunning;
			std::thread * m_Thread;	
			uint8_t m_Exploratory[32];
			const RouterInfo * m_LastFloodfill;
			i2p::util::Queue<I2NPMessage> m_Queue; // of I2NPDatabaseStoreMsg
	};

	extern NetDb netdb;
}
}

#endif

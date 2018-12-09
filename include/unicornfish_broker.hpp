// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#ifndef UNICORNFISH_BROKER_HPP
#define UNICORNFISH_BROKER_HPP

#include <map>
#include <unordered_set>
#include <list>
#include <unicornfish.hpp>
#include <unicornfish_socket.hpp>
#include <unicornfish_message.hpp>

namespace Uf
{
using namespace std;

class Socket;
class BrokerWorker;
class BrokerServiceInfo;

class BrokerWorker
{
  public:
	string name;
	string key;
	string serviceName;
	Frame identityFrame;
	int64_t expiry;
	BrokerServiceInfo *service = nullptr;
	size_t count;
	bool heartbeatSent = false;

	// does not takes ownership of identityFrame
	BrokerWorker()
	{
	}

	~BrokerWorker()
	{
	}

	operator bool() const { return (bool)identityFrame; }

	void RenewExpiry(int64_t new_expiry)
	{
		expiry = new_expiry;
	}

	void SendDisconnect(Socket &socket)
	{
		Message msg;
		msg.Create();
		msg.Push(Majordomo::Command::Disconnect);
		msg.Push(Majordomo::WorkerId);
		msg.Push(""); // Wrap(identityFrame);
		msg.Send(socket);
	}

	void SendHeartbeat(Socket &socket)
	{
		Message msg;
		msg.Create();
		msg.Push(Majordomo::Command::Heartbeat);
		msg.Push(Majordomo::WorkerId);
		msg.Push(""); // msg.Wrap(identityFrame);
		msg.Send(socket);
		heartbeatSent = true;
	}

	void SendCommand(Majordomo::Command command, Message &msg, Socket &socket)
	{
		string workerHexAddress = identityFrame.GetHexData();
		if (!msg)
			msg.Create();
		msg.Push(command);
		msg.Push(Majordomo::WorkerId);
		msg.Wrap(identityFrame);
		// if (verbose) printf("broker::service sending client request to worker %s\n", workerHexAddress.c_str());
		msg.Send(socket);
	}

	void Wait()
	{
		expiry = zclock_time() + Majordomo::DefaultHeartbeatIntervalMS;
	}
};

class BrokerServiceInfo
{
  public:
	string name;
	list<Message> requests;
	list<BrokerWorker *> waitingWorkers;
	size_t numWorkers;

	void AddWorker(BrokerWorker *worker)
	{
		if (!worker)
			return;
		waitingWorkers.push_back(worker);
		numWorkers++;
	}

	void RemoveWorker(BrokerWorker *worker)
	{
		if (!worker)
			return;
		waitingWorkers.remove(worker);
		numWorkers--;
	}

	// send requests to waiting workers
	void Dispatch(Socket &socket)
	{
		while (!waitingWorkers.empty() && !requests.empty())
		{
			BrokerWorker *worker = PopWorker();
			worker->SendCommand(Majordomo::Command::Request, requests.front(), socket);
			requests.pop_front();
		}
	}

	BrokerWorker *PopWorker()
	{
		if (waitingWorkers.empty())
			return nullptr;
		BrokerWorker *worker = waitingWorkers.front();
		waitingWorkers.pop_front();
		return worker;
	}
};

class Broker
{
  public:
	Broker();
	Broker(const char *endpoint);
	~Broker();

	operator bool() const { return (bool)brokerSocket; }

	bool Create(const char *endpoint);
	void Delete();
	bool ProcessWorkerMessage(Frame &sender, Message &msg);
	bool ProcessClientMessage(Frame &sender, Message &msg);
	void Purge();

	void Run();
	bool RunLoop();

	int const GetNumWorkers() const { return (int)workers.size(); }
	int const GetNumWaitingWorkers() const { return (int)waitingWorkers.size(); }
	int const GetNumRequests() const
	{
		int count = 0;
		for (auto &service : services)
			count += (int)service.second.requests.size();
		return count;
	}

  private:
	bool WorkerExists(const string &workerName) const;
	BrokerWorker *FindWorker(const string &workerName);
	void DisconnectWorker(BrokerWorker *worker);
	void DeleteWorker(const string &workerName);
	void WaitWorker(BrokerWorker *worker);

	bool ServiceExists(const string &serviceName) const;
	BrokerServiceInfo *FindService(const string &serviceName);
	void DispatchService(BrokerServiceInfo *service, Message &msg);
	void DispatchService(BrokerServiceInfo *service);

	string endpoint;
	Socket brokerSocket;
	std::map<string, BrokerServiceInfo> services;
	std::map<string, BrokerWorker> workers;
	list<BrokerWorker *> waitingWorkers;
	int64_t heartbeatTime = 0;
	bool verbose = false;
};
} // namespace Uf

#endif
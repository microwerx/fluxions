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
	struct Worker;
	struct ServiceInfo;

	struct Worker
	{
		string name;
		string key;
		string serviceName;
		Frame identityFrame;
		int64_t expiry;
		ServiceInfo *service = nullptr;
		size_t count;
		bool heartbeatSent = false;

		// does not takes ownership of identityFrame
		Worker()
		{
		}

		~Worker()
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

	struct ServiceInfo
	{
		string name;
		list<Message> requests;
		list<Worker *> waitingWorkers;
		size_t numWorkers;

		void AddWorker(Worker *worker)
		{
			if (!worker)
				return;
			waitingWorkers.push_back(worker);
			numWorkers++;
		}

		void RemoveWorker(Worker *worker)
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
				Worker *worker = PopWorker();
				worker->SendCommand(Majordomo::Command::Request, requests.front(), socket);
				requests.pop_front();
			}
		}

		Worker *PopWorker()
		{
			if (waitingWorkers.empty())
				return nullptr;
			Worker *worker = waitingWorkers.front();
			waitingWorkers.pop_front();
			return worker;
		}
	};

	bool WorkerExists(const string &workerName) const;
	Worker *FindWorker(const string &workerName);
	void DisconnectWorker(Worker *worker);
	void DeleteWorker(const string &workerName);
	void WaitWorker(Worker *worker);

	bool ServiceExists(const string &serviceName) const;
	ServiceInfo *FindService(const string &serviceName);
	void DispatchService(ServiceInfo *service, Message &msg);
	void DispatchService(ServiceInfo *service);

	string endpoint;
	Socket brokerSocket;
	map<string, ServiceInfo> services;
	map<string, Worker> workers;
	list<Worker *> waitingWorkers;
	int64_t heartbeatTime = 0;
	bool verbose = false;
};
} // namespace Uf

#endif
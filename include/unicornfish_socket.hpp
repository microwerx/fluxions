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
#ifndef UNICORNFISH_SOCKET_HPP
#define UNICORNFISH_SOCKET_HPP

#include <unicornfish.hpp>
#include <unicornfish_message.hpp>

namespace Uf
{

class Message;

class Socket
{
  public:
	Socket();
	~Socket();

	void Delete();

	// bind to PUB m_endpoint
	bool NewPub(const string &endpoint);
	// connect to SUB m_endpoint
	bool NewSub(const string &endpoint, const string &subPrefix);
	// connect to REQ m_endpoint
	bool NewReq(const string &endpoint);
	// bind to REP m_endpoint
	bool NewRep(const string &endpoint);
	// connect to DEALER m_endpoint
	bool NewDealer(const string &endpoint);
	// bind to ROUTER m_endpoint
	bool NewRouter(const string &endpoint);
	// connect to PUSH m_endpoint
	bool NewPush(const string &endpoint);
	// bind to PULL m_endpoint
	bool NewPull(const string &endpoint);
	// bind to XSUB m_endpoint
	bool NewXSub(const string &endpoint);
	// connect to XPUB m_endpoint
	bool NewXPub(const string &endpoint);
	// to PAIR m_endpoint
	bool NewPair(const string &endpoint);
	// to STREAM m_endpoint
	bool NewStream(const string &endpoint);

	bool Send(const char *picture, ...);
	bool Recv(const char *picture, ...);
	bool SendMessage(Message &msg);
	bool RecvMessage(Message &msg);
	bool Signal(unsigned char status);
	bool Wait();
	void Flush();
	bool Poll(long timeout_ms);

	operator bool() const { return socket != nullptr; }
	const string &GetEndpoint() const { return m_endpoint; }

	zsock_t *zsock() { return socket; }

	enum class SocketType
	{
		NONE,
		REQ,
		REP,
		PUB,
		SUB,
		ROUTER,
		DEALER,
		PUSH,
		PULL,
		XSUB,
		XPUB,
		PAIR,
		STREAM
	};

	SocketType GetType() const { return m_socketType; }

  private:
	string m_endpoint;
	string m_subPrefix;
	SocketType m_socketType = SocketType::NONE;
	zsock_t *socket = nullptr;
	zpoller_t *poller = nullptr;

	bool SetupSocket(string endpoint_, string subPrefix_, SocketType socketType_);
};

using SocketCPtr = Socket *;
using SocketPtr = unique_ptr<Socket>;
using SocketSharedPtr = shared_ptr<Socket>;
} // namespace Uf

#endif
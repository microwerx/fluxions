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
#ifndef UNICORNFISH_CLIENT_HPP
#define UNICORNFISH_CLIENT_HPP

#include <unicornfish.hpp>
#include <unicornfish_socket.hpp>
#include <unicornfish_message.hpp>

namespace Uf
{

class Client
{
  public:
	Client();
	Client(const char *endpoint);
	~Client();

	operator bool() const { return clientSocket; }

	bool ConnectToBroker(const char *endpoint);
	void Disconnect();
	void ClearReply();

	bool SendRequest(const char *service, const Message &msg);
	bool PollReply();
	bool WaitReply();

	bool SendWaitReply(const char *service, const Message &request);
	Message &GetReply() { return replyMessage; }
	const string &GetReplyService() { return replyService; }
	const string &GetReplyHeader() { return replyHeader; }

  private:
	string brokerEndpoint;
	Socket clientSocket;
	Message replyMessage;
	string replyHeader;
	string replyService;
	const int requestTimeoutMsec = 2500;
	const int requestRetries = 3;
	bool verbose = false;

	bool SendRequestToBroker(const char *service, const Message &msg);
};
} // namespace Uf

#endif

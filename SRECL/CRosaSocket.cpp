/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017~2018, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		CRosaSocket.cpp
* @brief	This File is RosaSocket Project.
* @author	alopex
* @version	v1.00a
* @date		2018-10-08	v1.00a	alopex	Create This File.
*/
#include "CRosaSocket.h"
#include "CThreadSafe.h"

#include <Windows.h>
#include <Ws2tcpip.h>
#include <process.h>

#pragma warning(disable:4996)

char CRosaSocket::m_pcLocalIP[SOB_IP_LENGTH] = { 0 };
USHORT CRosaSocket::m_sLocalPort = 0;

// CRosaSocket ���캯��
CRosaSocket::CRosaSocket()
{
	m_socket = NULL;
	m_SocketWriteEvent = NULL;
	m_SocketReadEvent = NULL;

	m_bIsConnected = false;
	m_sMaxCount = SOB_DEFAULT_MAX_CLIENT;

	m_nAcceptCount = 0;
	m_mapAccept.clear();

	memset(m_pcRemoteIP, 0, SOB_IP_LENGTH);
	m_sRemotePort = 0;

	memset(m_pcHostIP, 0, SOB_IP_LENGTH);
	m_sHostPort = 0;
}

// CRosaSocket ��������
CRosaSocket::~CRosaSocket()
{
	if (m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}

	if (m_SocketWriteEvent)
	{
		WSACloseEvent(m_SocketWriteEvent);
		m_SocketWriteEvent = NULL;
	}

	if (m_SocketReadEvent)
	{
		WSACloseEvent(m_SocketReadEvent);
		m_SocketReadEvent = NULL;
	}

}

// CRosaSocket ��ʼ��Socket
void CRosaSocket::CRosaSocketLibInit()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int nErr;

	wVersionRequested = MAKEWORD(2, 2);

	nErr = WSAStartup(wVersionRequested, &wsaData);	// ��ʼ��SOCKET����
	if (nErr != 0)
	{
		return;
	}
}

// CRosaSocket �ͷ�Socket
void CRosaSocket::CRosaSocketLibRelease()
{
	WSACleanup();	// ����SOCKET����
}

// CRosaSocket ����TCP�׽���
SOCKET CRosaSocket::CreateTCPSocket()
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	// TCP Socket

	// ���������������������Ϊ��
	if (s == INVALID_SOCKET)
	{
		s = NULL;
		m_nLastWSAError = WSAGetLastError();
	}

	// �����첽�¼�
	if (m_SocketWriteEvent == NULL)
	{
		m_SocketWriteEvent = WSACreateEvent();
	}

	if (m_SocketReadEvent == NULL)
	{
		m_SocketReadEvent = WSACreateEvent();
	}

	// ����SOCKET����
	const char chOpt = 1;
	int nRet = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));

	return s;
}

// CRosaSocket ����UDP�׽���
SOCKET CRosaSocket::CreateUDPSocket()
{
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	// UDP Socket

	// ���������������������Ϊ��
	if (s == INVALID_SOCKET)
	{
		s = NULL;
		m_nLastWSAError = WSAGetLastError();
	}

	// �����첽�¼�
	if (m_SocketWriteEvent == NULL)
	{
		m_SocketWriteEvent = WSACreateEvent();
	}

	if (m_SocketReadEvent == NULL)
	{
		m_SocketReadEvent = WSACreateEvent();
	}

	return s;
}

// CRosaSocket ���ý������ݳ�ʱʱ��
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSetRecvTimeOut(UINT uiMSec)
{
	UINT uiMseTimeOut = uiMSec;
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&uiMseTimeOut, sizeof(uiMseTimeOut));
}

// CRosaSocket ���÷������ݳ�ʱʱ��
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSetSendTimeOut(UINT uiMSec)
{
	UINT uiMseTimeOut = uiMSec;
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&uiMseTimeOut, sizeof(uiMseTimeOut));
}

// CRosaSocket ���ý����������鳤��
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSetRecvBufferSize(UINT uiByte)
{
	UINT uiBufferSize = uiByte;
	setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&uiBufferSize, sizeof(uiBufferSize));
}

// CRosaSocket ���÷����������鳤��
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSetSendBufferSize(UINT uiByte)
{
	UINT uiBufferSize = uiByte;
	setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&uiBufferSize, sizeof(uiBufferSize));
}

// CRosaSocket ��ȡSocket���
SOCKET ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketGetRawSocket() const
{
	return m_socket;
}

// CRosaSocket ��Socket�׽���
bool ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketAttachRawSocket(SOCKET s, bool bIsConnected)
{
	// �й�SOCKET
	m_socket = s;

	// �����йܵ�SOCKET��Ĭ��������
	m_bIsConnected = bIsConnected;

	// �����첽�¼�
	if (m_SocketWriteEvent == NULL)
	{
		m_SocketWriteEvent = WSACreateEvent();
	}

	if (m_SocketReadEvent == NULL)
	{
		m_SocketReadEvent = WSACreateEvent();
	}

	// ��ȡԶ����Ϣ
	if (bIsConnected)
	{
		struct sockaddr_in addrPeer;
		int nLen = sizeof(addrPeer);
		if (getpeername(s, (struct sockaddr*)&addrPeer, &nLen) == SOCKET_ERROR)
		{
			return false;
		}

		InetNtopA(AF_INET, &addrPeer.sin_addr, m_pcRemoteIP, SOB_IP_LENGTH);
		InetNtopW(AF_INET, &addrPeer.sin_addr, m_pwcRemoteIP, SOB_IP_LENGTH);

		m_sRemotePort = ntohs(addrPeer.sin_port);
	}

	return true;
}

// CRosaSocket �������˶˿�
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketDettachRawSocket()
{
	WSAEventSelect(m_socket, m_SocketWriteEvent, 0);
	WSAEventSelect(m_socket, m_SocketReadEvent, 0);

	WSACloseEvent(m_SocketWriteEvent);
	WSACloseEvent(m_SocketReadEvent);
	m_SocketWriteEvent = NULL;
	m_SocketReadEvent = NULL;

	m_bIsConnected = false;
	m_socket = NULL;
}

// CRosaSocket ��ȡԶ�̶�IP��ַ(ASCII)
const char * ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketGetRemoteIP() const
{
	return m_pcRemoteIP;
}

// CRosaSocket ��ȡԶ�̶�IP��ַ(Unicode)
const wchar_t * ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketGetRemoteIPW() const
{
	return m_pwcRemoteIP;
}

// CRosaSocket ��ȡԶ��IP��ַ(ULONG)
ULONG ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketGetRemoteIPUL() const
{
	return inet_addr(m_pcRemoteIP);
}

// CRosaSocket ��ȡԶ�˶˿ں�
USHORT ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketGetRemotePort() const
{
	return m_sRemotePort;
}

// CRosaSocket ��ȡ����״̬
bool ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketIsConnected() const
{
	return m_bIsConnected;
}

// CRosaSocket ɾ��SocketBase��
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketDestory()
{
	this->~CRosaSocket();
}

// CRosaSocket �󶨷���˶˿�
bool ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketBindOnPort(USHORT uPort)
{
	// ���socket��Ч���½���Ϊ�˿����ظ�����
	if (m_socket == NULL)
	{
		m_socket = CreateTCPSocket();
	}

	// �������ؼ���
	SOCKADDR_IN addrLocal;
	memset(&addrLocal, 0, sizeof(addrLocal));

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.s_addr = htonl(INADDR_ANY);
	addrLocal.sin_port = htons(uPort);

	m_sHostPort = uPort;

	// ��
	int nRet = bind(m_socket, (PSOCKADDR)&addrLocal, sizeof(addrLocal));

	// Ψһ��ԭ���Ƕ˿ڱ�ռ��
	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}

	return true;
}

// CRosaSocket ��������˶˿�
bool ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketListen()
{
	// ����
	int nRet = listen(m_socket, 5);

	// Ψһ��ԭ���Ƕ˿ڱ�ռ��
	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}

	return true;
}

// CRosaSocket ���տͻ�����������
bool ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketAccept(HANDLE_ACCEPT_THREAD pThreadFunc, HANDLE_ACCEPT_CALLBACK pCallback, DWORD dwUser, BOOL * pExitFlag, USHORT nLoopTimeOutSec)
{
	// ע�������¼�
	WSAResetEvent(m_SocketReadEvent);           // ���֮ǰ��δ�������¼�
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_ACCEPT | FD_CLOSE);

	// �ȴ�����
	while ((pExitFlag == NULL ? TRUE : !(*pExitFlag)))
	{
		DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nLoopTimeOutSec * 1000, FALSE);

		if (dwRet == WSA_WAIT_EVENT_0)
		{
			// ��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			WSAResetEvent(m_SocketReadEvent);
			WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

			if ((wsaEvents.lNetworkEvents & FD_ACCEPT) &&
				(wsaEvents.iErrorCode[FD_ACCEPT_BIT] == 0))
			{
				// �Ƿ�ﵽ���������
				if (m_nAcceptCount + 1 > m_sMaxCount)
				{
					// ���̳߳�������
					continue;
				}

				// ��¼Զ�̵�ַ
				SOCKADDR_IN addrRemote;
				memset(&addrRemote, 0, sizeof(addrRemote));
				int nAddrSize = sizeof(addrRemote);

				SOCKET sockRemote = accept(m_socket, (PSOCKADDR)&addrRemote, &nAddrSize);

				// ��Ч����
				if (sockRemote == INVALID_SOCKET)
				{
					m_nLastWSAError = WSAGetLastError();

					// ���̳߳�������
					continue;
				}

				// ��������̺߳����������߳�
				if (pThreadFunc)
				{
					// �����´����߳�
					HANDLE hThread;
					unsigned unThreadID;
					S_CLIENTINFO sClientInfo = { 0 };

					memset(&sClientInfo.SocketAddr, 0, sizeof(sClientInfo.SocketAddr));
					sClientInfo.Socket = sockRemote;
					sClientInfo.SocketAddr = addrRemote;

					hThread = (HANDLE)_beginthreadex(NULL, 0, pThreadFunc, (void*)(&sClientInfo), 0, &unThreadID);

					m_mapAccept.insert(pair<int, HANDLE>(m_nAcceptCount++, hThread));

					// �Ѿ�����Ҫ��HANDLE
					//CloseHandle(hThread);
				}
				else if (pCallback)		// �������ص�����лص�
				{
					pCallback(&addrRemote, sockRemote, dwUser);
				}
			}
		}
		else
		{
			// �ȴ���ʱ�����¿�ʼ
			continue;
		}
	}

	return true;
}

// CRosaSocket ���ͻ�������(����Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ)<����ȫ������>
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSendOnce(SOCKET Socket, char * pSendBuffer, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// ����ǰע���¼�
	WSAResetEvent(m_SocketWriteEvent);
	WSAEventSelect(Socket, m_SocketWriteEvent, FD_WRITE | FD_CLOSE);

	// ���Է���
	int nRet = send(Socket, pSendBuffer, (int)strlen(pSendBuffer), NULL);

	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();

		// ��������
		if (m_nLastWSAError == WSAEWOULDBLOCK)
		{
			DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec * 1000, FALSE);

			// ��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			if (dwRet == WSA_WAIT_EVENT_0)
			{
				WSAResetEvent(m_SocketWriteEvent);
				WSAEnumNetworkEvents(Socket, m_SocketWriteEvent, &wsaEvents);

				// ������Ϳ��Խ��в���û�д�����
				if ((wsaEvents.lNetworkEvents & FD_WRITE) &&
					(wsaEvents.iErrorCode[FD_WRITE_BIT] == 0))
				{
					// �ٴη����ı�
					nRet = (int)send(Socket, pSendBuffer, (int)strlen(pSendBuffer), NULL);

					if (nRet > 0)
					{
						// ��������ֽڴ���0���������ͳɹ�
						return SOB_RET_OK;
					}
				}
				else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
					(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
				{
					// �ͻ����Ѿ��ر�����
					return SOB_RET_CLOSE;
				}
			}
			else
			{
				// ��ʱ
				bIsTimeOut = true;
			}
		}
		else
		{

		}
	}
	else
	{
		// ��һ�α㷢�ͳɹ�
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// �����������ʧ��
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ���ͻ�������(����Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ)<����һ������>
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSendBuffer(SOCKET Socket, char * pSendBuffer, UINT uiBufferSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// ����ǰע���¼�
	WSAResetEvent(m_SocketWriteEvent);
	WSAEventSelect(Socket, m_SocketWriteEvent, FD_WRITE | FD_CLOSE);

	// ����������
	int nSent = 0;

	// �ܷ��ʹ�����Ϊ���ʹ�����һ���޶�
	int nSendTimes = 0;
	int nSendLimitTimes = (int)((float)uiBufferSize / 500 + 1.5);		// �ٶ���ǰÿ�η��Ϳ϶�������500�ֽ�
	UINT uiLeftBuffer = uiBufferSize;									// δ������Ļ����С

	// �����α�
	char* pcSentPos = pSendBuffer;

	// ֱ�����еĻ��嶼�������
	while (nSent < (int)uiBufferSize)
	{
		// ��鷢�ʹ����Ƿ���
		if (nSendTimes > nSendLimitTimes)
		{
			break;
		}

		int nRet = send(Socket, pcSentPos, uiLeftBuffer, NULL);

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();

			// ��������
			if (m_nLastWSAError == WSAEWOULDBLOCK)
			{
				DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec * 1000, FALSE);

				// ��������¼�����
				WSANETWORKEVENTS wsaEvents;
				memset(&wsaEvents, 0, sizeof(wsaEvents));

				if (dwRet == WSA_WAIT_EVENT_0)
				{
					WSAResetEvent(m_SocketWriteEvent);
					WSAEnumNetworkEvents(Socket, m_SocketWriteEvent, &wsaEvents);

					// ������Ϳ��Խ��в���û�д�����
					if ((wsaEvents.lNetworkEvents & FD_WRITE) &&
						(wsaEvents.iErrorCode[FD_WRITE_BIT] == 0))
					{
						// �ٴη����ı�
						nRet = send(Socket, pcSentPos, uiLeftBuffer, NULL);

						if (nRet > 0)
						{
							// ��������ֽڴ���0���������ͳɹ�
							nSendTimes++;

							nSent += nRet;
							uiLeftBuffer -= nRet;
							pcSentPos += nRet;
						}
						else
						{
							m_nLastWSAError = WSAGetLastError();

							// ������յ��¼�����ʱҲ�����ˣ���ȴ�һ�����ں�����
							if (m_nLastWSAError == WSAEWOULDBLOCK)
							{
								Sleep(1);
							}
							else
							{
								// ������������ֱ���˳�
								break;
							}
						}
					}
					else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
						(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
					{
						// �ͻ����Ѿ��ر�����
						return SOB_RET_CLOSE;
					}
				}
				else
				{
					// ��ʱ
					bIsTimeOut = true;
					break;
				}
			}
			else
			{
				// ��������֮��Ĵ���ֱ���˳�
				break;
			}
		}
		else
		{
			// ���ͳɹ����ۼӷ������������α�
			nSendTimes++;

			nSent += nRet;
			uiLeftBuffer -= nRet;
			pcSentPos += nRet;
		}
	}

	// ����������
	if (nSent == uiBufferSize)
	{
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// û�ܳɹ�����
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ���ջ�������(����Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ)<����ȫ������>
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketRecvOnce(SOCKET Socket, char * pRecvBuffer, UINT uiBufferSize, UINT & uiRecv, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// ����ǰע���¼�
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(Socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

	// ���Խ���
	int nRet = recv(Socket, pRecvBuffer, uiBufferSize, NULL);

	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();

		// ��������
		if (m_nLastWSAError == WSAEWOULDBLOCK)
		{
			DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

			// ��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			if (dwRet == WSA_WAIT_EVENT_0)
			{
				WSAResetEvent(m_SocketReadEvent);
				int nEnum = WSAEnumNetworkEvents(Socket, m_SocketReadEvent, &wsaEvents);

				// ������ܿ��Խ��в���û�д�����
				if ((wsaEvents.lNetworkEvents & FD_READ) &&
					(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
				{
					// �ٴν����ı�
					nRet = recv(Socket, pRecvBuffer, uiBufferSize, NULL);

					if (nRet > 0)
					{
						// ��������ֽڴ���0���������ͳɹ�
						uiRecv = nRet;
						return SOB_RET_OK;
					}
				}
				else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
					(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
				{
					// �ͻ����Ѿ��ر�����
					return SOB_RET_CLOSE;
				}
			}
			else
			{
				bIsTimeOut = true;
			}
		}
		else
		{
		}
	}
	else
	{
		// ��һ�α���ճɹ�
		uiRecv = nRet;
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// �����������ʧ��
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ���ջ�������(����Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ)<����һ������>
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketRecvBuffer(SOCKET Socket, char * pRecvBuffer, UINT uiBufferSize, UINT uiRecvSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// ����ǰע���¼�
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(Socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

	// ����������
	int nReceived = 0;

	// �ܽ��մ�����Ϊ���մ�����һ���޶�
	int nRecvTimes = 0;
	int nRecvLimitTimes = (int)((float)uiRecvSize / 500 + 1.5);      // �ٶ���ǰÿ�ν��տ϶�������500�ֽ�

	// �����α�
	char* pcRecvPos = pRecvBuffer;

	// ֱ�����յ��㹻ָ�����Ļ���
	while (nReceived < (int)uiRecvSize)
	{
		// �����մ����Ƿ���
		if (nRecvTimes > nRecvLimitTimes)
		{
			break;
		}

		int nRet = recv(Socket, pcRecvPos, uiBufferSize, NULL);

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();

			// ��������
			if (m_nLastWSAError == WSAEWOULDBLOCK)
			{
				DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

				// ��������¼�����
				WSANETWORKEVENTS wsaEvents;
				memset(&wsaEvents, 0, sizeof(wsaEvents));

				if (dwRet == WSA_WAIT_EVENT_0)
				{
					WSAResetEvent(m_SocketReadEvent);
					WSAEnumNetworkEvents(Socket, m_SocketReadEvent, &wsaEvents);

					// ������տ��Խ��в���û�д�����
					if ((wsaEvents.lNetworkEvents & FD_READ) &&
						(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
					{
						// �ٴν���
						nRet = recv(Socket, pcRecvPos, uiBufferSize, NULL);

						if (nRet > 0)
						{
							// ��������ֽڴ���0���������ͳɹ�
							nRecvTimes++;

							nReceived += nRet;
							uiBufferSize -= nRet;
							pcRecvPos += nRet;
						}
						else
						{
							m_nLastWSAError = WSAGetLastError();

							// ������յ��¼�����ʱҲ�����ˣ���ȴ�һ�����ں�����
							if (m_nLastWSAError == WSAEWOULDBLOCK)
							{
								Sleep(1);
							}
							else
							{
								// ������������ֱ���˳�
								break;
							}
						}
					}
					else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
						(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
					{
						// �ͻ����Ѿ��ر�����
						return SOB_RET_CLOSE;
					}
				}
				else
				{
					// ��ʱ
					bIsTimeOut = true;
					break;
				}
			}
			else
			{
				// ��������֮��Ĵ���ֱ���˳�
				break;
			}
		}
		else if (nRet == 0)		// �Է������ر�����
		{
			break;
		}
		else
		{
			// ���ճɹ����ۼӽ������������α�
			nRecvTimes++;

			nReceived += nRet;
			uiBufferSize -= nRet;
			pcRecvPos += nRet;
		}
	}

	// ����������
	if (nReceived == uiRecvSize)
	{
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// û�ܳɹ�����
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ��ȡ�����������
USHORT ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketGetConnectMaxCount() const
{
	return m_sMaxCount;
}

// CRosaSocket ��ȡ��ǰ���ӵ�����(�߳�)
int& ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketGetConnectCount()
{
	return m_nAcceptCount;
}

// CRosaSocket ��ȡ��ǰ���ӵ��߳�Map
map<int, HANDLE>& ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketGetConnectMap()
{
	return m_mapAccept;
}

// CRosaSocket ���������������
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSetConnectMaxCount(USHORT sMaxCount)
{
	m_sMaxCount = sMaxCount;
}

// CRosaSocket ���õ�ǰ��������
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSetConnectCount(int nAcceptCount)
{
	m_nAcceptCount = nAcceptCount;
}

// CRosaSocket ���ͷ�������������(�޲������ñ�ʾ����)
bool ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketConnect(const char * pcRemoteIP, USHORT sPort, USHORT nTimeOutSec)
{
	// ���socket��Ч���½���Ϊ�˿����ظ�����
	if (m_socket == NULL)
	{
		m_socket = CreateTCPSocket();
	}

	// �趨Զ��IP
	SOCKADDR_IN addrRemote;
	memset(&addrRemote, 0, sizeof(addrRemote));

	// �����Ҫ�����²����������ʾ�õ�ǰIP����
	if (pcRemoteIP != NULL && sPort != 0)
	{
		memset(m_pcRemoteIP, 0, SOB_IP_LENGTH);
		strcpy(m_pcRemoteIP, pcRemoteIP);

		m_sRemotePort = sPort;
	}

	addrRemote.sin_family = AF_INET;
	addrRemote.sin_addr.S_un.S_addr = inet_addr(m_pcRemoteIP);
	addrRemote.sin_port = htons(m_sRemotePort);

	// ע�������¼�
	WSAResetEvent(m_SocketWriteEvent);           // ���֮ǰ��δ�������¼�
	WSAEventSelect(m_socket, m_SocketWriteEvent, FD_CONNECT | FD_CLOSE);

	// ��������
	int nRet = connect(m_socket, (SOCKADDR*)&addrRemote, sizeof(addrRemote));

	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();

		// �����������
		if (m_nLastWSAError == WSAEWOULDBLOCK)
		{
			DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec * 1000, FALSE);

			// ��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			if (dwRet == WSA_WAIT_EVENT_0)
			{
				WSAResetEvent(m_SocketWriteEvent);
				WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

				// ������ӽ��ܲ���û�д�����
				if ((wsaEvents.lNetworkEvents & FD_CONNECT) &&
					(wsaEvents.iErrorCode[FD_CONNECT_BIT] == 0))
				{
					// ʵ�ʵĵ��Ծ��������ң����������ͣ�٣��ᱨ�����˷ѽ��10022����
					// ����ͬһsocket������connect���̫�̿����ǲ��е�,���������Magic Number��50������ʵ�����Ľ����Sleep(1)��һ��ʱ��Ƭ�ƺ�����
					Sleep(50);
					nRet = connect(m_socket, (SOCKADDR*)&addrRemote, sizeof(addrRemote));

					if (nRet == SOCKET_ERROR)
					{
						m_nLastWSAError = WSAGetLastError();

						// �������ʵ�����Ѿ�����
						if (m_nLastWSAError == WSAEISCONN)
						{
							m_bIsConnected = true;
						}
					}
					else    // ���������ӳɹ�
					{
						m_bIsConnected = true;
					}
				}
			}
		}
		else
		{
			m_bIsConnected = false;
		}

	}
	else
	{
		// ��һ�γ��Ա����ӳɹ�
		m_bIsConnected = true;
	}

	// �������ʧ��
	if (!m_bIsConnected)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}

	// ���ؽ������
	return m_bIsConnected;
}

// CRosaSocket �����������ӷ�����
bool ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketReConnect()
{
	CRosaSocketDisConnect();

	Sleep(100);

	return CRosaSocketConnect();
}

// CRosaSocket �Ͽ��������������
void ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketDisConnect()
{
	if (m_socket == NULL)
	{
		return;
	}

	// �����������״̬��ر�����
	if (m_bIsConnected)
	{
		shutdown(m_socket, SD_BOTH);

		m_bIsConnected = false;
	}

	closesocket(m_socket);
	m_socket = NULL;
}

// CRosaSocket ���ͻ�������(����Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ)<����ȫ������>
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSendOnce(char * pSendBuffer, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// �������״̬
	if (!m_bIsConnected)
	{
		return SOB_RET_FAIL;
	}

	// ����ǰע���¼�
	WSAResetEvent(m_SocketWriteEvent);
	WSAEventSelect(m_socket, m_SocketWriteEvent, FD_WRITE | FD_CLOSE);

	// ���Է���
	int nRet = send(m_socket, pSendBuffer, (int)strlen(pSendBuffer), NULL);

	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();

		// ��������
		if (m_nLastWSAError == WSAEWOULDBLOCK)
		{
			DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec * 1000, FALSE);

			// ��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			if (dwRet == WSA_WAIT_EVENT_0)
			{
				WSAResetEvent(m_SocketWriteEvent);
				WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

				// ������Ϳ��Խ��в���û�д�����
				if ((wsaEvents.lNetworkEvents & FD_WRITE) &&
					(wsaEvents.iErrorCode[FD_WRITE_BIT] == 0))
				{
					// �ٴη����ı�
					nRet = (int)send(m_socket, pSendBuffer, (int)strlen(pSendBuffer), NULL);

					if (nRet > 0)
					{
						// ��������ֽڴ���0���������ͳɹ�
						return SOB_RET_OK;
					}
				}
				else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
					(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
				{
					// �������Ѿ��ر�����
					return SOB_RET_CLOSE;
				}
			}
			else
			{
				// ��ʱ
				bIsTimeOut = true;
			}
		}
		else
		{
			m_bIsConnected = false;
		}
	}
	else
	{
		// ��һ�α㷢�ͳɹ�
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// �����������ʧ��
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ���ͻ�������(����Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ)<����һ������>
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketSendBuffer(char * pSendBuffer, UINT uiBufferSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// �������״̬
	if (!m_bIsConnected)
	{
		return SOB_RET_FAIL;
	}

	// ����ǰע���¼�
	WSAResetEvent(m_SocketWriteEvent);
	WSAEventSelect(m_socket, m_SocketWriteEvent, FD_WRITE | FD_CLOSE);

	// ����������
	int nSent = 0;

	// �ܷ��ʹ�����Ϊ���ʹ�����һ���޶�
	int nSendTimes = 0;
	int nSendLimitTimes = (int)((float)uiBufferSize / 500 + 1.5);      // �ٶ���ǰÿ�η��Ϳ϶�������500�ֽ�
	UINT uiLeftBuffer = uiBufferSize;									// δ������Ļ����С

	// �����α�
	char* pcSentPos = pSendBuffer;

	// ֱ�����еĻ��嶼�������
	while (nSent < (int)uiBufferSize)
	{
		// ��鷢�ʹ����Ƿ���
		if (nSendTimes > nSendLimitTimes)
		{
			break;
		}

		int nRet = send(m_socket, pcSentPos, uiLeftBuffer, NULL);

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();

			// ��������
			if (m_nLastWSAError == WSAEWOULDBLOCK)
			{
				DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec * 1000, FALSE);

				// ��������¼�����
				WSANETWORKEVENTS wsaEvents;
				memset(&wsaEvents, 0, sizeof(wsaEvents));

				if (dwRet == WSA_WAIT_EVENT_0)
				{
					WSAResetEvent(m_SocketWriteEvent);
					WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

					// ������Ϳ��Խ��в���û�д�����
					if ((wsaEvents.lNetworkEvents & FD_WRITE) &&
						(wsaEvents.iErrorCode[FD_WRITE_BIT] == 0))
					{
						// �ٴη����ı�
						nRet = send(m_socket, pcSentPos, uiLeftBuffer, NULL);

						if (nRet > 0)
						{
							// ��������ֽڴ���0���������ͳɹ�
							nSendTimes++;

							nSent += nRet;
							uiLeftBuffer -= nRet;
							pcSentPos += nRet;
						}
						else
						{
							m_nLastWSAError = WSAGetLastError();

							// ������յ��¼�����ʱҲ�����ˣ���ȴ�һ�����ں�����
							if (m_nLastWSAError == WSAEWOULDBLOCK)
							{
								Sleep(1);
							}
							else
							{
								// ������������ֱ���˳�
								break;
							}
						}
					}
					else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
						(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
					{
						// �������Ѿ��ر�����
						return SOB_RET_CLOSE;
					}
				}
				else
				{
					// ��ʱ
					bIsTimeOut = true;
					break;
				}
			}
			else
			{
				// ��������֮��Ĵ���ֱ���˳�
				m_bIsConnected = false;
				break;
			}
		}
		else
		{
			// ���ͳɹ����ۼӷ������������α�
			nSendTimes++;

			nSent += nRet;
			uiLeftBuffer -= nRet;
			pcSentPos += nRet;
		}
	}

	// ����������
	if (nSent == uiBufferSize)
	{
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// û�ܳɹ�����
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ���ջ�������(����Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ)<����ȫ������>
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketRecvOnce(char * pRecvBuffer, UINT uiBufferSize, UINT & uiRecv, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// �������״̬
	if (!m_bIsConnected)
	{
		return SOB_RET_FAIL;
	}

	// ����ǰע���¼�
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

	// ���Խ���
	int nRet = recv(m_socket, pRecvBuffer, uiBufferSize, NULL);

	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();

		// ��������
		if (m_nLastWSAError == WSAEWOULDBLOCK)
		{
			DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

			// ��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			if (dwRet == WSA_WAIT_EVENT_0)
			{
				WSAResetEvent(m_SocketReadEvent);
				int nEnum = WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

				// ������ܿ��Խ��в���û�д�����
				if ((wsaEvents.lNetworkEvents & FD_READ) &&
					(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
				{
					// �ٴν����ı�
					nRet = recv(m_socket, pRecvBuffer, uiBufferSize, NULL);

					if (nRet > 0)
					{
						// ��������ֽڴ���0���������ͳɹ�
						uiRecv = nRet;
						return SOB_RET_OK;
					}
				}
				else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
					(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
				{
					// �������Ѿ��ر�����
					return SOB_RET_CLOSE;
				}
			}
			else
			{
				bIsTimeOut = true;
			}
		}
		else
		{
			m_bIsConnected = false;
		}
	}
	else
	{
		// ��һ�α���ճɹ�
		uiRecv = nRet;
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// �����������ʧ��
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ���ջ�������(����Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ)<����һ������>
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketRecvBuffer(char * pRecvBuffer, UINT uiBufferSize, UINT uiRecvSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// �������״̬
	if (!m_bIsConnected)
	{
		return SOB_RET_FAIL;
	}

	// ����ǰע���¼�
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

	// ����������
	int nReceived = 0;

	// �ܽ��մ�����Ϊ���մ�����һ���޶�
	int nRecvTimes = 0;
	int nRecvLimitTimes = (int)((float)uiRecvSize / 500 + 1.5);      //�ٶ���ǰÿ�ν��տ϶�������500�ֽ�

	// �����α�
	char* pcRecvPos = pRecvBuffer;

	// ֱ�����յ��㹻ָ�����Ļ���
	while (nReceived < (int)uiRecvSize)
	{
		// �����մ����Ƿ���
		if (nRecvTimes > nRecvLimitTimes)
		{
			break;
		}

		int nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();

			// ��������
			if (m_nLastWSAError == WSAEWOULDBLOCK)
			{
				DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

				// ��������¼�����
				WSANETWORKEVENTS wsaEvents;
				memset(&wsaEvents, 0, sizeof(wsaEvents));

				if (dwRet == WSA_WAIT_EVENT_0)
				{
					WSAResetEvent(m_SocketReadEvent);
					WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

					// ������տ��Խ��в���û�д�����
					if ((wsaEvents.lNetworkEvents & FD_READ) &&
						(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
					{
						// �ٴν���
						nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

						if (nRet > 0)
						{
							// ��������ֽڴ���0���������ͳɹ�
							nRecvTimes++;

							nReceived += nRet;
							uiBufferSize -= nRet;
							pcRecvPos += nRet;
						}
						else
						{
							m_nLastWSAError = WSAGetLastError();

							// ������յ��¼�����ʱҲ�����ˣ���ȴ�һ�����ں�����
							if (m_nLastWSAError == WSAEWOULDBLOCK)
							{
								Sleep(1);
							}
							else
							{
								// ������������ֱ���˳�
								break;
							}
						}
					}
					else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
						(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
					{
						// �������Ѿ��ر�����
						return SOB_RET_CLOSE;
					}
				}
				else
				{
					// ��ʱ
					bIsTimeOut = true;
					break;
				}
			}
			else
			{
				// ��������֮��Ĵ���ֱ���˳�
				m_bIsConnected = false;
				break;
			}
		}
		else if (nRet == 0)		// �Է������ر�����
		{
			m_bIsConnected = false;
			break;
		}
		else
		{
			// ���ճɹ����ۼӽ������������α�
			nRecvTimes++;

			nReceived += nRet;
			uiBufferSize -= nRet;
			pcRecvPos += nRet;
		}
	}

	// ����������
	if (nReceived == uiRecvSize)
	{
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// û�ܳɹ�����
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ��UDP�˿�
bool ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketUDPBindOnPort(const char * pcRemoteIP, UINT uiPort)
{
	// ���socket��Ч���½���Ϊ�˿����ظ�����
	if (m_socket == NULL)
	{
		m_socket = CreateUDPSocket();
	}

	// �������ؼ���
	SOCKADDR_IN addrLocal;
	memset(&addrLocal, 0, sizeof(addrLocal));

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.s_addr = inet_addr(pcRemoteIP);
	addrLocal.sin_port = htons(uiPort);

	// ��
	int nRet = bind(m_socket, (PSOCKADDR)&addrLocal, sizeof(addrLocal));

	// Ψһ��ԭ���Ƕ˿ڱ�ռ��
	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}
	m_bIsConnected = true;

	return true;
}

// CRosaSocket ���ͻ�������(UDP)
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketUDPSendBuffer(const char * pcIP, SHORT sPort, char * pBuffer, UINT uiBufferSize, USHORT nTimeOutSec)
{
	if (m_socket == NULL)
	{
		m_socket = CreateUDPSocket();
	}

	bool bIsTimeOut = false;

	// ����������
	int nSent = 0;

	// �ܷ��ʹ�����Ϊ���ʹ�����һ���޶�
	int nSendTimes = 0;
	int nSendLimitTimes = nTimeOutSec * 1000 / 100;						// ��������������ȴ�100ms���ط�
	UINT uiLeftBuffer = uiBufferSize;									// δ������Ļ����С

	// ת��Զ�̵�ַ
	SOCKADDR_IN addrRemote;
	memset(&addrRemote, 0, sizeof(addrRemote));

	addrRemote.sin_family = AF_INET;
	addrRemote.sin_addr.s_addr = inet_addr(pcIP);
	addrRemote.sin_port = htons(sPort);

	// �����α�
	char* pcSentPos = pBuffer;

	// ֱ�����еĻ��嶼�������
	while (nSent < (int)uiBufferSize)
	{
		// ��鷢�ʹ����Ƿ���
		if (nSendTimes > nSendLimitTimes)
		{
			bIsTimeOut = true;
			break;
		}

		int nRet = sendto(m_socket, pcSentPos, uiLeftBuffer, NULL, (PSOCKADDR)&addrRemote, sizeof(addrRemote));

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();
			break;
		}
		else
		{
			// ���ͳɹ����ۼӷ������������α�
			nSendTimes++;

			nSent += nRet;
			uiLeftBuffer -= nRet;
			pcSentPos += nRet;
		}
	}

	// ����������
	if (nSent == uiBufferSize)
	{
		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// û�ܳɹ�����
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ���ջ�������(UDP)
int ROSASOCKET_CALLMODE CRosaSocket::CRosaSocketUDPRecvBuffer(char * pBuffer, UINT uiBufferSize, UINT & uiRecv, char * pcIP, USHORT & uPort, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	// ����ǰע���¼�
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ);

	// Զ����Ϣ
	SOCKADDR_IN addrRemote;
	int nAddrLen = sizeof(addrRemote);
	memset(&addrRemote, 0, nAddrLen);

	// ���Խ���
	int nRet = recvfrom(m_socket, pBuffer, uiBufferSize, NULL, (PSOCKADDR)&addrRemote, &nAddrLen);

	if (nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();

		// ��������
		if (m_nLastWSAError == WSAEWOULDBLOCK)
		{
			DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

			// ��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			if (dwRet == WSA_WAIT_EVENT_0)
			{
				WSAResetEvent(m_SocketReadEvent);
				int nEnum = WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

				// ������ܿ��Խ��в���û�д�����
				if ((wsaEvents.lNetworkEvents & FD_READ) &&
					(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
				{
					// �ٴν����ı�
					nRet = recvfrom(m_socket, pBuffer, uiBufferSize, NULL, (PSOCKADDR)&addrRemote, &nAddrLen);

					if (nRet > 0)
					{
						// ����ֽڴ���0���������ճɹ�
						uiRecv = nRet;

						// ����IP�Ͷ˿�
						strcpy(pcIP, inet_ntoa(addrRemote.sin_addr));
						uPort = ntohs(addrRemote.sin_port);

						return SOB_RET_OK;
					}
				}
				else if ((wsaEvents.lNetworkEvents & FD_CLOSE) &&
					(wsaEvents.iErrorCode[FD_CLOSE_BIT] == 0))
				{
					// �Ѿ��ر�����
					return SOB_RET_CLOSE;
				}
			}
			else
			{
				bIsTimeOut = true;
			}
		}
	}
	else
	{
		// ��һ�α���ճɹ�
		uiRecv = nRet;

		// ����IP�Ͷ˿�
		strcpy(pcIP, inet_ntoa(addrRemote.sin_addr));
		uPort = ntohs(addrRemote.sin_port);

		return SOB_RET_OK;
	}

	// �����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	// �����������ʧ��
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

// CRosaSocket ��ַת��ΪIP��ַ
bool CRosaSocket::ResolveAddressToIp(const char * pcAddress, char * pcIp)
{
	addrinfo adiHints, *padiResult;
	int	nRet;

	memset(&adiHints, 0, sizeof(addrinfo));

	// ������IPV4�ĵ�ַ
	adiHints.ai_flags = AI_CANONNAME;
	adiHints.ai_family = AF_INET;
	adiHints.ai_socktype = SOCK_STREAM;
	adiHints.ai_protocol = IPPROTO_TCP;

	// ת����ַ
	nRet = ::getaddrinfo(pcAddress, NULL, &adiHints, &padiResult);

	// �����
	if (nRet != 0)
	{
		freeaddrinfo(padiResult);
		return false;
	}

	// �������,ֻ������һ��
	if (padiResult->ai_addr != NULL)
	{
		::strcpy(pcIp, inet_ntoa(((sockaddr_in*)padiResult->ai_addr)->sin_addr));
	}

	freeaddrinfo(padiResult);

	return true;
}

// CRosaSocket ��ȡ����IP��ַ
void CRosaSocket::GetLocalIPAddr()
{
	char chHostName[MAX_PATH] = { 0 };

	if (gethostname(chHostName, sizeof(chHostName)) == 0)
	{
		hostent* pHostent = gethostbyname(chHostName);
		hostent& he = *pHostent;
		sockaddr_in sa;
		for (int nAdapter = 0; he.h_addr_list[nAdapter]; nAdapter++)
		{
			memset(m_pcLocalIP, 0, SOB_IP_LENGTH);
			memcpy(&sa.sin_addr.s_addr, he.h_addr_list[nAdapter], he.h_length);
			strcpy(m_pcLocalIP, inet_ntoa(*(struct in_addr *)he.h_addr_list[nAdapter]));
		}
	}

}

// CRosaSocket ��ȡ�����˿ں�
void ROSASOCKET_CALLMODE CRosaSocket::GetLocalIPPort()
{
	sockaddr addr;
	sockaddr_in* addr_v4;
	int addr_len = sizeof(addr);

	ZeroMemory(&addr, sizeof(addr));

	if (0 == getsockname(m_socket, &addr, &addr_len))
	{
		if (addr.sa_family == AF_INET)
		{
			addr_v4 = (sockaddr_in*)&addr;
			m_sLocalPort = addr_v4->sin_port;
		}
	}

}

// CRosaSocket ���ʱ���IP��ַ(��̬)
const char * CRosaSocket::GetLocalIP()
{
	return m_pcLocalIP;
}

// CRosaSocket ���ʱ����˿ں�(��̬)
USHORT CRosaSocket::GetLocalPort()
{
	return m_sLocalPort;
}

// CRosaSocket ���ñ���IP��ַ(��̬)
void CRosaSocket::SetLocalIP(const char * pLocalIP, int nSize)
{
	memset(m_pcLocalIP, 0, SOB_IP_LENGTH);
	memcpy_s(m_pcLocalIP, SOB_IP_LENGTH, pLocalIP, nSize);
}

// CRosaSocket ���ñ����˿ں�(��̬)
void CRosaSocket::SetLocalPort(USHORT sLocalPort)
{
	m_sLocalPort = sLocalPort;
}
#include <anet/socket.h>
#include <anet/iocomponent.h>
#include <anet/threadmutex.h>
#include <anet/stats.h>
#include <anet/log.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <anet/filecontrol.h>

namespace anet {

ThreadMutex Socket::_dnsMutex;

/*
 * ���캯��
 */
Socket::Socket() {
    _socketHandle = -1;
    _addressValid = false;
}

/*
 * ��������
 */
Socket::~Socket() {
    close();
}

/*
 * ���õ�ַ
 *
 * @param address  host��ip��ַ
 * @param port  �˿ں�
 * @return �Ƿ�ɹ�
 */

bool Socket::setAddress (const char *address, const int port) {
    // ��ʼ��
    memset(static_cast<void *>(&_address), 0, sizeof(_address));

    _address.sin_family = AF_INET;
    _address.sin_port = htons(static_cast<short>(port));

    bool rc = true;
    // �ǿ��ַ������ó�INADDR_ANY

    if (address == NULL || address[0] == '\0') {
        _address.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        char c = '\0';

        const char *p = address;

        bool isIPAddr = true;

        // ��ip��ַ��ʽ��?
        while ((c = (*p++)) != '\0') {
            if ((c != '.') && (!((c >= '0') && (c <= '9')))) {
                isIPAddr = false;
                break;
            }
        }

        if (isIPAddr) {
            _address.sin_addr.s_addr = inet_addr(address);
        } else {
            // ������������һ��
            _dnsMutex.lock();

            struct hostent *myHostEnt = gethostbyname(address);

            if (myHostEnt != NULL) {
                memcpy(&(_address.sin_addr), *(myHostEnt->h_addr_list),
                       sizeof(struct in_addr));
            } else {
                rc = false;
            }

            _dnsMutex.unlock();
        }
    }
    _addressValid = rc;
    return rc;
}

/*
 * socket ����Ƿ񴴽�
 */
bool Socket::checkSocketHandle() {
    if (_socketHandle != -1) {
        return true;
    }

    if ((_socketHandle = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return false;
    }
    if (!FileControl::setCloseOnExec(_socketHandle)) {
        close();
        return false;
    }
    return true;
}

/*
 * ���ӵ�_address��
 *
 * @return �Ƿ�ɹ�
 */
bool Socket::connect() {
    if (!_addressValid || !checkSocketHandle()) {
        return false;
    }
    
    return (0 == ::connect(_socketHandle, (struct sockaddr *)&_address, sizeof(_address)));
}

bool Socket::reconnect() {
    close();
    return connect();
}

/**
 * �ر�����
 */
void Socket::close() {
    if (_socketHandle != -1) {
        char tmp[32];
        getAddr(tmp, 32);
        ANET_LOG(DEBUG, "Closing socket, fd=%d, addr=%s", _socketHandle, tmp);
        ::close(_socketHandle);
        _socketHandle = -1;
    }
}

/*
 * �رն�д
 */
void Socket::shutdown() {
    if (_socketHandle != -1) {
        ::shutdown(_socketHandle, SHUT_WR);
    }
}

/**
 * ʹ��UDP��socket
 *
 * @return �Ƿ�ɹ�
 */
bool Socket::createUDP() {
    close();
    _socketHandle = socket(AF_INET, SOCK_DGRAM, 0);
    return (_socketHandle != -1);
}

/*
 * ��socketHandle,��ipaddress���õ���socket��
 *
 * @param  socketHandle: socket���ļ����
 * @param hostAddress: ��������ַ
 */

void Socket::setUp(int socketHandle, struct sockaddr *hostAddress) {
    close();
    _socketHandle = socketHandle;
    memcpy(&_address, hostAddress, sizeof(_address));
}

/*
 * �����ļ����
 *
 * @return �ļ����
 */
int Socket::getSocketHandle() {
    return _socketHandle;
}

/*
 * ����event attribute
 *
 * @return  IOComponent
 */
IOComponent *Socket::getIOComponent() {
    return _iocomponent;
}

/*
 * ����IOComponent
 *
 * @param IOComponent
 */
void Socket::setIOComponent(IOComponent *ioc) {
    _iocomponent = ioc;
}

/*
 * д����
 */
int Socket::write (const void *data, int len) {
    if (_socketHandle == -1) {
        return -1;
    }

    int res = -1;
    do {
        res = ::write(_socketHandle, data, len);
        if (res > 0) {
            ANET_COUNT_DATA_WRITE(res);
        }
    } while (res < 0 && errno == EINTR);
    return res;
}

/*
 * ������
 */
int Socket::read (void *data, int len) {
    if (_socketHandle == -1) {
        return -1;
    }

    int res = -1;
    do {
        res = ::read(_socketHandle, data, len);
        if (res > 0) {
            ANET_COUNT_DATA_READ(res);
        }
    } while (-1 == res && errno == EINTR);
    return res;
}

/*
 * ����int���͵�option
 */
bool Socket::setIntOption (int option, int value) {
    bool rc=false;
    if (checkSocketHandle()) {
        rc = (setsockopt(_socketHandle, SOL_SOCKET, option,
                         (const void *)(&value), sizeof(value)) == 0);
    }
    return rc;
}

bool Socket::setSoLinger(bool doLinger, int seconds) {
    bool rc=false;
    struct linger lingerTime;
    lingerTime.l_onoff = doLinger ? 1 : 0;
    lingerTime.l_linger = seconds;


  
    if (checkSocketHandle()) {
        rc = (setsockopt(_socketHandle, SOL_SOCKET, SO_LINGER,
                         (const void *)(&lingerTime), sizeof(lingerTime)) == 0);
    }

    return rc;
}

bool Socket::setTcpNoDelay(bool noDelay) {
    bool rc = false;
    int noDelayInt = noDelay ? 1 : 0;
    if (checkSocketHandle()) {
        rc = (setsockopt(_socketHandle, IPPROTO_TCP, TCP_NODELAY,
                         (const void *)(&noDelayInt), sizeof(noDelayInt)) == 0);
    }
    return rc;
}

/*
 * �Ƿ�����
 */
bool Socket::setSoBlocking(bool blockingEnabled) {
    bool rc=false;

    if (checkSocketHandle()) {
        int flags = fcntl(_socketHandle, F_GETFL, NULL);
        if (flags >= 0) {
            if (blockingEnabled) {
                flags &= ~O_NONBLOCK; // clear nonblocking
            } else {
                flags |= O_NONBLOCK;  // set nonblocking
            }

            if (fcntl(_socketHandle, F_SETFL, flags) >= 0) {
                rc = true;
            }
        }
    }

    return rc;
}

/*
 * �õ�ip��ַ, д��tmp��
 */
char *Socket::getAddr(char *dest, int len, bool active) {
    struct sockaddr_in addr;    
    if (!getSockAddr(addr, active)) {
        return NULL;
    }

    unsigned long ad = ntohl(addr.sin_addr.s_addr);
    snprintf(dest, len, "%d.%d.%d.%d:%d",
             static_cast<int>((ad >> 24) & 255),
             static_cast<int>((ad >> 16) & 255),
             static_cast<int>((ad >> 8) & 255),
             static_cast<int>(ad & 255),
             ntohs(addr.sin_port));
    return dest;
}

int Socket::getPort(bool active) {
    struct sockaddr_in addr;    
    if (getSockAddr(addr, active)) {
        return ntohs(addr.sin_port);
    } else {
        return -1;
    }
}

uint32_t Socket::getIpAddress(bool active) {
    struct sockaddr_in addr;
    if (getSockAddr(addr, active)) {
        return ntohl(addr.sin_addr.s_addr);
    } else {
        return (uint32_t)-1;
    }
}

bool Socket::getSockAddr(sockaddr_in &addr, bool active) {
    socklen_t addrLen = sizeof(addr);
    if (active) {
        int ret = getsockname(_socketHandle, (sockaddr*)&addr, &addrLen);
        if (ret != 0 || addrLen != sizeof(addr) ) {
            ANET_LOG(WARN, "getsockname() fail! [ret, len, fd]: [%d, %d, %d]."
                     "%s!", ret, addrLen, _socketHandle, strerror(errno));
            return false;
        }
    } else {
        addr = _address;
    }
    return true;
}

/*
 * �õ�socket����
 */
int Socket::getSoError () {
    if (_socketHandle == -1) {
        return EINVAL;
    }

    int lastError = Socket::getLastError();
    int  soError = 0;
    socklen_t soErrorLen = sizeof(soError);
    if (getsockopt(_socketHandle, SOL_SOCKET, SO_ERROR, (void *)(&soError), &soErrorLen) != 0) {
        return lastError;
    }
    if (soErrorLen != sizeof(soError))
        return EINVAL;

    return soError;
}

}

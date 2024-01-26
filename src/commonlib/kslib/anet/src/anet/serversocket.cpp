#include <anet/serversocket.h>
#include <anet/socket.h>
#include <string.h>
#include <assert.h>
#include <anet/log.h>
namespace anet {

/*
 * ���캯��
 */
ServerSocket::ServerSocket() {
    _backLog = 256;
}

/*
 * acceptһ���µ�����
 *
 * @return һ��Socket
 */
Socket *ServerSocket::accept() {
    Socket *handleSocket = NULL;

    struct sockaddr_in addr;
    int len = sizeof(addr);

    int fd = ::accept(_socketHandle, (struct sockaddr *) & addr, (socklen_t*) & len);

    if (fd >= 0) {
        handleSocket = new Socket();
        assert(handleSocket);
        handleSocket->setUp(fd, (struct sockaddr *)&addr);
    } else {
        int error = getLastError();
        if (error != EAGAIN) {
            ANET_LOG(ERROR, "%s(%d)", strerror(error), error);
        }
    }

    return handleSocket;
}

/*
 * �򿪼���
 *
 * @return �Ƿ�ɹ�
 */
bool ServerSocket::listen() {
    if (!isAddressValid()) {
        return false;
    }

    if (!checkSocketHandle()) {
        return false;
    }

    // ��ַ������
    setReuseAddress(true);

    if (::bind(_socketHandle, (struct sockaddr *)&_address,
               sizeof(_address)) < 0) {
        return false;
    }

    if (::listen(_socketHandle, _backLog) < 0) {
        return false;
    }

    return true;
}
}

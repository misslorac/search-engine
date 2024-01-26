/**
 * File name: httpstreamer.h
 * Author: zhangli
 * Create time: 2008-12-12 20:02:45
 * $Id: httpstreamer.h 15760 2008-12-30 08:03:52Z zhangli $
 * 
 * Description: ***add description here***
 * 
 */

#ifndef ANET_HTTPSTREAMER_H_
#define ANET_HTTPSTREAMER_H_
#include <anet/databuffer.h>
#include <anet/streamingcontext.h>
#include <anet/httpstreamingcontext.h>
#include <anet/defaultpacketstreamer.h>
#include <anet/ipacketfactory.h>
#include <anet/httppacket.h>

namespace anet {

class HTTPStreamer : public DefaultPacketStreamer
{
public:
    HTTPStreamer(IPacketFactory *factory);
    bool processData(DataBuffer *dataBuffer, StreamingContext *context);
    StreamingContext* createContext();

    bool getPacketInfo(DataBuffer *input, PacketHeader *header, bool *broken) {
        return false;
    }

    Packet *decode(DataBuffer *input, PacketHeader *header) {
//        ANET_LOG(ERROR, "SHOULD NOT INVOKE HTTPStreamer::decode(...)!");
        assert(false);
        return NULL;
    }

    static const size_t URI_LIMIT;
    static const size_t PKG_LIMIT;
    static const size_t HEADERS_LIMIT;
protected:
    static const char _table[256];
private:
    /**
     * return ture if start-line parsing completed.
     * set context broken if needed
     **/
    bool processStartLine(DataBuffer *databuffer,
                          HTTPStreamingContext *context);

    /**
     * return ture if all headers parsing completed.
     * set context broken if needed
     **/
    bool processHeaders(DataBuffer *databuffer,
                        HTTPStreamingContext *context);

    bool processHeadersOrTrailers(DataBuffer *databuffer,
                                  HTTPStreamingContext *context);

    bool processPerHeaderLine(char *pstart, char *pend, HTTPPacket *packet);

    bool processBody(DataBuffer *databuffer, HTTPStreamingContext *context);

    bool processLengthBody(DataBuffer *databuffer, 
                           HTTPStreamingContext *context);

    bool processChunkedBody(DataBuffer *databuffer, 
                            HTTPStreamingContext *context);

    bool processEOFBody(DataBuffer *databuffer, HTTPStreamingContext *context);

    bool processChunkSize(DataBuffer *databuffer, HTTPStreamingContext *context);
    bool getCRLF(DataBuffer *databuffer, HTTPStreamingContext *context);

    char* findNextWhiteSpace(char *begin, char *end);
    char* findNextWhiteSpaceAndReplaceZero(char *begin, char *end);
    char* skipWhiteSpaces(char *begin, char *end);
    bool isValidStatusCode(char *p);
    bool isValidToken(char *begin, char *end);
    bool isValidDigits(const char *dist);
    void replaceZero(char *begin, char *end);
    void trim(char *&begin, char *&end, bool left, bool right);
    bool findCRLF(char *start, char *end, char *&CR,  size_t &length);
    bool messageHeadersEnd(HTTPStreamingContext *context);
    bool processStatusLine(HTTPStreamingContext *context, 
                           char *start, char *end);
    bool processRequestLine(HTTPStreamingContext *context, 
                            char *start, char *end);
    inline bool isTokenCharacter(unsigned char c) {
            return _table[c];
    }
    friend class HTTPStreamerTF;
};

}/*end namespace anet*/

#endif /*ANET_HTTPSTREAMER_H_*/

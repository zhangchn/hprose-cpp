#ifndef HPROSE_CLIENT_ASIO_HPROSE_TCPCLIENT_HPP
#define HPROSE_CLIENT_ASIO_HPROSE_TCPCLIENT_HPP

#include <hprose/client/HproseClient.hpp>
#include <hprose/client/CookieManager.hpp>

#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>
#ifndef HPROSE_NO_OPENSSL
#include <boost/asio/ssl.hpp>
#endif
#include <boost/thread/mutex.hpp>

namespace hprose { namespace asio {

using boost::asio::ip::tcp;

class HproseTCPClient : public HproseClient {
public:
    HproseTCPClient(const std::string & uri = "tcp://localhost:8000") {
        UseService(uri);
    };
    
    virtual ~HproseTCPClient() {
    };
    
    virtual void UseService(const std::string & uri) {
        HproseClient::UseService(uri);
        ParseURI();
    }
    
    void SetKeepAlive(bool keepAlive, int timeout = 300) {
    }

protected:
    virtual void * GetInvokeContext() {
        boost::mutex::scoped_lock lock(mutex);
        if (pool.size() > 0 ) {
            TCPContext * ctx = pool.top();
            pool.pop();
            return ctx;
        } else {
            return new TCPContext(ios);
        }
    }

    virtual void SendData(void * context) {
        if (context) {
            TCPContext & ctx = *static_cast<TCPContext *>(context);
            ctx.Send(host, port, false);
        }
    }

    virtual void EndInvoke(void * context) {
        if (context) {
            boost::mutex::scoped_lock lock(mutex);
            TCPContext * ctx = static_cast<TCPContext *>(context);
            pool.push(ctx);
        }
    }

    virtual std::ostream & GetOutputStream(void * context) {
        if (context) {
            return (static_cast<TCPContext *>(context))->requestStream;
        } else {
            HPROSE_THROW_EXCEPTION("Can''t get output stream.");
        }
    } 

    virtual std::istream & GetInputStream(void * context) {
        if (context) {
            return (static_cast<TCPContext *>(context))->responseStream;
        } else {
            HPROSE_THROW_EXCEPTION("Can''t get input stream.");
        }
    }
 
private:
    void ParseURI() {
        std::string surl;
        std::string::size_type x, y;
        protocol = "tcp";
        user = "";
        password = "";
        port = "9090";
        x = uri.find("://");
        if (x != std::string::npos) {
            protocol = uri.substr(0, x);
            surl = uri.substr(x + 3);
        } else {
            surl = uri;
        }
        std::transform(protocol.begin(), protocol.end(), protocol.begin(), tolower);
	/*
        if (protocol == "https") {
            port = "443";
        }
	*/
        x = surl.find('@');
        y = surl.find('/');
        if ((x != std::string::npos) && ((x < y) || (y == std::string::npos))) {
            user = surl.substr(0, x);
            surl.erase(0, x + 1);
            x = user.find(':');
            if (x != std::string::npos) {
                password = user.substr(x + 1);
                user.erase(x);
            }
        }
        x = surl.find('/');
        if (x != std::string::npos) {
            host = surl.substr(0, x);
            surl.erase(0, x + 1);
        } else {
            host = surl;
            surl = "";
        }
        bool ipv6 = host[0] == '[';
        if (ipv6) {
            x = host.find(']');
            if ((x + 1 < host.size()) && (host[x + 1] == ':')) {
                port = host.substr(x + 2);
            }
            host = host.substr(1, x - 1);
        } else {
            x = host.find(':');
            if (x != std::string::npos) {
                port = host.substr(x + 1);
                host.erase(x);
            }
        }
        
        if (host == "") {
            host = "localhost";
        }
    };

    class TCPContext {
    public:
        TCPContext(boost::asio::io_service & ios)
            : resolver(ios),
            socket(ios),
#ifndef HPROSE_NO_OPENSSL
            sslContext(ios, boost::asio::ssl::context::sslv23),
            sslSocket(ios, sslContext),
#endif
            requestStream(&request),
            responseStream(&response) {
        };

    public:
        void Send(const std::string & host, const std::string & port, bool secure) {
            if (!Connect(
#ifndef HPROSE_NO_OPENSSL
                secure ?
                sslSocket.next_layer() :
#endif
                socket, host, port, secure)) return;
            boost::system::error_code error = boost::asio::error::host_not_found;
            boost::asio::streambuf header;
            std::iostream headerStream(&header);
            uint32_t rs = request.size();
            uint32_t payloadSize = boost::endian::native_to_big(rs);
            headerStream.write(reinterpret_cast<const char *>(&payloadSize), sizeof(payloadSize));
            //Write(header, secure);
            headerStream << &request;
            Write(header, secure);
            if (response.size()) {
                response.consume(response.size());
            }
            std::string s;
            Read(secure); 
            
        };
    private:
        inline void Clear() {
            aliveHost.clear();
            alivePort.clear();
            aliveTime = 0;
        };
        bool Connect(tcp::socket & socket, const std::string & host, const std::string & port, bool secure) {
            if (socket.is_open() && (aliveHost == host) && (alivePort == port) && (clock() < aliveTime)) {
                return true;
            }
            tcp::resolver::query query(host, port);
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
            tcp::resolver::iterator end;
            boost::system::error_code error = boost::asio::error::host_not_found;
            while (error && endpoint_iterator != end) {
                socket.close();
                socket.connect(*endpoint_iterator++, error);
            }
            if (error) {
                Clear();
                return false;
            } else {
#ifndef HPROSE_NO_OPENSSL
                if (secure) {
                    sslSocket.handshake(boost::asio::ssl::stream_base::client, error);
                    if (error) {
                        socket.close();
                        return false;
                    }
                }
#endif
                aliveHost = host;
                alivePort = port;
                return true;
            }
        };

        void Read(bool secure) {
            boost::asio::streambuf headerBuf(4);
            int headerSize = 4;
            while (headerSize > 0 ) {
#ifndef HPROSE_NO_OPENSSL
                headerSize -= 
                    secure ?
                    boost::asio::read(sslSocket, headerBuf) :
#endif
                    boost::asio::read(socket, headerBuf);
            }
            uint32_t payloadSize = boost::endian::big_to_native(boost::asio::buffer_cast<const int32_t *>(headerBuf.data())[0]);
            boost::asio::streambuf buf(payloadSize);
            std::iostream readStream(&buf);
            while (payloadSize > 0) {
                size_t s = 
#ifndef HPROSE_NO_OPENSSL
                    secure ?
                    boost::asio::read(sslSocket, buf) :
#endif
                    boost::asio::read(socket, buf);
                for (size_t i = 0; i < s; ++i) {
                    responseStream << (char)readStream.get();
                }
                payloadSize -= s;
            }
        };
 
        void Write(boost::asio::streambuf & buf, bool secure) {
#ifndef HPROSE_NO_OPENSSL
            if (secure) {
                boost::asio::write(sslSocket, buf);
            } else {
#endif
                boost::asio::write(socket, buf);
#ifndef HPROSE_NO_OPENSSL
            }
#endif
        };

    private:
        std::string aliveHost;
        std::string alivePort;
        clock_t aliveTime;

        tcp::resolver resolver;
        tcp::socket socket;

    #ifndef HPROSE_NO_OPENSSL
        boost::asio::ssl::context sslContext;
        boost::asio::ssl::stream<tcp::socket> sslSocket;
    #endif

        boost::asio::streambuf request;
        boost::asio::streambuf response;

    public:

        std::iostream requestStream;
        std::iostream responseStream;


    };

private:
    
    std::string protocol;
    std::string user;
    std::string password;
    std::string host;
    std::string port;
    
    std::map<std::string, std::string> headers;
    std::stack<TCPContext *> pool;
    boost::mutex mutex;
    boost::asio::io_service ios;
}; // class HproseTCPClient
}} // namespaces
#endif

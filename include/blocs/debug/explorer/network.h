#pragma once

#ifdef DEBUG
#define IPSTR "127.0.0.1"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>

#define MSG_NOSIGNAL 0
#define SHUT_RDWR    2
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <blocs/common.h>

namespace blocs
{
    namespace network
    {
        namespace
        {
            inline str create_request_line(const str& url, const str& method)
            {
                return method + " " + url + " HTTP/1.1\r\n";
            }

            inline str create_response_line(const i32 status = 200)
            {
                return "HTTP/1.1 " + std::to_string(status) + " OK\r\n";
            }

            inline str create_header_field(const str& key, const str& val)
            {
                return key + ": " + val + "\r\n";
            }

            inline str concat_headers(
                const std::vector<std::pair<str, str>> headers
            )
            {
                str output = "";

                for (const auto& [key, val] : headers)
                    output += create_header_field(key, val);

                return output;
            }

            inline i32 create_socket()
            {
                return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            }

            inline void log_socket_errors()
            {
#ifdef _WIN32
                char buffer[256];
                buffer[0] = '\0';
                i32 err   = WSAGetLastError();

                LOG_ERR("WSA Error: " << std::to_string(err));

                FormatMessage(
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    buffer, sizeof(buffer), NULL
                );

                if (!*buffer) LOG_ERR(buffer);
#else
                LOG_ERR(strerror(errno));
#endif
            }

            void close_socket(i32 socket, bool cleanup = false)
            {
#ifdef _WIN32
                closesocket(socket);
                if (cleanup) WSACleanup();
#else
                close(socket);
#endif
            }
        }

        inline i32 request(
            const i32 port, const str& url, const str& method = "GET",
            const str& payload = ""
        )
        {
            bool has_payload = method != "GET" && payload.size() > 0;

#ifdef _WIN32
            auto wsa_data = WSADATA();
            if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
            {
                LOG_ERR("WSAStartup failed");
                return 1;
            }
#endif

            i32 socket_fd;
            if ((socket_fd = create_socket()) < 0)
            {
                log_socket_errors();
                return 1;
            }

            struct sockaddr_in serv_addr;
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port   = htons(port);

            if (inet_pton(AF_INET, IPSTR, &serv_addr.sin_addr) <= 0)
            {
                log_socket_errors();
                return 1;
            }

            if (connect(
                    socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)
                ) < 0)
            {
                log_socket_errors();
                return 1;
            }

            LOG_INFO("Socket connected to port");

            str port_string = std::to_string(port);

            str headers =
                (create_request_line(url, method) +
                 concat_headers({
                     {"Host", str(IPSTR) + ":" + port_string},
                     {"Connection", has_payload ? "close" : "keep-alive"},
                     {"Accept", "*/*"},
                     {"Origin", "http://127.0.0.1:" + port_string},
                     {"X-Requested-With", "XMLHttpRequest"},
                     {"User-Agent",
                      "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) "
                      "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 "
                      "Safari/537.36"},
                     {"Content-Type", "application/json"},
                     {"Content-Length", std::to_string(payload.size())},
                     {"Referer", "http://127.0.0.1:" + port_string + "/"},
                     {"Accept-Encoding", "gzip, deflate, br"},
                     {"Accept-Language", "en-US,en;q=0.8"},
                 }) +
                 "\r\n");

            if (has_payload) headers += payload;

            cstr preq     = headers.c_str();
            i32  data_len = headers.size();
            i32  send_result;

            do
            {
                send_result = send(socket_fd, preq, data_len, MSG_NOSIGNAL);
                if (send_result < 0)
                {
                    log_socket_errors();
                    close_socket(socket_fd);
                    return 1;
                }
                preq += send_result;
                data_len -= send_result;
            } while (data_len > 0);
            LOG_GOOD("Sent request");

            char buffer[4096];
            str  response;

            while ((data_len = recv(socket_fd, buffer, sizeof(buffer), 0)) > 0)
            {
                i32 i = 0;
                do
                {
                    if (buffer[i] >= 32 || buffer[i] == '\n' ||
                        buffer[i] == '\r')
                    {
                        response += buffer[i];
                    }
                    ++i;
                } while (i < data_len);
            }

            if (data_len < 0)
            {
                LOG_ERR("Couldn't read response");
                close_socket(socket_fd);
                return 1;
            }

#ifdef DEBUG
            // LOG_DEBUG(response);
#endif

            close_socket(socket_fd, true);
            return 0;
        }

        inline i32 on(
            const i32 port, const str& url = "/",
            std::function<str()> func = []() { return ""; }
        )
        {
#ifdef _WIN32
            auto wsa_data = WSADATA();
            if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
            {
                LOG_ERR("WSAStartup failed");
                return 1;
            }
#endif

            i32 socket_fd;
            if ((socket_fd = create_socket()) < 0)
            {
                log_socket_errors();
                return 1;
            }

#ifdef _WIN32
            const bool true_flag = true;
            if (setsockopt(
                    socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&true_flag,
                    sizeof(int)
                ) < 0)
#else
            const i32 true_flag = 1;
            if (setsockopt(
                    socket_fd, SOL_SOCKET, SO_REUSEADDR, &true_flag, sizeof(int)
                ) < 0)
#endif
            {
                log_socket_errors();
                return 1;
            }

            struct sockaddr_in serv_addr;
            serv_addr.sin_family      = AF_INET;
            serv_addr.sin_port        = htons(port);
            serv_addr.sin_addr.s_addr = INADDR_ANY;

            if (bind(
                    socket_fd, (const struct sockaddr*)&serv_addr,
                    sizeof(serv_addr)
                ) < 0)
            {
                log_socket_errors();
                return 1;
            }

            LOG_INFO("Listening on port " << port);

            if (listen(socket_fd, SOMAXCONN) < 0)
            {
                log_socket_errors();
                return 1;
            }

            while (true)
            {
                struct sockaddr_in client_addr;
#ifdef _WIN32
                i32 addr_size = sizeof(client_addr);
#else
                uint addr_size = sizeof(client_addr);
#endif

                struct timeval timeout;
                timeout.tv_sec  = 5;
                timeout.tv_usec = 0;

                fd_set set;
                FD_ZERO(&set);
                FD_SET(socket_fd, &set);

                select(FD_SETSIZE, &set, NULL, NULL, &timeout);

                if (FD_ISSET(socket_fd, &set))
                {
                    i32 client_socket_fd;
                    if ((client_socket_fd = accept(
                             socket_fd, (struct sockaddr*)&client_addr,
                             &addr_size
                         )) < 0)
                    {
                        log_socket_errors();
                        return 1;
                    }

                    char buffer[4096];
                    i32  request;
                    if ((request =
                             recv(client_socket_fd, buffer, sizeof(buffer), 0) <
                             0))
                    {
                        log_socket_errors();
                    }

#ifdef DEBUG
                    // LOG_DEBUG(buffer);
#endif
                    str  response     = func();
                    bool has_response = response.size() > 0;

                    str headers =
                        (create_response_line(has_response ? 200 : 204) +
                         concat_headers({
                             {"Connection", "close"},
                             {"Content-Type", "application/json"},
                             {"Content-Length",
                              std::to_string(response.size())},
                         }) +
                         "\r\n");

                    if (has_response) headers += response;

                    cstr preq     = headers.c_str();
                    i32  data_len = headers.size();
                    i32  send_result;

                    do
                    {
                        send_result = send(client_socket_fd, preq, data_len, 0);
                        if (send_result < 0)
                        {
                            log_socket_errors();
                            close_socket(socket_fd);
                            return 1;
                        }
                        preq += send_result;
                        data_len -= send_result;
                    } while (data_len > 0);
                    LOG_GOOD("Sent response");

                    shutdown(client_socket_fd, SHUT_RDWR);
                    close_socket(client_socket_fd);
                }
            }

            LOG_GOOD("shutting down socket server");
            shutdown(socket_fd, SHUT_RDWR);
            return 0;
        }
    }
}
#endif

/* 
 * mumudvb - UDP-ize a DVB transport stream.
 * 
 * (C) 2009 Brice DUBOST
 * 
 * The latest version can be found at http://mumudvb.braice.net
 * 
 * Copyright notice:
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/**@file
 * @brief HTML unicast header
 */

#ifndef _UNICAST_H
#define _UNICAST_H

#include "mumudvb.h"

#define RECV_BUFFER_MULTIPLE 100
#define UNICAST_CONSECUTIVE_ERROR_LIMIT 42

#define HTTP_OK_REPLY "HTTP/1.0 200 OK\r\n"\
                      "Content-type: application/octet-stream\r\n"\
                      "\r\n"

/**@todo : Content length
Applications should use this field to indicate the size of the
   Entity-Body to be transferred, regardless of the media type of the
   entity. A valid Content-Length field value is required on all
   HTTP/1.0 request messages containing an entity body.
*/
#define HTTP_OK_TEXT_REPLY "HTTP/1.0 200 OK\r\n"\
                      "Content-type: text/plain\r\n"\
                      "\r\n"

#define HTTP_OK_HTML_REPLY "HTTP/1.0 200 OK\r\n"\
                      "Content-type: text/html\r\n"\
                      "\r\n"

#define HTTP_404_REPLY "HTTP/1.0 404 Not found\r\n"\
                      "Content-type: text/html\r\n"\
                      "\r\n"


#define HTTP_404_REPLY_HTML "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml10/DTD/xhtml10strict.dtd\">\r\n"\
                            "<html lang=\"en\">\r\n"\
                            "<head>\r\n"\
                            "<title>Not found</title>\r\n"\
                            "</head>\r\n"\
                            "<body>\r\n"\
                            "   <h1>404 Not found</h1>\r\n"\
                            "<hr />\r\n"\
                            "<a href=\"http://mumudvb.braice.net\">MuMuDVB</a>\r\n"\
                            "</body>\r\n"\
                            "</html>\r\n"\
                            "\r\n"

#define HTTP_CHANNELS_REPLY_START "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml10/DTD/xhtml10strict.dtd\">\r\n"\
                            "<html lang=\"en\">\r\n"\
                            "<head>\r\n"\
                            "<title>Channels list</title>\r\n"\
                            "</head>\r\n"\
                            "<body>\r\n"\
                            "   <h1>Channel list</h1>\r\n"\
                            "<hr />\r\n"\
                            "This is the list of actually streamed channels by the MuMuDVB server. To open a channel copy the link to your client or use multicast.\r\n"\
                            "<hr />\r\n"\

#define HTTP_CHANNELS_REPLY_END "<hr />\r\n"\
                            "See <a href=\"http://mumudvb.braice.net\">MuMuDVB</a> website for more details.\r\n"\
                            "</body>\r\n"\
                            "</html>\r\n"\
                            "\r\n"


#define HTTP_501_REPLY "HTTP/1.0 501 Not implemented\r\n"\
                      "\r\n"



typedef struct unicast_client_t{
  /**HTTP socket*/
  struct sockaddr_in SocketAddr;
  /**HTTP socket*/
  int Socket;
  /**Reception buffer*/
  char *buffer;
  /**Size of the buffer*/
  int buffersize;
  /**Position in the buffer*/
  int bufferpos;
  /**Number of consecutive errors*/
  int consecutive_errors;
  /**Channel : -1 if not associated yet*/
  int channel;
  /**Next client*/
  struct unicast_client_t *next;
  /**Previous client*/
  struct unicast_client_t *prev;
  /**Next client in the channel*/
  struct unicast_client_t *chan_next;
  /**Previous client in the channel*/
  struct unicast_client_t *chan_prev;
}unicast_client_t;


/**@brief The parameters for unicast
 *
 * @ipOut The "HTML" ip address
 * @portOut The "HTML" port
 * @sIn The HTTP input socket
 * @socketIn The HTTP input socket
 * @clients The clients, contains all the clients, associated to a channel or not
*/
typedef struct unicast_parameters_t{
  char ipOut[20];
  int portOut;
  struct sockaddr_in sIn;
  int socketIn;
  unicast_client_t *clients;
}unicast_parameters_t;


int unicast_accept_connection(unicast_parameters_t *unicast_vars);
int unicast_add_client(unicast_parameters_t *unicast_vars, struct sockaddr_in SocketAddr, int Socket);
void unicast_close_connection(unicast_parameters_t *unicast_vars, fds_t *fds, int Socket, mumudvb_channel_t *channels);

int unicast_del_client(unicast_parameters_t *unicast_vars, int Socket, mumudvb_channel_t *channels);
unicast_client_t *unicast_find_client(unicast_parameters_t *unicast_vars, int Socket);

int unicast_handle_message(unicast_parameters_t *unicast_vars, int fd, mumudvb_channel_t *channels, int num_of_channels);
int channel_add_unicast_client(unicast_client_t *client,mumudvb_channel_t *channel);

void unicast_freeing(unicast_parameters_t *unicast_vars, mumudvb_channel_t *channels);

#endif
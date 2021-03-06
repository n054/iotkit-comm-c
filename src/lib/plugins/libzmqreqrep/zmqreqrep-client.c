/*
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** @file zmqreqrep-client.c

    Provides functions to send and receive message to/from a Service it is connected to.
*/

#include "zmqreqrep-client.h"
#include "../inc/zhelpers.h"

/** @defgroup zmqreqrepclient
*  This is ZMQ requester client.
*  @{
*/

/** Creates the context object and requester socket. With the help of the ServiceQuery parameter, the requester socket
establishes, connection to the address and port to initiate communication.
* @param requestClientQuery an void pointer
* @param crypto authentication object
* @return The result code
*/
int init(void *requestClientQuery, Crypto *crypto) {
    #if DEBUG
        printf("context initialised\n");
    #endif
    ServiceQuery *serviceQuery = (ServiceQuery *)requestClientQuery;
    zmqContainer.context = zmq_ctx_new();
    int rc = -1;
    // This is client side
    zmqContainer.requester = zmq_socket(zmqContainer.context,ZMQ_REQ);
    char addr[128];
    sprintf(addr, "tcp://%s:%d",serviceQuery->address, serviceQuery->port);
    #if DEBUG
        printf("going to connect %s\n",addr);
    #endif
    rc = zmq_connect(zmqContainer.requester,addr);
    #if DEBUG
        printf("connect completed\n");
    #endif
    return rc;
}

/** Sending a message. The client can send a message to the service it is connected to.
* @param message a string message
* @param context a context message
* @return The result code
*/
int send(char *message,Context context) {
    #if DEBUG
        printf ("Sending message from Client %s...\n",message);
    #endif
    int rc = s_send(zmqContainer.requester, message);
    return rc;
}

/** Receive the message. The parameter in this function is used as a callback mechanism to pass the
received message.
* @param requestClientHandler a callback handler which takes message and context object as params
* @return The result code
*/
int receive(void (*requestClientHandler)(char *message, Context context)) {
    #if DEBUG
        printf("In receive\n");
    #endif
    int rc = 0;
    //  Read message contents
    char *contents = s_recv (zmqContainer.requester);
    #if DEBUG
        printf ("Received message in client: %s\n", contents);
    #endif
    if (contents == NULL) {
        rc = -1;
    }
    Context context;
    context.name = "event";
    context.value = "message";
    requestClientHandler(contents,context);
    free(contents);
    return rc;
}

/** Cleanup function. This function close the requester socket and destroy the context object.
* @return The result code
*/
int done() {
    int rc = -1;
    if (zmqContainer.requester != NULL) {
        rc = zmq_close(zmqContainer.requester);
        zmqContainer.requester = NULL;
        #if DEBUG
            printf("requester freed\n");
        #endif
    }
    if (zmqContainer.context != NULL) {
        rc = zmq_ctx_destroy(zmqContainer.context);
        zmqContainer.context = NULL;
        #if DEBUG
            printf("context freed\n");
        #endif
    }
    #if DEBUG
        printf("\nclosed\n");
    #endif
    return rc;
}
/** @} */ // end of zmqreqrepclient

/*
 * SocketMessageIO.h
 *
 *  Created on: 28 sept. 2011
 *      Author: Guillaume Chatelet
 */

#ifndef SOCKETMESSAGEIO_H_
#define SOCKETMESSAGEIO_H_

#include "MessageQueue.h"

#include <dukeapi/serialize/ProtobufSocket.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <stdexcept>

struct SocketSession : public google::protobuf::serialize::DukeSession {
    SocketSession(boost::asio::io_service& io_service, MessageQueue &in, MessageQueue &out) :
        DukeSession(io_service), inputQueue(in), outputQueue(out), closing(false) {
    }

protected:
    virtual void incomingConnection() {
        typedef boost::asio::ip::tcp::endpoint endpoint;
        endpoint local = socket().local_endpoint();
        endpoint remote = socket().remote_endpoint();
        std::cout << "Connected : " << local << " <=> " << remote << std::endl;
        sendLoopThread.reset(new boost::thread(&SocketSession::sendLoop, this));
    }

    virtual void incomingMessage(const google::protobuf::serialize::MessageHolder &msg) {
        using namespace google::protobuf::serialize;
        switch (msg.action()) {
            case MessageHolder_Action_CLOSE_AND_RELAUNCH_CONNECTION:
                relaunch(); // fallthrough next case
            case MessageHolder_Action_CLOSE_CONNECTION:
                closing = true;
                outputQueue.push(SharedHolder()); // pushing sentinel
                sendLoopThread->join();
                close();
                break;
            default:
                inputQueue.push(google::protobuf::serialize::makeSharedHolder(msg));
        }
    }

    virtual void disconnecting(const boost::system::error_code&e) {
        using namespace google::protobuf::serialize;
        if (!closing)
            std::cerr << "Abnormal termination : " << e.message() << std::endl;
    }

    void sendLoop() {
        using namespace google::protobuf::serialize;
        try {
            SharedHolder holder;
            while (true) {
                outputQueue.waitPop(holder);

                if (!holder || closing) // received sentinel or last sent message was close
                    break; // stopping
                else {
                    post(*holder);
                    switch (holder->action()) {
                        case MessageHolder_Action_CLOSE_AND_RELAUNCH_CONNECTION:
                        case MessageHolder_Action_CLOSE_CONNECTION:
                            closing = true;
                            break;
                        default:
                            break;
                    }
                }
            }
            // emptying output queue
            bool leftItems = false;
            while (outputQueue.tryPop(holder))
                leftItems = true;
            if (leftItems)
                std::cerr << "Disconnection occurred but messages are still in the output queue, discarding them all" << std::endl;
        } catch (std::exception &e) {
            std::cerr << "Exception occurred within send loop : " << e.what() << std::endl;
        }
    }

private:
    MessageQueue &inputQueue;
    MessageQueue &outputQueue;
    std::unique_ptr<boost::thread> sendLoopThread;
    bool closing;
};

#endif /* SOCKETMESSAGEIO_H_ */

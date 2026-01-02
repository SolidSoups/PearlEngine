#pragma once

// messages have the following characteristics:
//  a) Messages contain data
//  b) A user can subscribe to recieve a message type
//  c) Messages and message types have a strong 1:1 corrolation
//  d) Subscribing to recieve a message type should always produce the same data
//
// in order to improve type safety, I have devised the following plan
//  1) Messages types and message data are the same types (a) (c)
//  2) The user subscribes to a message type using a templated function (b) (d)
//    2.1) Casting a message to the wrong type should never happen, since
//    messages are a core
//         part of the program... messages should never crash the engine
//
//  notes:
//   - There is no constraint on the types of messages that can be sent, if a client
//     would subscribe to 'int' it would be fine

#include <string>

struct TestMessage {
  std::string foo;
};



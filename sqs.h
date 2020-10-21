#pragma once

#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/DeleteMessageRequest.h>
#include <aws/sqs/model/ReceiveMessageRequest.h>
#include <aws/sqs/model/ReceiveMessageResult.h>

#include <iostream>
#include <list>
#include <memory>
#include <ostream>
#include <stdexcept>

using Aws::SQS::Model::Message;

namespace SimpleAWS {

class Sqs {
  const std::string& queue_url;
  Aws::SQS::SQSClient sqs;

 public:
  Sqs(Aws::Client::ClientConfiguration& config, const Aws::String& queue_url)
      : queue_url(queue_url), sqs(config) {}

  Message ReceiveMessage() {
    Aws::SQS::Model::ReceiveMessageRequest rm_req;
    rm_req.SetQueueUrl(queue_url);
    rm_req.SetMaxNumberOfMessages(1);

    auto rm_out = sqs.ReceiveMessage(rm_req);
    if (!rm_out.IsSuccess())
      throw std::runtime_error("Error receiving message from queue ");

    const auto& messages = rm_out.GetResult().GetMessages();
    if (messages.size() == 0)
      throw std::runtime_error("No messages received from queue ");

    return messages[0];
  }

  void DeleteMessage(const Message& message) {
    Aws::SQS::Model::DeleteMessageRequest dm_req;
    dm_req.SetQueueUrl(queue_url);
    dm_req.SetReceiptHandle(message.GetReceiptHandle());

    auto dm_out = sqs.DeleteMessage(dm_req);
    if (!dm_out.IsSuccess())
      throw std::runtime_error(dm_out.GetError().GetMessage());
  }
};
}  // namespace SimpleAWS

std::ostream& operator<<(std::ostream& out, const Message& message) {
  return out << "\n  MessageId: " << message.GetMessageId()
             << "\n  ReceiptHandle: " << message.GetReceiptHandle()
             << "\n  Body: " << message.GetBody() << std::endl;
}

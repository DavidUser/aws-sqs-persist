#include <aws/core/Aws.h>

#include <chrono>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "dynamodb.h"
#include "sqs.h"

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace SimpleAWS;

using Aws::Client::ClientConfiguration;

constexpr auto DB_TABLE_NAME = "test";
constexpr auto MAX_SQS_LONG_POLLING = 30s;

void ConsumeMessage(ClientConfiguration& config, const Aws::String& queue_url) {
  try {
    Sqs sqs(config, queue_url);
    DynamoDB db(config);

    const auto& message = sqs.ReceiveMessage();
    std::cout << "Received message: " << message;

    db.insert(DB_TABLE_NAME,
              {{"Name", message.GetMessageId()}, {"Value", message.GetBody()}});
    std::cout << "Saved on database" << std::endl;

    sqs.DeleteMessage(message);
    std::cout << "Successfully deleted message " << message.GetMessageId()
              << " from queue " << queue_url << std::endl;
  } catch (std::exception& error) {
    std::cerr << "Error consuming queue: " << queue_url << "\n"
              << error.what() << std::endl;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <queue_url>" << std::endl;
    return 1;
  }

  Aws::SDKOptions options;
  Aws::InitAPI(options);
  {
    ClientConfiguration config;
    config.requestTimeoutMs = milliseconds(MAX_SQS_LONG_POLLING).count();
    ConsumeMessage(config, argv[1]);
  }
  Aws::ShutdownAPI(options);
  return 0;
}

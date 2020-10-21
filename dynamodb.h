#pragma once

#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>

#include <exception>
#include <map>
#include <stdexcept>
#include <string>

namespace SimpleAWS {

using std::string;
using namespace Aws::DynamoDB::Model;

class DynamoDB {
  Aws::DynamoDB::DynamoDBClient client;

 public:
  DynamoDB(Aws::Client::ClientConfiguration& config) : client(config) {}
  void insert(string table, std::map<string, string> attributes) {
    PutItemRequest request;
    request.SetTableName(table);

    for (auto& [key, value] : attributes)
      request.AddItem(key, AttributeValue(value));

    PutItemOutcome result = client.PutItem(request);
    if (!result.IsSuccess())
      throw std::runtime_error(result.GetError().GetMessage());
  }
};

}  // namespace SimpleAWS

// Created by Patryk Jędrzejczak and Jan Wojtach.

#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>
#include <regex>
#include <queue>

using namespace std;

namespace {
  using Node = uint32_t; // type representing signal
  using Gate = string;
  using Result = string;
  using NodeVector = vector<Node>;
  using NodeGraph = unordered_map<Node, NodeVector>;
  using NodeSet = set<Node>;
  using NodeQueue = queue<Node>;
  using GateMap = unordered_map<Node, Gate>;
  using LogicalValueMap = unordered_map<Node, bool>;
  using NodeMap = unordered_map<Node, Node>;

  const string NAND_TYPE = "NAND";
  const string AND_TYPE = "AND";
  const string NOR_TYPE = "NOR";
  const string OR_TYPE = "OR";
  const string XOR_TYPE = "XOR";

  NodeGraph outputNeighbors;
  NodeMap numOfInputNeighbors;
  GateMap gateType;
  NodeSet outputNodes;
  NodeSet nodes;
  NodeVector inputNodes;

  void setInputNodes() {
    for (Node node : nodes) {
      if (outputNodes.find(node) == outputNodes.end()) {
        inputNodes.push_back(node);
      }
    }
  }

  NodeQueue getInputNodesAsNodeQueue() {
    NodeQueue queue;

    for (Node node : inputNodes) {
      queue.push(node);
    }

    return queue;
  }

  bool neutralValue(Gate actGate) {
      if (actGate == NAND_TYPE) return false;
      else if (actGate == AND_TYPE) return true;
      else if (actGate == NOR_TYPE) return true;
      else if (actGate == OR_TYPE) return false;
      else if (actGate == XOR_TYPE) return false;
      else return false; // actGate == "NOT"
  }

  bool computeValue(Gate actGate, bool acc, bool newValue) {
    if (actGate == NAND_TYPE) return !(!acc & newValue);
    else if (actGate == AND_TYPE) return acc & newValue;
    else if (actGate == NOR_TYPE) return !(!acc | newValue);
    else if (actGate == OR_TYPE) return acc | newValue;
    else if (actGate == XOR_TYPE) return acc ^ newValue;
    else return !newValue; // actGate == "NOT"
  }

  // Uses BFS algorithm to compute logical value of every Node.
  void pushValues(LogicalValueMap& values,
                  NodeQueue& leaves, NodeMap& signalsReceived) {
    while (!leaves.empty()) {
      Node leaf = leaves.front();
      leaves.pop();
      for (Node neighbor : outputNeighbors[leaf]) {
        if (signalsReceived[neighbor] == 0) {
          values[neighbor] = neutralValue(gateType[neighbor]);
        }
        values[neighbor] = computeValue(gateType[neighbor],
                                        values[neighbor], values[leaf]);
        signalsReceived[neighbor]++;
        if (signalsReceived[neighbor] == numOfInputNeighbors[neighbor]) {
          leaves.push(neighbor);
        }
      }
    }
  }

  // Creates and returns result based on previously computed logical values.
  Result createResult(LogicalValueMap& values) {
    Result result = "";

    for (Node node : nodes) {
      result += values[node] ? "1" : "0";
    }

    return result;
  }

  // Returns result for particular input logical values.
  Result simulateAndGetResult(LogicalValueMap& values) {
    NodeMap signalsReceived;
    NodeQueue leaves = getInputNodesAsNodeQueue();

    pushValues(values, leaves, signalsReceived);

    return createResult(values);
  }

  // Computes possible results and writes them to stdout in a correct order.
  void computeResults(LogicalValueMap& nodeValue, size_t index) {
    if (index == inputNodes.size()) {
      cout << simulateAndGetResult(nodeValue) << '\n';
      return;
    }

    nodeValue[inputNodes[index]] = false;
    computeResults(nodeValue, index + 1);
    nodeValue[inputNodes[index]] = true;
    computeResults(nodeValue, index + 1);
  }

  // Checks if all signals were received (if every edge was used) during
  // execution of pushValues function.
  bool allSignalsReceived(NodeMap& signalsReceived) {
    for (Node node : nodes) {
      if (signalsReceived[node] != numOfInputNeighbors[node]) {
        return false;
      }
    }

    return true;
  }

  // Returns true if there is a cycle in outputNeighbors graph.
  bool cycleExists() {
    LogicalValueMap onlyFalseValues;
    NodeMap signalsReceived;
    NodeQueue leaves = getInputNodesAsNodeQueue();

    pushValues(onlyFalseValues, leaves, signalsReceived);

    // If not all signals were received, cycle exists.
    return !allSignalsReceived(signalsReceived);
  }

  // Updates global containers with data from one correct line.
  void updateContainers(Gate type, Node outputNode, NodeVector& newInputNodes) {
    gateType[outputNode] = type;
    outputNodes.insert(outputNode);
    nodes.insert(outputNode);
    numOfInputNeighbors[outputNode] += newInputNodes.size();
    for (Node node : newInputNodes) {
      outputNeighbors[node].push_back(outputNode);
      nodes.insert(node);
    }
  }

  // Reads data from one line. Returns false if outputNode has already been
  // assigned to an output.
  bool readData(unsigned long long lineIndex, string& line) {
    stringstream ss(line);
    Gate type;
    Node outputNode, nextIputNode;
    NodeVector newInputNodes;

    ss >> type >> outputNode;
    while (ss >> nextIputNode) {
      newInputNodes.push_back(nextIputNode);
    }

    if (outputNodes.find(outputNode) != outputNodes.end()) {
      cerr << "Error in line " << lineIndex << ": signal " << outputNode
           << " is assigned to multiple outputs.\n";
      return false;
    }

    updateContainers(type, outputNode, newInputNodes);

    return true;
  }

  // Processes line and returns false if it is incorrect.
  bool processLine(unsigned long long lineIndex, string& line) {
    static const string nAndnOrPattern = "(N{0,1}(AND|OR)(\\s+[1-9][0-9]{0,8}){3,})";
    static const string notPattern = "(NOT(\\s+[1-9][0-9]{0,8}){2})";
    static const string xorPattern = "(XOR(\\s+[1-9][0-9]{0,8}){3})";
    static regex pattern("\\s*(" + nAndnOrPattern + "|" + notPattern +
                         "|" + xorPattern + ")\\s*");

    if (!regex_match(line, pattern)) {
      cerr << "Error in line " << lineIndex << ": " << line << '\n';
      return false;
    }

    return readData(lineIndex, line);
  }

  // Processes input and returns false if input is incorrect or empty.
  bool processInput() {
    unsigned long long lineIndex = 0;
    bool invalidInput = false;
    string line;

    while (getline(cin, line)) {
      lineIndex++;
      invalidInput |= !processLine(lineIndex, line);
    }

    return !(invalidInput || lineIndex == 0);
  }
}

int main() {
  if (!processInput()) {
    return 0;
  }

  setInputNodes();

  if (cycleExists()) {
    cerr << "Error: sequential logic analysis has not yet been implemented.\n";
    return 0;
  }

  LogicalValueMap nodeValue;
  computeResults(nodeValue, 0);

  return 0;
}

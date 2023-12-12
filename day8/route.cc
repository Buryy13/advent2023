#include <fstream>
#include <iostream>
#include <unordered_map>
#include <list>

using std::cout;
using std::endl;
using ull = unsigned long long;

struct Node
{
  Node() = default;
  Node(std::string l, std::string r) : left(l), right(r) {}

  std::string left;
  std::string right;
};

int main()
{
  std::ifstream file;
  file.open("route.txt");
  std::string route;
  std::string str;
  std::unordered_map<std::string, Node> nodes;
  std::list<std::unordered_map<std::string, Node>::iterator> nodesItrs;

  std::getline(file, route); // read route
  std::getline(file, str); // read empty string

  // read all nodes
  while(std::getline(file, str))
  {
    Node node(str.substr(7,3), str.substr(12,3));
    nodes[str.substr(0,3)] = node;
    if(str[2] == 'A')
      nodesItrs.push_back(nodes.find(str.substr(0,3)));
  }

  ull steps = 0;
  bool finished = false;
  int amountOfFinishedNodes;
  int sizeOfItrNodes = nodesItrs.size();
  
  while(!finished)
  {
    for(int i = 0; i < route.size(); ++i)
    {
      amountOfFinishedNodes = 0;
      for(int j = 0; j < sizeOfItrNodes; ++j)
      {
        auto it = nodesItrs.front();
        if(route[i] == 'L') nodesItrs.push_back(nodes.find(it->second.left));
        if(route[i] == 'R') nodesItrs.push_back(nodes.find(it->second.right));
        if(it->first[2] == 'Z') amountOfFinishedNodes++;
        nodesItrs.pop_front();
      }
      if(amountOfFinishedNodes == sizeOfItrNodes)
      {
        finished = true;
        break;
      }
      steps++;
    }
  }
  cout << "Steps: " << steps << endl;
  // part 1
  // while(std::getline(file, str))
  // {
  //   Node node(str.substr(7,3), str.substr(12,3));
  //   nodes[str.substr(0,3)] = node;
  // }
  // auto node = nodes.find("AAA");
  // int steps = 0;
  // bool finished = false;
  // while(!finished)
  // {
  //   for(int i = 0; i < route.size(); ++i)
  //   {
  //     if(node->first == "ZZZ") { finished = true; break; }
  //     if(route[i] == 'L') node = nodes.find(node->second.left);
  //     if(route[i] == 'R') node = nodes.find(node->second.right);
  //     steps++;
  //   }
  // }
  // cout << "Steps: " << steps << endl;
  return 0;
}
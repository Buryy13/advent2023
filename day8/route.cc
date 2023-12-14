#include <fstream>
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <numeric>

using std::cout;
using std::endl;
using ull = unsigned long long;

struct Node
{
  Node() = default;
  Node(std::string n, std::string l, std::string r) : name(n), left(l), right(r) {}

  std::string name;
  std::string left;
  std::string right;
};
constexpr auto lcm(auto x, auto... xs)
{
    return ((x = std::lcm(x, xs)), ...);
}
int main()
{
  std::ifstream file;
  file.open("route.txt");
  std::string route;
  std::string str;
  std::unordered_map<std::string, Node> nodes;
  std::vector<std::pair<Node*, ull>> startNodes;
  startNodes.reserve(6);

  std::getline(file, route); // read route
  std::getline(file, str); // read empty string

  // read all nodes
  while(std::getline(file, str))
  {
    Node node(str.substr(0,3), str.substr(7,3), str.substr(12,3));
    nodes[str.substr(0,3)] = node;

    // save those which end with 'A'
    if(str[2] == 'A')
      startNodes.push_back({&(nodes.find(str.substr(0,3))->second), 0});
  }

  ull res = 1;
  bool finished = false;
  
  while(!finished)
  {
    for(int i = 0; i < route.size(); ++i)
    {
      auto it = startNodes.begin();
      while(it != startNodes.end())
      {
        auto& startNodePtr = it->first;
        auto& steps = it->second;
        if(startNodePtr->name[2] == 'Z') 
        {
          cout << "Steps: " << steps << endl;
          res = lcm(res, steps);
          it = startNodes.erase(it);
        }
        else
        {
          if(route[i] == 'L') startNodePtr = &(nodes.find(startNodePtr->left)->second);
          if(route[i] == 'R') startNodePtr = &(nodes.find(startNodePtr->right)->second);
          steps++;
          it++;
        }
      }
      if(startNodes.empty())
      {
        finished = true;
        break;
      }
    }
  }
  cout << "Res: " << res << endl;

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
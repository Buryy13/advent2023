#include <fstream>
#include <iostream>
#include <list>
#include <vector>

using std::cout;
using std::endl;
using ll = long long;
using Numbers = std::vector<ll>;
using ListNumbers = std::list<ll>;

ListNumbers getNumbersFromString(const std::string& str)
{
  // 83 86  6 
  // 012345678
  ListNumbers numbers;
  size_t delim = str.find_first_of(' ');
  size_t first = 0;
  while(delim != std::string::npos)
  {
    std::string strNum = str.substr(first, delim-first);
    if(strNum.size())
      numbers.push_back(std::stoll(strNum));
    first = delim+1;
    delim = str.find_first_of(' ', first);
  }
  numbers.push_back(std::stoll(str.substr(first, delim)));
  return numbers;
}

bool allZeros(const ListNumbers& numbers)
{
  for(const auto& n : numbers)
    if(n != 0) return false;
  return true;
}

void fillPlaceholders(std::vector<ListNumbers>& placeholders)
{
  while(!allZeros(*(placeholders.crbegin())))
  {
    // 0 3 6 9 12 15
    // 0 1 2 3 4  5
    ListNumbers next;
    const ListNumbers& prev = *(placeholders.crbegin());
    for(auto it = std::next(prev.begin()); it != prev.end(); it++)
      next.push_back(*it - *std::prev(it));

    for(const auto& n : next)
      cout << n << " ";
    cout << endl;
    placeholders.push_back(next);
  }
}

void getPredictions(std::vector<ListNumbers>& placeholders)
{
  // part 1
  // for(int i = placeholders.size()-1; i > 0; --i)
  // {
  //   auto& current = placeholders[i];
  //   auto& nextFromBottom = placeholders[i-1];
  //   auto& lastCurrent = current[current.size()-1];
  //   auto& lastNextFromBottom = nextFromBottom[nextFromBottom.size()-1];
  //   nextFromBottom.push_back(lastCurrent+lastNextFromBottom);
  // }

  // part 2
  for(auto rit = placeholders.rbegin(); rit != std::prev(placeholders.rend()); rit++)
  {
    auto& current = *rit;
    auto& nextFromBottom = *(rit+1);
    auto& firstCurrent = *(current.cbegin());
    auto& firstNextFromBottom = *(nextFromBottom.cbegin());
    nextFromBottom.push_front(firstNextFromBottom - firstCurrent);
  }
}

ll getPrediction(const ListNumbers& numbers)
{
  std::vector<ListNumbers> placeholders;
  placeholders.push_back(numbers);

  fillPlaceholders(placeholders);
  getPredictions(placeholders);
  
  // part 1
  // const ListNumbers& history = placeholders.at(0);
  // for(const auto& n : history)
  //   cout << n << " ";
  // cout << endl;
  // return *(history.rbegin());

  // part 2
  const ListNumbers& history = placeholders.at(0);
  return *(history.begin());
}

int main()
{
  std::ifstream file;
  file.open("oasis.txt");
  std::string str;
  ll res = 0;

  while(std::getline(file, str))
  {
    cout << str << endl;
    res += getPrediction(getNumbersFromString(str));
  }
  cout << "Result: " << res << endl;
  return 0;
}
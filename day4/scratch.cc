#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;

std::set<int> getNumbersFromString(const std::string& str)
{
  // 83 86  6 
  // 012345678
  std::set<int> numbers;
  size_t delim = str.find_first_of(' ');
  size_t first = 0;
  while(delim != std::string::npos)
  {
    std::string strNum = str.substr(first, delim-first);
    if(strNum.size())
      numbers.insert(std::stoi(strNum));
    first = delim+1;
    delim = str.find_first_of(' ', first);
  }
  numbers.insert(std::stoi(str.substr(first, delim)));
  return numbers;
}
std::pair<std::string, std::string> getWinAndGuesNumbers(const std::string& str)
{
  auto colon = str.find_first_of(":");
  auto vert = str.find_first_of("|", colon);
  std::string winStr = str.substr(colon+2, vert-colon-3);
  std::string guesStr = str.substr(vert+2);

  return {winStr, guesStr};
}
int main()
{
  std::ifstream file;
  file.open("scratch.txt");
  std::string str;
  int res = 0;
  int cardNumber = 0;
  std::unordered_map<int, int> cards;
  cards.reserve(194);
  for(int i = 1; i <= 194; ++i) cards[i]=1;
  while(std::getline(file, str))
  {
    cardNumber++;

    cout << "str: " << str << endl;

    const auto [winStr, guesStr] = getWinAndGuesNumbers(str);
    cout << "win str: " << winStr << endl;
    cout << "guess str: " << guesStr << endl;

    std::set<int> winNumbers = getNumbersFromString(winStr);
    std::set<int> guesNumbers = getNumbersFromString(guesStr);

    std::vector<int> intersect;
    std::set_intersection(winNumbers.begin(), winNumbers.end(),
                          guesNumbers.begin(), guesNumbers.end(),
                          std::back_inserter(intersect));


    int numOfCardsToCopy = intersect.size();
    cout << "NumCardsToCopy: " << numOfCardsToCopy << endl;
    for(int i = 1; i <= numOfCardsToCopy; ++i)
      cards[cardNumber+i]+=cards[cardNumber];
    // if(intersect.empty()) continue;
    // cout << "There are " << intersect.size() << " winning numbers!" << endl;
    // win<<=intersect.size()-1;
    // cout << "WIN: " << win << endl;
    // res+=win;
  }
  cout << "Total card processed: " << cardNumber << endl;
  for(const auto& [_, n] : cards) res+=n;
  cout << "result: " << res << endl;
  return 0;
}
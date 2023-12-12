
#include <fstream>
#include <iostream>
#include <map>

int main()
{
  const std::map<int, std::string> digitToString = {
    { 1, "one" },
    { 2, "two" },
    { 3, "three" },
    { 4, "four" },
    { 5, "five" },
    { 6, "six" },
    { 7, "seven" },
    { 8, "eight" },
    { 9, "nine" }
  };
  std::ifstream file;
  file.open("trebuchet.txt");
  std::string str;
  int n = 0;
  while(std::getline(file, str))
  {
    std::cout << str << std::endl;
    std::size_t first = str.find_first_of("0123456789");
    std::string firstDigit(1, str[first]);

    // find first digit word
    for(const auto& pair : digitToString)
    {
      auto indx = str.find(pair.second, 0);
      if(indx < first) 
      {
        first = indx;
        firstDigit = std::to_string(pair.first);
      }
    }
    std::cout << firstDigit << std::endl;
    
    std::size_t last = str.find_last_of("0123456789");
    std::string lastDigit(1, str[last]);
    if(last == std::string::npos)
    {
      lastDigit = firstDigit;
      last = first;
    }
    // find last digit word
    for(const auto& [digit, digitWord] : digitToString)
    {
      size_t indx;
      if(last != std::string::npos)
        indx = str.find(digitWord, last+1);

      while(indx != std::string::npos)
      {
        last = indx;
        indx = str.find(digitWord, last+digitWord.size());
        lastDigit = std::to_string(digit);
      }
    }
    std::cout << lastDigit << std::endl;
    std::string calVal = firstDigit+lastDigit;
    std::cout << calVal << std::endl;
    n += stoi(calVal);
    std::cout << n << std::endl;
  }
  // std::cout << n << std::endl;
  return 0;
}
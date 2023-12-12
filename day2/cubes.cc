#include <fstream>
#include <iostream>
#include <map>
#include <string>

int main()
{
  const std::map<int, std::string> colorLimits = {
    { 12, "red" },
    { 13, "green" },
    { 14, "blue" }
  };
  std::map<std::string, int> colorCubes = {
    {"red", 0},
    {"green", 0},
    {"blue", 0}
  };
  std::ifstream file;
  file.open("cubes.txt");
  std::string str;
  int i = 1;
  int res = 0;
  while(std::getline(file, str))
  {
    std::cout << str << std::endl;
    bool possible = true;
    while(str.size())
    {
      size_t delim = str.find(';');
      std::string substr(str, 0, delim);
      if(delim == std::string::npos) str = "";
      else str = str.substr(delim+1);
      // std::cout << delim << std::endl;
      std::cout << substr << std::endl;
      // std::cout << str << std::endl;

      // part 1
      // for(const auto& [limit, color] : colorLimits)
      // {
      //   size_t indx = substr.find(color);
      //   if(indx != std::string::npos)
      //   {
      //     int number;
      //     if(std::isdigit(substr[indx-3]))
      //       number = std::stoi(std::string(substr, indx-3, 2));
      //     else
      //       number = std::stoi(std::string(substr, indx-2, 1));
      //     if(number > limit) { possible = false; break; }
      //   }
      // }
    // }
    // if(possible) { res+=i; std::cout << "Adding game ID: " << i << std::endl; }
    // ++i;

      // part 2
      for(auto& [color, max] : colorCubes)
      {
        size_t indx = substr.find(color);
        if(indx != std::string::npos)
        {
          int number;
          if(std::isdigit(substr[indx-3]))
            number = std::stoi(std::string(substr, indx-3, 2));
          else
            number = std::stoi(std::string(substr, indx-2, 1));
          if(number > max) max = number;
        }
      }
    }

    // count and reset
    int power = 1;
    for (auto& [_, max] : colorCubes) 
    {
      power*=max;
      max = 0;
    }
    std::cout << "Power number to add: " << power << std::endl;
    res+=power;
  }
  std::cout << "Sum of possible game`s IDs: " << res << std::endl;
  return 0;
}
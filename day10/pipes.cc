#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using Col = int;
using Row = int;
std::pair<Row, Col> getStart(std::vector<std::string>& field)
{
  for(int i = 0; i < field.size(); ++i)
    for(int j = 0; j < field[i].size(); ++j)
      if(field[i][j] == 'S') return {i, j};
  return {};
}

bool matchTop(const char& prev, const char& next)
{
  cout << prev << " " << next << endl;
  if((prev == 'S' || prev == '|' || prev == 'L' || prev == 'J') && (next == '|' || next == 'F' || next == '7' || next == 'S')) return true;
  return false;
}
bool matchBot(const char& prev, const char& next)
{
  cout << prev << " " << next << endl;
  if((prev == 'S' || prev == '|' || prev == 'F' || prev == '7') && (next == '|' || next == 'L' || next == 'J' || next == 'S')) return true;
  return false;
}
bool matchLeft(const char& prev, const char& next)
{
  cout << prev << " " << next << endl;
  if((prev == 'S' || prev == '-' || prev == 'J' || prev == '7') && (next == '-' || next == 'L' || next == 'F' || next == 'S')) return true;
  return false;
}
bool matchRight(const char& prev, const char& next)
{
  cout << prev << " " << next << endl;
  if((prev == 'S' || prev == '-' || prev == 'L' || prev == 'F') && (next == '-' || next == 'J' || next == '7' || next == 'S')) return true;
  return false;
}

std::pair<Row, Col> getNextPos(std::vector<std::string>& field, std::pair<Row, Col> curPos, std::pair<Row, Col> prevPos)
{
  auto& [curRow, curCol] = curPos;
  auto& [prevRow, prevCol] = prevPos;
  cout << "Trying to get next pos" << endl;
  cout << curRow << " " << curCol << endl;
  cout << prevRow << " " << prevCol << endl;
  // check top
  if(curRow != 0 && prevRow != curRow-1 && matchTop(field[curRow][curCol], field[curRow-1][curCol]))
    return {curRow-1, curCol};
  // check bot
  if(curRow != field.size()-1 && prevRow != curRow+1 && matchBot(field[curRow][curCol], field[curRow+1][curCol]))
    return {curRow+1, curCol};
  // check left
  if(curCol != 0 && prevCol != curCol-1 && matchLeft(field[curRow][curCol], field[curRow][curCol-1]))
    return {curRow, curCol-1};
  // check right
  if(curCol != field.at(0).size()-1 && prevCol != curCol+1 && matchRight(field[curRow][curCol], field[curRow][curCol+1]))
    return {curRow, curCol+1};
  return {-1, -1};
}

int passTheRoute(std::vector<std::string>& field, const std::pair<Row, Col>& startPos, std::pair<Row, Col> curPos, std::pair<Row, Col> prevPos)
{
  static int steps = 0;
  if(steps != 0 && curPos.first == startPos.first && curPos.second == startPos.second) return steps;
  steps++;
  auto nextPos = getNextPos(field, curPos, prevPos);
  cout << "Next pos: " << nextPos.first << " " << nextPos.second << endl;
  return passTheRoute(field, startPos, nextPos, curPos);
}

int main()
{
  std::ifstream file;
  file.open("pipes.txt");
  std::string str;
  std::vector<std::string> field;

  while(std::getline(file, str))
  {
    field.push_back(str);
  }
  cout << "Fetched field" << endl;
  auto startPos = getStart(field);
  cout << "Got start pos: " << startPos.first << " " << startPos.second << endl;
  auto steps = passTheRoute(field, startPos, startPos, /*prevPos*/{-1, -1});
  cout << steps/2 << endl;
  return 0;
}
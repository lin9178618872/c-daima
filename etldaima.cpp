#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>

using namespace std;

// 学生结构体类型
struct student {
  int id;
  string name;
  string gender;
  int age;
  string cls;
  int score;
};

int main() {
  // 从CSV文件中读取学生数据
  ifstream fin("students.csv");

  vector<student> students;
  string line;
  while (getline(fin, line)) {
    stringstream ss(line);

    // 逐个字段读取数据
    string id, name, gender, age, cls, score;
    getline(ss, id, ',');
    getline(ss, name, ',');
    getline(ss, gender, ',');
    getline(ss, age, ',');
    getline(ss, cls, ',');
    getline(ss, score, ',');

    // 将学生数据转换为结构体类型
    student s = { stoi(id), name, gender, stoi(age), cls, stoi(score) };

    // 存储学生数据到容器中
    students.push_back(s);
  }

  fin.close();

  // 连接MySQL数据库
  sql::Driver *driver = get_driver_instance();
  sql::Connection *conn = driver->connect("tcp://localhost:3306", "username", "password");
  conn->setSchema("database_name");

  // 创建students表
  sql::Statement *stmt = conn->createStatement();
  stmt->execute("CREATE TABLE IF NOT EXISTS students (id INT PRIMARY KEY, name VARCHAR(50), gender VARCHAR(10), age INT, class VARCHAR(50), score INT)");
  delete stmt;

  // 插入学生数据到数据库中
  sql::PreparedStatement *prep_stmt = conn->prepareStatement("INSERT INTO students (id, name, gender, age, class, score) VALUES (?, ?, ?, ?, ?, ?)");

  for (auto s : students) {
    prep_stmt->setInt(1, s.id);
    prep_stmt->setString(2, s.name);
    prep_stmt->setString(3, s.gender);
    prep_stmt->setInt(4, s.age);
    prep_stmt->setString(5, s.cls);
    prep_stmt->setInt(6, s.score);
    prep_stmt->executeUpdate();
  }

  delete prep_stmt;
  delete conn;

  return 0;
}

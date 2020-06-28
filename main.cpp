//
//  main.cpp
//  fin
//
//  Created by Akerke Tolegen  on 6/27/20.
//  Copyright © 2020 Akerke Tolegen . All rights reserved.
//

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;
class Date {
    int Day;
    int Month;
    int Year;
public:
    Date(int y, int m, int d){
        if (m < 1 || m > 12){
            throw std::out_of_range("Month value is invalid: " + to_string(m));
        }else if (d < 1 || d > 31){
            throw std::out_of_range("Day value is invalid: " + to_string(d));
        }
        Year = y; Month = m; Day = d;
    }
    int GetYear() const{
        return Year;
    };
    int GetMonth() const{
        return Month;
    };
    int GetDay() const{
        return Day;
    };
};

std::ostream& operator << (std::ostream& stream, const Date& date)
{
    stream << setfill('0') << setw(4) << to_string(date.GetYear());
    stream << "-";
    stream << setfill('0') << setw(2) << to_string(date.GetMonth());
    stream << "-";
    stream << setfill('0') << setw(2) << to_string(date.GetDay());
    return stream;
}

bool operator < (const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() != rhs.GetYear()){
        return lhs.GetYear() < rhs.GetYear();
    }
    else if (lhs.GetMonth() != rhs.GetMonth()){
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else {
        return lhs.GetDay() < rhs.GetDay();
    }
}
Date ParseDate(const string& d){
    istringstream date_str(d);
    bool f = 1;
    
    int year = 0;
    f = f && (date_str >> year);
    f = f && (date_str.peek() == '-');
    date_str.ignore(1);
    
    int month = 0;
    f = f && (date_str >> month);
    f = f && (date_str.peek() == '-');
    date_str.ignore(1);
    
    int day = 0;
    f = f && (date_str >> day);
    f = f && date_str.eof();
    
    if (!f){
        throw logic_error("Wrong date format: " + d);
    }
    return Date(year, month, day);
}
class Database {
    map<Date, set<string>> db;
public:
    void AddEvent(const Date& date, const string& event){
        db[date].insert(event);
        
    };
  bool DeleteEvent(const Date& date, const string& event);
  int  DeleteDate(const Date& date);

  void Find(const Date& date) const;
  void FindForPrint(const Date& date) const;
  
  void Print() const;
};

bool Database::DeleteEvent(const Date &dat, const string &act){
    if (db.count(dat) > 0 && db[dat].count(act) > 0)
    {
        db[dat].erase(act);
        return true;
    }
    return false;
}

int Database::DeleteDate(const Date& dat){
    int ans = 0;
    if(db.count(dat) > 0) {
        ans = db.at(dat).size();
        db.erase(dat);
    }
    return ans;
}

void Database::Find(const Date &date) const{
    set<string> res;
    
    if( db.count(date) > 0) res = db.at(date);
    for (auto& i : res){
        cout << i << endl;
    }
    return;
}
void Database::FindForPrint(const Date &date) const {
    set<string> res;
    
    if( db.count(date) > 0) res = db.at(date);
    for (auto& i : res){
        cout << date << " " << i << endl;
    }
    return;
}
void Database::Print() const {
    for (const auto& i : db)
    {
        FindForPrint(i.first);
    }
}

int main(int argc, const char * argv[]) {
    try{

        Database db;
          
        string command;
        while (getline(cin, command)) {
            if (command == "") continue;
            stringstream input(command);
            string op;
            input >> op;
            if (op == "Add") {
                string d, ev;
                input >> d;
                const Date date = ParseDate(d);
                input >> ev;
                if (ev != "" || ev != " ") db.AddEvent(date, ev);
            }
            else if (op == "Del"){
                string d;
                input >> d;

                const Date date = ParseDate(d);
                if(input.eof()) {
                    int ans = db.DeleteDate(date);
                    cout << "Deleted " << ans << " events" << endl;
                }
                else {
                    string ev;
                    input >> ev;
                    bool ans = db.DeleteEvent(date, ev);
                    if (ans == true){
                        cout << "Deleted successfully" << endl;
                    }else cout << "Event not found" << endl;
                }
                    
            }else if (op == "Print"){
                db.Print();
            }else if (op == "Find"){
                string d;
                input >> d;
                Date date = ParseDate(d);
                db.Find(date);
            }else {
                cout << "Unknown command: " << command << endl;
            }
        }
    } catch (exception& e){
        cout << e.what() << endl;
    }
    return 0;
}

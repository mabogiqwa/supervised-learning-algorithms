#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

//Represents a single training instance pair (input-output)
struct Example {
    std::map<std::string, std::string> attributes; //attribute name -> value
    std::string label;
};

//Represents a decision tree node
struct Node {
    bool isLeaf;
    std::string label;
    std::string attribute;
    std::map<std::string, Node*> children;

    Node () : isLeaf(false) {}
};

std::string plurality_value(const std::vector<Example>& examples);
//Precondition: The vector examples must not be empty where each Example
//object must have a valid string label
//Postcondition: Returns a string that occurs most frequently in examples.
//if there is a tie between 1> labels, then the first one encountered will
//be returned.

int main()
{
    std::set<std::string> attributes = {"Alt","Bar","Fri/Sat","Hungry","Patrons","Price","Rain","Reservation","Type","WaitEstimate"};

    std::vector<Example> examples = {
        {{{{"Alt","Yes"}, {"Bar","No"}, {"Fri/Sat","No"}, {"Hungry","Yes"}, {"Patrons","Some"}, {"Price","$$$"}, {"Rain","No"}, {"Reservation","Yes"}, {"Type","French"}, {"WaitEstimate","0-10"}}}, "Yes"},
        {{{{"Alt","Yes"}, {"Bar","No"}, {"Fri/Sat","No"}, {"Hungry","Yes"}, {"Patrons","Full"}, {"Price","$"}, {"Rain","No"}, {"Reservation","No"}, {"Type","Thai"}, {"WaitEstimate","30-60"}}}, "No"},
        {{{{"Alt","No"}, {"Bar","Yes"}, {"Fri/Sat","No"}, {"Hungry","No"}, {"Patrons","Some"}, {"Price","$"}, {"Rain","No"}, {"Reservation","No"}, {"Type","Burger"}, {"WaitEstimate","0-10"}}}, "Yes"},
        {{{{"Alt","Yes"}, {"Bar","No"}, {"Fri/Sat","Yes"}, {"Hungry","Yes"}, {"Patrons","Full"}, {"Price","$"}, {"Rain","Yes"}, {"Reservation","No"}, {"Type","Thai"}, {"WaitEstimate","10-30"}}}, "Yes"},
        {{{{"Alt","Yes"}, {"Bar","No"}, {"Fri/Sat","Yes"}, {"Hungry","No"}, {"Patrons","Full"}, {"Price","$$$"}, {"Rain","No"}, {"Reservation","Yes"}, {"Type","French"}, {"WaitEstimate",">60"}}}, "No"},
        {{{{"Alt","No"}, {"Bar","Yes"}, {"Fri/Sat","No"}, {"Hungry","Yes"}, {"Patrons","Some"}, {"Price","$$"}, {"Rain","Yes"}, {"Reservation","Yes"}, {"Type","Italian"}, {"WaitEstimate","0-10"}}}, "Yes"},
        {{{{"Alt","No"}, {"Bar","Yes"}, {"Fri/Sat","No"}, {"Hungry","No"}, {"Patrons","None"}, {"Price","$"}, {"Rain","Yes"}, {"Reservation","No"}, {"Type","Burger"}, {"WaitEstimate","0-10"}}}, "No"},
        {{{{"Alt","No"}, {"Bar","No"}, {"Fri/Sat","No"}, {"Hungry","Yes"}, {"Patrons","Some"}, {"Price","$"}, {"Rain","Yes"}, {"Reservation","Yes"}, {"Type","Thai"}, {"WaitEstimate","0-10"}}}, "Yes"},
        {{{{"Alt","No"}, {"Bar","Yes"}, {"Fri/Sat","Yes"}, {"Hungry","No"}, {"Patrons","Full"}, {"Price","$"}, {"Rain","Yes"}, {"Reservation","No"}, {"Type","Burger"}, {"WaitEstimate",">60"}}}, "No"},
        {{{{"Alt","Yes"}, {"Bar","Yes"}, {"Fri/Sat","Yes"}, {"Hungry","Yes"}, {"Patrons","Full"}, {"Price","$$$"}, {"Rain","No"}, {"Reservation","Yes"}, {"Type","Italian"}, {"WaitEstimate","10-30"}}}, "Yes"},
        {{{{"Alt","No"}, {"Bar","No"}, {"Fri/Sat","No"}, {"Hungry","No"}, {"Patrons","None"}, {"Price","$"}, {"Rain","No"}, {"Reservation","No"}, {"Type","Thai"}, {"WaitEstimate","0-10"}}}, "No"},
        {{{{"Alt","Yes"}, {"Bar","Yes"}, {"Fri/Sat","Yes"}, {"Hungry","Yes"}, {"Patrons","Full"}, {"Price","$"}, {"Rain","No"}, {"Reservation","No"}, {"Type","Burger"}, {"WaitEstimate","30-60"}}}, "Yes"}
    };


    return 0;
}

std::string plurality_value(const std::vector<Example>& examples)
{
    std::map<std::string, int> count;
    for (const auto& e : examples) {
        count[e.label]++;
    }

    auto best = count.begin();
    for (auto it = count.begin(); it != count.end(); ++it) {
        if (it->second > best->second) {
            best = it;
        }
    }

    return best->first;
}

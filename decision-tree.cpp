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

bool same_classification(const std::vector<Example>& examples);
//Precondition: Each example object in the vector must have a valid label
//Postcondition: The function return true if examples is empty or all label values in examples are identical

std::string importance_function(const std::set<std::string>& attributes, const std::vector<Example>& examples) {
    return *attributes.begin();
}

void print_tree(Node* tree, std::string indent="") {
    if (tree->isLeaf) {
        std::cout << indent << "Leaf: " << tree->label << std::endl;
    } else {
        std::cout << indent << "Attribute: " << tree->attribute << std::endl;
        for (const auto& [val, child] : tree->children) {
            std::cout << indent << "  If " << tree->attribute << " = " << val << ": " << std::endl;
            print_tree(child, indent + "    ");
        }
    }
}

Node* learn_decision_tree(std::vector<Example> examples, std::set<std::string> attributes, std::vector<Example> parentExamples) {
    Node* tree = new Node();

    if (examples.empty()) {
        tree->isLeaf = true;
        tree->label = plurality_value(parentExamples);
        return tree;
    }
    else if (same_classification(examples)) {
        tree->isLeaf = true;
        tree->label = examples[0].label;
        return tree;
    }
    else if (attributes.empty()) {
        tree->isLeaf = true;
        tree->label = plurality_value(examples);
        return tree;
    }
    else {
        std::string A = importance_function(attributes, examples);
        tree->attribute = A;

        //Collecting all possible values of A
        std::set<std::string> values;
        for (const auto& e : examples) {
            values.insert(e.attributes.at(A));
        }

        for (const std::string& v : values) {
            std::vector<Example> exmples;
            for (const auto& e : examples) {
                if (e.attributes.at(A) == v)
                    exmples.push_back(e);
            }
            std::set<std::string> newAttributes = attributes;
            newAttributes.erase(A);

            Node* subtree = learn_decision_tree(exmples, newAttributes, examples);
            tree->children[v] = subtree;
        }
        return tree;
    }
}

void deallocate_memory(Node* tree) {
    if (tree == nullptr)
        return;

    for (auto& [val, child] : tree->children) {
        deallocate_memory(child);
    }

    delete tree;
}
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

    //std::cout << plurality_value(examples);

    //std::cout << same_classification(examples);

    Node* tree = learn_decision_tree(examples, attributes, {});
    print_tree(tree);
    deallocate_memory(tree);
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

bool same_classification(const std::vector<Example>& examples) {
    if (examples.empty())
        return true;

    std::string firstLabel = examples[0].label;

    for (const auto& e : examples) {
        if (e.label != firstLabel)
            return false;
    }
    return true;
}

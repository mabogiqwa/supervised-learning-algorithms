#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

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

double entropy(const std::vector<Example>& examples);
//Precondition: Each example object must have a label and the label should not
//be empty
//Postcondition: Returns a double value representing Shannon entropy

std::map<std::string, std::vector<Example>> splitByAttribute(const std::vector<Example>& examples, const std::string& attr) {
    std::map<std::string, std::vector<Example>> subsets;
    for (const auto& e : examples) {
        auto it = e.attributes.find(attr);
        if (it != e.attributes.end())
            subsets[it->second].push_back(e);
    }
    return subsets;
}

std::string choose_best_attribute(std::vector<Example>& examples, std::vector<std::string>& attributes) {
    double baseEntropy = entropy(examples);
    double bestGain = -1.0;
    std::string bestAttr;

    for (const auto& attr : attributes) {
        auto subsets = splitByAttribute(examples, attr);
        double newEntropy = 0.0;
        for (const auto& pair : subsets) {
            double p = (double)pair.second.size() / examples.size();
            newEntropy += p * entropy(pair.second);
        }
        double infoGain = baseEntropy - newEntropy;
        if (infoGain > bestGain) {
            bestGain = infoGain;
            bestAttr = attr;
        }
    }
    return bestAttr;
}


std::string importance_function(const std::set<std::string>& attributes, const std::vector<Example>& examples) {
    return *attributes.begin();
}

void print_tree(Node* tree, std::string indent = "") {
    if (tree->isLeaf) {
        std::cout << indent << "Leaf: " << tree->label << std::endl;
    } else {
        std::cout << indent << "Attribute: " << tree->attribute << std::endl;
        for (const auto& [val, child] : tree->children) {
            std::cout << indent << "  If " << tree->attribute << " = " << val << ":" << std::endl;
            print_tree(child, indent + "    ");
        }
    }
}

Node* learn_decision_tree(std::vector<Example> examples, std::vector<std::string> attributes) {
    Node* tree = new Node();

    if (examples.empty()) {
        tree->label = "Unknown";
        tree->isLeaf = true;
        return tree;
    }
    if (same_classification(examples)) {
        tree->label = examples[0].label;
        tree->isLeaf = true;
        return tree;
    }
    if (attributes.empty()) {
        tree->label = plurality_value(examples);
        tree->isLeaf = true;
        return tree;
    }

    std::string bestAttr = choose_best_attribute(examples, attributes);
    tree->attribute = bestAttr;

    auto subsets = splitByAttribute(examples, bestAttr);
    std::vector<std::string> remainingAttrs;
    for (const auto& attr : attributes) if (attr != bestAttr) remainingAttrs.push_back(attr);

    for (const auto& pair : subsets) {
        tree->children[pair.first] = learn_decision_tree(pair.second, remainingAttrs);
    }

    return tree;
}

void deallocate_memory(Node* tree) {
    if (tree == nullptr)
        return;

    for (auto& [val, child] : tree->children) {
        deallocate_memory(child);
    }

    delete tree;
}

std::string predict(Node* tree, const Example& e) {
    if (tree->isLeaf || !tree->label.empty()) return tree->label;

    auto it = e.attributes.find(tree->attribute);
    if (it != e.attributes.end()) {
        auto childIt = tree->children.find(it->second);
        if (childIt != tree->children.end())
            return predict(childIt->second, e);
    }
    return "Unknown";
}

int main()
{
    std::vector<std::string> attributes = {"Alt","Bar","Fri/Sat","Hungry","Patrons","Price","Rain","Reservation","Type","WaitEstimate"};

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

    Node* tree = learn_decision_tree(examples, attributes);

    print_tree(tree);

    Example test = {
        {{"Alt","Yes"}, {"Bar","No"}, {"Fri/Sat","No"}, {"Hungry","Yes"},
         {"Patrons","Some"}, {"Price","$$$"}, {"Rain","No"}, {"Reservation","Yes"},
         {"Type","French"}, {"WaitEstimate","0-10"}}, ""};

    std::cout << std::endl;
    std::cout << "Prediction: " << predict(tree, test) << std::endl;

    deallocate_memory(tree);
    return 0;
}

double entropy(const std::vector<Example>& examples) {
    std::map<std::string, int> labelCounts;
    for (const auto& e : examples) labelCounts[e.label]++;

    double ent = 0.0;
    int total = examples.size();
    for (const auto& pair : labelCounts) {
        double p = (double)pair.second / total;
        ent -= p * log2(p);
    }
    return ent;
}

std::string plurality_value(const std::vector<Example>& examples) {
    std::map<std::string, int> count;
    for (const auto& e : examples) count[e.label]++;

    auto best = count.begin();
    for (auto it = count.begin(); it != count.end(); ++it) {
        if (it->second > best->second) best = it;
    }
    return best->first;
}

bool same_classification(const std::vector<Example>& examples) {
    if (examples.empty()) return true;
    std::string firstLabel = examples[0].label;
    for (const auto& e : examples) if (e.label != firstLabel) return false;
    return true;
}

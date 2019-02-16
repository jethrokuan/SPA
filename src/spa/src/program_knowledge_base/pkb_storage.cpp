#include "program_knowledge_base/pkb_storage.h"
#include <iostream>

namespace PKB {

PKBStorage::PKBStorage(){};
PKBStorage::~PKBStorage(){};

void PKBStorage::storeAST(const std::shared_ptr<ProcedureNode> proc) {
  ast = proc;
};

void PKBStorage::storeLine(const std::shared_ptr<Node> node) {
  lines.push_back(node);
}

// TODO error handling
// TODO refactor to use hashmap instead
// would need to implement hash function of the node
// as well as a hash table
// return line number
// returns empty string if node does not exist
std::string PKBStorage::getLineFromNode(const std::shared_ptr<Node> node) {
  // loop through vector of nodes
  for (std::size_t i = 0; i < lines.size(); i++) {
    if (lines[i] == node) {
      return std::to_string(i + 1);  // +1 due to 0 based index
    }
  }
  return "";  // TODO error handling
}

void PKBStorage::storeFollowsRelation(const LineBefore line_before,
                                      const LineAfter line_after) {
  follows_set.insert(std::pair<LineBefore, LineAfter>(line_before, line_after));
  // line_before_line_after_map.insert(line_before, line_after);
  // line_after_line_before_map.insert(line_after, line_before);
}

// void storeFollowsRelationS(const LineBefore, const LineAfter);
// void storeParentRelation(const ParentLine, const ChildLine);
// void storeParentRelationS(const ParentLine, const ChildLine);
// void storeProcedureUsesVarRelation(const Procedure, const Variable);
// void storeLineUsesVarRelation(const Line, const Variable);
// void storeProcedureModifiesVarRelation(const Procedure, const Variable);
// void storeLineModifiesVarRelation(const Line, const Variable);

void PKBStorage::storeVariable(const Variable var) {
  var_set.insert(var);
  var_list.push_back(var);
}

void PKBStorage::storeAssign(const Line line) {
  assign_set.insert(line);
  assign_list.push_back(line);
}

void PKBStorage::storeStatement(const Line line) {
  statement_set.insert(line);
  statement_list.push_back(line);
}

void PKBStorage::storePrint(const Line line) {
  print_set.insert(line);
  print_list.push_back(line);
}

void PKBStorage::storeRead(const Line line) {
  read_set.insert(line);
  read_list.push_back(line);
}

void PKBStorage::storeWhile(const Line line) {
  while_set.insert(line);
  while_list.push_back(line);
}

void PKBStorage::storeIf(const Line line) {
  if_set.insert(line);
  if_list.push_back(line);
}

void PKBStorage::storeConstant(const Constant num) {
  constant_set.insert(num);
  constant_list.push_back(num);
}

void PKBStorage::storeProcedure(const Procedure proc) {
  procedure_set.insert(proc);
  procedure_list.push_back(proc);
}

void PKBStorage::addToVectorMap(
    std::unordered_map<std::string, std::vector<std::string>> umap,
    std::string index, std::string data) {
  if (umap.find(index) == umap.end()) {
    // create new vector
    std::vector<std::string> v;
    v.push_back(data);
    umap[index] = v;
  } else {
    // retrieve vector and add element
    umap.at(index).push_back(data);
  }
}

}  // namespace PKB
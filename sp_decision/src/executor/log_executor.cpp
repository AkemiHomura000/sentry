#include "executor/log_executor.hpp"

void LogExecutor::info(std::string commit) { DEBUG_LOG(commit); }
void LogExecutor::info(std::string commit, auto text) { DEBUG_LOG(commit << text); }
void LogExecutor::info(std::stringstream &commit) { DEBUG_LOG(commit.str()); }
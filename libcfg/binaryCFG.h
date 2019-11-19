//
// Created by oueru on 2019-11-14.
//
#pragma once
#ifndef SOLIDITY_BINARYCFG_H
#define SOLIDITY_BINARYCFG_H

#endif //SOLIDITY_BINARYCFG_H

#include <libdevcore/Common.h>
#include <libevmasm/AssemblyItem.h>
#include <libsolidity/ast/AST.h>


namespace dev {
    namespace cfg {


        struct BasicBlock {
            unsigned begin = 0;
            unsigned end = 0;

            std::vector<unsigned> next;
        };

        using BasicBlocks = std::vector<BasicBlock>;

        class FunctionControlFlowGraph {
        public:
            void appendBlock(unsigned _block_id, unsigned start_index, unsigned end_index);

            void appendEdge(unsigned src, unsigned tgt);

            unsigned gernarateBlockId() { return m_lastUsedId++; }

        private:
            unsigned m_lastUsedId = 0;
            std::map<unsigned, BasicBlock> m_blocks;
        };

        class Annotation {
        private:
            std::map<unsigned, u256> m_jumptarget;
            std::map<Declaration const*, u256> m_function_entry;
        public:
            void appendJumptarget(unsigned item_index, const u256& tag) { m_jumptarget[item_index] = tag;}
            void appendFunctiontag(Declaration const* _function, const u256& tag) {m_function_entry[_function] = tag;}
            std::string printJumpTgt() const{
                std::string result = "";
                auto iter = m_jumptarget.begin();
                for(;iter != m_jumptarget.end(); iter++){
                    result += std::to_string(iter->first) + " ";
                    result += std::string(iter->second) + "\n";
                }
                return result;
            }
            std::string printFunctionEntry() const{
                std::string result = "";
                auto iter = m_function_entry.begin();
                for(;iter!=m_function_entry.end();iter++){
                    result += std::string(iter->second);
                    result += "\n";
                }
                return result;
            }
        };

        struct OptimzedItem{
            unsigned source_start;
            unsigned source_end;
            std::vector<eth::AssemblyItem> optimzedAssemblyItems;

            OptimzedItem(unsigned int sourceStart, unsigned int sourceEnd,
                         const std::vector<eth::AssemblyItem> &optimzedAssemblyItems) : source_start(sourceStart),
                                                                                        source_end(sourceEnd),
                                                                                        optimzedAssemblyItems(
                                                                                                optimzedAssemblyItems) {}
            OptimzedItem(unsigned int sourceStart, unsigned int sourceEnd) : source_start(sourceStart), source_end(sourceEnd) {}
        };

        class OptimizedAnnotation {
        public:
            OptimizedAnnotation(unsigned int type, const std::string &functionName, const OptimzedItem &optimzedItem)
                    : type(type), function_name(functionName), optimzedItem(optimzedItem) {}

            std::string OptimizedAnnotationStr() const {
                std::string result = "";
                result += std::to_string(type) + " ";
                result += function_name + " ";
                result += std::to_string(optimzedItem.source_start) + " ";
                result += std::to_string(optimzedItem.source_end) + " \n";
                if(optimzedItem.optimzedAssemblyItems.size() > 0){
                    result += "=\n";
                    for(auto iter = optimzedItem.optimzedAssemblyItems.begin();iter != optimzedItem.optimzedAssemblyItems.end();iter++){
                        result += iter->toAssemblyText()+"\n";
                    }
                }
                return result;
            }
        private:
            unsigned type; // optimize
            std::string function_name;
            OptimzedItem optimzedItem;
        };


    }
}

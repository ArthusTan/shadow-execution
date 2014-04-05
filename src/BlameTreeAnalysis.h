/**
 * @file BlameTreeAnalysis.h
 * @brief BlameTreeAnalysis Declarations.
 */

/*
 * Copyright (c) 2013, UC Berkeley All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 * 1.  Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the UC Berkeley nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY UC BERKELEY ''AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL UC BERKELEY BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

// Author: Cuong Nguyen

#ifndef BLAME_TREE_ANALYSIS_H
#define BLAME_TREE_ANALYSIS_H

#include "BlameNode.h"
#include "BlameTreeUtilities.h"

class BlameTreeAnalysis {
  private:
    map<BlameNodeID, BlameNode> nodes; // map from a pair (node id, precision) to node 

    /**
     * Construct blame node given a binary operation expression. This function
     * connects the node of left to the node of right01 or right02 if it can
     * blame right01 or right02 given the precision constraint.
     *
     * @param left the left value of the binary operation
     * @param precision the precision constraint on left
     * @param right01 the first operand of the binary operation
     * @param right02 the second oeprand of the binary opeartion
     *
     * @return the blame node associated with left and precision constraint,
     * that connects to nodes it blames
     */
    BlameNode constructBlameNode(BlameTreeShadowObject<BlameTree::HIGHPRECISION> left,
        BlameTree::PRECISION precision,
        BlameTreeShadowObject<BlameTree::HIGHPRECISION> right01,
        BlameTreeShadowObject<BlameTree::HIGHPRECISION> right02);

  public:

    map<BlameNodeID, BlameNode> getNodes() { return nodes; };

    /**
     * Construct the blame graph given the execution trace and a node to start
     * with. The constructed graph contains all the nodes that the start node
     * can blame given its precision constraint.
     *
     * @param trace the program execution trace
     * @param bnID a node to start with
     * @return the blame graph
     */
    BlameNode constructBlameGraph(map<int, vector<BlameTreeShadowObject<BlameTree::HIGHPRECISION> > > trace, BlameNodeID bnID); 
};

#endif

// Author: Cuong Nguyen

#include "BlameTree.h"

BlameNode
BlameTree::constructFuncBlameNode(BlameTreeShadowObject<HIGHPRECISION> left,
								  PRECISION precision,
								  BlameTreeShadowObject<HIGHPRECISION> right) {
	int dpc;
	std::map<BlameNodeID, BlameNode>::iterator it;

	dpc = left.getDPC();
	BlameNodeID bnID(dpc, precision);
	it = nodes.find(bnID);

	//
	// nodes remember all nodes that are processed before
	// process again only if this is not processed before
	//
	if (it == nodes.end()) {
		//
		// variables declaration
		//
		int pc, fid, dpc01;
		HIGHPRECISION value;
		string func;
		PRECISION i;
		vector<vector<BlameNodeID>> blameNodeIds;
		vector<bool> edgeAttributes;

		//
		// variables definition
		//
		pc = left.getPC();
		fid = left.getFileID();
		value = left.getValue(precision);
		func = left.getFunc();

		//
		// construct a node associate with the function result
		//
		BlameNode node(dpc, pc, fid, false, precision, blameNodeIds,
					   edgeAttributes);

		dpc01 = right.getDPC();

		//
		// try different combination of precision of argument
		//
		for (i = BITS_FLOAT; i < PRECISION_NO; i = PRECISION(i + 1)) {
			HIGHPRECISION value01 = right.getValue(i);
			HIGHPRECISION lowvalue01 = right.getValue(BITS_FLOAT);

			safe_assert(value ==
						BlameTreeUtilities::clearBits(
							value, 52 - BlameTreeUtilities::exactBits(precision)));
			if (BlameTreeUtilities::equalWithPrecision(
						value, BlameTreeUtilities::evalFunc(value01, func), precision)) {
				/*
				if (BlameTreeUtilities::clearBits(value, 52 -
				    BlameTreeUtilities::exactBits(precision)) ==
				  BlameTreeUtilities::clearBits(BlameTreeUtilities::evalFunc(value01,
				      func), 52 - BlameTreeUtilities::exactBits(precision))) {
				      */
				//
				// Construct edges for each blame
				//
				BlameNodeID bnID01(dpc01, i);
				vector<BlameNodeID> blamedNodes;

				//
				// Blame only if the operand cannot be in the lowest precision (right
				// now BITS_23)
				//
				if (i != BITS_FLOAT &&
						BlameTreeUtilities::clearBits(
							lowvalue01, 52 - BlameTreeUtilities::exactBits(i)) != value01) {
					blamedNodes.push_back(bnID01);
				}

				node.addBlamedNodes(blamedNodes);

				node.addEdgeAttribute(false);

				//
				// Do not try larger i because it subsumes what have been tried
				//
				break;
			}
		}

		//
		// Determined whether node is highlighted
		//
		if (value != (LOWPRECISION) value) {
			node.setHighlight(true);
		}

		nodes[bnID] = node;

		return node;

	}

	return it->second;
}

BlameNode
BlameTree::constructBlameNode(BlameTreeShadowObject<HIGHPRECISION> left,
							  PRECISION precision,
							  BlameTreeShadowObject<HIGHPRECISION> right01,
							  BlameTreeShadowObject<HIGHPRECISION> right02) {

	int dpc;
	std::map<BlameNodeID, BlameNode>::iterator it;

	dpc = left.getDPC();
	BlameNodeID bnID(dpc, precision);
	it = nodes.find(bnID);

	//
	// nodes remember all nodes that are processed before
	// process again only if this is not processed before
	//
	if (it == nodes.end()) {
		//
		// variables declaration
		//
		int pc, fid, dpc01, dpc02;
		HIGHPRECISION value;
		BINOP bop;
		PRECISION i, j, max_j;
		vector<vector<BlameNodeID>> blameNodeIds;
		vector<bool> edgeAttributes;

		//
		// variables definition
		//
		pc = left.getPC();
		fid = left.getFileID();
		value = left.getValue(precision);
		bop = left.getBinOp();

		//
		// construct a node associate with the binary operation result
		//
		BlameNode node(dpc, pc, fid, false, precision, blameNodeIds,
					   edgeAttributes);

		dpc01 = right01.getDPC();
		dpc02 = right02.getDPC();

		//
		// try different combination of precision of the two operands
		//
		max_j = PRECISION_NO;
		for (i = BITS_FLOAT; i < PRECISION_NO; i = PRECISION(i + 1)) {
			HIGHPRECISION value01 = right01.getValue(i);
			HIGHPRECISION lowvalue01 = right01.getValue(BITS_FLOAT);

			for (j = BITS_FLOAT; j < max_j; j = PRECISION(j + 1)) {
				HIGHPRECISION value02 = right02.getValue(j);
				HIGHPRECISION lowvalue02 = right02.getValue(BITS_FLOAT);

				safe_assert(value ==
							BlameTreeUtilities::clearBits(
								value, 52 - BlameTreeUtilities::exactBits(precision)));
				if (BlameTreeUtilities::equalWithPrecision(
							value, BlameTreeUtilities::eval(value01, value02, bop),
							precision)) {
					/*
					if (value ==
					BlameTreeUtilities::clearBits(BlameTreeUtilities::eval(value01,
					    value02, bop), 52 - BlameTreeUtilities::exactBits(precision)))
					{*/
					//
					// Construct edges for each blame
					//
					BlameNodeID bnID01(dpc01, i);
					BlameNodeID bnID02(dpc02, j);
					vector<BlameNodeID> blamedNodes;

					//
					// Blame only if the operand cannot be in the lowest precision (right
					// now BITS_23)
					//
					if (i != BITS_FLOAT &&
							BlameTreeUtilities::clearBits(
								lowvalue01, 52 - BlameTreeUtilities::exactBits(i)) != value01) {
						blamedNodes.push_back(bnID01);
					}

					if (j != BITS_FLOAT &&
							BlameTreeUtilities::clearBits(
								lowvalue02, 52 - BlameTreeUtilities::exactBits(j)) != value02) {
						blamedNodes.push_back(bnID02);
					}

					node.addBlamedNodes(blamedNodes);

					//
					// Determine the edge attribute
					//
					node.addEdgeAttribute(!BlameTreeUtilities::equalWithPrecision(
											  value, BlameTreeUtilities::feval(value01, value02, bop),
											  precision));
					/*
					node.addEdgeAttribute(BlameTreeUtilities::clearBits(value, 52 -
					      BlameTreeUtilities::exactBits(precision)) !=
					    BlameTreeUtilities::clearBits(BlameTreeUtilities::feval(value01,
					        value02, bop), 52 -
					      BlameTreeUtilities::exactBits(precision)));
					      */

					//
					// Do not try larger j because it subsumes what have been tried
					//
					break;
				}
			}
			//
			// Do not try larger j because it subsumes what have been tried
			//
			max_j = j;
		}

		//
		// Determined whether node is highlighted
		//
		if (value != (LOWPRECISION) value) {
			node.setHighlight(true);
		}

		nodes[bnID] = node;

		return node;
	}

	return it->second;

}

BlameNode BlameTree::constructBlameGraph(
	vector<vector<BlameTreeShadowObject<HIGHPRECISION>>> trace) {

	queue<BlameNodeID> workList;

	workList.push(rootNode);

	while (!workList.empty()) {

		//
		// Variable declarations.
		//
		int dpc;
		PRECISION precision;
		vector<BlameTreeShadowObject<HIGHPRECISION>> startNode;
		vector<vector<BlameNodeID>> blameEdges;
		set<BlameNodeID> nodeIds;
		BlameNode blameGraph;

		//
		// Variable definitions.
		//
		BlameNodeID bnID = workList.front();
		workList.pop();
		if (nodes.find(bnID) != nodes.end()) {
			continue;
		}

		dpc = bnID.getDPC();
		precision = bnID.getPrecision();
		startNode = trace[dpc];

		//
		// We are assuming that each element of the trace has three elements.
		// Construct blameGraph given the start node. Recursively construct
		// blameGraph for all operands that are blamed by the start node.
		//
		switch (startNode[0].getIntrType()) {
			case BIN_INTR:
				safe_assert(startNode.size() == 3);
				blameGraph = constructBlameNode(startNode[0], precision, startNode[1],
												startNode[2]);
				break;
			case CALL_INTR:
				safe_assert(startNode.size() == 2);
				blameGraph =
					constructFuncBlameNode(startNode[0], precision, startNode[1]);
				break;
			default:
				DEBUG_STDERR("Unsupport kind of instruction.");
				safe_assert(false);
		}

		blameEdges = blameGraph.getEdges();

		for (vector<vector<BlameNodeID>>::iterator edgesIt = blameEdges.begin();
				edgesIt != blameEdges.end(); ++edgesIt) {
			vector<BlameNodeID> blameNodes;

			blameNodes = *edgesIt;

			for (vector<BlameNodeID>::iterator nodesIt = blameNodes.begin();
					nodesIt != blameNodes.end(); ++nodesIt) {
				BlameNodeID blameNode = *nodesIt;

				//
				// Construct graph for this node if it is never constructed before
				//
				nodeIds.insert(blameNode);
			}
		}

		for (set<BlameNodeID>::iterator nodesIt = nodeIds.begin();
				nodesIt != nodeIds.end(); ++nodesIt) {
			BlameNodeID blameNode = *nodesIt;
			if (nodes.find(blameNode) == nodes.end()) {
				workList.push(blameNode);
			}
		}
	}

	return nodes[rootNode];
}

std::string BlameTree::edgeToDot(BlameNode graph) {
	std::ostringstream dot;
	std::set<BlameNodeID> bnIDs;
	vector<vector<BlameNodeID>> edges;

	edges = graph.getEdges();

	dot << "\t" << graph.edgeToDot(nodes) << endl;

	return dot.str();
}

std::string BlameTree::toDot() {
	std::ostringstream dot;
	std::set<BlameNodeID> bnIDs;
	vector<vector<BlameNodeID>> edges;
	map<BlameNodeID, BlameNode>::reverse_iterator it;

	//int nodeCnt = 0;

	dot << "digraph G { " << endl;

	for (it = nodes.rbegin(); it != nodes.rend(); ++it) {
		BlameNode bn = it->second;

		if (bn.isHighlight()) {
			dot << "\t" << bn.toDot() << "[color=red]" << endl;
		}

		dot << edgeToDot(bn);

		/*
		cout << nodeCnt << endl;

		nodeCnt++;
		if (nodeCnt > 500) break;
		*/
	}

	dot << "}" << endl;

	return dot.str();
}

void BlameTree::printResult() {
	queue<BlameNodeID> workList;
	set<BlameNodeID> cacheNodes; // set of already considered blame nodes
	map<pair<int, int>, vector<bool>>
								   result;                  // map from pc to a pair of boolean, the first
	// boolean indicates whether the result
	// requires higherprecision, the second
	// boolean indicates whether the operator
	// requires higher precision
	workList.push(rootNode);
	cacheNodes.insert(rootNode);

	while (!workList.empty()) {
		int pc;
		int file;

		vector<vector<BlameNodeID>> edges;
		vector<bool> edgeAttributes;
		bool highlight;
		bool edgeHighlight;

		BlameNodeID bnID = workList.front();
		BlameNode bn = nodes[bnID];
		workList.pop();
		pc = bn.getPC();
		file = bn.getFileID();

		highlight = bn.isHighlight();
		edges = bn.getEdges();
		edgeAttributes = bn.getEdgeAttributes();

		if (!edges.empty()) {
			vector<BlameNodeID> bnIDs;

			edgeHighlight = edgeAttributes[0];
			//
			// save the result for the current node
			//
			if (result.find(make_pair(file, pc)) == result.end()) {
				vector<bool> nodeResult;

				nodeResult.push_back(highlight);
				nodeResult.push_back(edgeHighlight);
				result[make_pair(file, pc)] = nodeResult;
			} else {
				vector<bool> nodeResult;

				nodeResult = result[make_pair(file, pc)];
				nodeResult[0] = nodeResult[0] || highlight;
				nodeResult[1] = nodeResult[1] || edgeHighlight;
				result[make_pair(file, pc)] = nodeResult;
			}

			bnIDs = edges[0];
			for (vector<BlameNodeID>::iterator it = bnIDs.begin(); it != bnIDs.end();
					++it) {
				BlameNodeID bnID = *it;
				if (cacheNodes.find(bnID) == cacheNodes.end()) {
					cacheNodes.insert(bnID);
					workList.push(bnID);
				}
			}
		}
	}

	//
	// print result
	//
	for (map<pair<int, int>, vector<bool>>::iterator it = result.begin();
			it != result.end(); it++) {
		int pc, file;
		bool highlight, edgeHighlight;

		file = it->first.first;
		pc = it->first.second;
		highlight = it->second[0];
		edgeHighlight = it->second[1];

		if (highlight || edgeHighlight) {
			cout << "\t File: " << file << ", Line " << pc << ":" << endl;
			if (highlight) {
				cout << "\t\t Result: double precision" << endl;
			}
			if (edgeHighlight) {
				cout << "\t\t Operator: double precision" << endl;
			}
		}
	}
}
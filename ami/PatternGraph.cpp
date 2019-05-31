#include "PatternGraph.h"
#include "Pattern.h"

#include <sstream>
#include <unordered_map>
#include <functional>

#include <QMetaEnum>
#include <QDebug>

namespace ami
{
	PatternGraph::PatternGraph()
	{
		addNode({ Operation::Type::NONE });
	}

	PatternGraph::PatternGraph(const Pattern & pattern)
		:
		PatternGraph()
	{
		append(pattern);
	}

	void PatternGraph::append(const Pattern & pattern)
	{
		unsigned int roundIndex = 1;
		for (auto & round : pattern.getRounds())
		{
			// check if round is possible
			int availableStitches = getAvailableStitches();
			int requiredStitches = 0;
			for (auto & op : round)
			{
				std::unique_ptr<GraphOperation> operation = GraphOperation::getGraphOperation(op);
				if (operation)
				{
					requiredStitches += operation->consumedStitches();
				}
			}
			if (requiredStitches > availableStitches)
			{
				throw RoundStitchesCountException(roundIndex, availableStitches, requiredStitches);
			}

			unsigned int operationIndex = 1;
			for (auto & op : round)
			{
				std::unique_ptr<GraphOperation> operation = GraphOperation::getGraphOperation(op);
				if (operation)
				{
					if (!operation->apply(*this))
					{
						// fails to apply operation, invalid pattern
						throw PatternInvalidException(roundIndex, operationIndex);
					}
				}

				operationIndex++;
			}

			roundIndex++;
		}
	}

	void PatternGraph::addOutline(const NodeIterator & it)
	{
		m_outline.push_front(it.id);
	}

	bool PatternGraph::popOutline(unsigned int n)
	{
		if (getAvailableStitches() < n)
		{
			return false;
		}

		for (unsigned int i = 0; i < n; i++)
		{
			m_outline.pop_back();
		}
		return true;
	}

	PatternGraph::NodeIterator PatternGraph::addNode(const NodeData & data)
	{
		Node node;
		node.id = m_nodes.size();
		node.data = data;
		node.next = 0; // there is no next yet!
		NodeIterator it(m_nodes, node.id);
		m_nodes.push_back(node);

		if (!m_outline.empty())
		{
			NodeIterator last = front();
			it.node().last = last.id;
			last.node().next = node.id; // asign ourselves as our last's next
		}

		addOutline(it); // add to outline
		return it;
	}

	void PatternGraph::addEdge(int from, int to, float distance)
	{
		Edge edge;
		edge.from = from;
		edge.to = to;
		edge.distance = distance;
		m_edges.push_back(edge);
	}

	void PatternGraph::addJoint(int from, int to)
	{
		Joint joint;
		joint.from = from;
		joint.to = to;
		m_joints.push_back(joint);
	}

	void PatternGraph::addFace(int a, int b, int c)
	{
		if (a == b || a == c || b == c) return; // check triangles are valid
		Face face;
		face.ids[0] = a;
		face.ids[1] = b;
		face.ids[2] = c;
		m_faces.push_back(face);
	}

	PatternGraph PatternGraph::join(const PatternGraph & graph)
	{
		PatternGraph pattern(*this);

		// save number of nodes, edges and faces
		unsigned int nnodes = pattern.getNodes().size();
		unsigned int nedges = pattern.getEdges().size();
		unsigned int nfaces = pattern.getFaces().size();
		unsigned int njoints = pattern.getJoints().size();
		unsigned int noutline = graph.getOutline().size();

		// append new nodes, edges and faces
		pattern.m_nodes.insert(pattern.m_nodes.end(), graph.getNodes().begin(), graph.getNodes().end());
		pattern.m_edges.insert(pattern.m_edges.end(), graph.getEdges().begin(), graph.getEdges().end());
		pattern.m_faces.insert(pattern.m_faces.end(), graph.getFaces().begin(), graph.getFaces().end());
		pattern.m_joints.insert(pattern.m_joints.end(), graph.getJoints().begin(), graph.getJoints().end());
		pattern.m_outline.insert(pattern.m_outline.begin(), graph.m_outline.begin(), graph.m_outline.end());

		// transform nodes, edges and faces so they have unique ids;
		std::transform(pattern.m_nodes.begin() + nnodes, pattern.m_nodes.end(), pattern.m_nodes.begin() + nnodes,
			[&nnodes](Node & node)
		{
			node.id += nnodes;
			node.last += nnodes;
			node.next += nnodes;
			//node.under += nnodes;
			return node;
		});

		std::transform(pattern.m_edges.begin() + nedges, pattern.m_edges.end(), pattern.m_edges.begin() + nedges,
			[&nnodes](Edge & edge)
		{
			edge.from += nnodes;
			edge.to += nnodes;
			return edge;
		});

		std::transform(pattern.m_faces.begin() + nfaces, pattern.m_faces.end(), pattern.m_faces.begin() + nfaces,
			[&nnodes](Face & face)
		{
			for (auto & id : face.ids)
			{
				id += nnodes;
			}
			return face;
		});

		std::transform(pattern.m_joints.begin() + njoints, pattern.m_joints.end(), pattern.m_joints.begin() + njoints,
			[&nnodes](Joint & joint)
		{
			joint.from += nnodes;
			joint.to += nnodes;
			return joint;
		});

		std::transform(pattern.m_outline.begin(), pattern.m_outline.begin() + noutline, pattern.m_outline.begin(),
			[&nnodes, &pattern](int & it)
		{
			it += nnodes;
			return it;
		});

		return pattern;
	}

	bool LoopOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::LP });
		PatternGraph::Node & node = node_it.node();

		// loop only adds the base stitch
		//node.under = node.id;
		node.last = node.id;
		node.next = node.id;

		return true;
	}

	bool ChainOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::CH });
		PatternGraph::Node & node = node_it.node();

		pattern.addEdge(node.id, node.last, 1.f); // connect next node with our node

		return true;
	}

	bool SingleCrochetOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::SC });
		PatternGraph::Node & node = node_it.node();

		PatternGraph::NodeIterator under = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			qDebug() << "Not enough points to apply Single Crochet";
			return false;
		}
		PatternGraph::NodeIterator under2 = pattern.back();

		pattern.addEdge(node.id, node.last, 1.f);
		pattern.addEdge(node.id, under.id, 1.f);
		pattern.addEdge(node.id, under2.id, 1.f);

		pattern.addFace(node.id, node.last, under.id);
		pattern.addFace(node.id, under.id, under2.id);

		return true;
	}

	bool IncreaseOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::INC });
		PatternGraph::Node & node = node_it.node();

		PatternGraph::NodeIterator under = pattern.back();

		pattern.addEdge(node.id, node.last, 1.f);
		pattern.addEdge(node.id, under.id, 1.f);

		pattern.addFace(node.id, node.last, under.id);

		return true;
	}

	bool DecreaseOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::DEC });
		PatternGraph::Node & node = node_it.node();

		PatternGraph::NodeIterator under = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			qDebug() << "Not enough points to apply Decrease";
			return false;
		}
		PatternGraph::NodeIterator under2 = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			qDebug() << "Not enough points to apply Decrease";
			return false;
		}
		PatternGraph::NodeIterator under3 = pattern.back();

		pattern.addEdge(node.id, node.last, 1.f);
		pattern.addEdge(node.id, under.id, 1.f);
		pattern.addEdge(node.id, under2.id, 1.f);
		pattern.addEdge(node.id, under3.id, 1.f);

		pattern.addFace(node.id, node.last, under.id);
		pattern.addFace(node.id, under.id, under2.id);
		pattern.addFace(node.id, under2.id, under3.id);

		return true;
	}

	bool MagicRingOperation::apply(PatternGraph & pattern) const
	{
		IncreaseOperation incOp;
		return incOp.apply(pattern);
	}

	bool SlipStitchOperation::apply(PatternGraph & pattern) const
	{
		// this operation does not add a new vertex
		PatternGraph::NodeIterator under = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			qDebug() << "Not enough points to apply Slip Stitch";
			return false;
		}
		PatternGraph::NodeIterator under2 = pattern.back();
		PatternGraph::NodeIterator node = pattern.front();
		pattern.addEdge(node.id, under2.id, 1.f);
		pattern.addFace(node.id, under.id, under2.id);

		return true;
	}

	bool JoinOperation::apply(PatternGraph & pattern) const
	{
		// this operation does not add a new vertex, just joins them
		pattern.addJoint(from, with);
		return true;
	}

	bool FinishOffOperation::apply(PatternGraph & pattern) const
	{
		// this operation does not add a new vertex
		PatternGraph::NodeIterator under = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			qDebug() << "Not enough points to apply Finish Off";
			return false;
		}
		PatternGraph::NodeIterator under2 = pattern.back();

		PatternGraph::NodeIterator nextClose = under2;
		PatternGraph::NodeIterator lastClose = pattern.front();
		while (nextClose < lastClose) // until last and next meets. Careful!
		{
			// add constraint
			pattern.addJoint(nextClose.id, lastClose.id);
			if (!pattern.popOutline()) // use up a node
			{
				qDebug() << "Not enough points to apply Finish Off";
				return false;
			}

			nextClose = pattern.back(); // advance next
			lastClose = lastClose.last(); // go back in last
		}

		return true;
	}

	bool SkipOperation::apply(ami::PatternGraph & pattern) const
	{
		// does nothing, just uses up a node
		if (!pattern.popOutline()) // use up a node
		{
			qDebug() << "Not enough points to apply Skip";
			return false;
		}
		return true;
	}

	std::unique_ptr<GraphOperation> GraphOperation::getGraphOperation(Operation::Type type)
	{
		static std::unordered_map<Operation::Type, std::function<std::unique_ptr<GraphOperation>()>> operationFactory
		{
			{ Operation::Type::CH,		[]() { return std::make_unique<ChainOperation>(); } },
			{ Operation::Type::DEC,		[]() { return std::make_unique<DecreaseOperation>(); } },
			{ Operation::Type::FO,		[]() { return std::make_unique<FinishOffOperation>(); } },
			{ Operation::Type::INC,		[]() { return std::make_unique<IncreaseOperation>(); } },
			{ Operation::Type::LP,		[]() { return std::make_unique<LoopOperation>(); } },
			{ Operation::Type::MR,		[]() { return std::make_unique<MagicRingOperation>(); } },
			{ Operation::Type::SC,		[]() { return std::make_unique<SingleCrochetOperation>(); } },
			{ Operation::Type::SKIP,	[]() { return std::make_unique<SkipOperation>(); } },
			{ Operation::Type::SLST,	[]() { return std::make_unique<SlipStitchOperation>(); } },
			{ Operation::Type::NONE,	[]() { return std::unique_ptr<GraphOperation>(); } },
		};

		auto it = operationFactory.find(type);
		if (it != operationFactory.end())
		{
			return it->second(); // return unique_ptr
		}

		// Missing operation
		assert(false);
		return std::unique_ptr<GraphOperation>();
	}
}

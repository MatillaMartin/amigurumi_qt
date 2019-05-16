#include "PatternGraph.h"
#include "Pattern.h"

#include <sstream>
#include <unordered_map>
#include <functional>

#include <QMetaEnum>

namespace ami
{
	PatternGraph::PatternGraph()
	{}

	PatternGraph::PatternGraph(const Pattern & pattern)
	{
		append(pattern);
	}

	void PatternGraph::append(const Pattern & pattern)
	{
		unsigned int roundIndex = 0;
		for (auto & round : pattern.getRounds())
		{
			unsigned int operationIndex = 0;
			for (auto & op : round)
			{
				try
				{
					GraphOperation::getGraphOperation(op)->apply(*this);
				}
				catch (std::runtime_error & e)
				{
					std::stringstream ss;
					ss << "Round " << std::to_string(roundIndex) << ", Operation " << QString::number(operationIndex).toStdString() << " failed: " << e.what();
					throw std::runtime_error(ss.str());
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
		if (m_outline.size() < n)
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

	void LoopOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::LP });
		PatternGraph::Node & node = node_it.node();

		// loop only adds the base stitch
		//node.under = node.id;
		node.last = node.id;
		node.next = node.id;
	}

	void ChainOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::CH });
		PatternGraph::Node & node = node_it.node();

		pattern.addEdge(node.id, node.last, 1.f); // connect next node with our node

		// add node to back too! we want to be able to use it twice
	}

	void SingleCrochetOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::SC });
		PatternGraph::Node & node = node_it.node();

		PatternGraph::NodeIterator under = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			throw std::runtime_error("Not enough points to apply SC");
		}
		PatternGraph::NodeIterator under2 = pattern.back();

		pattern.addEdge(node.id, node.last, 1.f);
		pattern.addEdge(node.id, under.id, 1.f);
		pattern.addEdge(node.id, under2.id, 1.f);

		pattern.addFace(node.id, node.last, under.id);
		pattern.addFace(node.id, under.id, under2.id);
	}

	void IncreaseOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::INC });
		PatternGraph::Node & node = node_it.node();

		PatternGraph::NodeIterator under = pattern.back();

		pattern.addEdge(node.id, node.last, 1.f);
		pattern.addEdge(node.id, under.id, 1.f);

		pattern.addFace(node.id, node.last, under.id);
	}

	void DecreaseOperation::apply(PatternGraph & pattern) const
	{
		PatternGraph::NodeIterator node_it = pattern.addNode(PatternGraph::NodeData{ Operation::Type::DEC });
		PatternGraph::Node & node = node_it.node();

		PatternGraph::NodeIterator under = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			throw std::runtime_error("Not enough points to apply DEC");
		}
		PatternGraph::NodeIterator under2 = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			throw std::runtime_error("Not enough points to apply DEC");
		}
		PatternGraph::NodeIterator under3 = pattern.back();

		pattern.addEdge(node.id, node.last, 1.f);
		pattern.addEdge(node.id, under.id, 1.f);
		pattern.addEdge(node.id, under2.id, 1.f);
		pattern.addEdge(node.id, under3.id, 1.f);

		pattern.addFace(node.id, node.last, under.id);
		pattern.addFace(node.id, under.id, under2.id);
		pattern.addFace(node.id, under2.id, under3.id);
	}

	void MagicRingOperation::apply(PatternGraph & pattern) const
	{
		IncreaseOperation incOp;
		incOp.apply(pattern);
	}

	void SlipStitchOperation::apply(PatternGraph & pattern) const
	{
		// this operation does not add a new vertex
		PatternGraph::NodeIterator under = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			throw std::runtime_error("Not enough points to apply SLST");
		}
		PatternGraph::NodeIterator under2 = pattern.back();
		PatternGraph::NodeIterator node = pattern.front();
		pattern.addEdge(node.id, under2.id, 1.f);
		pattern.addFace(node.id, under.id, under2.id);
	}

	void JoinOperation::apply(PatternGraph & pattern) const
	{
		// this operation does not add a new vertex, just joins them
		pattern.addJoint(from, with);
	}

	void FinishOffOperation::apply(PatternGraph & pattern) const
	{
		// this operation does not add a new vertex
		PatternGraph::NodeIterator under = pattern.back();
		if (!pattern.popOutline()) // use up a node
		{
			throw std::runtime_error("Not enough points to apply FO");
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
				throw std::runtime_error("Not enough points to apply FO");
			}

			nextClose = pattern.back(); // advance next
			lastClose = lastClose.last(); // go back in last
		}
	}

	void SkipOperation::apply(ami::PatternGraph & pattern) const
	{
		// does nothing
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
		};

		auto & it = operationFactory.find(type);
		if (it != operationFactory.end())
		{
			return it->second(); // run command
		}
		else
		{
			throw std::runtime_error("Can't find graph operation for " + std::string(QMetaEnum::fromType<Operation::Type>().valueToKey(type)) );
		}
	}
}
